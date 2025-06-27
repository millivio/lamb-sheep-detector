#include <HCSR04.h>
#include<Servo.h>

Servo filterGate;
Servo entryGate;
const int SERVO_PIN = 10;  //PWM pin

UltraSonicDistanceSensor sensor2(6, 7);  // Entry sensor 2
UltraSonicDistanceSensor sensor1(4, 5);  // Entry sensor 1
UltraSonicDistanceSensor lambValidationSensor(2, 3);  // Lamb confirm
UltraSonicDistanceSensor sheepValidationSensor(8, 9);  // Sheep confirm

#define THRESHOLD 4  // cm
#define VALIDATION_DELAY 100  // ms

int lambDetectedCount = 0;
int sheepDetectedCount = 0;

void setup() {
  Serial.begin(9600);
  filterGate.attach(SERVO_PIN);
  entryGate.attach(SERVO_PIN);
  filterGate.write(90); 
  entryGate.write(0);
}

void loop() {
  float d1 = sensor1.measureDistanceCm();
  float d2 = sensor2.measureDistanceCm();

    // Serial.print("sensor1 distance") ;
    // Serial.println(d1);
    // Serial.print("sensor2 distance") ;
    // Serial.println(d2);

  if (d1 < THRESHOLD && d2 >= THRESHOLD) {

    lambDetectedCount++;
    Serial.println("Lamb detected, lamb count:");
    Serial.println(lambDetectedCount);
    filterGate.write(55);
    entryGate.write(180);
    delay(VALIDATION_DELAY);
      
  }
  if ( d2 < THRESHOLD) {

    sheepDetectedCount++;
    Serial.print("Sheep detected, sheep count:");
    Serial.println(sheepDetectedCount);
    filterGate.write(125);
    entryGate.write(180);
    delay(VALIDATION_DELAY);
  }

  if(lambDetectedCount)
  {
    float lambDistance = lambValidationSensor.measureDistanceCm();
    Serial.print("lambValidationSensor distance") ;
    Serial.println(lambDistance);

    if (lambDistance < THRESHOLD)
    {
        lambDetectedCount--;
        entryGate.write(0);
    }
  
  }
  if(sheepDetectedCount)
  {
    float sheepDistance = sheepValidationSensor.measureDistanceCm();
    Serial.print("sheepValidationSensor distance") ;
    Serial.println(sheepDistance);
    
    if (sheepDistance < THRESHOLD)
    {
        sheepDetectedCount--;
        entryGate.write(0);
    }
  }

  delay(200);
}
