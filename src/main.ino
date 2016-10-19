#include <Arduino.h>
#include <FS.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Hash.h>

#include <ArduinoJson.h>
#include <Servo.h>

#include "config.h"



const char* WEMOS_PIN[] = {
  "D3",       // GPIO 0
  "TX",       // GPIO 1
  "D4",       // GPIO 2
  "RX",       // GPIO 3
  "D2",       // GPIO 4
  "D1",       // GPIO 5

  "GPIO6",    // GPIO 6
  "GPIO7",    // GPIO 7
  "GPIO8",    // GPIO 8
  "GPIO9",    // GPIO 9
  "GPIO10",   // GPIO 10
  "GPIO11",   // GPIO 11

  "D6",       // GPIO 12
  "D7",       // GPIO 13
  "D5",       // GPIO 14
  "D8",       // GPIO 15
  "D0",       // GPIO 16

  "A0"        // GPIO 17
};



// SCL GPIO5 (D2)
// SDA GPIO4 (D1)
Adafruit_SSD1306 display;


#define IS_DEBUG_MODE         0
#define HANDLING_PER_SECOND  50


byte mac[6];
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
uint8_t wsClientNum;

int numberOfHandlingThisSecond = 0;
unsigned long nextResetTime;


const int SETUP_BUTTON_PIN = A0;
#define APP_MODE_NORMAL 0
#define APP_MODE_SETUP  1
int appMode = APP_MODE_NORMAL;

const int SENSOR_PIN     = A0;
int sensorValue          = 0;
const int NUM_OF_SENSORS = 0;

const int SERVO_PINS[]   = {D6, D7, D8};
int servoPositions[]     = {90, 90, 90};
const int NUM_OF_SERVOS  = sizeof(servoPositions)/sizeof(int);
Servo servos[NUM_OF_SERVOS];




void initState() {
  Serial.println("");
  Serial.println("Init state :");
  Serial.println("------------");

  if (NUM_OF_SENSORS) {
    Serial.println(String("Sensor 0 on ") + WEMOS_PIN[SENSOR_PIN] + " -> OK");
  }

  for (int i = 0; i < NUM_OF_SERVOS; i++) {
    servos[i].attach(SERVO_PINS[i]);
    Serial.println("Servo " + String(i) + " on " + WEMOS_PIN[SERVO_PINS[i]] + " -> OK");
  }
}



void applyState() {
  for (int i = 0; i < NUM_OF_SERVOS; i++) {
    servos[i].write(servoPositions[i]);
  }
}



void updateState(JsonObject& json) {
  if (sizeof(json["data"]) > 0) {
    int jsonIteration = 0;

    for (int i = 0; i < NUM_OF_SERVOS; i++) {
      servoPositions[i] = json["data"][jsonIteration++];
    }
  }
}



void sendState() {
  String response = "{\"data\": [";
  int element = 0;

  for (int i = 0; i < NUM_OF_SERVOS; i++) {
    if (element++ > 0) { response += ","; }
    response += String(servoPositions[i]);
  }

  if (NUM_OF_SENSORS) {
    if (element++ > 0) { response += ","; }
    sensorValue = analogRead(SENSOR_PIN);
    response += String(sensorValue);
  }

  response += "]}";

  webSocket.broadcastTXT(response);
}





/***************************\
 * NEW CONNECTION PIN
\***************************/
String newConnectionPin;

void generateNewPin() {
  newConnectionPin = String(random(1000, 9999));
}





/***************************\
 * PAGES
\***************************/
void sendFile(String filename, String typeMime) {
  File f = SPIFFS.open(filename, "r");

  if (!f) {
    Serial.println("file open failed");
    handleNotFound();
    return;
  }

  String html = f.readString();

  server.send(200, typeMime, html);
}


/**
 * Homepage
 */
void handleRootPage() {
  Serial.println(server.uri());

  String pin = server.arg("pin");
  if (pin == newConnectionPin) {
    webSocket.disconnect(wsClientNum);
    generateNewPin();
    updateInfoDisplay();

    String filepath;

    switch (appMode) {
      case APP_MODE_NORMAL:
        filepath = "/index.html";
        break;

      case APP_MODE_SETUP:
        filepath = "/setup.html";
        break;
    }

    sendFile(filepath, "text/html");
  } else {
    handleNotFound();
  }
}


