
#ifndef __MICRO_NET__
#define __MICRO_NET__

#define MICRO_NET_MDNS_NAME_MAX_LENGTH 64


class MicroNetBase
{
private:
protected:
  char _name[MICRO_NET_MDNS_NAME_MAX_LENGTH];
  uint8_t _mac[6];
  virtual void connect() = 0;
  void shortDelay()
  {
    randomSeed(micros() + analogRead(0) + analogRead(1));
    delay(1000 + random(1000));
  }

public:
  void begin(char *name)
  {

    strcpy(_name, name);

   // LOG("MicroNet", "mDNS Name:", _name);
    connect();
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
    return _name;
  }

  virtual void announceUDPService(const char *name, uint16_t port) = 0;
  virtual void announceTCPService(const char *name, uint16_t port) = 0;
};

#endif
