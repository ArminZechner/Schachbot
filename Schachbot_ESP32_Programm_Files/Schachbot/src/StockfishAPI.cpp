#include "StockfishAPI.h"

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

#include "Common.h"

// Neue öffentliche Stockfish API
#define STOCKFISH_API_URL "https://stockfish.online/api/s/v2.php"

StockfishAPI::StockfishAPI()
{
    _lastEvaluation = 0;
}

int StockfishAPI::getLastEvaluation() const
{
    return _lastEvaluation;
}

MoveResult StockfishAPI::getTurn(
    char board[SCHACHBRETT_SIZE_Y][SCHACHBRETT_SIZE_Y],
    bool whiteTurn
)
{
    String fen = boardToFEN(board, whiteTurn);

    //Serial.println("=== GENERATED FEN ===");
    //Serial.println(fen);

    return requestStockfish(fen);
}

char StockfishAPI::pieceToFen(char piece)
{
    switch(piece)
    {
        case whitePawn:   return 'P';
        case whiteTower:  return 'R';
        case whiteHorse:  return 'N';
        case whiteBishop: return 'B';
        case whiteQueen:  return 'Q';
        case whiteKing:   return 'K';

        case blackPawn:   return 'p';
        case blackTower:  return 'r';
        case blackHorse:  return 'n';
        case blackBishop: return 'b';
        case blackQueen:  return 'q';
        case blackKing:   return 'k';

        case empty:
        default:
            return 0;
    }
}

String StockfishAPI::boardToFEN(
    char board[SCHACHBRETT_SIZE_Y][SCHACHBRETT_SIZE_Y],
    bool whiteTurn
)
{
    String fen = "";

    // Schwarz oben / Weiß unten
    for(int y = 7; y >= 0; y--)
    {
        int emptyCount = 0;

        for(int x = 0; x < 8; x++)
        {
            char piece = board[x][y];

            if(piece == empty)
            {
                emptyCount++;
                continue;
            }

            char f = pieceToFen(piece);

            if(f == 0)
            {
                emptyCount++;
                continue;
            }

            if(emptyCount > 0)
            {
                fen += String(emptyCount);
                emptyCount = 0;
            }

            fen += f;
        }

        if(emptyCount > 0)
        {
            fen += String(emptyCount);
        }

        if(y > 0)
        {
            fen += "/";
        }
    }

    fen += whiteTurn ? " w " : " b ";

    // Keine Rochade / En Passant
    fen += "- - 0 1";

    return fen;
}

MoveResult StockfishAPI::requestStockfish(const String& fen)
{
    MoveResult result;

    result.success = false;
    result.bestMove = "";
    result.pv = "";
    result.evaluation = 0;
    result.mateIn = 0;
    result.depth = 0;

    // WLAN prüfen
    if(WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi not connected");
        return result;
    }

    // HTTPS Client DIREKT auf dem Stack
    WiFiClientSecure client;

    client.setInsecure();
    client.setTimeout(15000);

    HTTPClient https;

    // FEN encoden
    String encodedFen = fen;
    encodedFen.replace(" ", "%20");

    // URL bauen
    String url =
        String(STOCKFISH_API_URL) +
        "?fen=" + encodedFen +
        "&depth=15";

    //Serial.println("=== REQUEST URL ===");
    //Serial.println(url);

    // HTTPS Verbindung starten
    if(!https.begin(client, url))
    {
        Serial.println("HTTPS begin failed");
        return result;
    }

    yield();

    // Request senden
    int httpCode = https.GET();

    yield();

    if(httpCode <= 0)
    {
        Serial.printf("HTTP ERROR: %d\n", httpCode);

        https.end();

        return result;
    }

    //Serial.printf("HTTP CODE: %d\n", httpCode);

    // Antwort holen
    String payload = https.getString();

    yield();

    https.end();

    //Serial.println("=== RESPONSE ===");
    //Serial.println(payload);

    // JSON Dokument
    DynamicJsonDocument doc(2048);

    DeserializationError error =
        deserializeJson(doc, payload);

    yield();

    if(error)
    {
        Serial.println("JSON parse error");
        return result;
    }

    // success
    if(doc.containsKey("success"))
    {
        result.success = doc["success"];
    }

    // evaluation
    if(doc.containsKey("evaluation"))
    {
        float eval = doc["evaluation"];
        result.evaluation = (int)(eval * 100.0f);
    }

    // mate
    if(doc.containsKey("mate") &&
       !doc["mate"].isNull())
    {
        result.mateIn = doc["mate"];
    }

    // bestmove
    if(doc.containsKey("bestmove"))
    {
        String bestmove =
            doc["bestmove"].as<String>();

        // Beispiel:
        // "bestmove e7e5 ponder g1f3"

        int firstSpace =
            bestmove.indexOf(' ');

        int secondSpace =
            bestmove.indexOf(
                ' ',
                firstSpace + 1
            );

        if(firstSpace >= 0 &&
           secondSpace > firstSpace)
        {
            result.bestMove =
                bestmove.substring(
                    firstSpace + 1,
                    secondSpace
                );
        }

        result.pv = bestmove;
    }

    // depth
    if(doc.containsKey("depth"))
    {
        result.depth = doc["depth"];
    }

    //Serial.println("=== PARSED RESULT ===");
    //Serial.println(result.bestMove);
    //Serial.println(result.evaluation);

    _lastEvaluation = result.evaluation;

    yield();

    return result;
}