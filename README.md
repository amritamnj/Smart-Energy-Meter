## **📌 Smart Energy Consumption Meter**
### *An IoT-based system for real-time energy monitoring and efficiency tracking*

### **🔹 Overview**
The **Smart Energy Consumption Meter** is an IoT-based system designed to help users **monitor and manage energy consumption in real time**. The system uses an **ESP32 microcontroller**, sensors, and cloud platforms (**Firebase & Blynk**) to track **voltage, current, and power usage**. It provides **alerts** when voltage exceeds a safe threshold and allows users to **remotely control devices** via the Blynk dashboard.

---

### **📜 Features**
✅ **Real-time Energy Monitoring** – Measures **voltage, current, and power usage** dynamically.  
✅ **Cloud Storage with Firebase** – Stores and retrieves historical energy data.  
✅ **Blynk Dashboard Integration** – Displays real-time readings & allows remote relay control.  
✅ **Overvoltage Alerts** – Sends notifications via Blynk when voltage exceeds **18V**.  
✅ **LCD Display (Waveshare 1602)** – Shows **live current, voltage, and power values**.  
✅ **Remote Device Control** – Users can **turn off devices remotely** to save energy.  
✅ **Data Export to Excel** – Converts Firebase logs into **Excel format** for analysis.

---

### **🛠️ Hardware Components**
| Component | Purpose |
|------------|------------|
| **ESP32-S3** | Microcontroller for data collection and cloud communication |
| **AC Current Sensor** | Measures current consumption |
| **Voltage Divider Circuit** | Adjusts voltage readings for ESP32 |
| **Relay Module** | Controls appliances remotely via Blynk |
| **16x2 LCD Display** | Shows real-time current, voltage, and power values |
| **Capacitor (470μF)** | Stabilizes voltage readings |
| **Power Source (12V AC)** | Simulates real household electricity |

---

### **💻 Software & Cloud Integration**
📌 **Programming Languages:**  
- Arduino C++ (for ESP32 firmware)  
- Python (for Firebase data extraction and visualization)  

📌 **Cloud Platforms:**  
- **Firebase** – Stores historical energy consumption data.  
- **Blynk** – Provides a **mobile & web dashboard** for real-time monitoring & control.  

📌 **Data Processing:**  
- The ESP32 reads sensor data and uploads it to **Firebase** every **5 seconds**.  
- A **Python script** extracts data from Firebase and converts it into **Excel** format for visualization.  

---

### **📷 System Architecture**
🔗 **Hardware Setup:**  
- The system consists of **5 bulbs (0.3A each) connected in parallel** to simulate energy consumption.  
- **ESP32 reads voltage and current** and sends the data to Firebase & Blynk.  
- The **relay module allows remote control** of the bulbs via the Blynk app.  
- An **LCD display** shows real-time sensor values.  

🔗 **Software Flow:**  
- The ESP32 continuously **measures current, voltage, and power**.  
- If **voltage exceeds 18V**, an **alert is triggered** on Blynk.  
- Data is **logged in Firebase** with timestamps for historical analysis.  
- Users can **turn off the circuit remotely** using the Blynk dashboard.  

---

### **🚀 Getting Started**
#### **1️⃣ Clone the Repository**
```sh
git clone https://github.com/amritamnj/Smart-Energy-Meter.git
cd Smart-Energy-Meter
```

#### **2️⃣ Add Your Credentials (`secrets.h`)**
Before running the code, create a `secrets.h` file to store your **Firebase and Blynk credentials**:
```cpp
#ifndef SECRETS_H
#define SECRETS_H

#define FIREBASE_HOST "https://your-firebase-db.firebaseio.com/"
#define FIREBASE_AUTH "your-firebase-auth-token"
#define BLYNK_AUTH "your-blynk-auth-token"
#define WIFI_SSID "your-wifi-ssid"
#define WIFI_PASS "your-wifi-password"

#endif
```
📌 **Important:** Do **not** upload `secrets.h` to GitHub. Add it to `.gitignore` to keep your credentials safe.

#### **3️⃣ Flash the ESP32**
- Open the **Arduino IDE** and install the required libraries (`FirebaseESP32.h`, `BlynkSimpleEsp32.h`, etc.).
- Upload `smart_energy_meter.ino` to the ESP32.
- Ensure the ESP32 is **connected to WiFi**.

#### **4️⃣ View Data on Blynk & Firebase**
- Open the **Blynk app** and check **real-time sensor values**.
- Log in to **Firebase Realtime Database** to see historical readings.

#### **5️⃣ Export Data to Excel**
- Run the `firebase_json_to_excel.py` script to convert Firebase data into an **Excel file**.
```sh
python firebase_json_to_excel.py
```
---

### **📊 Performance Evaluation**
| Parameter | Expected Value | Measured Value (Avg) | Accuracy (%) |
|-----------|---------------|----------------------|--------------|
| **Current (A)** | 1.5A | 1.357A | 90.5% |
| **Voltage (V)** | 12V | 15.12V | 73.99% |
| **Power (W)** | 18W | 20.03W | 88.67% |

📌 **Observations:**
- **Current measurement achieved high accuracy (90.5%)**.
- **Voltage accuracy (73.99%) needs calibration** due to minor fluctuations.
- **Power tracking performed well (88.67%)**, with minor variations.

---

### **🔮 Future Improvements**
✅ **Improve voltage measurement accuracy** by fine-tuning the voltage divider circuit.  
✅ **Adapt for AC mains usage** to monitor real household electricity consumption.  
✅ **Integrate AI-based energy recommendations** for better efficiency tracking.  
✅ **Optimize ESP32 power consumption** by implementing sleep modes.  

---

### **📜 License & Credits**
This project was developed as part of the **CITS5506 Internet of Things coursework** at **UWA**.  
📌 **Team Members:**  
👩‍💻 **Amrita Manoj (23981621)**  
👨‍💻 **Jiahe Fan (24014534)**  
👨‍💻 **Thanh Vinh Tong (22800814)**  
👨‍💻 **Zaid Sayed (23882963)**  
