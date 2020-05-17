/////////////////////////////////////////////////////////////////
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "Free_Fonts.h" // Include the header file attached to this sketch
#include "SPI.h"
#include "TFT_eSPI.h"
#include "Button2.h";

/////////////////////////////////////////////////////////////////

#define BUTTON_A_PIN  35
#define BUTTON_B_PIN  0

/////////////////////////////////////////////////////////////////

Button2 buttonA = Button2(BUTTON_A_PIN);
Button2 buttonB = Button2(BUTTON_B_PIN);
String TEXT = "ONE";

const char* ssid = "wifissid";
const char* password = "password";
WebServer server(80);
const int led = 13;

/////////////////////////////////////////////////////////////////

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", TEXT);
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

//display Use hardware SPI
TFT_eSPI tft = TFT_eSPI();
unsigned long drawTime = 0;

void setup() {
  Serial.begin(9600);
  delay(50);
  Serial.println("\n\nMultiple Buttons Demo");
  
  buttonA.setClickHandler(click);
  buttonB.setClickHandler(click);

  //display
  tft.begin();
  tft.setRotation(1);
  
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
//  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

/////////////////////////////////////////////////////////////////

void loop() {
  server.handleClient();
  
  buttonA.loop();
  buttonB.loop();
  Serial.println(TEXT);
  
//display
  tft.fillScreen(TFT_BLACK);
  tft.setFreeFont(FF24);
  tft.drawString(TEXT, 80, 43, GFXFF);
//  delay(1000);
  
}

/////////////////////////////////////////////////////////////////

void click(Button2& btn) {
    if (btn == buttonA) {
      TEXT = "ONE";
      Serial.println("A clicked");
    } else if (btn == buttonB) {
      TEXT = "TWO";
      Serial.println("B clicked");
    }
}

/////////////////////////////////////////////////////////////////
