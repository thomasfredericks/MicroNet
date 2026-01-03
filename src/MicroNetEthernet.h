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
public:
  enum class Configuration
  {
    UNDEFINED,
    ATOM_POE_WITH_ATOM_LITE,
    ATOM_POE_WITH_ATOMS3
  };

private:
  Configuration configuration_;

public:
  MicroNetEthernet() : configuration_(Configuration::UNDEFINED)
  {
  }

  MicroNetEthernet(Configuration configuration)
  {
    configuration_ = configuration;
  }

protected:
  void customReadMac(uint8_t mac_[6]) override
  {
    esp_efuse_mac_get_default(mac_);
    // First byte changed to 0x02 to indicate a link-local address */
    mac_[0] = 0x02;
  }

  void customBegin() override
  {

    switch (configuration_)
    {
    case Configuration::ATOM_POE_WITH_ATOM_LITE:
      SPI.begin(22, 23, 33, 19); // ATOM LITE POE
      Ethernet.init(19);         // ATOM LITE POE
      break;
    case Configuration::ATOM_POE_WITH_ATOMS3:
      SPI.begin(5, 7, 8, 6); // ATOM S3 POE
      Ethernet.init(6);      // ATOM S3 POE
      break;
    }

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
        // yield();
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