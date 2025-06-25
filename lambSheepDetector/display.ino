#include <LiquidCrystal_I2C.h>
#include <Wire.h>

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


// pins a0, a1, a2 determine address: all 0 (000) -> 0x27; a0 = 1 (001) -> 0x26; a1 = 1 (010) -> 0x25; ...
LiquidCrystal_I2C lcd(0x20, 16, 2);
int sheep_counter = 0;
int lamb_counter = 0;

void setup()
{
  lcd.init();
  lcd.backlight();
  print_sheep_counter(sheep_counter);
  print_lamb_counter(lamb_counter);

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

void loop()
{
  print_sheep_counter(sheep_counter++);
  delay(2000);
  print_lamb_counter(lamb_counter++);
  delay(2000);
}

void print_sheep_counter(int count)
{
  lcd.setCursor(0, 0);
  lcd.print("Sheep: ");
  lcd.setCursor(8, 0);
  lcd.print(count++);
}

void print_lamb_counter(int count)
{
  lcd.setCursor(0, 1);
  lcd.print("Lamb: ");
  lcd.setCursor(8, 1);
  lcd.print(count++);
}
// find I2C addresses
// #include <Wire.h>

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
