// Snapshot Jan31b

/*
  This code is for the Amram Board project. It includes functionality for displaying time on an 8x16 LED matrix, adjusting the time using two switches, fetching an email address from a server, and connecting to a WiFi network.

  TODO List:
  X TOMRWW: ADD THE PASSWORD FETCH MECHANISM

  Legend:
  / : Done
  ? : Done, but not tested
  O : In progress
  X : Not done
  - : N/A
*/

// --- INITIALIZE --- //

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <TimeLib.h>
#include <RTClib.h>

#include <WiFi.h>
#include <WebServer.h>
#include <time.h>
#include <AutoConnect.h>

//#include <Firebase_ESP_Client.h>
//#include <addons/TokenHelper/TokenHelper.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

#include "amrled.h"

Adafruit_8x16minimatrix matrix = Adafruit_8x16minimatrix(); // Matrix

const int sw1 = 16;
const int sw2 = 14;
byte cnt = 0;
byte mde = 0;

time_t t = now();
int h1 = hour(t) / 10;
int h2 = hour(t) % 10;
int m1 = minute(t) / 10;
int m2 = minute(t) % 10;

long pressTime = 0;

// --- FIREBASE INIT --- //

#define SSID "Amramconnect"
#define PASSWORD "danunaidum123"
#define API_KEY "AIzaSyAN8iGMfpIZEnS5mxrW5uRfMqWXpeAuSSc"
#define FIREBASE_PROJECT_ID "amramdotclick"
#define USER_EMAIL emailFetch()
#define USER_PASSWORD passFetch()

//FirebaseData fbdo;
//FirebaseAuth auth;
//FirebaseConfig config;

bool taskCompleted = false;
unsigned long dataMillis = 0;

// --- DRAWING FUNCTIONS --- //

void drawGlyph(const byte* glyph, int startDraw, bool clear = false) {
  if (clear) {
    matrix.clear();
  }

  for (int row = 0; row < 8; ++row) {
    byte glyphByte = pgm_read_byte_near(glyph + row);
    for (int col = 0; col < 3; ++col) {
      if (bitRead(glyphByte, col) == 1) {
        matrix.drawPixel(row, (col + 13 - startDraw), LED_ON);
      }
    }
  }

  matrix.writeDisplay();
}

const byte* chars[10] = {char0, char1, char2, char3, char4, char5, char6, char7, char8, char9};

void drawDigit(int digit, int startDraw, bool clear = false) {
  if ((0 <= digit) && (digit <= 9)) {
    const byte* charId = chars[digit];
    drawGlyph(charId, startDraw, clear);
  } else {
    matrix.clear();
  }
}


// --- TIME FUNCTIONS --- //

string addZeros(string s, int length) {
  while (s.length() < length) {
    s = "0" + s;
  }
  return s;
}


void resetTime(bool doReset = false) {
  if (doReset) {
    setTime(8,0,0,1,1,2024);
  }
}

// --- SPECIAL INIT FOR WIFI --- //

#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Amramconnect";
const char* password = "danunaidum123";

WiFiServer server(80);

String header;
//String email = "";
//String pass = "";

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

// --- SETUP --- //

void setup() {
  Serial.begin(115200);
  resetTime(false); // Default should be FALSE
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Amramconnect Services");
  while ((WiFi.status() != WL_CONNECTED) && (millis() - currentTime < 60000)) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected to Amramconnect. IP: ");
  Serial.print(WiFi.localIP());

  //Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  //config.api_key = API_KEY;
  //auth.user.email = USER_EMAIL;
  //auth.user.password = USER_PASSWORD;

  //config.token_status_callback = tokenStatusCallback;
  //Firebase.reconnectNetwork(true);
  //fbdo.setBSSLBufferSize(4096, 1024);
  //fbdo.setResponseSize(1024);

  //Firebase.begin(&config, &auth);
  server.begin();
}

int val1 = digitalRead(sw1);
int val2 = digitalRead(sw2);

// --- TIME CHANGE MECHANISM --- //

