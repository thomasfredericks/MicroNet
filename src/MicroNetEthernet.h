#ifndef __MICRO_NET_ETHERNET__
#define __MICRO_NET_ETHERNET__

/*

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetBonjour.h>
#include <MicroNet.h>


// ---------------------------------------------------
bool _MicroNetEthernetReturned = false;
IPAddress _MicroNetEthernetIp = INADDR_NONE;

void _MicroNetEthernetCallback(const char* name, const byte ipAddr[4]) {
  if (NULL != ipAddr) {
    _MicroNetEthernetIp = IPAddress(ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);
  } else {
    _MicroNetEthernetIp = INADDR_NONE;
  }
  _MicroNetEthernetReturned = true;
}


// ---------------------------------------------------
class MicroNetEthernet : public MicroNetBase {

protected:

  virtual void connect() {

    // START ETHERNET LOOKING FOR DHCP
    Ethernet.begin(_mac);

    while (Ethernet.localIP() == INADDR_NONE) {
      LOG("MicroNet", "Did not find DHCP server!");
      shortDelay();
      Ethernet.begin(_mac);
    }

    EthernetBonjour.begin(_name);

    update();
  }

public:

  virtual IPAddress resolveName(const char* hostName) {
    EthernetBonjour.setNameResolvedCallback(_MicroNetEthernetCallback);
    
    
    while ( true ) {
      _MicroNetEthernetIp = INADDR_NONE;
      LOG("MicroNetEthernet", "resolving", hostName);
      EthernetBonjour.resolveName(hostName, 5000);

      while ( _MicroNetEthernetReturned == false ) {
        EthernetBonjour.run();
        yield();
      }

      _MicroNetEthernetReturned = false;
      if ( _MicroNetEthernetIp == INADDR_NONE ) {
        LOG("MicroNetEthernet","FAILED");
        shortDelay();
      } else {
        LOG("MicroNetEthernet","FOUND");
        break;
      }
       
    }

    return _MicroNetEthernetIp;
  }

  virtual void update() {
    EthernetBonjour.run();
  }


  virtual void announceUDPService(const char *serviceName, uint16_t servicePort ) {
    EthernetBonjour.addServiceRecord(serviceName, servicePort, MDNSServiceUDP);
  }
  virtual void announceTCPService(const char *serviceName,  uint16_t servicePort ) {
    EthernetBonjour.addServiceRecord(serviceName, servicePort, MDNSServiceTCP);
  }


};
*/


#endif