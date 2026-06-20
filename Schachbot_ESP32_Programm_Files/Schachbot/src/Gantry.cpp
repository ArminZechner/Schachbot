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
    position.x = 0;
    position.y = 0;
    pinMode(pinSpule, OUTPUT);
    digitalWrite(pinSpule, LOW);

    pinMode(pinButtonX, INPUT);
    pinMode(pinButtonY, INPUT);

    stepperX.setMaxSpeed(20000.0);
    stepperX.setAcceleration(20000.0);
    stepperX.setCurrentPosition(0);

    stepperY.setMaxSpeed(20000.0);
    stepperY.setAcceleration(20000.0);
    stepperY.setCurrentPosition(0);

    bewegungsAblauf.isWorking = false;
    bewegungsAblauf.moveToZero = false;
    bewegungsAblauf.ablaufState = ABLAUF::schritt0;

if (Serial.availableForWrite()) {
    Serial.println("Gantry startet");
}
}
void Gantry::setChessBoard(char soll_schachbrett[SCHACHBRETT_SIZE_Y][SCHACHBRETT_SIZE_Y],
                           char soll_schachbrettFriedhof[2][SCHACHBRETT_SCHLAGBREITE][SCHACHBRETT_SIZE_Y]){
    memcpy(this->soll_schachbrett, soll_schachbrett, sizeof(this->soll_schachbrett));
    memcpy(this->soll_schachbrettFriedhof, soll_schachbrettFriedhof, sizeof(this->soll_schachbrettFriedhof));
}
void Gantry::moveToZero(){
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
        }
        return;
    }

    if(abs(stepperX.distanceToGo()) < 1 && abs(stepperY.distanceToGo()) < 1 && bewegungsAblauf.isWorking == true){
        switch(bewegungsAblauf.ablaufState){
            case ABLAUF::goToStart_x:
                digitalWrite(pinSpule, HIGH);
                bewegungsAblauf.XOrY_goToStart_x == x ? stepperX.move(bewegungsAblauf.goToStart_x) : stepperY.move(-bewegungsAblauf.goToStart_x);
                bewegungsAblauf.ablaufState = goToStart_y;
            break;

            case ABLAUF::goToStart_y:
                bewegungsAblauf.XOrY_goToStart_y == x ? stepperX.move(bewegungsAblauf.goToStart_y) : stepperY.move(-bewegungsAblauf.goToStart_y);
                bewegungsAblauf.ablaufState = schritt0;
            break;

            case ABLAUF::schritt0:
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
                digitalWrite(pinSpule, LOW);
                bewegungsAblauf.XOrY_Schritt3 == x ? stepperX.move(bewegungsAblauf.schritt3) : stepperY.move(-bewegungsAblauf.schritt3);
                bewegungsAblauf.ablaufState = finish;
            break;

            case ABLAUF::finish:
                if(!stepperX.isRunning() && !stepperY.isRunning()){
                    bewegungsAblauf.isWorking = false;
                }
            break;
        }
    }
}
Koordinate Gantry::searchField(int8_t soll_figur, int8_t ist_figur){
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            if(soll_schachbrett[x][y] == soll_figur && ist_schachbrett[x][y] == ist_figur){
                return (Koordinate){x + 2, y};
            }
        }
    }
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 2; x++){
            if(soll_schachbrettFriedhof[0][x][y] == soll_figur && ist_schachbrettFriedhof[0][x][y] == ist_figur){
                return (Koordinate){x, y};
            }
            if(soll_schachbrettFriedhof[1][x][y] == soll_figur && ist_schachbrettFriedhof[1][x][y] == ist_figur){
                return (Koordinate){x + 10, y};
            }
        }
    }
}
void Gantry::searchAvailableMove(){
    //HAUPTSCHACHBRETT
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            if(soll_schachbrett[x][y] != ist_schachbrett[x][y]){
                if(soll_schachbrett[x][y] == e_figuren::empty){
                    goTo(searchField(soll_schachbrett[x][y], ist_schachbrett[x][y]), (Koordinate){x+2, y});
                    return;
                }
                if(ist_schachbrett[x][y] == e_figuren::empty){
                    goTo((Koordinate){x+2, y}, searchField(soll_schachbrett[x][y], ist_schachbrett[x][y]));
                    return;
                }
            }
        }
    //FRIEDHOF
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 2; x++){
            if(soll_schachbrettFriedhof[0][x][y] != ist_schachbrettFriedhof[0][x][y]){
                if(soll_schachbrettFriedhof[0][x][y] == e_figuren::empty){
                    goTo(searchField(soll_schachbrettFriedhof[0][x][y], ist_schachbrettFriedhof[0][x][y]), (Koordinate){x, y});
                    return;
                }
                if(ist_schachbrettFriedhof[0][x][y] == e_figuren::empty){
                    goTo((Koordinate){x, y}, searchField(soll_schachbrettFriedhof[0][x][y], ist_schachbrettFriedhof[0][x][y]));
                    return;
                }
            }
            else if(soll_schachbrettFriedhof[1][x][y] != ist_schachbrettFriedhof[1][x][y]){
                if(soll_schachbrettFriedhof[1][x][y] == e_figuren::empty){
                    goTo(searchField(soll_schachbrettFriedhof[1][x][y], ist_schachbrettFriedhof[1][x][y]), (Koordinate){x+10, y});
                    return;
                }
                if(ist_schachbrettFriedhof[1][x][y] == e_figuren::empty){
                    goTo((Koordinate){x+10, y}, searchField(soll_schachbrettFriedhof[1][x][y], ist_schachbrettFriedhof[1][x][y]));
                    return;
                }
            }
        }
    }
}
}
void Gantry::goTo(Koordinate start, Koordinate ende){
    if(position.x == -1 || position.y == -1){
        return;
    }
    int xDir = ende.x - start.x;
    int yDir = ende.y - start.y;

    bewegungsAblauf.ablaufState = ABLAUF::goToStart_x;

    bewegungsAblauf.XOrY_Schritt0 = x;
    bewegungsAblauf.XOrY_Schritt1 = x;
    bewegungsAblauf.XOrY_Schritt2 = x;
    bewegungsAblauf.XOrY_Schritt3 = x;

    bewegungsAblauf.schritt0 = 0;
    bewegungsAblauf.schritt1 = 0;
    bewegungsAblauf.schritt2 = 0;
    bewegungsAblauf.schritt3 = 0;
    //GOTOSTART
    bewegungsAblauf.XOrY_goToStart_x = x;
    bewegungsAblauf.XOrY_goToStart_y = y;

    bewegungsAblauf.goToStart_x = (start.x - position.x)*FIELD_STEPS;
    bewegungsAblauf.goToStart_y = (start.y - position.y)*FIELD_STEPS;
    //
    if(xDir < 0 && yDir > 0){

        bewegungsAblauf.XOrY_Schritt0 = x;
        bewegungsAblauf.schritt0 = -HALF_FIELD_STEPS;

        bewegungsAblauf.XOrY_Schritt1 = y;
        bewegungsAblauf.schritt1 = yDir * FIELD_STEPS - HALF_FIELD_STEPS;

        bewegungsAblauf.XOrY_Schritt2 = x;
        bewegungsAblauf.schritt2 = xDir * FIELD_STEPS + HALF_FIELD_STEPS;

        bewegungsAblauf.XOrY_Schritt3 = y;
        bewegungsAblauf.schritt3 = HALF_FIELD_STEPS;
    }
    else if(xDir == 0 && yDir > 0){

        bewegungsAblauf.XOrY_Schritt0 = x;
        bewegungsAblauf.schritt0 = -HALF_FIELD_STEPS;

        bewegungsAblauf.XOrY_Schritt1 = y;
        bewegungsAblauf.schritt1 = yDir * FIELD_STEPS;

        bewegungsAblauf.XOrY_Schritt2 = x;
        bewegungsAblauf.schritt2 = HALF_FIELD_STEPS;
    }
    else if(xDir > 0 && yDir > 0){

        bewegungsAblauf.XOrY_Schritt0 = x;
        bewegungsAblauf.schritt0 = HALF_FIELD_STEPS;

        bewegungsAblauf.XOrY_Schritt1 = y;
        bewegungsAblauf.schritt1 = yDir * FIELD_STEPS - HALF_FIELD_STEPS;

        bewegungsAblauf.XOrY_Schritt2 = x;
        bewegungsAblauf.schritt2 = xDir * FIELD_STEPS - HALF_FIELD_STEPS;

        bewegungsAblauf.XOrY_Schritt3 = y;
        bewegungsAblauf.schritt3 = HALF_FIELD_STEPS;
    }
    else if(xDir < 0 && yDir == 0){

        bewegungsAblauf.XOrY_Schritt0 = y;
        bewegungsAblauf.schritt0 = -HALF_FIELD_STEPS;

        bewegungsAblauf.XOrY_Schritt1 = x;
        bewegungsAblauf.schritt1 = xDir * FIELD_STEPS;

        bewegungsAblauf.XOrY_Schritt2 = y;
        bewegungsAblauf.schritt2 = HALF_FIELD_STEPS;
    }
    else if(xDir > 0 && yDir == 0){

        bewegungsAblauf.XOrY_Schritt0 = y;
        bewegungsAblauf.schritt0 = -HALF_FIELD_STEPS;

        bewegungsAblauf.XOrY_Schritt1 = x;
        bewegungsAblauf.schritt1 = xDir * FIELD_STEPS;

        bewegungsAblauf.XOrY_Schritt2 = y;
        bewegungsAblauf.schritt2 = HALF_FIELD_STEPS;
    }
    else if(xDir < 0 && yDir < 0){

        bewegungsAblauf.XOrY_Schritt0 = x;
        bewegungsAblauf.schritt0 = HALF_FIELD_STEPS;

        bewegungsAblauf.XOrY_Schritt1 = y;
        bewegungsAblauf.schritt1 = yDir * FIELD_STEPS + HALF_FIELD_STEPS;

        bewegungsAblauf.XOrY_Schritt2 = x;
        bewegungsAblauf.schritt2 = xDir * FIELD_STEPS + HALF_FIELD_STEPS;

        bewegungsAblauf.XOrY_Schritt3 = y;
        bewegungsAblauf.schritt3 = -HALF_FIELD_STEPS;
    }
    else if(xDir == 0 && yDir < 0){

        bewegungsAblauf.XOrY_Schritt0 = x;
        bewegungsAblauf.schritt0 = HALF_FIELD_STEPS;

        bewegungsAblauf.XOrY_Schritt1 = y;
        bewegungsAblauf.schritt1 = yDir * FIELD_STEPS;

        bewegungsAblauf.XOrY_Schritt2 = x;
        bewegungsAblauf.schritt2 = -HALF_FIELD_STEPS;
    }
    else if(xDir > 0 && yDir < 0){

        bewegungsAblauf.XOrY_Schritt0 = x;
        bewegungsAblauf.schritt0 = HALF_FIELD_STEPS;

        bewegungsAblauf.XOrY_Schritt1 = y;
        bewegungsAblauf.schritt1 = yDir * FIELD_STEPS + HALF_FIELD_STEPS;

        bewegungsAblauf.XOrY_Schritt2 = x;
        bewegungsAblauf.schritt2 = xDir * FIELD_STEPS - HALF_FIELD_STEPS;

        bewegungsAblauf.XOrY_Schritt3 = y;
        bewegungsAblauf.schritt3 = -HALF_FIELD_STEPS;
    }

    bewegungsAblauf.isWorking = true;
}