void changeTime() {
  // Setup
  bool changing = false;
  unsigned long blinkTime = millis();

  // Hour
  while (true) {
    int val1 = digitalRead(sw1);
    int val2 = digitalRead(sw2);
    t = now();
    h1 = hour(t) / 10;
    h2 = hour(t) % 10;
    m1 = minute(t) / 10;
    m2 = minute(t) % 10;

    drawDigit(h1, 0, true);
    drawDigit(h2, 4);
    
    if (val1 == LOW) {
      adjustTime(3600);
      delay(200);
    } else if (val2 == LOW) {
      while (val2 == LOW) { 
        val2 = digitalRead(sw2);
      };
      break;
    }
  }

  // Minute
  while (true) {
    int val1 = digitalRead(sw1);
    int val2 = digitalRead(sw2);

    t = now();
    h1 = hour(t) / 10;
    h2 = hour(t) % 10;
    m1 = minute(t) / 10;
    m2 = minute(t) % 10;

    drawDigit(m1, 9, true);
    drawDigit(m2, 13);

    if (val1 == LOW) {
      adjustTime(60);
      delay(200); 
    } else if (val2 == LOW) {
      break;
    }
  }
}

/*

// --- EMAIL FETCH --- //

string emailFetch() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Connected to Amramconnect Client")
    String currentLine = "";
    currentTime = millis();
    previousTime = currentTime;

    while (client.connected() && currentTime - previousTime <= 300000) {
      currentTime = millis();
      if (client.available()) {

        if (header.indexOf("GET /email") >= 0) {
          HTTPClient http;
          http.begin("https://amram.click/data/email.html");
          int httpCode = http.GET();

          if (httpCode > 0) {
            String payloadhttp.getString();
            email = payload;
            Serial.println(email);
            return email;
          } else {
            Serial.println("Error on HTTP request");
          }

      http.end();
    }
  }
}}}

string passFetch() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Connected to Amramconnect Client")
    String currentLine = "";
    currentTime = millis();
    previousTime = currentTime;

    while (client.connected() && currentTime - previousTime <= 300000) {
      currentTime = millis();
      if (client.available()) {

        if (header.indexOf("GET /pass") >= 0) {
          HTTPClient http;
          http.begin("https://amram.click/data/password.html");
          int httpCode = http.GET();

          if (httpCode > 0) {
            String payloadhttp.getString();
            password = payload;
            Serial.println(password);
            return password;
          } else {
            Serial.println("Error on HTTP request");
          }

      http.end();
    }
  }
}}
}

*/

// --- FIREBASE --- //

std::string firebaseLoop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Connected to Amramconnect Client");
    String currentLine = "";
    currentTime = millis();
    previousTime = currentTime;

    while (client.connected() && currentTime - previousTime <= 300000) {
      currentTime = millis();
      if (client.available()) {

        if (header.indexOf("GET /data") >= 0) {
          HTTPClient http;
          http.begin("https://amram.click/data/getmeds.html");
          int httpCode = http.GET();

          if (httpCode > 0) {
            String payload = http.getString();
            String dat = payload;
            std::string DATA = dat.c_str();
            Serial.println(dat);
            return DATA;
          } else {
            Serial.println("Error on HTTP request");
          }

      http.end();
    }
  }
}}
}

// --- MEDICINE NOTIFICATION --- //

std::vector<std::string> groups(384);
std::vector<std::string> temp(384);
std::vector<std::vector<std::string>> MEDDATA(32, std::vector<std::string>(12));

std::vector<std::string> split(const std::string& str, char separator) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = str.find(separator);

    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(separator, start);
    }

    result.push_back(str.substr(start, end));
    return result;
}

void printData() {
  for (const auto &innerVec : MEDDATA) {
    for (const auto &str : innerVec) {
        Serial.print(str.c_str());
        Serial.print(" ");
    }
    Serial.println();
  }
}

void getData() {
  groups = split(firebaseLoop(), ';');
  temp.clear();

  for (int i = 0; i < groups.size(); i++) {
    split(groups[i], ',');
    MEDDATA[i] = temp;
    temp.clear();
  }

  printData();
}

// --- MAIN LOOP --- //

void loop() { 
  t = now();
  h1 = hour(t) / 10;
  h2 = hour(t) % 10;
  m1 = minute(t) / 10;
  m2 = minute(t) % 10;

  int val1 = digitalRead(sw1);
  int val2 = digitalRead(sw2);

  long presstime = 0;

  drawDigit(h1, 0, true); // Should only be true for this one
  drawDigit(h2, 4);
  drawDigit(m1, 9);
  drawDigit(m2, 13);
  
  static unsigned long pressTime = 0;
  if (val1 == LOW && val2 == LOW) {
    if (pressTime == 0) {
      pressTime = millis();
    } else if (millis() - pressTime >= 1000) {
      // Wait until both switches are released
      while (digitalRead(sw1) == LOW || digitalRead(sw2) == LOW) {
        // Do nothing, just wait
      }
      changeTime();
      pressTime = 0;
    }
  } else {
    pressTime = 0;
  }

  getData();

  delay(200);
}