# pankha-fan
A fan that changes its speed according to surrounding factors; for now, only temperature.
I plan on adding humidity and human presence as factors later.
I also plan on adding an auto calibration system to make sure the fan adapts to different temperature and humidity trends.
For now, the fan turns on at 25 degrees Celsius, and for every 0.2 increase in temperature, the fan level increases by 1, until it reaches its maximum of 5.

Mainly built as an environment-friendly and cheap alternative cooling system. Ceiling fans dont have temperature sensing, do they? Air conditioners are too expensive, and even pollute the environment at often times.
Removes the hassle of needing to use a remote.

Components used: 
Arduino Uno R3
400-point breadboard
10K Potentiometer
1N4007 M7 SMD Diode
2N222 NPN Transistor
DC Fan 12V 3.1"
DHT11 Sensor Module?
9V Battery with connector
Switch
220Ω and 1kΩ Resistors (1x each)
Male-to-Male jumper wires


Software:
Arduino IDE for running code
Adafruit Unified Sensor
DHT sensor library
LiquidCrystal I2C library






