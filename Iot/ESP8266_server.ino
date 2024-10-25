#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const int ledPin = D7;  // GPIO for the LED
bool ledState = false;  // LED state

ESP8266WebServer server(80);

// Handleing the root URL that wil be fetched by server "/"
void handleRoot() {
  String page = "<html><head><title>ESP8266 Control Panel</title></head><body>";
  page += "<h1>ESP8266 Control Panel</h1>";
  page += "<button onclick=\"toggleLED()\">Toggle LED & Show Camera</button>";
  page += "<div id=\"cameraFeed\"></div>";
  page += "<script>";
  page += "function toggleLED() {";
  page += "fetch('/toggleLED').then(response => response.text()).then(state => {";
  page += "if(state === 'on') {";
  page += "document.getElementById('cameraFeed').innerHTML = '<img src=\"http://192.168.4.2/stream\" width=\"320\" height=\"240\" />';";
  page += "} else {";
  page += "document.getElementById('cameraFeed').innerHTML = '';"; 
  page += "}";
  page += "});";
  page += "}";
  page += "</script>";
  page += "</body></html>";

  server.send(200, "text/html", page);  
}

void handleToggleLED() {
  ledState = !ledState;  /
  digitalWrite(ledPin, ledState ? HIGH : LOW);  
  server.send(200, "text/plain", ledState ? "on" : "off"); 
}

void setup() {
  pinMode(ledPin, OUTPUT);  
  digitalWrite(ledPin, LOW); 

  
  WiFi.softAP("ESP8266_AP");

  
  server.on("/", handleRoot);
  server.on("/toggleLED", handleToggleLED);

  
  server.begin();
}

void loop() {
  server.handleClient();  

  
  if (ledState) {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  }
}