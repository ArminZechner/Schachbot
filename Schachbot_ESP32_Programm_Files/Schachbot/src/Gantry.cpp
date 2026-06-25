#include "Gantry.h"

AccelStepper stepperX(AccelStepper::DRIVER, 25, 26);
AccelStepper stepperY(AccelStepper::DRIVER, 32, 33);

enum DIR_X_OR_Y{
    x,
    y
};

Gantry::Gantry(){

}
void Gantry::init(){
    position.x = -1;
    position.y = -1;
    pinMode(pinSpule, OUTPUT);
    digitalWrite(pinSpule, LOW);

    pinMode(pinButtonX, INPUT);
    pinMode(pinButtonY, INPUT);

    stepperX.setMaxSpeed(20000.0);
    stepperX.setAcceleration(5000.0);
    stepperX.setCurrentPosition(0);

    stepperY.setMaxSpeed(20000.0);
    stepperY.setAcceleration(5000.0);
    stepperY.setCurrentPosition(0);
    stepperX.disableOutputs();
    stepperY.disableOutputs();
    bewegungsAblauf.isWorking = false;
    bewegungsAblauf.moveToZero = false;
    bewegungsAblauf.ablaufState = ABLAUF::schritt0;
    for (int x = 0; x < FRIEDHOF_X; x++) {
        for (int y = 0; y < FRIEDHOF_Y; y++) {
            ist_schachbrettFriedhof[DIRECTION::left][x][y] = e_figuren::empty;
            ist_schachbrettFriedhof[DIRECTION::right][x][y] = e_figuren::empty;
        }
    }
    for (int x = 0; x < HAUPT_SCHACHBRETT_X; x++) {
        for (int y = 0; y < HAUPT_SCHACHBRETT_Y; y++) {
            ist_schachbrett[x][y] = init_Liste[y][x];
        }
    }
    for (int x = 0; x < FRIEDHOF_X; x++) {
        for (int y = 0; y < FRIEDHOF_Y; y++) {
            soll_schachbrettFriedhof[DIRECTION::left][x][y] = e_figuren::empty;
            soll_schachbrettFriedhof[DIRECTION::right][x][y] = e_figuren::empty;
        }
    }
    for (int x = 0; x < HAUPT_SCHACHBRETT_X; x++) {
        for (int y = 0; y < HAUPT_SCHACHBRETT_Y; y++) {
            soll_schachbrett[x][y] = init_Liste[y][x];
        }
    }

if (Serial.availableForWrite()) {
    Serial.println("Gantry startet");
}
}
void Gantry::setChessBoard(char soll_schachbrett[SCHACHBRETT_SIZE_Y][SCHACHBRETT_SIZE_Y],
                           char soll_schachbrettFriedhof[2][SCHACHBRETT_SCHLAGBREITE][SCHACHBRETT_SIZE_Y]){
    memcpy(this->soll_schachbrett, soll_schachbrett, sizeof(this->soll_schachbrett));
    memcpy(this->soll_schachbrettFriedhof, soll_schachbrettFriedhof, sizeof(this->soll_schachbrettFriedhof));
}
int8_t Gantry::getField(Koordinate pos)
{
    if(pos.x >= 2 && pos.x < 10){
        return ist_schachbrett[pos.x - 2][pos.y];
    }

    if(pos.x < 2){
        return ist_schachbrettFriedhof[0][pos.x][pos.y];
    }

    return ist_schachbrettFriedhof[1][pos.x - 10][pos.y];
}

