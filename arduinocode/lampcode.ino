#include <Arduino.h>
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
//#elif defined(ESP32)
//  #include <WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "YOUR_SSID_HERE"
#define WIFI_PASSWORD "YOUR_PASSWORD_HERE" 

#define API_KEY "YOUR_KEY_HERE"
#define DATABASE_URL "https://te***-****-******-****.firebaseio.com/" 

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

/* variable setup */
int currcolor = -1;
int def = 0;


void setup(){
  Serial.begin(115200);
  pinMode(16, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(16,LOW);
  digitalWrite(14,LOW);
  digitalWrite(12,LOW);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.println("Connecting...");
  }
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("Firebase sign up complete");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 120000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    if (Firebase.RTDB.getInt(&fbdo, "/test/color")) {
      if (fbdo.dataType() == "int") {
        int v = fbdo.intData();
        
        if(v == 0){
          digitalWrite(14,HIGH);
          digitalWrite(16,LOW);
          digitalWrite(12,LOW);
        }
        else if(v == 1){
          digitalWrite(12,HIGH);
          digitalWrite(14,LOW);
          digitalWrite(16,LOW);
          
        }
        else if(v == 2){
          digitalWrite(16,HIGH);
          digitalWrite(14,LOW);
          digitalWrite(12,LOW);
        }
        else if(v == 3){
          digitalWrite(16,LOW);
          digitalWrite(14,LOW);
          digitalWrite(12,LOW);
        }
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }
  }
}
