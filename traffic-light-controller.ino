

// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

// Replace with your network credentials
const char* ssid = "Jakes Traffic Light";
const char* password = "password";

IPAddress local_IP(10,1,1,1);
IPAddress gateway(10,1,1,1);
IPAddress subnet(255,255,255,0);

// Set LED GPIO
const int ledPin = 2;
// Stores LED state
String ledState;

bool trafficLightPatternRunning = false;
bool isComplete = false;
int curMillis = millis();

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(ledPin)){
      ledState = "ON";
    }
    else{
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  } else if (var == "RELAY1"){
  return "UNAVAIL";
}
else if (var == "RELAY2"){
  return "UNAVAIL";
}
else if (var == "RELAY3"){
  return "UNAVAIL";
} 
}

void relayOneOn() {
  digitalWrite(D5, LOW);
}

void relayOneOff() {
  digitalWrite(D5, HIGH);
}

void relayTwoOn() {
  digitalWrite(D6, LOW);
}

void relayTwoOff() {
  digitalWrite(D6, HIGH);
}

void relayThreeOn() {
  digitalWrite(D7, LOW);
}

void relayThreeOff() {
  digitalWrite(D7, HIGH);
}

void startTrafficLight() {
  trafficLightPatternRunning = true;
  isComplete = true;
}

void stopTrafficLight() {
  trafficLightPatternRunning = false;
  relayOneOff();
  relayTwoOff();
  relayThreeOff();
}

void trafficLightPattern() {
//  Serial.println(millis());
//  Serial.println(curMillis);
  if (isComplete) {
      curMillis = millis();
      isComplete = false;
    }
  if (curMillis + 1000 == millis()) {
    Serial.println("change one");
    relayOneOn();
    relayTwoOff();
    relayThreeOff();
  } else if (curMillis + 6000 == millis()) {
    Serial.println("change two");
    relayOneOff();
    relayTwoOn();
    relayThreeOff();
    } else if (curMillis + 11000 == millis()) {
    Serial.println("change two");
    relayOneOff();
    relayTwoOff();
    relayThreeOn();
    
    } else if (curMillis + 16000 == millis()) {
      isComplete = true;
    }
  
}
 
void setup(){
  // Serial port for debugging purposes
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "IP Ready" : "IP Failed!");
  Serial.println(WiFi.softAP(ssid) ? "AP Ready" : "AP Failed!");
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Route to load index.js file
  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.js", "text/javascript");
  });
    // Route to load patterns.js file
  server.on("/patterns.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/patterns.js", "text/javascript");
  });

  // Route to set GPIO to HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to set GPIO to LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Relay One Routes
  server.on("/relay1/on", HTTP_GET, [](AsyncWebServerRequest *request){
    relayOneOn();   
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
    server.on("/relay1/off", HTTP_GET, [](AsyncWebServerRequest *request){
    relayOneOff();   
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

    // Relay Two Routes
  server.on("/relay2/on", HTTP_GET, [](AsyncWebServerRequest *request){
    relayTwoOn();   
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
    server.on("/relay2/off", HTTP_GET, [](AsyncWebServerRequest *request){
    relayTwoOff();   
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

    // Relay Three Routes
  server.on("/relay3/on", HTTP_GET, [](AsyncWebServerRequest *request){
    relayThreeOn();   
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
    server.on("/relay3/off", HTTP_GET, [](AsyncWebServerRequest *request){
    relayThreeOff();   
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Pattern Routes
  server.on("/pattern/start", HTTP_GET, [](AsyncWebServerRequest *request){
    startTrafficLight();   
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/pattern/stop", HTTP_GET, [](AsyncWebServerRequest *request){
    stopTrafficLight();   
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });


  // Start server
  server.begin();
}

void loop() {
  
  if (trafficLightPatternRunning) {
    trafficLightPattern();
  }
}
