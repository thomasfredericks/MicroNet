/*
# TODO
- Add license information
- Resolve name in MicroNetEthernet needs to not block forever
- Remove logging from library code
*/



#ifndef __MICRO_NET__
#define __MICRO_NET__

#define MICRO_NET_NAME_MAX_LENGTH 32

#include <Arduino.h>
#include <esp_mac.h>

class MicroNetBase
{
private:
protected:
  char name_[MICRO_NET_NAME_MAX_LENGTH];
  uint8_t mac_[6];
  virtual void connect() = 0;
  bool gotMac_ = false;
 //bool _logOutputSet = false;
 // Stream * logOutput_ = nullptr;

  virtual void readMac()
  {
    esp_efuse_mac_get_default(mac_);
    gotMac_ = true;
  }

  void shortDelay()
  {
    delay(250 + random(30));
  }

public:
  void begin(const char *name)
  {

    strcpy(name_, name);
    esp_efuse_mac_get_default(mac_);

    // LOG("MicroNet", "mDNS Name:", name_);
    connect();
  }
   
  // Appends the last `numBytes` of a MAC address as hex to a C-string buffer
  void appendMacToCString(
      char *dest,
      size_t destMaxSize,
      uint8_t numBytes)
  {
    if (!gotMac_)
    {
      readMac();
    }

    size_t len = strlen(dest);

    // Each byte = 2 hex chars
    size_t needed = numBytes * 2;

    if (len + needed + 1 > destMaxSize)
    {
      // Not enough space, abort safely
      return;
    }

    for (uint8_t i = 0; i < numBytes; i++)
    {
      uint8_t b = mac_[6 - numBytes + i];
      snprintf(dest + len, destMaxSize - len, "%02X", b);
      len += 2;
    }
  }

  void copyMac(uint8_t mac[6])
  {
    if (!gotMac_)
    {
      readMac();
    }
    memcpy(mac, mac_, 6);
  }
  virtual IPAddress getIP() = 0;
  virtual IPAddress resolveName(const char *hostName) = 0;
  bool ipIsValid(IPAddress ip)
  {
    return ip != INADDR_NONE;
  }
  virtual void update() = 0;
  const char *getName()
  {
    return name_;
  }

  virtual void announceUDPService(const char *name, uint16_t port) = 0;
  virtual void announceTCPService(const char *name, uint16_t port) = 0;
};

#endif
