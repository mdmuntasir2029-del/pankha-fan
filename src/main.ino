#include <LiquidCrystal.h>
#include <DHT.h>

// LCD Setup: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define DHTPIN 8
#define DHTTYPE DHT11
#define FAN_PIN 9
#define SWITCH_SENSE_PIN A0 

DHT dht(DHTPIN, DHTTYPE);

const float baseTemp = 25.0 ;   
const float tempStep = 0.20;   

void setup() {
  dht.begin();
  lcd.begin(16, 2); 
  pinMode(FAN_PIN, OUTPUT);
  pinMode(SWITCH_SENSE_PIN, INPUT);
  
  lcd.print("System Booting...");
  delay(1500);
  lcd.clear();
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  
  // Read the voltage after the manual switch
  int switchVal = analogRead(SWITCH_SENSE_PIN);
  
  if (isnan(temp)) {
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!   ");
    return;
  }

  int fanLevel = 0;
  int pwmValue = 0;

  // Calculate desired speed based on Temp
  if (temp >= baseTemp) {
    fanLevel = (int)((temp - baseTemp) / tempStep) + 1;
    if (fanLevel > 5) fanLevel = 5;
    pwmValue = map(fanLevel, 1, 5, 80, 255); 
  }

  // Apply PWM to the transistor
  analogWrite(FAN_PIN, pwmValue);

  // LINE 1: Data Display
  lcd.setCursor(0, 0);
  lcd.print("T:"); lcd.print(temp, 1);
  lcd.print("C H:"); lcd.print((int)hum);
  lcd.print("%   ");

  // LINE 2: Status Display
  lcd.setCursor(0, 1);
  
  // 1. Check if the physical switch is OFF
  // (Assuming A0 is connected to the motor's positive terminal)
  if (switchVal < 50) { 
    lcd.print("MANUAL OFF     ");
  } 
  // 2. If switch is ON, check if we are below the temp threshold
  else if (fanLevel == 0) {
    lcd.print("Fan: Standby   ");
  } 
  // 3. If switch is ON and it's hot enough, show the level
  else {
    lcd.print("Level: ");
    lcd.print(fanLevel);
    lcd.print("/5      ");
  }

  delay(500); 
}
