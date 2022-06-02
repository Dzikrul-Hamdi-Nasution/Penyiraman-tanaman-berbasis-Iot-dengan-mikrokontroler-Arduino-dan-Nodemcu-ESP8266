#include <Wire.h>
#include "DS3231.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX
RTClib RTC;
int jam, menit, detik, tahun, bulan, hari;
int sensorPin_1 = A0;
int sensorPin_2 = A1;
int sensorPin_3 = A2;
int sensorValue_1, sensorValue_2, sensorValue_galon;

int relay_pompa = 9;
int relay_ultrasonic = 8;
int menit_saat_ini, kunci;
String status_tanah, status_air;
String dataIn;
String dt[10];
int i, kunci_waktu;
boolean parsing = false;

int jam_pagi, jam_sore;
int lembap = 5;
int tangki = 4;

void setup () {
  Serial.begin(9600);
  mySerial.begin(9600);
  Wire.begin();
  pinMode(relay_pompa, OUTPUT);
  pinMode(relay_ultrasonic, OUTPUT);
  digitalWrite(relay_pompa, HIGH);
  digitalWrite(relay_ultrasonic, HIGH);
  pinMode(lembap, OUTPUT);
  pinMode(tangki, OUTPUT);
}

void loop () {
  DateTime now = RTC.now();
  tahun = now.year();
  bulan = now.month();
  hari = now.day();
  jam = now.hour();
  menit = now.minute();
  detik = now.second();
  cek_sensor();
  ultrasonic();
  cek_pompa();

  Serial.print(tahun);
  Serial.print(" / ");
  Serial.print(bulan);
  Serial.print(" / ");
  Serial.print(hari);
  Serial.print(" ");
  Serial.print(jam);
  Serial.print(": ");
  Serial.print(menit);
  Serial.print(": ");
  Serial.print(detik);
  Serial.println();


  // kirim();
  if (mySerial.available() > 0)
  {
    char inChar = (char)mySerial.read();
    dataIn += inChar;
    if (inChar == '\n') {
      parsing = true;
    }
  }
  if (parsing)
  {
    parsingData();
    parsing = false;
    dataIn = "";
  }
  delay(100);
}

void parsingData()
{
  int j = 0;
  dt[j] = "";
  for (i = 1; i < dataIn.length(); i++)
  {
    if ((dataIn[i] == '#') || (dataIn[i] == ','))
    {
      j++;
      dt[j] = ""; //inisialisasi variabel array dt[j]
    }
    else
    {
      dt[j] = dt[j] + dataIn[i];
    }
  }
  Serial.print("data 1 : ");
  Serial.print(dt[0]);
  if (jam == dt[0].toInt()) {
     kunci_waktu = 1;
    cek_pompa();
  }
  Serial.print("\n");
  Serial.print("data 2 : ");
  Serial.print(dt[1]);
  if (jam == dt[1].toInt()) {
     kunci_waktu = 1;
    cek_pompa();
  }
  Serial.print("\n\n");
}


void kirim() {
  mySerial.print("*");
  mySerial.print(status_air);
  mySerial.print(",");
  mySerial.print(status_tanah);
  mySerial.println("#");
}


void cek_sensor() {
  sensorValue_1 = analogRead(sensorPin_1);
  sensorValue_2 = analogRead(sensorPin_2);
  sensorValue_galon = analogRead(sensorPin_3);
}

void cek_pompa() {
  if (sensorValue_1 < 700 and sensorValue_2 < 700) {
    if (kunci_waktu == 1) {
      digitalWrite(relay_pompa, HIGH);
      kunci_waktu = 0;
    }
    Serial.println("Tidak Menyiram");
    status_tanah = "Basah";
    digitalWrite(lembap, LOW);
    kunci_waktu = 0;
  }
  else {
    if (kunci_waktu == 1) {
      digitalWrite(relay_pompa, LOW);
    }
    status_tanah = "Kering";
    Serial.println("Menyiram");
    digitalWrite(lembap, HIGH);
  }

  if (sensorValue_galon > 700) {
    status_air = "Kosong";
    digitalWrite(tangki, HIGH);
  }
  else {
    status_air = "Penuh";
    digitalWrite(tangki, LOW);
  }
}

void ultrasonic() {
  if (kunci == 0) {
    menit_saat_ini = menit + 2;
  }

  if (menit <= menit_saat_ini) {
    digitalWrite(relay_ultrasonic, LOW);
    kunci = 1;
    Serial.print("waktu target : ");
    Serial.println(menit_saat_ini);
    Serial.println("Ultrasonic hidup");
  }

  if (menit > menit_saat_ini) {
    digitalWrite(relay_ultrasonic, HIGH);
    Serial.println("Ultrasonic mati");
  }

  if (kunci == 1) {
    if (menit > (menit_saat_ini + 2)) {
      kunci = 0;
      Serial.println("Mode kunci status");
    }
  }
}
