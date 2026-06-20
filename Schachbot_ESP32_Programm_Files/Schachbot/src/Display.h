#pragma once
#include <SPI.h>
#include <TFT_eSPI.h>
#include "Common.h"
#include <string>
#define MAX_MASSAGE_SIZE 127
#define FIELD_SIZE_X 40
#define FIELD_SIZE_Y 40

class Display{
    public:
    Display();
    void setSelectedFields(Koordinate field1, Koordinate field2);
    void printChessfield();
    void setMessage(String message);
    void printInformationBoard();
    void setChessBoard(char soll_schachbrett[SCHACHBRETT_SIZE_Y][SCHACHBRETT_SIZE_Y], char soll_schachbrettFriedhof[2][SCHACHBRETT_SCHLAGBREITE][SCHACHBRETT_SIZE_Y]);
    int displayMode = -1;
    private:
	char soll_schachbrett[SCHACHBRETT_SIZE_Y][SCHACHBRETT_SIZE_Y];
	char soll_schachbrettFriedhof[2][SCHACHBRETT_SCHLAGBREITE][SCHACHBRETT_SIZE_Y];
    int getWhiteOrBlack(int figur);
    TFT_eSPI tft;
    String message;
    bool inv = true;
    Koordinate selectedFields[2];
};