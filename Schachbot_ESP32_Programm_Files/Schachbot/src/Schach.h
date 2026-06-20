#pragma once
#include <string>
#include "Common.h"

class Schach
{
public:
	Schach();

	void start_Schachbrett();
	std::string moveFigur(int x, int y, int destX, int destY);
	s_Schachbrett getSchachbrett();
		//Gegner
	void printChessBoardSerial();
	bool getCurrentColor();
	void gegnerischerZug();
private:
	s_Schachbrett schachbrett;
	//Initialisieren des Schachfeldes
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
	//Berechnen eines eingegebenen Zuges
	int getWhiteOrBlack(int x, int y);
	bool checkIfMoveIsOnTheChessfield(int x, int y, int destX, int destY);
	int getPieceWithOutColor(int p);
	int direction(int pos, int dest);
	void stelleFigurAufFriedhof(int x, int y);

	MoveInformation pawnCheck(int x, int y, int destX, int destY, bool colorOnTurn, bool enpassant);
	MoveInformation towerCheck(int x, int y, int destX, int destY, bool flagRochade);
	MoveInformation horseCheck(int x, int y, int destX, int destY);
	MoveInformation bishopCheck(int x, int y, int destX, int destY);
	MoveInformation queenCheck(int x, int y, int destX, int destY);
	MoveInformation checkIfKingIsAttacked(bool colorOnTurn);
	bool checkIfKingIsCheckMate();
	bool checkIfAreaIsEmpty(int x, int y, int destX, int destY);
	MoveInformation kingCheck(int x, int y, int destX, int destY);
	//Info
	bool rochade[2][2] = { {possible,possible},{possible,possible} }; //COLOR, ROCHADE LEFT UND RIGHT
	bool colorOnTurn;
	Koordinate enPassant = { -1, -1 };
	int enPassantLebensDauer = -1;
};