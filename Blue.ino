#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// Define GPIO pins

const int ledPin1 = 5;  
const int ledPin2 = 7; 
const int ledPin3 = 8; 
const int analogPin = 34; /
// BLE Characteristics
BLECharacteristic *pCharacteristic;
float temperature = 0.0;
String ledStatus = "";
String condition = "";

// BLE UUIDs
#define SERVICE_UUID           "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID    "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// Function to simulate temperature from analog input
float readTemperature() {
  int analogValue = analogRead(analogPin);
  float temp = (analogValue / 4095.0) * 100; // Simulate temperature (0-100°C)
  return temp;
}



// Function to update LED status based on temperature
void updateLEDs(float temp) 
{
  
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

// BLE callback
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
  }
  void onDisconnect(BLEServer* pServer) {
  }
};

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize GPIO pins for LEDs
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  // Initialize BLE
  BLEDevice::init("ESP32 Temperature Monitor");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  pCharacteristic->addDescriptor(new BLE2902());

  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();
}

void loop() {
  // Read and calculate temperature
  temperature = readTemperature();
  updateLEDs(temperature);

  // Send data over BLE
  String data = "Temperature: " + String(temperature, 1) + "C, " + ledStatus + "              " + condition;
  pCharacteristic->setValue(data.c_str());
  pCharacteristic->notify();


  SerialBT.flush();
  Serial.println(data);
  delay(1000);
}
