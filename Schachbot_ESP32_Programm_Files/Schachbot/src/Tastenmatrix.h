#pragma once
#include <Keypad.h>
#include "Common.h"
#define ROWS 4
#define COLS 4

class Tastenmatrix{
    public:
    Tastenmatrix();
    int getKey();
    void clearKeyMatrix();
    Koordinate getEingabe(uint8_t key);
    Koordinate getEingabeChar(uint8_t key);
    bool checkifBufferIsFull();
    char keys[ROWS][COLS] = {
        {'H','D','8','4'},
        {'G','C','7','3'},
        {'F','B','6','2'},
        {'E','A','5','1'}
    };
    private:
    byte rowPins[ROWS] = {27, 14, 12, 13};
    byte colPins[COLS] = {16, 17, 19, 21};

    Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

    char keyBuffer[4];
};