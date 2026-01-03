#ifndef __MICRO_NET_WIFI__
#define __MICRO_NET_WIFI__

#include <MicroNet.h>
#include <WiFi.h>
#include <ESPmDNS.h>
//#include "esp_mac.h"  // For MAC address querying on ESP32
// https://github.com/tzapu/WiFiManager
#include <WiFiManager.h>

#ifdef ESP32
class MicroNetWiFi : public MicroNetBase {
  WiFiManager wifiManager_;

public:
  MicroNetWiFi() {
#ifdef LOG_ACTIVATED
    wifiManager_.setDebugOutput(true);
#else
    wifiManager_.setDebugOutput(false);
#endif
  }


protected:
  void customReadMac(uint8_t mac_[6]) override {
    esp_efuse_mac_get_default(mac_);
  }

  void customBegin() override {


    //LOG("NetworkManager", "Starting WiFi Manager with name", name_);
    WiFi.mode(WIFI_STA);  // explicitly set mode, esp defaults to STA+AP

    //reset settings - wipe credentials for testing
    //wm.resetSettings();
    //LOG("NetworkManager", "Trying to connect WiFi");
    wifiManager_.setEnableConfigPortal(true);
    wifiManager_.setConfigPortalBlocking(false);
    wifiManager_.setCaptivePortalEnable(true);
    bool isConnected = wifiManager_.autoConnect(name_);

    while ( isConnected == false ) isConnected = wifiManager_.process();

    MDNS.begin((const char*)name_);

  }
public:

  virtual IPAddress  resolveName(const char* hostName) {
    return MDNS.queryHost(hostName, 1000);
  }

   virtual IPAddress getIP() {
    return WiFi.localIP();
   }

  virtual void update() {
    wifiManager_.process();
  }

  virtual void announceUDPService(const char *name, uint16_t port)  {
    MDNS.addService(name, "udp", port);
  }

  virtual void announceTCPService(const char *name, uint16_t port)  {
    MDNS.addService(name, "tcp", port);
  }

  void resetWiFiCredentials() {
    wifiManager_.resetSettings();
  }

};

#endif


#endif