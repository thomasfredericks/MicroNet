
#ifndef __MICRO_NET__
#define __MICRO_NET__


#define MICRO_NET_MDNS_NAME_MAX_LENGTH 64

#ifndef __MICRO_LOG__
  #define LOG(...)
#endif


class MicroNetBase {
private:
  
protected:
  char _name[MICRO_NET_MDNS_NAME_MAX_LENGTH]; 
  uint8_t _mac[6];
  virtual void connect()=0;
  void shortDelay() {
    randomSeed(micros()+analogRead(0)+analogRead(1));
    delay(1000 + random(1000));
  }
public:

  void begin( char * prefix, uint8_t (&mac)[6], uint8_t appendMacValues=0) {
 	
	for (int i = 0; i < 6; i++) {
            _mac[i] = mac[i];  
        }
   
    appendMacValues = constrain(appendMacValues,0,6);
    size_t nameLength = min(strlen(prefix), (size_t)(MICRO_NET_MDNS_NAME_MAX_LENGTH - 1 - (appendMacValues*2)) );
    // copy the prefix
    strncpy(_name, prefix, nameLength);
    char * appendPointer = _name + nameLength;
    for ( int i =0 ; i < appendMacValues; i++ ) {
      sprintf(appendPointer , "%2x", mac[i+(6-appendMacValues)] );
      appendPointer += 2;
    }
	LOG("MicroNet", "mDNS Name:", _name);
    connect();
  }

  virtual IPAddress resolveName(const char* hostName) = 0;
  bool ipIsValid(IPAddress ip) {
    return ip != INADDR_NONE;
  }
  virtual void update() = 0;
  const char * getName() {
    return _name;
  }

  virtual void announceUDPService(const char *name, uint16_t port) = 0;
  virtual void announceTCPService(const char *name, uint16_t port) = 0;
};

#endif


