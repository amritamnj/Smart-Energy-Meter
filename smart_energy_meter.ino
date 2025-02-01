#define V_REF 3.3            // Reference voltage
#define ADC_RESOLUTION 4096  // ADC resolution (12-bit)
#define CALIBRATION 5.0      // Calibration constant for the sensor

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <FirebaseESP32.h>
#include "Waveshare_LCD1602.h"
#include "secrets.h"  // Include credentials from a separate file

// Firebase Configuration
FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth authFirebase;

// Hardware pins
const int sensorPin = A2;        
const int voltagePin = D8;       
const int relayPin = D0;         
Waveshare_LCD1602 lcd(16, 2);    

unsigned long previousMillisLCD = 0;  
unsigned long previousMillisBlynk = 0;  
unsigned long lastSendTime = 0;  // Variable to store the last time data was sent
const long intervalLCD = 5000;         // Interval of 5 seconds
const long intervalBlynk = 5000;       // Interval of 1 second
const long sendInterval = 5000; // 15 seconds in milliseconds
int displayState = 0;              
unsigned long warningStartTime = 0;
bool isInWarningRange = false;

void setup() {
  Serial.begin(115200);           
  Wire.begin(5, 6);               

  // Initialize LCD
  lcd.init();
  lcd.setCursor(0, 0);
  lcd.send_string("Smart Energy Meter");
  lcd.setCursor(0, 1);
  lcd.send_string("Connecting...");

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Firebase configuration
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  // Initialize Firebase
  Firebase.begin(&config, &authFirebase);
  Firebase.reconnectWiFi(true);

  // Start Blynk
  Blynk.begin(BLYNK_AUTH, WIFI_SSID, WIFI_PASS);
  pinMode(relayPin, OUTPUT);
}

BLYNK_WRITE(V4) {
  int buttonState = param.asInt();
  digitalWrite(relayPin, buttonState == 0 ? HIGH : LOW);
}

bool notificationSent = false;

void storeDataToFirebase(float current, float voltageValue, float power) {
  // Get the current timestamp
  unsigned long timestamp = millis();

  // Store data in Firebase
  Firebase.setFloat(firebaseData, "/SmartEnergy/" + String(timestamp) + "/current", current);
  Firebase.setFloat(firebaseData, "/SmartEnergy/" + String(timestamp) + "/voltage", voltageValue);
  Firebase.setFloat(firebaseData, "/SmartEnergy/" + String(timestamp) + "/power", power);
}

void loop() {
  unsigned long currentMillis = millis();

  int rawValue = analogRead(sensorPin);
  float voltage = rawValue * (V_REF / ADC_RESOLUTION);
  float current = voltage * CALIBRATION;

  int voltageRawValue = analogRead(voltagePin);
  float voltageValue = voltageRawValue * (V_REF / ADC_RESOLUTION) * 6;

  float power = voltageValue * current;

  if (voltageValue > 15.0 && voltageValue < 17.0) {
    if (!isInWarningRange) {
      isInWarningRange = true;
      warningStartTime = millis();
    } else if (millis() - warningStartTime >= 60000) {
      Blynk.logEvent("reminder", "Reminder: Voltage remains between 15.0V and 17.0V!");
      isInWarningRange = false;
    }
  } else {
    isInWarningRange = false;
  }

  if (voltageValue > 18.0 && !notificationSent) {
      Serial.println("High voltage detected!");
      Blynk.logEvent("voltagedetected", "High voltage detected! Please be cautious.");
      notificationSent = true;
  } else if (voltageValue <= 18.0) {
      notificationSent = false;
  }

  // Send data to Firebase every 15 seconds
  if (currentMillis - lastSendTime >= sendInterval) {
    lastSendTime = currentMillis;  // Update the last send time
    Serial.println("Sending data to Firebase...");
    storeDataToFirebase(current, voltageValue, power);
  }

  // Update LCD display every 5 seconds
  if (currentMillis - previousMillisLCD >= intervalLCD) {
    previousMillisLCD = currentMillis;
    lcd.clear();

    if (displayState == 0) {
      lcd.setCursor(0, 0);
      lcd.send_string("Current:");
      lcd.setCursor(0, 1);
      lcd.send_string(String(current).c_str());
      lcd.send_string(" A");
    } else if (displayState == 1) {
      lcd.setCursor(0, 0);
      lcd.send_string("Voltage:");
      lcd.setCursor(0, 1);
      lcd.send_string(String(voltageValue).c_str());
      lcd.send_string(" V");
    } else if (displayState == 2) {
      lcd.setCursor(0, 0);
      lcd.send_string("Power:");
      lcd.setCursor(0, 1);
      lcd.send_string(String(power).c_str());
      lcd.send_string(" W");
    }

    displayState = (displayState + 1) % 3;
  }

  // Send data to Blynk every second
  if (currentMillis - previousMillisBlynk >= intervalBlynk) {
    previousMillisBlynk = currentMillis;
    Blynk.virtualWrite(V1, current);
    Blynk.virtualWrite(V2, voltageValue);
    Blynk.virtualWrite(V3, power);

    Serial.print("Current: ");
    Serial.print(current);
    Serial.print(" Voltage: ");
    Serial.print(voltageValue);
    Serial.print(" Power: ");
    Serial.println(power);
  }

  Blynk.run();
}
