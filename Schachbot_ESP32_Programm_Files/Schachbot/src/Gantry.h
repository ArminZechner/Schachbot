#pragma once
#include "Common.h"
#include <AccelStepper.h>

#define FIELD_STEPS_Y 2150
#define FIELD_STEPS_X 1878
#define HALF_FIELD_STEPS_X (int)(FIELD_STEPS_X/2)
#define HALF_FIELD_STEPS_Y (int)(FIELD_STEPS_Y/2)

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
    Koordinate start = {-1, -1};
    Koordinate ende = {-1, -1};
    int ablaufState = 0;
    bool startgoToStart_x = 0;
    bool startgoToStart_y = 0;
    int goToStart_x = 0;
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
    Koordinate searchField(int8_t soll_figur, int8_t ist_figur, Koordinate ausschlussFeld);
    Koordinate searchFriedhofField(int8_t soll_figur, int8_t ist_figur);
    Bewegungsablauf_T bewegungsAblauf;
    private:
    int8_t getField(Koordinate pos);
    void setField(Koordinate pos, int8_t figur);
    void updateChessBoard();
    Koordinate position;
    int8_t pinSpule = 22;
    int8_t pinButtonX = 35;
    int8_t pinButtonY = 34;
    	char init_Liste[HAUPT_SCHACHBRETT_X][HAUPT_SCHACHBRETT_Y] = {
	blackTower, blackHorse, blackBishop, blackQueen, blackKing, blackBishop,  blackHorse, blackTower,
	blackPawn, blackPawn, blackPawn, blackPawn, blackPawn, blackPawn, blackPawn, blackPawn,
	empty, empty, empty, empty, empty, empty, empty, empty,
	empty, empty, empty, empty, empty, empty, empty, empty,
	empty, empty, empty, empty, empty, empty, empty, empty,
	empty, empty, empty, empty, empty, empty, empty, empty,
	whitePawn, whitePawn, whitePawn, whitePawn, whitePawn, whitePawn, whitePawn, whitePawn,
	whiteTower, whiteHorse, whiteBishop, whiteQueen, whiteKing, whiteBishop,  whiteHorse, whiteTower,
	};
    char ist_schachbrett[SCHACHBRETT_SIZE_Y][SCHACHBRETT_SIZE_Y];
	char ist_schachbrettFriedhof[2][SCHACHBRETT_SCHLAGBREITE][SCHACHBRETT_SIZE_Y];

    char soll_schachbrett[SCHACHBRETT_SIZE_Y][SCHACHBRETT_SIZE_Y];
	char soll_schachbrettFriedhof[2][SCHACHBRETT_SCHLAGBREITE][SCHACHBRETT_SIZE_Y];
};