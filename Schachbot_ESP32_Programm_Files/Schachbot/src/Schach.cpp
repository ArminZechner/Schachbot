#include "Schach.h"

/*--------------------------------------PUBLIC-------------------------------------------*/
Schach::Schach() {
    colorOnTurn = Color::white;
}

void Schach::start_Schachbrett() {
    for (int x = 0; x < FRIEDHOF_X; x++) {
        for (int y = 0; y < FRIEDHOF_Y; y++) {
            schachbrett.friedhof[DIRECTION::left][x][y] = e_figuren::empty;
            schachbrett.friedhof[DIRECTION::right][x][y] = e_figuren::empty;
        }
    }
    for (int x = 0; x < HAUPT_SCHACHBRETT_X; x++) {
        for (int y = 0; y < HAUPT_SCHACHBRETT_Y; y++) {
            schachbrett.hauptSchachbrett[x][y] = init_Liste[y][x];
        }
    }
    colorOnTurn = Color::white;
}

std::string Schach::moveFigur(int x, int y, int destX, int destY) {
    s_Schachbrett schachbrettBackup = schachbrett;
    if((x < 0 || x > 7) || (y < 0 || y > 7) || (destX < 0 || destX > 7) || (destY < 0 || destY > 7)){
        return "intern_error";
    }
    bool rochadebackup[2][2] = { {rochade[0][0], rochade[0][1]}, {rochade[1][0], rochade[1][1]} };
    Koordinate enpassantBackup = enPassant;

    if (getWhiteOrBlack(x, y) != colorOnTurn) {
        return "Du bist nicht\nam Zug!";
    }
    if (getWhiteOrBlack(x, y) == getWhiteOrBlack(destX, destY)) {
        return "Du kannst nicht\ndeine eigenen\nFiguren schlagen";
    }
    MoveInformation info;
    switch (getPieceWithOutColor(schachbrett.hauptSchachbrett[x][y])) {
    case e_figuren::whitePawn:
        info = pawnCheck(x, y, destX, destY, colorOnTurn ,1);
        break;
    case e_figuren::whiteTower:
        info = towerCheck(x, y, destX, destY, 1);
        break;
    case e_figuren::whiteHorse:
        info = horseCheck(x, y, destX, destY);
        break;
    case e_figuren::whiteBishop:
        info = bishopCheck(x, y, destX, destY);
        break;
    case e_figuren::whiteQueen:
        info = queenCheck(x, y, destX, destY);
        break;
    case e_figuren::whiteKing:
        info = kingCheck(x, y, destX, destY);
        break;
    }
    if (info.error != "") {
        return info.error;
    }
    if(destY == (colorOnTurn ? 7 : 0) && e_figuren::whitePawn == getPieceWithOutColor(schachbrett.hauptSchachbrett[x][y])) {
        schachbrett.hauptSchachbrett[x][y] = (colorOnTurn ? e_figuren::blackPawnQueen : e_figuren::whitePawnQueen);
    }
    if(info.zusatzFigur_start.x != -1 && info.zusatzFigur_start.y != -1 && info.zusatzFigur_ende.x == -1 && info.zusatzFigur_ende.y == -1) {
        stelleFigurAufFriedhof(info.zusatzFigur_start.x, info.zusatzFigur_start.y);
        schachbrett.hauptSchachbrett[info.zusatzFigur_start.x][info.zusatzFigur_start.y] = e_figuren::empty;
    }
    else if (info.zusatzFigur_start.x != -1 && info.zusatzFigur_start.y != -1 && info.zusatzFigur_ende.x != -1 && info.zusatzFigur_ende.y != -1) {
        schachbrett.hauptSchachbrett[info.zusatzFigur_ende.x][info.zusatzFigur_ende.y] = schachbrett.hauptSchachbrett[info.zusatzFigur_start.x][info.zusatzFigur_start.y];
        schachbrett.hauptSchachbrett[info.zusatzFigur_start.x][info.zusatzFigur_start.y] = e_figuren::empty;
    }
    if(schachbrett.hauptSchachbrett[destX][destY] != e_figuren::empty) {
        stelleFigurAufFriedhof(destX, destY);
    }
    schachbrett.hauptSchachbrett[destX][destY] = schachbrett.hauptSchachbrett[x][y];
    schachbrett.hauptSchachbrett[x][y] = e_figuren::empty;
    info.error = checkIfKingIsAttacked(colorOnTurn).error;
    if(info.error != "") {
        schachbrett = schachbrettBackup;
        for(int i = 0; i < 2; i++) {
            for(int j = 0; j < 2; j++) {
                rochade[i][j] = rochadebackup[i][j];
            }
        }
        enPassant = enpassantBackup;
        return info.error;
    }

    if(checkIfKingIsCheckMate()) {
        if(checkIfKingIsAttacked(!colorOnTurn).error != "") {
            return "checkmate";
        }
        return "patt";
    }
    colorOnTurn = !colorOnTurn;

    if(enPassantLebensDauer >= 0) {
        enPassantLebensDauer--;
    }
    return "";
}
s_Schachbrett Schach::getSchachbrett() {
	return schachbrett;
}
void Schach::printChessBoardSerial(){
    for (int x = 0; x < HAUPT_SCHACHBRETT_X; x++) {
        for (int y = 0; y < HAUPT_SCHACHBRETT_Y; y++) {
            Serial.printf("%d ", schachbrett.hauptSchachbrett[x][y]);
        }
        Serial.println();
    }
}
bool Schach::getCurrentColor(){
    return colorOnTurn;
}

