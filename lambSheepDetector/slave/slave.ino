#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// BLE
SoftwareSerial BTSerial(10, 11);

// alarm
const int alarmPin = 8;     
const int buttonPin = 2;

bool alarmActive = false;
bool lastButtonState = HIGH;
bool sheepInDanger = false;

// display
LiquidCrystal_I2C lcd(0x27, 16, 2);

int sheep_counter = 0;
int lamb_counter = 0;

void setup() {
  setupSerials();
  setupAlarm();
  setupDisplay();
}
 
void loop() {

  // check received data and print
  if(BTSerial.available()) {
    int rx = BTSerial.read();
    Serial.print("integer: ");
    Serial.println(rx);
    Serial.print("char: ");
    Serial.println((char)rx);
    if((char)rx == 'd') // d for danger
    {
      sheepInDanger = true;
    }
    else if((char)rx == 's') // s for sheep
    {
      while(true)
      {
        rx = BTSerial.read();
        if(rx != -1)
          break;
        delay(10);
      }
      
      printSheepCount(rx);
      Serial.print("integer: ");
      Serial.println(rx);
      Serial.print("char: ");
      Serial.println((char)rx);
    }
    else if((char)rx == 'l') // l for lamb
    {
      while(true)
      {
        rx = BTSerial.read();
        if(rx != -1)
          break;
        delay(10);
      }

      printLambCount(rx);
      Serial.print("integer: ");
      Serial.println(rx);
      Serial.print("char: ");
      Serial.println((char)rx);
    }
  }

  // check danger state
  if(sheepInDanger && !alarmActive) {
    alarmActive = true;
  }

  // fire alarm in case of danger
  if(alarmActive) {
    digitalWrite(alarmPin, HIGH);  
    delay(3);                       
    digitalWrite(alarmPin, LOW);   
    delay(50);                     
  }

  // stop alarm if button is pressed
  bool currentButtonState = digitalRead(buttonPin);
  if(lastButtonState == HIGH && currentButtonState == LOW) {
    alarmActive = false;
    sheepInDanger = false; 
    digitalWrite(alarmPin, LOW);
    BTSerial.write('f'); // f for fine
  }
  lastButtonState = currentButtonState;
}

void setupDisplay()
{
  lcd.init();
  lcd.backlight();

  byte earLeft[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00011,
    0b11100,
    0b10000,
    0b01000,
    0b00001
  };

  byte eyes[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b00000,
    0b00000,
    0b00000,
    0b10001
  };

  byte earRight[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b11000,
    0b00111,
    0b00001,
    0b00010,
    0b10000
  };

  byte jawLeft[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00100,
    0b00110,
    0b00011,
    0b00000
  };

  byte nose[8] = {
    0b00000,
    0b00000,
    0b01110,
    0b00100,
    0b00100,
    0b01010,
    0b00000,
    0b00000
  };

  byte jawRight[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00100,
    0b01100,
    0b11000,
    0b00000
  };

  printSheepCount(sheep_counter);
  printLambCount(lamb_counter);

  lcd.createChar(0, earLeft);
  lcd.createChar(1, eyes);
  lcd.createChar(2, earRight);
  lcd.createChar(3, jawLeft);
  lcd.createChar(4, nose);
  lcd.createChar(5, jawRight);

  lcd.setCursor(13, 0);
  lcd.write(byte(0));
  lcd.write(byte(1));
  lcd.write(byte(2));

  lcd.setCursor(13, 1);
  lcd.write(byte(3));
  lcd.write(byte(4));
  lcd.write(byte(5));
}

void setupAlarm()
{
  pinMode(alarmPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void setupSerials()
{
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void printSheepCount(int count)
{
  lcd.setCursor(0, 0);
  lcd.print("Sheep: ");
  lcd.setCursor(8, 0);
  lcd.print(count);
}

void printLambCount(int count)
{
  lcd.setCursor(0, 1);
  lcd.print("Lamb: ");
  lcd.setCursor(8, 1);
  lcd.print(count);
}

// find display address
// void setup() {
//   Wire.begin();
//   Serial.begin(9600);
// }

// void loop() {
//   for (byte address = 1; address < 127; address++) {
//     Wire.beginTransmission(address);
//     if (Wire.endTransmission() == 0) {
//       Serial.print("I2C device found at 0x");
//       Serial.println(address, HEX);
//     }
//     delay(5);
//   }
//   delay(1000);
// }