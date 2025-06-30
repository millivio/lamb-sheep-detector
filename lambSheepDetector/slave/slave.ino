#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// BLE
SoftwareSerial BTSerial(10, 11);

// alarm
const int alarmPin = 8;     
const int buttonPin = 2;
const int ledButtonPin= 3; 
const int ledControlPin =4;
const int blueButtonPin = 5;
const int blueLedPin  = 6;  // SIG1  – LED always on


bool alarmActive = false;
bool lastButtonState = HIGH;
bool sheepInDanger = false;
bool lastLedButtonState = HIGH;
bool lastBlueBtnState  = HIGH;

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
      
      printSheepCount(rx, false);
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

      printLambCount(rx, false);
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
    digitalWrite(ledControlPin, HIGH); 
    delay(3);                       
    digitalWrite(alarmPin, LOW);   
    delay(50);                     
  }else {

    digitalWrite(ledControlPin, LOW); 
  }

 bool currentBlueBtnState = digitalRead(blueButtonPin);

  if (lastBlueBtnState == HIGH && currentBlueBtnState == LOW) {
      BTSerial.write('r');    // r for reset
      Serial.println("Reset sent");
      printLambCount(0, true);
      printSheepCount(0, true); 
  }

 bool currentButtonState = digitalRead(buttonPin);
 bool currentLedButtonState = digitalRead(ledButtonPin);
 
    if ((lastButtonState == HIGH && currentButtonState == LOW) ||

        (lastLedButtonState == HIGH && currentLedButtonState == LOW)) {

      alarmActive = false;

      sheepInDanger = false;

      digitalWrite(alarmPin, LOW);

      BTSerial.write('f'); // f for fine

    }
 
 lastButtonState = currentButtonState;
 lastLedButtonState = currentLedButtonState;
 lastBlueBtnState = currentBlueBtnState;

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

  printSheepCount(sheep_counter, false);
  printLambCount(lamb_counter, false);

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
  pinMode(ledButtonPin, INPUT_PULLUP);
  pinMode(ledControlPin, OUTPUT);
  pinMode(blueButtonPin, INPUT_PULLUP);  
  pinMode(blueLedPin,    OUTPUT); 
  digitalWrite(blueLedPin, HIGH);  
}

void setupSerials()
{
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void printSheepCount(int count, bool reset)
{
  lcd.setCursor(0, 0);
  lcd.print("Sheep: ");
  lcd.setCursor(8, 0);
  if(reset){
    lcd.print("     ");
  }else {
  lcd.print(count);
  }
}

void printLambCount(int count, bool reset)
{
  lcd.setCursor(0, 1);
  lcd.print("Lamb: ");
  lcd.setCursor(8, 1);
  if(reset){
    lcd.print("     ");
  }else
  {
  lcd.print(count);
  }
}