void Gantry::setField(Koordinate pos, int8_t figur)
{
    if(pos.x >= 2 && pos.x < 10){
        ist_schachbrett[pos.x - 2][pos.y] = figur;
        return;
    }

    if(pos.x < 2){
        ist_schachbrettFriedhof[0][pos.x][pos.y] = figur;
        return;
    }

    ist_schachbrettFriedhof[1][pos.x - 10][pos.y] = figur;
}
void Gantry::updateChessBoard()
{
    Koordinate start = bewegungsAblauf.start;
    Koordinate ende  = bewegungsAblauf.ende;
    position = ende;
    start.y = 7 - start.y;
    ende.y  = 7 - ende.y;
    int8_t figur = getField(start);
    setField(start, e_figuren::empty);
    setField(ende, figur);
}
void Gantry::moveToZero(){
    stepperX.enableOutputs();
    stepperY.enableOutputs();
    stepperX.setMaxSpeed(90000.0);
    stepperX.setAcceleration(10000.0);

    stepperY.setMaxSpeed(40000.0);
    stepperY.setAcceleration(40000.0);
    bewegungsAblauf.moveToZero = true;
    bewegungsAblauf.isWorking = true;
    bewegungsAblauf.ablaufState = 100;
    stepperX.moveTo(100000);
    stepperY.moveTo(100000);
}
bool Gantry::isAtZero(){
    if(digitalRead(pinButtonX) == HIGH && digitalRead(pinButtonY) == HIGH){
        return true;
    }
    return false;
}
void Gantry::move(){
    stepperX.run();
    stepperY.run();
    if(bewegungsAblauf.moveToZero == true && bewegungsAblauf.isWorking == true){
        if(digitalRead(pinButtonX) == HIGH){
            stepperX.stop();
            stepperX.moveTo(0);
            stepperX.setCurrentPosition(0);
        }
        if(digitalRead(pinButtonY) == HIGH){
            stepperY.stop();
            stepperY.moveTo(0);
            stepperY.setCurrentPosition(0);
        }
        if(digitalRead(pinButtonX) == HIGH && digitalRead(pinButtonY) == HIGH){
            bewegungsAblauf.isWorking = false;
            bewegungsAblauf.moveToZero = false;
            position.x = 11;
            position.y = 0;
            Serial.println("Is at Zero");
            stepperX.disableOutputs();
            stepperY.disableOutputs();
        }
        return;
    }
    if(bewegungsAblauf.startgoToStart_x == true){
        stepperX.setMaxSpeed(40000.0);
        stepperX.setAcceleration(40000.0);
        stepperX.move(bewegungsAblauf.goToStart_x);
        bewegungsAblauf.startgoToStart_x = false;
        digitalWrite(pinSpule, LOW);
    }
    if(bewegungsAblauf.startgoToStart_y == true){
    stepperY.setMaxSpeed(40000.0);
    stepperY.setAcceleration(40000.0);
        stepperY.move(bewegungsAblauf.goToStart_y);
        bewegungsAblauf.startgoToStart_y = false;
        digitalWrite(pinSpule, LOW);
    }
    if((abs(stepperX.distanceToGo()) < 1 && abs(stepperY.distanceToGo()) < 1 && bewegungsAblauf.isWorking == true)){
        switch(bewegungsAblauf.ablaufState){
            case ABLAUF::schritt0:
                stepperX.setMaxSpeed(20000.0);
                stepperX.setAcceleration(20000.0);

                stepperY.setMaxSpeed(20000.0);
                stepperY.setAcceleration(20000.0);
                digitalWrite(pinSpule, HIGH);
                bewegungsAblauf.XOrY_Schritt0 == x ? stepperX.move(bewegungsAblauf.schritt0) : stepperY.move(-bewegungsAblauf.schritt0);
                bewegungsAblauf.ablaufState = schritt1;
            break;

            case ABLAUF::schritt1:
                bewegungsAblauf.XOrY_Schritt1 == x ? stepperX.move(bewegungsAblauf.schritt1) : stepperY.move(-bewegungsAblauf.schritt1);
                bewegungsAblauf.ablaufState = schritt2;
            break;

            case ABLAUF::schritt2:
                bewegungsAblauf.XOrY_Schritt2 == x ? stepperX.move(bewegungsAblauf.schritt2) : stepperY.move(-bewegungsAblauf.schritt2);
                bewegungsAblauf.ablaufState = schritt3;
            break;

            case ABLAUF::schritt3:
                bewegungsAblauf.XOrY_Schritt3 == x ? stepperX.move(bewegungsAblauf.schritt3) : stepperY.move(-bewegungsAblauf.schritt3);
                bewegungsAblauf.ablaufState = finish;
            break;

            case ABLAUF::finish:
                digitalWrite(pinSpule, LOW);
                if(!stepperX.isRunning() && !stepperY.isRunning()){
                    updateChessBoard();
                    bewegungsAblauf.isWorking = false;
                }
                stepperX.disableOutputs();
                stepperY.disableOutputs();
            break;
        }
    }
}
// Ein zur Dame umgewandelter Bauer ist physisch immer noch derselbe
// Spielstein wie ein normaler Bauer (es gibt keine extra Dame-Figur).
// Fuer die Gantry zaehlt nur der physische Stein, deshalb werden
// pawnQueen und pawn beim Suchen gleich behandelt.
static int8_t normalizeFigur(int8_t figur)
{
    if(figur == e_figuren::whitePawnQueen){
        return e_figuren::whitePawn;
    }
    if(figur == e_figuren::blackPawnQueen){
        return e_figuren::blackPawn;
    }
    return figur;
}
Koordinate Gantry::searchField(int8_t soll_figur, int8_t ist_figur, Koordinate ausschlussFeld)
{
    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){

            if(
                normalizeFigur(soll_schachbrett[x][y]) != normalizeFigur(ist_schachbrett[x][y]) &&
                normalizeFigur(soll_schachbrett[x][y]) == normalizeFigur(soll_figur) &&
                normalizeFigur(ist_schachbrett[x][y]) == normalizeFigur(ist_figur) &&
                !(ausschlussFeld.x == (x + 2) &&
                  ausschlussFeld.y == y)
            ){
                return {x + 2, y};
            }
        }
    }

    return {-1, -1};
}
Koordinate Gantry::searchFriedhofField(int8_t soll_figur, int8_t ist_figur)
{
    for(int seite = 0; seite < 2; seite++){
        for(int x = 0; x < FRIEDHOF_X; x++){
            for(int y = 0; y < FRIEDHOF_Y; y++){

                if(
                    normalizeFigur(soll_schachbrettFriedhof[seite][x][y]) !=
                    normalizeFigur(ist_schachbrettFriedhof[seite][x][y]) &&

                    normalizeFigur(soll_schachbrettFriedhof[seite][x][y]) == normalizeFigur(soll_figur) &&
                    normalizeFigur(ist_schachbrettFriedhof[seite][x][y]) == normalizeFigur(ist_figur)
                ){
                    if(seite == 0){
                        return {x, y};
                    }
                    else{
                        return {10 + x, y};
                    }
                }
            }
        }
    }

    return {-1, -1};
}
void Gantry::searchAvailableMove()
{
    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){

            // pawnQueen (soll) und pawn (ist) gelten als derselbe Stein,
            // damit eine fertige Umwandlung nicht erneut "korrigiert" wird.
            if(normalizeFigur(soll_schachbrett[x][y]) == normalizeFigur(ist_schachbrett[x][y])){
                continue;
            }

            // ==========================================================
            // Auf diesem Feld steht eine Figur, die hier nicht hingehört
            //   - soll == empty  -> Figur muss nur weg (normaler Zug)
            //   - soll != empty  -> Figur muss geschlagen werden:
            //     sie kommt zuerst auf den Friedhof, damit die
            //     schlagende Figur danach auf dieses Feld ziehen kann
            // ==========================================================
            if(ist_schachbrett[x][y] != e_figuren::empty)
            {
                Koordinate start = {x + 2, y};

                // Gibt es ein Zielfeld auf dem Brett?
                Koordinate ziel =
                    searchField(
                        ist_schachbrett[x][y],
                        e_figuren::empty,
                        start
                    );

                if(ziel.x != -1)
                {
                    // Normaler Zug
                    start.y = 7 - start.y;
                    ziel.y  = 7 - ziel.y;

                    goTo(start, ziel);
                    return;
                }

                // Keine Zielposition gefunden -> Friedhof
                ziel =
                    searchFriedhofField(
                        ist_schachbrett[x][y],
                        e_figuren::empty
                    );

                if(ziel.x != -1)
                {
                    start.y = 7 - start.y;
                    ziel.y  = 7 - ziel.y;

                    goTo(start, ziel);
                    return;
                }
            }

            // =====================================
            // Figur erscheint auf diesem Feld
            // =====================================
            else if(ist_schachbrett[x][y] == e_figuren::empty &&
                    soll_schachbrett[x][y] != e_figuren::empty)
            {
                Koordinate ziel = {x + 2, y};

                Koordinate start =
                    searchField(
                        e_figuren::empty,
                        soll_schachbrett[x][y],
                        ziel
                    );

                if(start.x != -1)
                {
                    start.y = 7 - start.y;
                    ziel.y  = 7 - ziel.y;

                    goTo(start, ziel);
                    return;
                }
            }
        }
    }
}
void Gantry::goTo(Koordinate start, Koordinate ende){
    Serial.printf("START: [%d][%d] ENDE: [%d][%d]\n", start.x, start.y, ende.x, ende.y);
    if(position.x == -1 || position.y == -1){
        return;
    }
    if(start.x == ende.x && start.y == ende.y){
        //Serial.println("Start == Ende");
        return;
    }
    if(start.x == -1 || start.y == -1 || ende.x == -1 || ende.y == -1){
        return;
    }
    int xDir = ende.x - start.x;
    int yDir = ende.y - start.y;
    bewegungsAblauf.start = start;
    bewegungsAblauf.ende = ende;
    bewegungsAblauf.ablaufState = ABLAUF::schritt0;

    bewegungsAblauf.XOrY_Schritt0 = x;
    bewegungsAblauf.XOrY_Schritt1 = x;
    bewegungsAblauf.XOrY_Schritt2 = x;
    bewegungsAblauf.XOrY_Schritt3 = x;

    bewegungsAblauf.schritt0 = 0;
    bewegungsAblauf.schritt1 = 0;
    bewegungsAblauf.schritt2 = 0;
    bewegungsAblauf.schritt3 = 0;
    //GOTOSTART
    bewegungsAblauf.goToStart_x = (start.x - position.x)*FIELD_STEPS_X;
    bewegungsAblauf.goToStart_y = -(start.y - position.y)*FIELD_STEPS_Y;
    bewegungsAblauf.startgoToStart_x = true;
    bewegungsAblauf.startgoToStart_y = true;
    //
    if(xDir < 0 && yDir > 0){

    bewegungsAblauf.XOrY_Schritt0 = x;
    bewegungsAblauf.schritt0 = -HALF_FIELD_STEPS_X;

    bewegungsAblauf.XOrY_Schritt1 = y;
    bewegungsAblauf.schritt1 = yDir * FIELD_STEPS_Y - HALF_FIELD_STEPS_Y;

    bewegungsAblauf.XOrY_Schritt2 = x;
    bewegungsAblauf.schritt2 = xDir * FIELD_STEPS_X + HALF_FIELD_STEPS_X;

    bewegungsAblauf.XOrY_Schritt3 = y;
    bewegungsAblauf.schritt3 = HALF_FIELD_STEPS_Y;
    }
    else if(xDir == 0 && yDir > 0){

    bewegungsAblauf.XOrY_Schritt0 = x;
    bewegungsAblauf.schritt0 = -HALF_FIELD_STEPS_X;

    bewegungsAblauf.XOrY_Schritt1 = y;
    bewegungsAblauf.schritt1 = yDir * FIELD_STEPS_Y;

    bewegungsAblauf.XOrY_Schritt2 = x;
    bewegungsAblauf.schritt2 = HALF_FIELD_STEPS_X;
}
else if(xDir > 0 && yDir > 0){

    bewegungsAblauf.XOrY_Schritt0 = x;
    bewegungsAblauf.schritt0 = HALF_FIELD_STEPS_X;

    bewegungsAblauf.XOrY_Schritt1 = y;
    bewegungsAblauf.schritt1 = yDir * FIELD_STEPS_Y - HALF_FIELD_STEPS_Y;

    bewegungsAblauf.XOrY_Schritt2 = x;
    bewegungsAblauf.schritt2 = xDir * FIELD_STEPS_X - HALF_FIELD_STEPS_X;

    bewegungsAblauf.XOrY_Schritt3 = y;
    bewegungsAblauf.schritt3 = HALF_FIELD_STEPS_Y;
}
else if(xDir < 0 && yDir == 0){

    bewegungsAblauf.XOrY_Schritt0 = y;
    if(start.y == 0){
        bewegungsAblauf.schritt0 = HALF_FIELD_STEPS_Y;
    }
    else{
        bewegungsAblauf.schritt0 = -HALF_FIELD_STEPS_Y;
    }

    bewegungsAblauf.XOrY_Schritt1 = x;
    bewegungsAblauf.schritt1 = xDir * FIELD_STEPS_X;

    bewegungsAblauf.XOrY_Schritt2 = y;
    if(start.y == 0){
        bewegungsAblauf.schritt2 = -HALF_FIELD_STEPS_Y;
    }
    else{
        bewegungsAblauf.schritt2 = HALF_FIELD_STEPS_Y;
    }
}
else if(xDir > 0 && yDir == 0){

    bewegungsAblauf.XOrY_Schritt0 = y;
    if(start.y == 0){
        bewegungsAblauf.schritt0 = HALF_FIELD_STEPS_Y;
    }
    else{
        bewegungsAblauf.schritt0 = -HALF_FIELD_STEPS_Y;
    }

    bewegungsAblauf.XOrY_Schritt1 = x;
    bewegungsAblauf.schritt1 = xDir * FIELD_STEPS_X;

    bewegungsAblauf.XOrY_Schritt2 = y;
    if(start.y == 0){
        bewegungsAblauf.schritt2 = -HALF_FIELD_STEPS_Y;
    }
    else{
        bewegungsAblauf.schritt2 = HALF_FIELD_STEPS_Y;
    }
}
else if(xDir < 0 && yDir < 0){
bewegungsAblauf.XOrY_Schritt0 = x;
bewegungsAblauf.schritt0 = -HALF_FIELD_STEPS_X;

bewegungsAblauf.XOrY_Schritt1 = y;
bewegungsAblauf.schritt1 = yDir * FIELD_STEPS_Y + HALF_FIELD_STEPS_Y;

bewegungsAblauf.XOrY_Schritt2 = x;
bewegungsAblauf.schritt2 = xDir * FIELD_STEPS_X + HALF_FIELD_STEPS_X;

bewegungsAblauf.XOrY_Schritt3 = y;
bewegungsAblauf.schritt3 = -HALF_FIELD_STEPS_Y;
}
else if(xDir == 0 && yDir < 0){

    bewegungsAblauf.XOrY_Schritt0 = x;
    bewegungsAblauf.schritt0 = HALF_FIELD_STEPS_X;

    bewegungsAblauf.XOrY_Schritt1 = y;
    bewegungsAblauf.schritt1 = yDir * FIELD_STEPS_Y;

    bewegungsAblauf.XOrY_Schritt2 = x;
    bewegungsAblauf.schritt2 = -HALF_FIELD_STEPS_X;
}
else if(xDir > 0 && yDir < 0){

    bewegungsAblauf.XOrY_Schritt0 = x;
    bewegungsAblauf.schritt0 = HALF_FIELD_STEPS_X;

    bewegungsAblauf.XOrY_Schritt1 = y;
    bewegungsAblauf.schritt1 = yDir * FIELD_STEPS_Y + HALF_FIELD_STEPS_Y;

    bewegungsAblauf.XOrY_Schritt2 = x;
    bewegungsAblauf.schritt2 = xDir * FIELD_STEPS_X - HALF_FIELD_STEPS_X;

    bewegungsAblauf.XOrY_Schritt3 = y;
    bewegungsAblauf.schritt3 = -HALF_FIELD_STEPS_Y;
}

    bewegungsAblauf.isWorking = true;
    stepperX.enableOutputs();
    stepperY.enableOutputs();
    return;
}