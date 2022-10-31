// including the necessary libraries
#include <LiquidCrystal_I2C.h>
#include <dht.h>

// instantiating a dht library object
dht DHT;



// initializing the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
LiquidCrystal_I2C lcd(0x27, 16, 2);


// assigning some to pin numbers
int moistureSensor = A0;
int relaySwitch = 9;
#define DHT11_PIN 8

// creating variables
int moistureContent;
float moisturePercentageValue;
bool waterAvailable = HIGH; 
float temp;
float humidity;

// values gotten from calibration
float drySoilMoisturePercentage = 50.0;
float wetSoilMoisturePercentage = 80.0;
float realSoilMoisturePercentage;


void setup() {
  // setting up the LCD's number of columns and rows:
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(relaySwitch, OUTPUT);
  digitalWrite(relaySwitch, LOW);

  // read moisture content value from moisture sensor
  moistureContent = analogRead(moistureSensor);
  moisturePercentageValue = 100 - ((moistureContent/1023.0) * 100); // since we are using a simulator to simulate the soil we assume the maximum moisture content value which can be read is 1023
}

void loop() {
  // reading the sensors
  moistureContent = analogRead(moistureSensor);
  moisturePercentageValue = 100 - ((moistureContent/1023.0) * 100); // since we are using a simulator to simulate the soil we assume the maximum moisture content value which can be read is 1023

  realSoilMoisturePercentage = map(moisturePercentageValue, drySoilMoisturePercentage, wetSoilMoisturePercentage, 0, 100);

  Serial.println(realSoilMoisturePercentage);

  // temperature and humidity
  DHT.read11(DHT11_PIN);
  humidity = DHT.humidity;
  temp = DHT.temperature;
  
  // check if any reads failed and exit early
  if (isnan(humidity) || isnan(temp) || isnan(moistureContent)) {
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Error In Reading");
    return;
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temp);
    lcd.print("*C");
    lcd.setCursor(0, 1);
    lcd.print("Moisture %: ");
    lcd.print(realSoilMoisturePercentage);
    lcd.print("%");
    if (realSoilMoisturePercentage < 54)   // for dry soil, threshold is equal to 54%
    { 
        lcd.setCursor(0, 2);
        lcd.print("SOIL: DRY");

        // check for the availability of water
        if (waterAvailable == HIGH){
          digitalWrite(relaySwitch, HIGH);
          Serial.println("Relay ON");
          lcd.setCursor(0, 3);
//          lcd.print("PUMP: ON");
        } else {
          digitalWrite(relaySwitch, LOW);
          lcd.setCursor(0, 3);
          lcd.print("NO WATER, PUMP: OFF");
        }
    }
 
    else if (realSoilMoisturePercentage >= 54) // for moist soil
    { 
        lcd.setCursor(0, 2);
//        lcd.print("SOIL: MOIST");
        digitalWrite(relaySwitch, LOW);
        lcd.setCursor(0, 3);
//        lcd.print("PUMP: OFF");
    }
    delay(1000);  
  }
}