/**
 * style.css
 */
void handleStyleCss() {
  Serial.println(server.uri());
  sendFile("/style.css", "text/css");
}


/**
 * app.js
 */
void handleAppJs() {
  Serial.println(server.uri());
  sendFile("/app.js", "application/json");
}


/**
 * actions.js
 */
void handleActionsJs() {
  Serial.println(server.uri());
  sendFile("/actions.js", "application/json");
}


/**
 * Page not found
 */
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}



/***************************\
 * WEBSOCKET EVENT LISTENER
\***************************/
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
  numberOfHandlingThisSecond++;

  switch(type) {
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;

    case WStype_CONNECTED:
      {
        wsClientNum = num;
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

				// send message to client
				// webSocket.sendTXT(num, "Connected");
        sendState();
      }
      break;

    case WStype_TEXT:
      {
        if (numberOfHandlingThisSecond < HANDLING_PER_SECOND) {
          if (IS_DEBUG_MODE) { Serial.printf("[%u] get Text: %s\n", num, payload); }

          StaticJsonBuffer<200> jsonBuffer;
          JsonObject& jsonObject = jsonBuffer.parseObject((char*)payload);

          if (!jsonObject.success()) {
            Serial.println("parseObject() failed");
            delay(250);
            break;
          }

          updateState(jsonObject);
          applyState();
          sendState();
        }
      }
      break;
  }
}





/***************************\
 * TOOLS
\***************************/
const int ITERATION_PER_SECOND = 50;
const int MILLISECOND_PER_ITERATION = 1000 / ITERATION_PER_SECOND;


/**
 * @param int pin       GPIO
 * @param int duration  In milliseconds
 */
void fadeIn(int pin, int duration) {
  int stepPerIteration = 255 * MILLISECOND_PER_ITERATION / duration;

  for (int i = 0; i < 255; i += stepPerIteration) {
    analogWrite(pin, i);
    delay(MILLISECOND_PER_ITERATION);
  }
}


/**
 * @param int pin       GPIO
 * @param int duration  In milliseconds
 */
void fadeOut(int pin, int duration) {
  int stepPerIteration = 255 * MILLISECOND_PER_ITERATION / duration;

  for (int i = 255; i > 0; i -= stepPerIteration) {
    analogWrite(pin, i);
    delay(MILLISECOND_PER_ITERATION);
  }
}


/**
 * @param int pin       GPIO
 * @param int duration  In milliseconds
 */
void fadeInAndOut(int pin, int duration) {
    duration /= 2;
    fadeIn(pin, duration);
    fadeOut(pin, duration);
}




void displayLoaderOnce(int duration) {
  int stepPerIteration = 360 * MILLISECOND_PER_ITERATION / duration;
  int i;
  float angle;

  for (i = 0; i < 360; i += stepPerIteration) {
    angle = i * 6.28 / 360;
    display.clearDisplay();
    display.drawPixel(32 + cos(angle)*16, 24 + sin(angle)*16, WHITE);
    display.drawPixel(32 + cos(angle +1.256)*16, 24 + sin(angle +1.256)*16, WHITE);
    display.drawPixel(32 + cos(angle -1.256)*16, 24 + sin(angle -1.256)*16, WHITE);
    display.drawPixel(32 + cos(angle +2.512)*16, 24 + sin(angle +2.512)*16, WHITE);
    display.drawPixel(32 + cos(angle -2.512)*16, 24 + sin(angle -2.512)*16, WHITE);

    display.drawPixel(32 + cos(-angle)*10, 24 + sin(-angle)*10, WHITE);
    display.drawPixel(32 + cos(-angle +2.09)*10, 24 + sin(-angle +2.09)*10, WHITE);
    display.drawPixel(32 + cos(-angle -2.09)*10, 24 + sin(-angle -2.09)*10, WHITE);
    display.display(); delay(MILLISECOND_PER_ITERATION);
  }
}



