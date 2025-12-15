# MicroNet

MicroNet is a network connection manager library for **Arduino** and **PlatformIO**.

It provides:
- A **unified interface** for Wi-Fi or Ethernet (switch from one to the other by changing only a few lines of code).
- **DHCP client** support.
- **mDNS (Bonjour)** support.
- A **captive Wi-Fi configuration portal**.

## Supported hardware

- ESP32
- Wiznet Ethernet controller chips  
  Tested with the [M5Stack Atom POE](https://docs.m5stack.com/en/atom/atom_poe)

## Examples

### Wi-Fi

```cpp
#include <Arduino.h>
#include <MicroNetWifi.h>
MicroNetWifi microNet;
void setup(){
    microNet.begin("myName");
}
void loop() {
   microNet.update()
}
```

### Ethernet

```cpp
#include <Arduino.h>
#include <MicroNetEthernet.h>
MicroNetEthernet microNet;
void setup(){
    SPI.begin(22, 23, 33, 19); // ATOM LITE POE
    Ethernet.init(19);  // ATOM LITE POE
    microNet.begin("myName");
}
void loop() {
   microNet.update()
}
```

## Integration

### Global declarations

For **Ethernet** :
```cpp
#include <MicroNetEthernet.h>
MicroNetEthernet microNet;
```

For **Wi-Fi** :
```cpp
#include <MicroNetWiFi.h>
MicroNetWiFi microNet;
```

### Setup code


If you are using **Ethernet**, configure the hardware **before** starting MicroNet (do **not** call `Ethernet.begin()`).

Example configuration for an **Atom Lite POE**:

```cpp
SPI.begin(22, 23, 33, 19); // ATOM LITE POE
Ethernet.init(19);        // ATOM LITE POE
```

Start MicroNet with `microNet.begin()` - the method **only** returns once it gets an IP from the DHCP server :
```cpp
microNet.begin(name);
```
* `name` — C string (`const char*`) used as the device and mDNS name.

#### What happens when begin is called

- For **Wi‑Fi**:
    - MicroNet attempts to connect to the configured access point.
    - If it fails, a captive configuration portal is opened using the provided name.
    - Connect to this portal to enter Wi‑Fi credentials.
- For **Wi‑Fi** and **Ethernet**:
    - Once connected to the network, MicroNet requests an IP address via DHCP.
    - After an IP is obtained, the device registers its mDNS name.

### Loop integration

The `microNet.update()` method performs periodic network maintenance tasks and must be called as often as possible:
```cpp
    microNet.update();
```

## Additional configuration

### Maximum name length

The maximum length of the device name (including the null terminator), that you should respect, is defined by :
```cpp
MICRO_NET_NAME_MAX_LENGTH
```

### Append part of the MAC address to a "prefix"

You can append part of the MAC address to an existing C string to generate a unique name:
```cpp
microNet.appendMacToCString(prefix, destMaxSize, numBytes)
```
- `prefix` — Destination C string (`char*`) containing the name prefix.
- `destMaxSize` — Maximum size of the destination buffer (`size_t`).
- `numBytes` — Number of MAC address bytes (`uint8_t`) to append  (each byte adds two hexadecimal characters).

Example producing a name like `atom-932AE4`:
```cpp
  char myName[MICRO_NET_NAME_MAX_LENGTH] = "atom-"; // name prefix
  microNet.appendMacToCString(myName, MICRO_NET_NAME_MAX_LENGTH, 3);
  // START MicroNet with the custom name
  microNet.begin(myName);
```

### Getting the MAC address

Copies the device MAC address into the provided buffer :
```cpp
microNet.copyMac(mac);
```
- `mac` — A 6‑byte array (`uint8_t mac[6]`) that will receive the MAC address.

### Get the IP address

Returns the current IP address of the device :
```cpp
IPAddress ip = microNet.getIP();
```

## Additional mDNS (Bonjour) code

###  Resolving an mDNS hostname

The `resolveName()` method will resolve an mDNS host name to its IP. The method **only** returns if the host is found!
```cpp
IPAddress ip = microNet.resolveName(hostName);
```
- `hostName` — C string (`const char*`) containing the mDNS hostname.

### Announce TCP or UDP service

Announce TCP service :
```cpp
microNet.announceTCPService(serviceName, servicePort);
```
- `serviceName` — C string (`const char*`) with the service name.
- `servicePort` — TCP port number (`uint16_t`).

Announce UDP service :
```cpp
microNet.announceUDPService(serviceName, servicePort);
```
- `serviceName` — C string (`const char*`) with the service name.
- `servicePort` — TCP port number (`uint16_t`).
