#ifndef __MICRO_NET_WIFI__
#define __MICRO_NET_WIFI__

#include <MicroNet.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include "esp_mac.h"  
// https://github.com/tzapu/WiFiManager
#include <WiFiManager.h>

#ifdef ESP32
class MicroNetWiFi : public MicroNetBase {
  WiFiManager wifiManager;

public:
  MicroNetWiFi() {
#ifdef LOG_ACTIVATED
    wifiManager.setDebugOutput(true);
#else
    wifiManager.setDebugOutput(false);
#endif
  }


protected:

  virtual void connect() {


    LOG("NetworkManager", "Starting WiFi Manager with name", _name);
    WiFi.mode(WIFI_STA);  // explicitly set mode, esp defaults to STA+AP

    //reset settings - wipe credentials for testing
    //wm.resetSettings();
    LOG("NetworkManager", "Trying to connect WiFi");
    wifiManager.setEnableConfigPortal(true);
    wifiManager.setConfigPortalBlocking(false);
    wifiManager.setCaptivePortalEnable(true);
    bool isConnected = wifiManager.autoConnect(_name);

    while ( isConnected == false ) isConnected = wifiManager.process();

    LOG("NetworkManager", "Connected to network" );
    LOG("NetworkManager", "Starting MDNS", _name, "with IP", WiFi.localIP() );
    MDNS.begin((const char*)_name);

  }
public:

  virtual IPAddress  resolveName(const char* hostName) {
    return MDNS.queryHost(hostName);
  }

   virtual IPAddress getIP() {
    return WiFi.localIP();
   }

  virtual void update() {
    wifiManager.process();
  }

  virtual void announceUDPService(const char *name, uint16_t port)  {
    MDNS.addService(name, "udp", port);
  }

  virtual void announceTCPService(const char *name, uint16_t port)  {
    MDNS.addService(name, "tcp", port);
  }

};

#endif


#endif