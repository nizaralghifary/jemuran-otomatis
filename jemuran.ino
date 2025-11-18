// isi sendiri
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

// ini juga isi sendiri
char ssid[] = "";
char pass[] = "";

Servo jemuran;

int rpin = 34;
int spin = 14;

int mode = 0; // 0 = auto, 1 = manual
int rvalue = 3000;

int angle = 90;

BLYNK_WRITE(V0) { 
  mode = param.asInt();
  Serial.print("Mode: ");
  Serial.println(mode ? "MANUAL" : "AUTO");
}

BLYNK_WRITE(V1) {
  if (mode == 1) {
    angle = param.asInt();
    jemuran.write(angle);
  }
}

void setup() {
  Serial.begin(115200);

  jemuran.setPeriodHertz(50);
  jemuran.attach(spin, 500, 2400);
  jemuran.write(angle);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pinMode(rpin, INPUT);
}

void loop() {
  Blynk.run();

  if (mode == 0) {
    int nilai = analogRead(rpin);
    Serial.println(nilai);

    if (nilai < rvalue) {
      angle = 0;
      jemuran.write(angle);
      Blynk.virtualWrite(V2, "Hujan! Menutup jemuran... ☔");
    } else {
      angle = 90;
      jemuran.write(angle);
      Blynk.virtualWrite(V2, "Kering! Membuka jemuran... ☀️");
    }
  }
}
