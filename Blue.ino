#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// Define GPIO pins for LEDs
const int ledPin1 = 5;  
const int ledPin2 = 7;  
const int ledPin3 = 8; 
const int analogPin = 34; 

float temperature = 0.0;
String ledStatus = "";
String condition = "";

// Function to simulate temperature from analog input
float readTemperature() {
  int analogValue = analogRead(analogPin);
  float temp = (analogValue / 4095.0) * 100; // Simulate temperature (0-100°C)
  return temp;
}

// Function to update LED status based on temperature
void updateLEDs(float temp) {
  if (temp < 33) {
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, LOW);
      ledStatus = "LED1 OFF, LED2 OFF, LED3 OFF";
      condition = "It's kinda cold in here.";
      
    } else if (temp >= 33 && temp < 50) {
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, HIGH);
      ledStatus = "LED1 OFF, LED2 OFF, LED3 ON";
      condition = "Nice and cozy";

    } else if (temp >= 50 && temp < 66) {
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, HIGH);
      digitalWrite(ledPin3, HIGH);
      ledStatus = "LED1 OFF, LED2 ON, LED3 ON";
      condition = "Getting a bit hot.";
      
    } else {
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, HIGH);
      digitalWrite(ledPin3, HIGH);
      ledStatus = "LED1 ON, LED2 ON, LED3 ON";
      condition = "I'M BURNING";
      //printf(temp);
    }
}

void setup() {
  Serial.begin(115200);

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  // Start Classic Bluetooth
  SerialBT.begin("ESP32"); 
  Serial.println("The device started, now you can pair it with Bluetooth!");
}

void loop() {
  // Read temperature
  temperature = readTemperature();
  updateLEDs(temperature);

  // Prepare data string
  String data = "Temperature: " + String(temperature, 1) + "C, " + ledStatus + "              " + condition;
  SerialBT.println(data); 
  Serial.println(data);

  delay(100); 
}