/*--------------------------------------PRIVATE-------------------------------------------*/
void Schach::stelleFigurAufFriedhof(int x, int y) {
    for(int i = 0; i < FRIEDHOF_Y; i++) {
        for(int j = 0; j < FRIEDHOF_X; j++) {
            if(schachbrett.hauptSchachbrett[x][y] == (colorOnTurn ? e_figuren::whitePawnQueen : e_figuren::blackPawnQueen)) {
                if((colorOnTurn ? schachbrett.friedhof[DIRECTION::left][j][i] : schachbrett.friedhof[DIRECTION::right][j][i]) == e_figuren::empty &&
                    getPieceWithOutColor(init_Liste[j][i]) == e_figuren::whitePawn) {
                    (colorOnTurn ? schachbrett.friedhof[DIRECTION::left][j][i] : schachbrett.friedhof[DIRECTION::right][FRIEDHOF_X - j - 1][i]) = (colorOnTurn ? e_figuren::whitePawn : e_figuren::blackPawn);
                    return;
                }
            }
            if((colorOnTurn ? schachbrett.friedhof[DIRECTION::left][j][i] : schachbrett.friedhof[DIRECTION::right][FRIEDHOF_X - j - 1][i]) == e_figuren::empty &&
                getPieceWithOutColor(init_Liste[j][i]) == getPieceWithOutColor(schachbrett.hauptSchachbrett[x][y])) {
                (colorOnTurn ? schachbrett.friedhof[DIRECTION::left][j][i] : schachbrett.friedhof[DIRECTION::right][FRIEDHOF_X - j - 1][i]) = schachbrett.hauptSchachbrett[x][y];
                return;
            }
        }
    }
    schachbrett.hauptSchachbrett[x][y] = e_figuren::empty;
}
int Schach::getWhiteOrBlack(int x, int y) {
    if (schachbrett.hauptSchachbrett[x][y] == e_figuren::empty) {
        return -1;
    }
    if(schachbrett.hauptSchachbrett[x][y] == e_figuren::whitePawnQueen) {
        return Color::white;
    }
    else if(schachbrett.hauptSchachbrett[x][y] == e_figuren::blackPawnQueen) {
        return Color::black;
    }
    if (schachbrett.hauptSchachbrett[x][y] >= e_figuren::whitePawn &&
        schachbrett.hauptSchachbrett[x][y] <= e_figuren::whiteKing) {
        return Color::white;
    }
    else {
        return Color::black;
    }
}
bool Schach::checkIfMoveIsOnTheChessfield(int x, int y, int destX, int destY) {
    if (x < 0 || x > HAUPT_SCHACHBRETT_X - 1 ||
        y < 0 || y > HAUPT_SCHACHBRETT_Y ||
        destX < 0 || destX > HAUPT_SCHACHBRETT_X - 1 ||
        destY < 0 || destY > HAUPT_SCHACHBRETT_Y) {
        return false;
    }
    return true;
}
int Schach::getPieceWithOutColor(int p) {
    if (p == e_figuren::empty) {
        return e_figuren::empty;
    }
    if(p == e_figuren::whitePawnQueen || p == e_figuren::blackPawnQueen) {
        return whiteQueen;
    }
    if (p >= e_figuren::whitePawn &&
        p <= e_figuren::whiteKing) {
        return p;
    }
    else {
        return p - 6;
    }
}
int Schach::direction(int pos, int dest) {
    if (pos > dest) {
        return -1;
    }
    else {
        return 1;
    }
}
//Checks
MoveInformation Schach::pawnCheck(int x, int y, int destX, int destY, bool colorOnTurn, bool enpassant) {
    MoveInformation info;
    if((colorOnTurn ? ((destY - y) == 1) : ((destY - y) == -1)) && //Prueft ob die Figur in die richtige Richtung faehrt
        std::abs(destX - x) == 1 && //Prueft ob sie nur ein Feld nach links oder rechts schlaegt
        (schachbrett.hauptSchachbrett[destX][destY] != e_figuren::empty ||//Prueft ob auf dem Feld ein Figur steht auf dem Sie zuschlaegt
        ((destX == enPassant.x && destY == enPassant.y) && (enPassantLebensDauer >= 0)))) {//Prueft EnPassant
        if(schachbrett.hauptSchachbrett[destX][destY] == e_figuren::empty && enPassantLebensDauer >= 0) {
            info.zusatzFigur_start.x = destX;
            info.zusatzFigur_start.y = destY + (colorOnTurn ? 3 : -3);
            info.zusatzFigur_ende.x = -1;
            info.zusatzFigur_ende.y = -1;
        }
        return info;
    }
    if(x != destX) {
        info.error = "Der Bauer darf\nnur gerade fahren";
        return info;
    }
    if(schachbrett.hauptSchachbrett[destX][destY] != e_figuren::empty) {
        info.error = "Feld auf das der Bauer zieht muss leer sein";
        return info;
    }
    if(std::abs(destY - y) > 2) {
        info.error = "Der Bauer darf\nnicht so weit\nfahren";
        return info;
    }
    if(colorOnTurn ? ((destY - y) == -1) : ((destY - y) == 1)) {
        if(std::abs(destY - y) != 1) {
            info.error = "Der Bauer darf\nnur ein Feld\nfahren";
        }
        else {
            info.error = "Der Bauer darf nur\nRichtung Gegner Seite\nfahren";
        }
        return info;
    }
    if(std::abs(destY - y) == 2 && (//Bauer bewegt sich 2 Felder
        (colorOnTurn ? (y != 1) : (y != 6)) || //Bauer startet bei Feld 1 oder 6 nach Farbe
        (colorOnTurn ? (schachbrett.hauptSchachbrett[x][2] != e_figuren::empty) : (schachbrett.hauptSchachbrett[x][5] != e_figuren::empty)))) {//Raum zwischen Bauer Sprung ist leer
        info.error = "Der Bauer darf\nso nicht ziehen";
        return info;
    }
    if(std::abs(destY - y) == 2) {
        if(enpassant) {
            enPassant.y = y + (colorOnTurn ? 2 : -2);
            enPassant.x = x;
            enPassantLebensDauer = 1;
        }
    }
    return info;
}
MoveInformation Schach::towerCheck(int x, int y, int destX, int destY, bool flagRochade) {
    MoveInformation info;
    if (x == destX && y == destY) {
        info.error = "Der Turm darf\nnur gerade fahren";
        return info;
    }
    if (x != destX && y != destY) {
        info.error = "Der Turm darf\nnur gerade fahren";
        return info;
    }
    if (x == destX) {
        for (int i = 1; i < std::abs(destY - y); i++) {
            if (schachbrett.hauptSchachbrett[x][y + (i * direction(y, destY))] != e_figuren::empty) {
                info.error = "Der Turm darf nicht\ndurch Figuren\nhindurch fahren";
                return info;
            }
        }
    }
    else if (y == destY) {
        for (int i = 1; i < std::abs(destX - x); i++) {
            if (schachbrett.hauptSchachbrett[x + (i * direction(x, destX))][y] != e_figuren::empty) {
                info.error = "Der Turm darf nicht\ndurch Figuren\nhindurch fahren";
                return info;
            }
        }
    }
    /*----------------------ROCHADE----------------------------*/
    if(flagRochade) {
        if((x == 0 && y == 7) && rochade[Color::white][DIRECTION::left] == ROCHADE_POSSIBLE::possible) {
            rochade[Color::white][DIRECTION::left] = ROCHADE_POSSIBLE::blocked;
        }
        else if((x == 7 && y == 7) && rochade[Color::white][DIRECTION::right] == ROCHADE_POSSIBLE::possible) {
            rochade[Color::white][DIRECTION::right] = ROCHADE_POSSIBLE::blocked;
        }
        if((x == 0 && y == 0) && rochade[Color::black][DIRECTION::right] == ROCHADE_POSSIBLE::possible) {
            rochade[Color::black][DIRECTION::right] = ROCHADE_POSSIBLE::blocked;
        }
        else if((x == 7 && y == 0) && rochade[Color::black][DIRECTION::left] == ROCHADE_POSSIBLE::possible) {
            rochade[Color::black][DIRECTION::left] = ROCHADE_POSSIBLE::blocked;
        }
    }
    return info;
}
MoveInformation Schach::horseCheck(int x, int y, int destX, int destY) {
    MoveInformation info;
    if ((std::abs(destX - x) == 1 && std::abs(destY - y) == 2) ||
        (std::abs(destX - x) == 2 && std::abs(destY - y) == 1)) {
        return info;
    }
    info.error = "Das Pferd darf\nnicht so springen";
    return info;
}
MoveInformation Schach::bishopCheck(int x, int y, int destX, int destY) {
    MoveInformation info;
    if (std::abs(destX - x) != std::abs(destY - y)) {
        info.error = "Der Laeufer darf\nnur diagonal\nfahren";
        return info;
    }
    for (int i = 1; i < std::abs(destX - x); i++) {
        if (schachbrett.hauptSchachbrett[x + (i * direction(x, destX))][y + (i * direction(y, destY))] != e_figuren::empty) {
            info.error = "Der Laefer darf nicht\ndurch Figuren\nhindurch fahren";
            return info;
        }
    }
    return info;
}
MoveInformation Schach::queenCheck(int x, int y, int destX, int destY) {
    MoveInformation info;
    info = bishopCheck(x, y, destX, destY);
    if (info.error == "") {
        return info;
    }
    info = towerCheck(x, y, destX, destY, 0);
    if (info.error == "") {
        return info;
    }
    info.error = "Die Dame darf\nnicht so fahren";
    return info;
}
bool Schach::checkIfAreaIsEmpty(int x, int y, int destX, int destY) {
    for (int yy = y; yy < (unsigned int)(destY - y); yy++) {
        for (int xx = x; xx < (unsigned int)(destY - x); xx++) {
            if (schachbrett.hauptSchachbrett[xx][yy] != e_figuren::empty) {
                return false;
            }
        }
    }
    return true;
}
MoveInformation Schach::kingCheck(int x, int y, int destX, int destY) {
    MoveInformation info;
    if (colorOnTurn == Color::white && (x == 4 && y == 7) && (destX == 2 && destY == 7) && rochade[Color::white][DIRECTION::left] == ROCHADE_POSSIBLE::possible && checkIfAreaIsEmpty(1, 7, 3, 7)) {
        rochade[Color::white][DIRECTION::left] = ROCHADE_POSSIBLE::blocked;
        info.zusatzFigur_start = { 0, 7};
        info.zusatzFigur_ende = { 3, 7 };
        return info;
    }
    else if (colorOnTurn == Color::white && (x == 4 && y == 7) && (destX == 6 && destY == 7) && rochade[Color::white][DIRECTION::right] == ROCHADE_POSSIBLE::possible && checkIfAreaIsEmpty(5, 7, 6, 7)) {
        rochade[Color::white][DIRECTION::right] = ROCHADE_POSSIBLE::blocked;
        info.zusatzFigur_start = { 7, 7 };
        info.zusatzFigur_ende = { 5, 7 };
        return info;
    }
    if (colorOnTurn == Color::black && (x == 4 && y == 0) && (destX == 6 && destY == 0) && rochade[Color::black][DIRECTION::left] == ROCHADE_POSSIBLE::possible && checkIfAreaIsEmpty(5, 0, 6, 0)) {
        rochade[Color::black][DIRECTION::left] = ROCHADE_POSSIBLE::blocked;
        info.zusatzFigur_start = { 7, 0 };
        info.zusatzFigur_ende = { 5, 0 };
        return info;
    }
    else if (colorOnTurn == Color::black && (x == 4 && y == 0) && (destX == 2 && destY == 0) && rochade[Color::black][DIRECTION::right] == ROCHADE_POSSIBLE::possible && checkIfAreaIsEmpty(1, 0, 3, 0)) {
        rochade[Color::black][DIRECTION::right] = ROCHADE_POSSIBLE::blocked;
        info.zusatzFigur_start = { 0, 0 };
        info.zusatzFigur_ende = { 3, 0 };
        return info;
    }
    if (std::abs(destX - x) <= 1 && std::abs(destY - y) <= 1) {
        rochade[colorOnTurn][DIRECTION::left] = ROCHADE_POSSIBLE::blocked;
        rochade[colorOnTurn][DIRECTION::right] = ROCHADE_POSSIBLE::blocked;
        return info;
    }
    info.error = "Der Koenig darf nur ein Feld weit fahren";
    return info;
}
MoveInformation Schach::checkIfKingIsAttacked(bool colorOnTurn) {
    MoveInformation info;
    Koordinate koordsKing = {-1, -1};
    for(int x = 0; x < HAUPT_SCHACHBRETT_X; x++) {
        for(int y = 0; y < HAUPT_SCHACHBRETT_Y; y++) {
            if(schachbrett.hauptSchachbrett[x][y] == (colorOnTurn ? e_figuren::blackKing : e_figuren::whiteKing)) { //<--------------- Fix
                koordsKing.x = x;
                koordsKing.y = y;
             }
        }
    }
    if(koordsKing.x == -1 || koordsKing.y == -1) {
        return info;
    }
    for(int x = 0; x < HAUPT_SCHACHBRETT_X; x++) {
		for(int y = 0; y < HAUPT_SCHACHBRETT_Y; y++) {
            if(schachbrett.hauptSchachbrett[x][y] != e_figuren::empty && getWhiteOrBlack(x, y) == !colorOnTurn) {
                std::string welcheFigur = "";
                bool flag = false;
				switch(getPieceWithOutColor(schachbrett.hauptSchachbrett[x][y])) {
				case e_figuren::whitePawn:
					welcheFigur = pawnCheck(x, y, koordsKing.x, koordsKing.y, !colorOnTurn, 0).error;
                    if(welcheFigur == "") {
                        welcheFigur = "Bauer";
                        flag = true;
                    }
					break;
				case e_figuren::whiteTower:
                    welcheFigur = towerCheck(x, y, koordsKing.x, koordsKing.y, 0).error;
                    if(welcheFigur == "") {
                        welcheFigur = "Turm";
                        flag = true;
                    }
					break;
				case e_figuren::whiteHorse:
                    welcheFigur = horseCheck(x, y, koordsKing.x, koordsKing.y).error;
                    if(welcheFigur == "") {
                        welcheFigur = "Pferd";
                        flag = true;
                    }
					break;
				case e_figuren::whiteBishop:
                    welcheFigur = bishopCheck(x, y, koordsKing.x, koordsKing.y).error;
                    if(welcheFigur == "") {
                        welcheFigur = "Laeufer";
                        flag = true;
                    }
					break;
				case e_figuren::whiteQueen:
                    welcheFigur = queenCheck(x, y, koordsKing.x, koordsKing.y).error;
                    if(welcheFigur == "") {
                        welcheFigur = "Koenigin";
                        flag = true;
                    }
					break;
                case e_figuren::whiteKing:
                    welcheFigur = kingCheck(x, y, koordsKing.x, koordsKing.y).error;
                    if(welcheFigur == "") {
                        welcheFigur = "Koenig";
                        flag = true;
                    }
                    break;
				}
				if(flag) {
                    info.error = "Der Koenig wird\nbedroht";
                    info.error += " von ";
                    info.error += welcheFigur;
                    info.error += "[" + std::string(1, 'A' + x) + std::string(1, '1' + (7-y)) + "]";
					return info;
				}
            }
		}
    }
    return info;
}
bool Schach::checkIfKingIsCheckMate() {
    s_Schachbrett schachbrettBackup = schachbrett;
    bool rochadebackup[2][2] = { {rochade[0][0], rochade[0][1]}, {rochade[1][0], rochade[1][1]} };
    Koordinate enpassantBackup = enPassant;
    for(int x = 0; x < HAUPT_SCHACHBRETT_X; x++) {
        for(int y = 0; y < HAUPT_SCHACHBRETT_Y; y++) {
            if(schachbrett.hauptSchachbrett[x][y] != e_figuren::empty && getWhiteOrBlack(x, y) == !colorOnTurn) {
                for(int xdest = 0; xdest < HAUPT_SCHACHBRETT_X; xdest++) {
                    for(int ydest = 0; ydest < HAUPT_SCHACHBRETT_Y; ydest++) {
                        if(getWhiteOrBlack(xdest, ydest) == !colorOnTurn) continue; // eigene Figur

                        MoveInformation info;
                        switch(getPieceWithOutColor(schachbrett.hauptSchachbrett[x][y])) {
                        case e_figuren::whitePawn:
                            info = pawnCheck(x, y, xdest, ydest, !colorOnTurn, 0);
                            break;
                        case e_figuren::whiteTower:
                            info = towerCheck(x, y, xdest, ydest, 0);
                            break;
                        case e_figuren::whiteHorse:
                            info = horseCheck(x, y, xdest, ydest);
                            break;
                        case e_figuren::whiteBishop:
                            info = bishopCheck(x, y, xdest, ydest);
                            break;
                        case e_figuren::whiteQueen:
                            info = queenCheck(x, y, xdest, ydest);
                            break;
                        case e_figuren::whiteKing:
                            info = kingCheck(x, y, xdest, ydest);
                            break;
                        default:
                            info.error = "skip";
                            break;
                        }

                        if(info.error == "") {
                            schachbrett = schachbrettBackup;
                            for(int i = 0; i < 2; i++) {
                                for(int j = 0; j < 2; j++) {
                                    rochade[i][j] = rochadebackup[i][j];
                                }
                            }
                            enPassant = enpassantBackup;
                            if(schachbrett.hauptSchachbrett[xdest][ydest] != e_figuren::empty) {
                                stelleFigurAufFriedhof(xdest, ydest);
                            }
                            schachbrett.hauptSchachbrett[xdest][ydest] = schachbrett.hauptSchachbrett[x][y];
                            schachbrett.hauptSchachbrett[x][y] = e_figuren::empty;

                            colorOnTurn = !colorOnTurn;
                            MoveInformation attackInfo = checkIfKingIsAttacked(colorOnTurn);
                            colorOnTurn = !colorOnTurn;

                            schachbrett = schachbrettBackup;

                            if(attackInfo.error == "") {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}
void Schach::gegnerischerZug() {
    Serial.println("SUCHE ZUG");
    randomSeed(millis());
    int xStart = random(8);
    int yStart = random(8);
    int xDestStart = random(8);
    int yDestStart = random(8);
    for(int x = xStart; x < HAUPT_SCHACHBRETT_X + xStart; x++) {
        for(int y = yStart; y < HAUPT_SCHACHBRETT_Y + yStart; y++) {
            if(schachbrett.hauptSchachbrett[x % 8][y % 8] != e_figuren::empty) {
                for(int xdest = xDestStart; xdest < HAUPT_SCHACHBRETT_X + xDestStart; xdest++) {
                    for(int ydest = yDestStart; ydest < HAUPT_SCHACHBRETT_Y + yDestStart; ydest++) {
                        if(moveFigur(x % 8, y % 8, xdest % 8, ydest % 8) == "") {
                            return;
                        }
                    }
                }
            }
        }
    }
}