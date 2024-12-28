#include <MFRC522v2.h>
#include <SPI.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

// define pins
#define RST_PIN 21   // my reset pin for the mfrc522 reader
#define SS_PIN 5     // slave select (SS) pin for the reader

// xreate mfrc522 instance
MFRC522DriverPinSimple ss_pin(SS_PIN);   // slave Select pin
MFRC522DriverSPI mfrc522_driver(ss_pin);  // driver for the MFRC522
MFRC522 mfrc522(mfrc522_driver);          // create MFRC522 instance with the driver

void setup() {
  Serial.begin(115200);     // start serial communication (115200 baud)
  while (!Serial);          // waiting for serial to init
  
  SPI.begin();              // init SPI bus
  mfrc522.PCD_Init();       // init MFRC522 reader
  
  Serial.println(F("MFRC522 RFID Reader Initialized"));
}

void loop() {
  // wait and look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println(F("Card detected"));

    // log the UID of the card
    Serial.print(F("UID: "));
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();

    // halt the card and stop encryption
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }

  delay(100);  // small delay to avoid serial getting too much data 
}
