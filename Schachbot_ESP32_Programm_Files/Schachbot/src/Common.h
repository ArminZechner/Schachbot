#pragma once
#include <Arduino.h>
#include <string.h>
#define SCHACHBRETT_SIZE_X 12
#define SCHACHBRETT_SIZE_Y 8
#define SCHACHBRETT_SCHLAGBREITE 2

#define KEIN_ERROR "0"
#define KOORDINATEN_ANZAHL 2
#define HAUPT_SCHACHBRETT_X 8
#define HAUPT_SCHACHBRETT_Y 8
#define FRIEDHOF_X 2
#define FRIEDHOF_Y 8
#define UNDEF_FRIEDHOF -2

enum GAMEMODE{
	roboter,
	mensch
};
struct Koordinate{
    int8_t x;
    int8_t y;
};
struct MoveResult{
        String bestMove;
        String pv;

        int evaluation;
        int mateIn;
        int depth;

        bool success;
};
enum e_figuren{
		whitePawn,
		whiteTower,
		whiteHorse,
		whiteBishop,
		whiteQueen,
		whiteKing,

		blackPawn,
		blackTower,
		blackHorse,
		blackBishop,
		blackQueen,
		blackKing,

		empty,
		whitePawnQueen,
		blackPawnQueen
	};
enum Color {
	white,
	black
};
enum DIRECTION {
	left,
	right
};
enum ROCHADE_POSSIBLE {
	possible,
	blocked
};
struct MoveInformation {
	Koordinate zusatzFigur_start = {-1, -1};
	Koordinate zusatzFigur_ende = {-1, -1};

	std::string error = "";
};
struct s_Schachbrett {
	char hauptSchachbrett[HAUPT_SCHACHBRETT_X][HAUPT_SCHACHBRETT_Y];
	char friedhof[2][FRIEDHOF_X][FRIEDHOF_Y];
};
enum DISPLAY_MODE{
	infoBoard,
	chessBoard
};