// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

// Replace with your network credentials
const char* ssid = "Jakes Traffic Light";
const char* password = "Tanner01";

IPAddress local_IP(10,1,1,1);
IPAddress gateway(10,1,1,1);
IPAddress subnet(255,255,255,0);

// Set LED GPIO
const int ledPin = 2;
// Stores LED state
String ledState;

// Get or set application state

struct AppState {
    bool isRelayOneOn = false;
    bool isRelayTwoOn = false;
    bool isRelayThreeOn = false;
    bool trafficLightPatternRunning = false;
    bool isComplete = false;
    int greenLightDuration = 5000;
    int yellowLightDuration = 3000;
    int redLightDuration = 5000;
    bool readFromFile = false;
  };

struct AppState state;

//bool isRelayOneOn = state.isRelayOneOn;
//bool isRelayTwoOn = state.isRelayTwoOn;
//bool isRelayThreeOn = state.isRelayThreeOn;
//bool trafficLightPatternRunning = state.trafficLightPatternRunning;
//bool isComplete = state.isComplete;
//int greenLightDuration = state.greenLightDuration;
//int yellowLightDuration = state.yellowLightDuration;
//int redLightDuration = state.redLightDuration;


bool readFromFile = state.readFromFile;

int curMillis = millis();

// Try to read from json
bool readFromJson(char* fileName) {
  
  File stateFile = SPIFFS.open(fileName, "r");
  if (!stateFile) {
    Serial.println("Failed to open config file");
    return false;
  }
    StaticJsonDocument<512> doc;
    DeserializationError err = deserializeJson(doc, stateFile);

    if (err) {
      Serial.print(F("deserializeJson() failed with code ")); 
      Serial.println(err.c_str());
    }

    state.isRelayOneOn = doc["isRelayOneOn"];
    state.isRelayTwoOn = doc["isRelayTwoOn"];
    state.isRelayThreeOn = doc["isRelayThreeOn"];
    state.trafficLightPatternRunning = doc["trafficLightPatternRunning"];
    state.redLightDuration = doc["redLightDuration"];
    state.yellowLightDuration = doc["yellowLightDuration"];
    state.greenLightDuration = doc["greenLightDuration"];
    state.readFromFile = true;

    return true;
  }

bool saveToJsonFile(char* fileName) {
  
    StaticJsonDocument<512> doc;
     doc["isRelayOneOn"] = state.isRelayOneOn;
     doc["isRelayTwoOn"] = state.isRelayTwoOn;
     doc["isRelayThreeOn"] = state.isRelayThreeOn;
     doc["trafficLightPatternRunning"] = state.trafficLightPatternRunning;
     doc["redLightDuration"] = state.redLightDuration;
     doc["yellowLightDuration"] = state.yellowLightDuration;
     doc["greenLightDuration"] = state.greenLightDuration;

     char output[256];
     serializeJson(doc, output);

     File stateFile = SPIFFS.open(fileName, "w");
     if (!stateFile) {
        Serial.println("Failed to open config file");
        return false;
      };

      stateFile.print(output);

    return true;
  }

void setRelayState() {
  // Relay one state
  if (state.isRelayOneOn) {
      relayOneOn();
    }
   if (!state.isRelayOneOn) {
      relayOneOff();
    }
    // Relay two state
  if (state.isRelayTwoOn) {
      relayTwoOn();
    }
   if (!state.isRelayTwoOn) {
      relayTwoOff();
    }
   // Relay three state
  if (state.isRelayThreeOn) {
      relayThreeOn();
    }
   if (!state.isRelayThreeOn) {
      relayThreeOff();
    }
 }

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  if (var=="curRedLightDuration") {
    return String(state.redLightDuration / 1000);
  } else if (var=="curYellowLightDuration") {
    return String(state.yellowLightDuration / 1000);
  } else if (var=="curGreenLightDuration") {
    return String(state.greenLightDuration / 1000);
  }
  
}

void relayOneOn() {
  digitalWrite(D5, LOW);
  state.isRelayOneOn = true;
}

