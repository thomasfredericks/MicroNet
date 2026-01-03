# MicroNet

MicroNet is a network connection manager library for **Arduino** and **PlatformIO**.

It provides:
- A **unified interface** for Wi-Fi or Ethernet (switch from one to the other by changing only a few lines of code).
- **DHCP client** support.
- **mDNS (Bonjour)** support.
- A **captive Wi-Fi configuration portal**.

## Tested hardware

### Microcontrollers

- ESP32

### Ethernet boards
- Wiznet Ethernet controller chips  
    - Tested with the [M5Stack Atom POE](https://docs.m5stack.com/en/atom/atom_poe)

## Tiny Examples

### Wi-Fi

```cpp
#include <Arduino.h>
#include <MicroNetWifi.h>
MicroNetWifi myMicroNet;
void setup(){
    myMicroNet.begin("myName");
}
void loop() {
   myMicroNet.update()
}
```

### Ethernet

```cpp
#include <Arduino.h>
#include <MicroNetEthernet.h>
MicroNetEthernet myMicroNet;
void setup(){
    SPI.begin(22, 23, 33, 19); // ATOM LITE POE
    Ethernet.init(19);  // ATOM LITE POE
    // DO NOT CALL Ethernet.begin(), call the following instead:
    myMicroNet.begin("myName");
}
void loop() {
   myMicroNet.update()
}
```

## Integration

### Global declarations

For **Ethernet**:
```cpp
#include <MicroNetEthernet.h>
MicroNetEthernet myMicroNet;
```

For **Wi-Fi**:
```cpp
#include <MicroNetWiFi.h>
MicroNetWiFi myMicroNet;
```

### Setup code


If you are using **Ethernet**, configure the hardware **before** starting MicroNet (do **NOT** call `Ethernet.begin()`).

Example configuration for an **Atom Lite POE**:

```cpp
SPI.begin(22, 23, 33, 19); // ATOM LITE POE
Ethernet.init(19);        // ATOM LITE POE
```

> [!WARNING]
> Do **NOT** call `Ethernet.begin()`. The following `myMicroNet.begin(name)` should be called instead!

Start MicroNet with `myMicroNet.begin()` - the method will **only** return once it gets an IP from the DHCP server:
```cpp
myMicroNet.begin(name);
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

The `myMicroNet.update()` method performs periodic network maintenance tasks and must be called as often as possible:
```cpp
    myMicroNet.update();
```

## Additional configuration

### Maximum name length

The maximum length of the device name (including the null terminator), that you should respect, is defined by:
```cpp
MICRO_NET_NAME_MAX_LENGTH
```

### Append part of the MAC address to a "prefix"

You can append part of the MAC address to an existing C string to generate a unique name:
```cpp
myMicroNet.appendMacToCString(prefix, destMaxSize, numBytes)
```
- `prefix` — Destination C string (`char*`) containing the name prefix.
- `destMaxSize` — Maximum size of the destination buffer (`size_t`).
- `numBytes` — Number of MAC address bytes (`uint8_t`) to append  (each byte adds two hexadecimal characters).

Example producing a name like `atom-932AE4`:
```cpp
  char myName[MICRO_NET_NAME_MAX_LENGTH] = "atom-"; // name prefix
  myMicroNet.appendMacToCString(myName, MICRO_NET_NAME_MAX_LENGTH, 3);
  // START MicroNet with the custom name
  myMicroNet.begin(myName);
```

### Getting the MAC address

Copies the device MAC address into the provided buffer:
```cpp
myMicroNet.copyMac(mac);
```
- `mac` — A 6‑byte array (`uint8_t[6]`) that will receive the MAC address.

### Get the IP address

Returns the current IP address of the device:
```cpp
IPAddress ip = myMicroNet.getIP();
```

## Additional mDNS (Bonjour) code

###  Resolving an mDNS hostname

The `resolveName()` method will resolve an mDNS host name to its IP. The method **only** returns if the host is found!
```cpp
IPAddress ip = myMicroNet.resolveName(hostName);
```
- `hostName` — C string (`const char*`) containing the mDNS hostname.

### Announce TCP or UDP service

Announce TCP service:
```cpp
myMicroNet.announceTCPService(serviceName, servicePort);
```
- `serviceName` — C string (`const char*`) with the service name.
- `servicePort` — TCP port number (`uint16_t`).

Announce UDP service:
```cpp
myMicroNet.announceUDPService(serviceName, servicePort);
```
- `serviceName` — C string (`const char*`) with the service name.
- `servicePort` — TCP port number (`uint16_t`).
