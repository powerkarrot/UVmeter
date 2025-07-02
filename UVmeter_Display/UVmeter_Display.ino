#include "SevSeg.h" 
SevSeg sevseg; 

const long interval = 200; // refresh display every second
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  byte numDigits = 4;
  byte digitPins[] = {2, 3, 4, 5};
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_ANODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
}

/*
UV Meter with GUVA-S12SD UV Sensor 
Module: https://wiki.dfrobot.com/UV_sensor_SKU__SEN0162
Sensor: https://cdn-shop.adafruit.com/datasheets/1918guva.pdf

Helpful information from the Adafruit model: https://www.adafruit.com/product/1918
  --> "The output voltage is: Vo = 4.3 * Diode-Current-in-uA. So if the photocurrent is 1uA (9 mW/cm^2), the output voltage is 4.3V. 
      You can also convert the voltage to UV Index by dividing the output voltage by 0.1V. So if the output voltage is 0.5V, the UV Index is about 5."

Helpful information on UV measurements with Arduino: https://instesre.org/ArduinoBook/UVmeasurements.pdf
*/

void loop() {

  float uvi;
  float mW2;
  float analogValue = float(analogRead(A0)) ;
  float sensorVoltage;
  float sensorVoltage2;
  float mV;
  float uWcm2;
  unsigned long currentMillis = millis();
  float averagedSensor;
  unsigned int sval = 0;
  
  sval += analogValue;
  
  // Average out sensor value
  if (currentMillis - previousMillis2 >= 100) {
    previousMillis2 = currentMillis;
    averagedSensor = sval;
  }

  // Update display every #interval ms
  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis;
  
    sensorVoltage = averagedSensor/1024.0*4.3; // takes account the photothingylingy from the GUVA
    sensorVoltage2 = averagedSensor * 5.0 / 1024.0;

    mV = (averagedSensor * 5.0 / 1024.0) * 1000; //milliVolts
    uvi = 0.05 * averagedSensor - 1.0; // UVI Rating
    mW2 = (sensorVoltage * 9.0) * 0.1; // uv intensity in microWatts / cm2
    uWcm2 = mW2 * 100; // uv intensity in macroWatts / cm2
   
  }

  sevseg.setNumberF(uWcm2,1); 
  sevseg.refreshDisplay(); 
  sevseg.setBrightness(100); 
}

