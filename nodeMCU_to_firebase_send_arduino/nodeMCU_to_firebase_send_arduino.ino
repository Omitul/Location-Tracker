#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <FirebaseESP8266.h> //we are using the ESP8266

SoftwareSerial SerialGPS(4, 5); 
TinyGPSPlus gps;
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Our Galaxy"
#define WIFI_PASSWORD "5678@&dcba"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDNnMFwA4bC72F6VDHayU2KGanHixuQHN8"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://ltracker-64a48-default-rtdb.asia-southeast1.firebasedatabase.app/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;                     //since we are doing an anonymous sign in 

void setup(){
  Serial.begin(9600);
  SerialGPS.begin(9600);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = "AIzaSyDNnMFwA4bC72F6VDHayU2KGanHixuQHN8";

  /* Assign the RTDB URL (required) */
  config.database_url = "https://ltracker-64a48-default-rtdb.asia-southeast1.firebasedatabase.app/";

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

float Latitude, Longitude;
void loop(){
  while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read())) {
      if (gps.location.isValid()) {
        Latitude = gps.location.lat();
        Longitude = gps.location.lng();
       
        Serial.println(Latitude, 6);
        Serial.println(Longitude, 6);
        sendToFirebase(String(Latitude,6), String(Longitude,6));
      }
    }
  }
  delay(100);
}

void sendToFirebase(String Latitude, String Longitude) {
if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    
    if (Firebase.setString(fbdo, "Bus Location/1/Latitude", Latitude)){
      Serial.println("date sent");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.setString(fbdo, "Bus Location/1/Longitude", Longitude)){
      Serial.println("data sent");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
}
