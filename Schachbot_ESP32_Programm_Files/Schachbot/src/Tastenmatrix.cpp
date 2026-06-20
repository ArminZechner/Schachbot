#include "Tastenmatrix.h"

Tastenmatrix::Tastenmatrix(){
    kpd.setDebounceTime(50);
    clearKeyMatrix();
    pinMode(16, INPUT_PULLDOWN);
    pinMode(17, INPUT_PULLDOWN);
    pinMode(19, INPUT_PULLDOWN);
    pinMode(21, INPUT_PULLDOWN);
}

int Tastenmatrix::getKey(){
  char lastKey = '\0';
  bool change = false;
  kpd.getKeys();
  //GetKey
  for (int i=0; i<LIST_MAX; i++){
        if ( kpd.key[i].stateChanged ){
                if (kpd.key[i].kstate == PRESSED) {
                    lastKey = kpd.key[i].kchar;
                    Serial.printf("KEY: %c\n", lastKey);
                    change = true;
                }
            }
    }

    if(lastKey != '\0'){
      //Keys aus buffer loeschen
      if(keyBuffer[3] != '\0'){
        return change;
      }
      //Key hinzufuegen
      for(int i = 0; i < 4; i++){
        if(keyBuffer[i] == '\0'){
            if(((i == 0 || i == 2) && (lastKey >= 'A' && lastKey <= 'H')) ||
               ((i == 1 || i == 3) && (lastKey >= '0' && lastKey <= '8'))){
                keyBuffer[i] = lastKey;
            }
          break;
        }
      }
      //Keys printen
      for(int i = 0; i < 4; i++){
          Serial.printf("%c ", keyBuffer[i]);
        }
        Serial.println();
      }
      return change;
}

void Tastenmatrix::clearKeyMatrix(){
    for(int i = 0; i < 4; i++){
        keyBuffer[i] = '\0';
    }
}
Koordinate Tastenmatrix::getEingabe(uint8_t key){
    if(key > 1){
        return {-1, -1};
    }
    if(key == 0){
    return {(int8_t)(keyBuffer[0]-'A'), (int8_t)(keyBuffer[1] -'0')-(int8_t)1};
    }
    else{
    return {(int8_t)(keyBuffer[2]-'A'), (int8_t)(keyBuffer[3] -'0')-(int8_t)1};
    }
}
    Koordinate Tastenmatrix::getEingabeChar(uint8_t key){
           if(key > 1){
        return {-1, -1};
    }
    if(key == 0){
    return {(int8_t)(keyBuffer[0]), (int8_t)(keyBuffer[1])};
    }
    else{
    return {(int8_t)(keyBuffer[2]), (int8_t)(keyBuffer[3])};
    } 
    }
bool Tastenmatrix::checkifBufferIsFull(){
    if(keyBuffer[2] != '\0' && keyBuffer[3] != '\0'){
        return true;
    }
    return false;
}