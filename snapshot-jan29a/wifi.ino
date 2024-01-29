#include <WiFi.h>

const char* ssid = "Amram";
const char* password = "danunaidum123";

WiFiServer server(80);

String header;
String email = "";

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

}