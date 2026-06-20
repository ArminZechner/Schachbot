#pragma once
#include "Common.h"
#include <AccelStepper.h>

#define FIELD_STEPS 2150
#define HALF_FIELD_STEPS (FIELD_STEPS/2)

enum ABLAUF{
    goToStart_x,
    goToStart_y,
    schritt0,
    schritt1,
    schritt2,
    schritt3,
    finish
};

struct Bewegungsablauf_T{
    bool moveToZero = 0;
    int ablaufState = 0;
    bool XOrY_goToStart_x = 0;
    int goToStart_x = 0;
    bool XOrY_goToStart_y = 0;
    int goToStart_y = 0;
    bool XOrY_Schritt0 = 0;
    int schritt0 = 0;
    bool XOrY_Schritt1 = 0;
    int schritt1 = 0;
    bool XOrY_Schritt2 = 0;
    int schritt2 = 0;
    bool XOrY_Schritt3 = 0;
    int schritt3 = 0;
    bool isWorking = false;
};
    extern AccelStepper stepperX;
    extern AccelStepper stepperY;

class Gantry{
    public:
    Gantry();
    void move();
    void moveToZero();
    void init();
    bool isAtZero();
    void searchAvailableMove();
    void goTo(Koordinate start, Koordinate ende);
    void setChessBoard(char soll_schachbrett[SCHACHBRETT_SIZE_Y][SCHACHBRETT_SIZE_Y], char soll_schachbrettFriedhof[2][SCHACHBRETT_SCHLAGBREITE][SCHACHBRETT_SIZE_Y]);
    Koordinate searchField(int8_t soll_figur, int8_t ist_figur);
    Bewegungsablauf_T bewegungsAblauf;
    private:
    Koordinate position;
    int8_t pinSpule = 22;
    int8_t pinButtonX = 35;
    int8_t pinButtonY = 34;
    char ist_schachbrett[SCHACHBRETT_SIZE_Y][SCHACHBRETT_SIZE_Y];
	char ist_schachbrettFriedhof[2][SCHACHBRETT_SCHLAGBREITE][SCHACHBRETT_SIZE_Y];

    char soll_schachbrett[SCHACHBRETT_SIZE_Y][SCHACHBRETT_SIZE_Y];
	char soll_schachbrettFriedhof[2][SCHACHBRETT_SCHLAGBREITE][SCHACHBRETT_SIZE_Y];
};