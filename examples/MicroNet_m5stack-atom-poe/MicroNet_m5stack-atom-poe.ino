// REQUIRED LIBRARIES
// M5Atom
// EthernetBonjour https://github.com/TrippyLighting/EthernetBonjour
// MicroOsc version 0.1.7 or higher https://github.com/thomasfredericks/MicroOsc
// MicroLog https://github.com/thomasfredericks/MicroLog

// This example requieres MicroOsc 
#include <MicroOscUdp.h>

// MicroLog is used for logging
#include <MicroLog.h>

#define MICROCONTROLLER_PORT 7000
#define COMPUTER_PORT 8000
#define COMPUTER_NAME "B1387-00"

#include <M5Atom.h>

CRGB embeddedPixel;

#include <MicroNetEthernet.h>
MicroNetEthernet myMicroNet;

EthernetUDP myUdp;

MicroOscUdp<1024> myMicroOsc(&myUdp);

unsigned long myChronoStart = 0;  // VARIABLE USED TO LIMIT THE SPEED OF THE SENDING OF OSC MESSAGES


void setupNetwork() {
    // Show red pixel as the device is getting ready to connect
  embeddedPixel = CRGB(255, 255, 0);
  FastLED.show();

  // CONFIGURE ATOM POE ETHERNET
  SPI.begin(22, 23, 33, 19);
  Ethernet.init(19);

  // Start NetworkManager (with Ethernet or WiFi as set by the include)
  LOG("Starting Network");

  // Get MAC address
  uint8_t mac[6];
  esp_efuse_mac_get_default(mac);
  
    // This will loop until the DHCP server is found
  myMicroNet.begin("esp-", mac, 3); // Start with "esp-" as a prefix, followed by the three last values of the MAC address

  // Resolve the IP of the COMPUTER_NAME
  LOG("Attempting to resolve remote name");
  IPAddress remoteIp = myMicroNet.resolveName(COMPUTER_NAME);

  LOG("Found", COMPUTER_NAME, "at", remoteIp);

  LOG("Setting OSC destination to", remoteIp, COMPUTER_PORT);
  myMicroOsc.setDestination(remoteIp, COMPUTER_PORT);

  embeddedPixel = CRGB(0, 255, 0);
  FastLED.show();

  LOG("Starting UDP");
  myUdp.begin(MICROCONTROLLER_PORT);

  LOG("Annoucing OSC UDP on port", MICROCONTROLLER_PORT);
  myMicroNet.announceUDPService("OSC", MICROCONTROLLER_PORT);
}


/********
  SETUP
*********/
void setup() {

  M5.begin(false, false, false);

  Serial.begin(115200);

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(&embeddedPixel, 1);

  // Animation de démarrage
  while (millis() < 5000) {
    embeddedPixel = CHSV((millis() / 5) % 255, 255, 255 - (millis() * 255 / 5000));
    FastLED.show();
    delay(50);
  }

  setupNetwork();

  LOG("Starting loop()");
}

/****************
  myOnOscMessageReceived is triggered when a message is received
*****************/
void myOnOscMessageReceived(MicroOscMessage& oscMessage) {

  // CHECK THE ADDRESS OF THE OSC MESSAGE
  if (oscMessage.checkOscAddress("/pixel")) {

    int red = oscMessage.nextAsInt();
    int green = oscMessage.nextAsInt();
    int blue = oscMessage.nextAsInt();
    embeddedPixel = CRGB(red, green, blue);
    FastLED.show();
  }
}

/*******
  LOOP
********/
void loop() {

  M5.update();

  myMicroNet.update();

  // TRIGGER myOnOscMessageReceived() IF AN OSC MESSAGE IS RECEIVED :
  myMicroOsc.onOscMessageReceived(myOnOscMessageReceived);

  // SEND OSC MESSAGES (EVERY 50 MILLISECONDS) :
  if (millis() - myChronoStart >= 50) {  // IF 50 MS HAVE ELLAPSED
    myChronoStart = millis();            // RESTART CHRONO

    myMicroOsc.sendInt("/millis", millis());

  }
}
