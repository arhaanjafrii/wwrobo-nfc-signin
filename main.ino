#include <WiFi.h>
#include <HTTPClient.h>
#include <MFRC522v2.h>
#include <SPI.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

// define pins
#define RST_PIN 21   
#define SS_PIN 5     

// wifi credentials
const char* ssid = "";
const char* password = "";

// web app URL
String googleScriptURL = "";

//  MFRC522 instance
MFRC522DriverPinSimple ss_pin(SS_PIN);   
MFRC522DriverSPI mfrc522_driver(ss_pin);
MFRC522 mfrc522(mfrc522_driver);

// mode: "register" or "attendance"
String mode = "attendance";

void setup() {
  Serial.begin(115200);
  while (!Serial);

  SPI.begin();
  mfrc522.PCD_Init();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  Serial.println(F("MFRC522 RFID Reader Initialized"));
  Serial.println("Type 'register' or 'attendance' to change mode.");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input == "register" || input == "attendance") {
      mode = input;
      Serial.println("Mode changed to: " + mode);
    }
  }

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String uid = getUID();

    if (mode == "register") {
      Serial.println("Card detected for registration. UID: " + uid);
      registerUser(uid);
    } else if (mode == "attendance") {
      Serial.println("Card detected for attendance. UID: " + uid);
      markAttendance(uid);
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }

  delay(100);
}

String getUID() {
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }
  return uid;
}

void registerUser(String uid) {
  Serial.println("Enter a name for this UID:");
  while (!Serial.available());
  String name = Serial.readStringUntil('\n');
  name.trim();
  
  String url = googleScriptURL + "?mode=register&uid=" + uid + "&name=" + name;
  sendRequest(url);
}

void markAttendance(String uid) {
  String url = googleScriptURL + "?mode=attendance&uid=" + uid;
  sendRequest(url);
}

void sendRequest(String url) {
  HTTPClient http;
  http.begin(url);
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    Serial.println("Response: " + http.getString());
  } else {
    Serial.println("Error code: " + String(httpResponseCode));
  }

  http.end();
}