void displayEndOfLoader(int duration) {
  int stepPerIteration = 360 * MILLISECOND_PER_ITERATION / duration;
  int i, offset;
  float angle;

  for (i = 0; i < 360; i += stepPerIteration) {
    angle = i * 6.28 / 360;
    offset = i /10;
    display.clearDisplay();
    display.drawPixel(32 + cos(angle)*(16+offset), 24 + sin(angle)*(16+offset), WHITE);
    display.drawPixel(32 + cos(angle +1.256)*(16+offset), 24 + sin(angle +1.256)*(16+offset), WHITE);
    display.drawPixel(32 + cos(angle -1.256)*(16+offset), 24 + sin(angle -1.256)*(16+offset), WHITE);
    display.drawPixel(32 + cos(angle +2.512)*(16+offset), 24 + sin(angle +2.512)*(16+offset), WHITE);
    display.drawPixel(32 + cos(angle -2.512)*(16+offset), 24 + sin(angle -2.512)*(16+offset), WHITE);

    display.drawPixel(32 + cos(-angle)*(10+offset), 24 + sin(-angle)*10, WHITE);
    display.drawPixel(32 + cos(-angle +2.09)*(10+offset), 24 + sin(-angle +2.09)*(10+offset), WHITE);
    display.drawPixel(32 + cos(-angle -2.09)*(10+offset), 24 + sin(-angle -2.09)*(10+offset), WHITE);
    display.display(); delay(MILLISECOND_PER_ITERATION);
  }
}





void resetHandlingCounter() {
  if (numberOfHandlingThisSecond >= HANDLING_PER_SECOND && millis() >= nextResetTime) {
    Serial.println("Max number of iteration per second reached. Actual : " + String(numberOfHandlingThisSecond) + " iterations");

    numberOfHandlingThisSecond = 0;
    nextResetTime = millis() + 1000;
    sendState();
  }
}





/***************************\
 * DISPLAY INFO
\***************************/
IPAddress myIP;

void updateInfoDisplay() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 7);

  display.print("SSID: ");
  display.println(SSID);

  display.print("IP:   ");
  display.println(myIP);

  display.println("---------------------");
  display.println(" new connection pin:");
  display.println("");
  display.setTextSize(2);
  display.print("/?pin=");
  display.println(newConnectionPin);

  display.display();
}




/***************************\
 * SETUP
\***************************/
void setup(void){
  Serial.begin(115200);
  SPIFFS.begin();
  Serial.println("");

  // Init OLED DISPLAY
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C

  // Init Board.

  if (analogRead(SETUP_BUTTON_PIN) > 512) {
    appMode = APP_MODE_SETUP;
  }


  generateNewPin();

  initState();
  applyState();

  nextResetTime = millis() + 1000;


  // AP MODE
  // WiFi.softAP(SSID, PASSWORD);
  // IPAddress myIP = WiFi.softAPIP();
  // /AP MODE


  // CLIENT MODE
  WiFi.begin(SSID, PASSWORD);
  Serial.println("");
  Serial.print("Connecting ");

  // Wait for connection.
  while (WiFi.status() != WL_CONNECTED) {
    displayLoaderOnce(500);
    Serial.print(".");
  }

  displayEndOfLoader(400);

  myIP = WiFi.localIP();
  // /CLIENT MODE



  WiFi.macAddress(mac);


  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(SSID);
  Serial.print("IP address: ");
  Serial.println(myIP);

  Serial.print("MAC: ");
  Serial.print(mac[0],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.println(mac[5],HEX);

  display.clearDisplay();

  if (appMode == APP_MODE_SETUP) {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("SETUP MODE");
  }

  updateInfoDisplay();


  // Define server routes.
  server.on("/",            handleRootPage);
  server.on("/style.css",   handleStyleCss);
  server.on("/app.js",      handleAppJs);
  server.on("/actions.js",  handleActionsJs);
  server.onNotFound(        handleNotFound);

  // Start server.
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  Serial.println("HTTP server started");
}



/***************************\
 * LOOP
\***************************/
void loop(){
  server.handleClient();
  webSocket.loop();
  resetHandlingCounter();

  // sendState();
}
