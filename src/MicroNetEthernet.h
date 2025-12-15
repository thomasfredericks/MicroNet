#ifndef __MICRO_NET_ETHERNET__
#define __MICRO_NET_ETHERNET__

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetBonjour.h>
#include <MicroNet.h>

// ---------------------------------------------------
bool _MicroNetEthernetReturned = false;
IPAddress _MicroNetEthernetIp = INADDR_NONE;

void _MicroNetEthernetCallback(const char *name, const byte ipAddr[4])
{
  if (NULL != ipAddr)
  {
    _MicroNetEthernetIp = IPAddress(ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);
  }
  else
  {
    _MicroNetEthernetIp = INADDR_NONE;
  }
  _MicroNetEthernetReturned = true;
}

// ---------------------------------------------------
class MicroNetEthernet : public MicroNetBase
{

protected:
  virtual void connect()
  {

    // START ETHERNET LOOKING FOR DHCP
    Ethernet.begin(mac_);

    while (Ethernet.localIP() == INADDR_NONE)
    {

      shortDelay();
      Ethernet.begin(mac_);
    }

    EthernetBonjour.begin(name_);

    update();
  }

public:
  virtual IPAddress getIP()
  {
    return Ethernet.localIP();
  }

    virtual void update()
  {
    EthernetBonjour.run();
  }


  virtual IPAddress resolveName(const char *hostName)
  {
    EthernetBonjour.setNameResolvedCallback(_MicroNetEthernetCallback);

    while (true)
    {
      _MicroNetEthernetIp = INADDR_NONE;

      EthernetBonjour.resolveName(hostName, 1000);

      while (_MicroNetEthernetReturned == false)
      {
        update();
        //yield();
      }

      _MicroNetEthernetReturned = false;
      if (_MicroNetEthernetIp == INADDR_NONE)
      {
        shortDelay();
      }
      else
      {
        break;
      }
    }

    return _MicroNetEthernetIp;
  }


  virtual void announceUDPService(const char *serviceName, uint16_t servicePort)
  {
    EthernetBonjour.addServiceRecord(serviceName, servicePort, MDNSServiceUDP);
  }
  virtual void announceTCPService(const char *serviceName, uint16_t servicePort)
  {
    EthernetBonjour.addServiceRecord(serviceName, servicePort, MDNSServiceTCP);
  }
};

#endif