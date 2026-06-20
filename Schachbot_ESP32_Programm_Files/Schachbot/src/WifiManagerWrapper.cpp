#include "WifiManagerWrapper.h"
#include <WiFiManager.h>
#include <WiFi.h>

// interne WiFiManager Instanz
static WiFiManager wm;

void WifiManagerWrapper::begin()
{
    //Serial.println("[WIFI] WiFiManager start");

    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);

    // optional: timeout fürs Portal (verhindert "hängende Config")
    wm.setConfigPortalTimeout(120);

    // AutoConnect:
    // - verbindet sich mit gespeicherten Daten
    // - sonst startet AP "Schachbrett-Setup"
    bool ok = wm.autoConnect("Schachbrett-Setup");

    if (!ok)
    {
        //Serial.println("[WIFI] AutoConnect fehlgeschlagen");
    }
    else
    {
        //Serial.println("[WIFI] verbunden!");
        Serial.println(WiFi.localIP());
    }

    initialized = true;
}

void WifiManagerWrapper::handle()
{
    // WiFiManager läuft intern event-basiert
    // hier nur Status-Check / optional Erweiterung

    if (!initialized)
        return;

    if (WiFi.status() == WL_CONNECTED)
    {
        // optional debug
    }
}

bool WifiManagerWrapper::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

String WifiManagerWrapper::getIP()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return WiFi.localIP().toString();
    }
    return "0.0.0.0";
}