#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "Common.h"

class StockfishAPI
{
public:
    StockfishAPI();

    MoveResult getTurn(
        char board[SCHACHBRETT_SIZE_Y][SCHACHBRETT_SIZE_Y],
        bool whiteTurn
    );

    int getLastEvaluation() const;

private:

    int _lastEvaluation;

    String boardToFEN(
        char board[SCHACHBRETT_SIZE_Y][SCHACHBRETT_SIZE_Y],
        bool whiteTurn
    );

    char pieceToFen(char piece);

    MoveResult requestStockfish(const String& fen);
};