void relayOneOff() {
  digitalWrite(D5, HIGH);
  state.isRelayOneOn = false;
}

void relayTwoOn() {
  digitalWrite(D6, LOW);
  state.isRelayTwoOn = true;
}

void relayTwoOff() {
  digitalWrite(D6, HIGH);
  state.isRelayTwoOn = false;
}

void relayThreeOn() {
  digitalWrite(D7, LOW);
  state.isRelayThreeOn = true;
}

void relayThreeOff() {
  digitalWrite(D7, HIGH);
  state.isRelayThreeOn = false;
}

void startTrafficLight() {
  state.trafficLightPatternRunning = true;
  state.isComplete = true;
}

void stopTrafficLight() {
  state.trafficLightPatternRunning = false;
  relayOneOff();
  relayTwoOff();
  relayThreeOff();
}

void trafficLightPattern() {
//  Serial.println(millis());
//  Serial.println(curMillis);
  if (state.isComplete) {
      curMillis = millis();
      state.isComplete = false;
    }
  if (curMillis == millis()) {
    Serial.println("change one");
    relayOneOn();
    relayTwoOff();
    relayThreeOff();
  } else if (curMillis + state.redLightDuration == millis()) {
    Serial.println("change two");
    relayOneOff();
    relayTwoOn();
    relayThreeOff();
    } else if (curMillis + state.redLightDuration + state.yellowLightDuration == millis()) {
    Serial.println("change two");
    relayOneOff();
    relayTwoOff();
    relayThreeOn();
    } 
    else if (curMillis + state.redLightDuration + state.yellowLightDuration + state.greenLightDuration == millis()) {
      state.isComplete = true;
    }
  
}
 
void setup(){
  // Serial port for debugging purposes
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "IP Ready" : "IP Failed!");
  Serial.println(WiFi.softAP(ssid, password) ? "AP Ready" : "AP Failed!");
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);
  digitalWrite(D7, HIGH);

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  //
  Serial.println();
  Serial.println("should be 5000");
  Serial.print("Red Duration: " );
  Serial.println(state.redLightDuration);

  bool successRead = readFromJson("/state.json");

  Serial.print("Success? ");
  Serial.println(successRead);

  Serial.println();
  Serial.println("should be 6000");
  Serial.print("Red Duration: " );
  Serial.println(state.redLightDuration);

  Serial.println("Try writing:");
  saveToJsonFile("/state.json");

  Serial.println();
  Serial.println("after save - should be 10000");
  Serial.print("Red Duration: " );
  Serial.println(state.redLightDuration);
  

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
  // Route to load state file
  server.on("/state", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/state.json", "text/javascript");
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

  // Save State to JSON file
  server.on("/save-state", HTTP_GET, [] (AsyncWebServerRequest *request){
     saveToJsonFile("/state.json");
     request->send(SPIFFS, "/index.html", String(), false, processor);
    });

  // Route to update the traffic light timing
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
      if (request->hasParam("redLightDuration")) {
          AsyncWebParameter* p = request->getParam("redLightDuration");
          if (atoi(p->value().c_str()) != 0) {
            state.redLightDuration = atoi(p->value().c_str());
            }
        }
       if (request->hasParam("yellowLightDuration")) {
          AsyncWebParameter* p = request->getParam("yellowLightDuration");
          if (atoi(p->value().c_str()) != 0) {
              state.yellowLightDuration = atoi(p->value().c_str());
            }
        } if (request->hasParam("greenLightDuration")) {
          AsyncWebParameter* p = request->getParam("greenLightDuration");
          if (atoi(p->value().c_str()) != 0) {
              state.greenLightDuration = atoi(p->value().c_str());
            }
        }
        state.isComplete = true;
        saveToJsonFile("/state.json");
      request->send(SPIFFS, "/index.html", String(), false, processor);
    });

  // Start server
  server.begin();

  //Set initial relay state;
  setRelayState();
}

void loop() {
  
  if (state.trafficLightPatternRunning) {
    trafficLightPattern();
  }
}
