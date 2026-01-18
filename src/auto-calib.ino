// code for auto calibrating system

#include <LiquidCrystal.h>
#include <DHT.h>

// 1. LCD Setup: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// 2. Sensor & Fan Setup
#define DHTPIN 8
#define DHTTYPE DHT11
#define FAN_PIN 9
#define SWITCH_SENSE_PIN A0 

DHT dht(DHTPIN, DHTTYPE);

// 3. Variables
float baseTemp = 0.0;        // Will be set during setup()
const float tempStep = 0.20; // 0.2C increase per level
float stableSwitchVal = 1023.0; // Filtered switch value

void setup() {
  dht.begin();
  lcd.begin(16, 2); 
  pinMode(FAN_PIN, OUTPUT);
  pinMode(SWITCH_SENSE_PIN, INPUT);
  
  // --- AUTO-CALIBRATION PHASE ---
  lcd.print("Calibrating...");
  float totalTemp = 0;
  
  // Take 5 samples over 4 seconds to find the room's average temperature
  for(int i = 0; i < 5; i++) {
    delay(800);
    float currentT = dht.readTemperature();
    if(!isnan(currentT)) {
      totalTemp += currentT;
    }
    lcd.setCursor(0, 1);
    lcd.print("Sample "); lcd.print(i+1);
  }
  
  // Set the base temperature to 0.5°C above the current room temperature
  baseTemp = (totalTemp / 5.0) + 0.5; 
  
  lcd.clear();
  lcd.print("Base Set To:");
  lcd.setCursor(0, 1);
  lcd.print(baseTemp); lcd.print("C");
  delay(2000);
  lcd.clear();
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  
  // 4. Manual Switch Stability Filter
  int rawSwitch = analogRead(SWITCH_SENSE_PIN);
  stableSwitchVal = (stableSwitchVal * 0.85) + (rawSwitch * 0.15);
  
  if (isnan(temp)) {
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!   ");
    return;
  }

  int fanLevel = 0;
  int pwmValue = 0;

  // 5. Fan Scaling Logic (1-5)
  if (temp >= baseTemp) {
    fanLevel = (int)((temp - baseTemp) / tempStep) + 1;
    if (fanLevel > 5) fanLevel = 5;
    
    // Map levels to PWM (80 is the starting floor for the motor)
    pwmValue = map(fanLevel, 1, 5, 80, 255); 
  }

  // Override: If physical switch is OFF, force motor to stop
  if (stableSwitchVal < 100) {
    pwmValue = 0;
  }

  analogWrite(FAN_PIN, pwmValue);

  // --- LCD Display ---
  // Line 1: Real-time Data
  lcd.setCursor(0, 0);
  lcd.print("T:"); lcd.print(temp, 1);
  lcd.print("C H:"); lcd.print((int)hum);
  lcd.print("%   ");

  // Line 2: Status Logic
  lcd.setCursor(0, 1);
  if (stableSwitchVal < 100) { 
    lcd.print("MANUAL OFF     "); // Switch is cut
  } 
  else if (fanLevel == 0) {
    lcd.print("Fan: Standby   "); // Switch is on, but room is cool
  } 
  else {
    lcd.print("Level: ");
    lcd.print(fanLevel);
    lcd.print("/5      ");      // Fan is running
  }

  delay(200); 
}
