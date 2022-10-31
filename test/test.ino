#include <dht.h>

#include <LiquidCrystal_I2C.h>


//dht
dht DHT;

#define DHT11_PIN 8

//moisture sensor pin
int moistureSensor = A0;

const int airValue = 520;
const int waterValue = 310;
int soilMoistureValue = 0;
int moisturePercentageValue = 0;
float temp;
float humidity;

LiquidCrystal_I2C lcd(0x27, 16, 2);

// output pin
const int output = 9;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  soilMoistureValue = analogRead(moistureSensor);
  moisturePercentageValue = 100 - ((soilMoistureValue/1023.0) * 100);
  
  temp = DHT.read11(DHT11_PIN); // read the temperature value from the DHT11 sensor
  
  lcd.setCursor(0,0);
  lcd.print("Moisture: ");
  lcd.print(moisturePercentageValue);
  lcd.print("%");
  
  lcd.setCursor(0,1);
  lcd.print("Temp: ");
  lcd.print(DHT.temperature);
  lcd.print("%");
  Serial.println(DHT.temperature);
  Serial.println(moisturePercentageValue);
  delay(1000);
  if (moisturePercentageValue >  50) {
    digitalWrite(output, HIGH);
  }
};
