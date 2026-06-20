#ifndef WIFI_MANAGER_WRAPPER_H
#define WIFI_MANAGER_WRAPPER_H

#include <Arduino.h>

class WifiManagerWrapper
{
public:
    void begin();
    void handle();

    bool isConnected();
    String getIP();

private:
    bool initialized = false;
};

#endif