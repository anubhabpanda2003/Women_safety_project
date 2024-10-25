#include <ESP8266WiFi.h>
#include <Wire.h>
#include <MPU6050.h>

const char* ssid = "Galaxy M34 5G 62E4"; 
const char* password = "omm@2003";      
WiFiServer server(80);

const int redLED = D5;    
const int greenLED = D6;  
const int yellowLED = D7; 
const int blueLED = D8;   
MPU6050 mpu;
bool showDirection = false;

unsigned long lastReconnectAttempt = 0;
const long reconnectInterval = 1000; 

void setup() {
  Serial.begin(115200);

  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(blueLED, OUTPUT);

  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(blueLED, LOW);

  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
  } else {
    Serial.println("MPU6050 connection successful");
  }

  connectToWiFi();
  server.begin();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastReconnectAttempt >= reconnectInterval) {
      lastReconnectAttempt = currentMillis;
      reconnectToWiFi();
    }
  }

  WiFiClient client = server.available(); /
  
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();
    
    Serial.println("Request: " + request);
    
    if (request.indexOf("/SOS=START") != -1) {
      
      showDirection = true;
    } else if (request.indexOf("/SOS=STOP") != -1) {
      
      showDirection = false;
      digitalWrite(redLED, LOW);
      digitalWrite(greenLED, LOW);
      digitalWrite(yellowLED, LOW);
      digitalWrite(blueLED, LOW);
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<h1>ESP8266 SOS Button</h1>");
    client.println("<button onclick=\"location.href='/SOS=START'\">Start SOS</button>");
    client.println("<button onclick=\"location.href='/SOS=STOP'\">Stop SOS</button>");
    client.println("</html>");
    client.stop();
    Serial.println("Client disconnected.");
  }

  if (showDirection) {
    displayDirection();
    delay(1000);
  }
}

void setLED(int ledPin) {
 
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(blueLED, LOW);

  
  digitalWrite(ledPin, HIGH);
}
void displayDirection() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  float pitch = atan2(ay, az) * (180 / PI);
  float roll = atan2(ax, az) * (180 / PI);
  if (abs(pitch) > abs(roll)) {
    if (pitch > 0) {
     
      setLED(redLED); // Red LED
    } else {
      
      setLED(blueLED); // Blue LED
    }
  } else {
    if (roll > 0) {
      // Right
      setLED(greenLED); // Green LED
    } else {
      // Left
      setLED(yellowLED); // Yellow LED
    }
  }
  Serial.println("Direction updated.");
}
void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) { // 10 seconds timeout
    delay(100);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Failed to connect to WiFi. Will retry.");
  }
}

void reconnectToWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) { // 10 seconds timeout
      delay(100);
      Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("");
      Serial.println("Reconnected to WiFi");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      
      if (digitalRead(redLED) == HIGH || digitalRead(greenLED) == HIGH ||
          digitalRead(yellowLED) == HIGH || digitalRead(blueLED) == HIGH) {
        setLED(redLED); // Example: Restore the last state (you can enhance this part)
      }
    } else {
      Serial.println("");
      Serial.println("Failed to reconnect to WiFi. Will retry.");
    }
  }
}
