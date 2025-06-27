#include <HCSR04.h>
#include <Servo.h>
#include <SoftwareSerial.h>

// === Bluetooth ===
SoftwareSerial BTSerial(10, 11); // RX, TX

// === Servomotoare ===
Servo filterGate; // Braț de direcție (stânga/dreapta)
Servo entryGate;  // Poarta de acces pe traseu

// === Pini pentru servo ===
const int FILTER_GATE_PIN = 12;
const int ENTRY_GATE_PIN = 13;

// === Senzori FSR ===
const int fsr1Pin = A0; // pentru oaie
const int fsr2Pin = A1; // pentru miel

// === Senzori ultrasonici ===
UltraSonicDistanceSensor sensor2(6, 7);  // Senzor 2
UltraSonicDistanceSensor sensor1(4, 5);  // Senzor 1

// === Praguri și variabile ===
#define THRESHOLD 3           // Prag distanță (cm)
#define VALIDATION_TIME 5000 // 5 secunde
const int sensorMinValue = 100;

bool animalOnTrack = false;
int fsr1Count = 0;
int fsr2Count = 0;

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);

  // Atașare servo
  filterGate.attach(FILTER_GATE_PIN);
  entryGate.attach(ENTRY_GATE_PIN);

  // Inițializare poziții
  filterGate.write(90);   // Poziție neutră
  entryGate.write(0);     // Poarta închisă
}

void loop() {
  if (!animalOnTrack) { 
    float d1 = sensor1.measureDistanceCm();
    float d2 = sensor2.measureDistanceCm();

    if (d1 < THRESHOLD || d2 < THRESHOLD) {
      animalOnTrack = true;

      // Deschide poarta pentru acces
      //entryGate.write(0);
      //delay(500);
      entryGate.write(180); // Închide imediat după intrare
      delay(500);

      // Clasificare animal
      bool isLamb = (d1 < THRESHOLD && d2 >= THRESHOLD);
      if (d1 < THRESHOLD && d2 >= THRESHOLD) {
        Serial.println("Miel detectat");
        filterGate.write(55); // Direcție miel
      } 
      
      if( d2 < THRESHOLD) {
        Serial.println("Oaie detectată");
        filterGate.write(125); // Direcție oaie
        isLamb = false;
      }

      // Așteaptă validarea FSR
      unsigned long startTime = millis();
      bool validated = false;

      while (millis() - startTime < VALIDATION_TIME) {
        int fsr1 = analogRead(fsr1Pin);
        int fsr2 = analogRead(fsr2Pin);

        if (isLamb && fsr2 > sensorMinValue) {
          fsr2Count++;
          Serial.println("lamb validat");
          BTSerial.write('L');
          validated = true;
          break;
        }

        if (!isLamb && fsr1 > sensorMinValue) {
          fsr1Count++;
          Serial.println("sheep validată");
          BTSerial.write('S');
          validated = true;
          break;
        }

        delay(100);
      }

      if (!validated) {
        Serial.println(" Alarma: animalul nu a fost validat!");
        BTSerial.write('D');
      }

      // Resetare
      //filterGate.write(90);       // Reveniți la poziția neutră
      animalOnTrack = false;  
      entryGate.write(0);    // Permite următorul animal
    }
  }

  // Debug serial pentru FSR
  int fsr1 = analogRead(fsr1Pin);
  int fsr2 = analogRead(fsr2Pin);
  Serial.print("FSR1 (oaie): "); Serial.print(fsr1);
  Serial.print(" | FSR2 (miel): "); Serial.print(fsr2);
  Serial.print(" || Total sheep: "); Serial.print(fsr1Count);
  Serial.print(" | Total lamb: "); Serial.println(fsr2Count);

  delay(200);
}
