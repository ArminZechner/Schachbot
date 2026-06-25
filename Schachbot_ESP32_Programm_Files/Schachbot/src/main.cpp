#include <Arduino.h>
#include "Tastenmatrix.h"
#include "Gantry.h"
#include "Display.h"
#include "Schach.h"
#include "StockfishAPI.h"
#include "WifiManagerWrapper.h"

struct TIME_PERIODE{
  unsigned long getKey = 0;
  unsigned long zusatzZeitKey = 0;
  unsigned long wlanConnect = 0;
  unsigned long displayInfoBoard = 0;
};

TIME_PERIODE timePeriode;

Display display;
Gantry gantry;
Tastenmatrix tasten;
Schach zugCheck;
StockfishAPI stockfish;

WifiManagerWrapper wifi;
bool gameMode = GAMEMODE::roboter;
void setup() {
    delay(500);
    Serial.begin(115200);
    Serial.println("SYSTEM_START");
    timePeriode.getKey = millis();
    gantry.init();
    zugCheck.start_Schachbrett();
    display.setChessBoard(zugCheck.getSchachbrett().hauptSchachbrett, zugCheck.getSchachbrett().friedhof);
    display.printChessfield();
    Serial.println("Start Program");
    //wifi.begin();
    gantry.moveToZero();
    display.setGameMode(gameMode);
}
void loop() {
    gantry.move();
    if(gantry.bewegungsAblauf.isWorking == false){
      gantry.searchAvailableMove();
    }
    //wifi.handle();
    if ((millis()-timePeriode.getKey)>(50+timePeriode.zusatzZeitKey)) {
      timePeriode.zusatzZeitKey = 0;
      if(tasten.getKey() == true){
        timePeriode.zusatzZeitKey = 300;
        timePeriode.displayInfoBoard = millis();
        display.setSelectedFields(tasten.getEingabeChar(0), tasten.getEingabeChar(1));
        display.printInformationBoard();
      }
      timePeriode.getKey = millis();
    }
    if(display.displayMode == DISPLAY_MODE::infoBoard && (millis()-timePeriode.displayInfoBoard) > 3000){
      display.setChessBoard(zugCheck.getSchachbrett().hauptSchachbrett, zugCheck.getSchachbrett().friedhof);
      display.printChessfield();
    }
    if(tasten.checkifBufferIsFull()){
      Serial.printf("[%d,%d] [%d,%d]\n", tasten.getEingabe(0).x, tasten.getEingabe(0).y, tasten.getEingabe(1).x, tasten.getEingabe(1).y);
      if(tasten.getEingabe(0).x == 6 && tasten.getEingabe(0).y==0 && tasten.getEingabe(1).x==6 && tasten.getEingabe(1).y==0){
        gameMode = !gameMode;
        display.setGameMode(gameMode);
      }
      std::string info;
      info = zugCheck.moveFigur(tasten.getEingabe(0).x, 7-tasten.getEingabe(0).y,
                                tasten.getEingabe(1).x, 7-tasten.getEingabe(1).y);
      if(gameMode == GAMEMODE::roboter){
      if(info == ""){
          MoveResult result;
          result = stockfish.getTurn(zugCheck.getSchachbrett().hauptSchachbrett, zugCheck.getCurrentColor());
          Serial.printf("Result: %s, %d, %d\n", result.bestMove.c_str(), result.evaluation, zugCheck.getCurrentColor()); 
          //KI
          Serial.printf("[%d,%d] [%d,%d]\n", result.bestMove.charAt(0)-'a', result.bestMove.charAt(1)-'0', result.bestMove.charAt(2)-'a', result.bestMove.charAt(3)-'0');
          if(zugCheck.moveFigur(result.bestMove.charAt(0) - 'a',8 - (result.bestMove.charAt(1) - '0'),
                                result.bestMove.charAt(2) - 'a',8 - (result.bestMove.charAt(3) - '0')) != ""){
                                  Serial.println("GEGNERISCHER ZUG");
                                  zugCheck.gegnerischerZug();
          }
          display.setChessBoard(zugCheck.getSchachbrett().hauptSchachbrett, zugCheck.getSchachbrett().friedhof);
          display.printChessfield();
          display.setMessage("");
        }
      else{
          if(info != "intern_error"){
          Serial.printf("[INFO]: %s\n", info.c_str());
          display.setMessage(info.c_str());
          }
      }
    }
      tasten.clearKeyMatrix();
      gantry.setChessBoard(zugCheck.getSchachbrett().hauptSchachbrett, zugCheck.getSchachbrett().friedhof);
    }
    delayMicroseconds(500);
}