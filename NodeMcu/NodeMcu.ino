#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#define FIREBASE_HOST "monitoring-taman-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "bMvHTOaSDyAMAKj6KKb8BLKTC3eMUQ8b0riv68MG"
#define WIFI_SSID "Server Project"
#define WIFI_PASSWORD "Master75wew"

int jam_pagi, jam_sore;
int lembap = 5;
int tangki = 4;

void setup() {

  Serial.begin(9600);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  // Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  pinMode(lembap, INPUT_PULLUP);
  pinMode(tangki, INPUT_PULLUP);
}


void loop() {


  jam_pagi = Firebase.getString("pagi").toInt();
  jam_sore = Firebase.getString("sore").toInt();


  Serial.print("*");
  Serial.print(jam_pagi);
  Serial.print(",");
  Serial.print(jam_sore);
  Serial.println("#");
  parsingData();

}



void parsingData()
{

  if(digitalRead(lembap)==LOW){
    Firebase.setString("kelembapan","Lembap" );
  }
  else{
    Firebase.setString("kelembapan", "Kering");
  }
  
  if(digitalRead(tangki)==LOW){
    Firebase.setString("water", "Penuh");
  }
  else{
    Firebase.setString("water", "Kosong");
  }
}


