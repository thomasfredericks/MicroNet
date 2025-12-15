#include <Arduino.h>

#include <MicroNetEthernet.h>
MicroNetEthernet microNet;

// CONFIGURER LE NOM MDNS A RESOUDRE -----------------------------|
char nameToResolve[] = "m3-air"; // Do not append ".local"
// ---------------------------------------------------------------|

#define MICRO_LOG_OUTPUT Serial
#include <MicroLog.h>

void setup()
{
  
  MICRO_LOG_BEGIN(115200);
  delay(1000);
  
  MICRO_LOG("Starting");
  // CONFIGURER LE MATERIEL ETHERNET ----------------------------|
  SPI.begin(22, 23, 33, 19); // ATOM LITE POE
  Ethernet.init(19);  // ATOM LITE POE
  // ------------------------------------------------------------|

  // CRÉER UN NOM AVEC UN PREFIXE ET LE MAC EN SUFFIXE ----------| 
  char myName[MICRO_NET_NAME_MAX_LENGTH] = "atom-"; // name prefix
  microNet.appendMacToCString(myName, MICRO_NET_NAME_MAX_LENGTH, 3);
   // -----------------------------------------------------------|
  
  MICRO_LOG("Starting MicroNet with name:", myName);
  // DÉMARRER MICRO-NET (ETHERNET + DCHP IP + mDNS) -------------|
  microNet.begin(myName);
  // ------------------------------------------------------------|
  
  MICRO_LOG("MicroNet got IP:", microNet.getIP().toString() );
  
  MICRO_LOG("MicroNet resolving name:", nameToResolve  );
  // TROUVER IP D'UN NOM MDNS -----------------------------------|
  IPAddress ipCible = microNet.resolveName(nameToResolve);
  // ------------------------------------------------------------|
  MICRO_LOG("Resolved", nameToResolve , "to IP:", ipCible.toString() );

}

void loop()
{
  // MàJ MicroNet -----------------------------------------------|
  microNet.update();
  // ------------------------------------------------------------|

}
