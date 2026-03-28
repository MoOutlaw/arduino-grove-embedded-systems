# 🔌 Arduino Embedded Systems Project — Group 26

A 3-task embedded systems project built using the **Grove Beginner Kit for Arduino**, completed as part of the DG1FDS (Fundamentals of Digital Systems) module at Aston University.

## 🛠 Hardware & Tech Stack
- **Board:** Arduino Uno (Grove Beginner Kit)
- **Language:** C++ (Arduino)
- **Sensors:** DHT20 (temperature/humidity), BMP280 (air pressure), Light sensor
- **Output:** SSD1306 OLED display, LEDs, Buzzer
- **Libraries:** `Wire.h`, `DHT20`, `Seeed_BMP280`, `U8g2lib`

---

## 📋 Tasks

### Task 1 — Mini Weather Station
**Folder:** `task1-weather-station/`

Monitors temperature, humidity, and air pressure in real time using the DHT20 and BMP280 sensors. Readings are displayed on an OLED screen. A buzzer triggers an alert when temperature exceeds a user-adjustable threshold set via potentiometer.

**Extensions implemented:**
- Temperature bar graph on OLED
- Dynamic weather icons (sun / cloud / droplet) based on conditions
- Potentiometer-controlled alarm threshold (15°C – 30°C range)
- Memory-optimised OLED rendering using U8g2 page-buffer mode

---

### Task 2 — Smart Night-Light System
**Folder:** `task2-night-light/`

An automatic light system that monitors ambient light levels. When light drops below a set threshold, the onboard LED turns on automatically — simulating a real-world night-light. Status is shown on the OLED display.

---

### Task 3 — Mini Pedestrian Crossing System
**Folder:** `task3-pedestrian-crossing/`

Simulates a pedestrian traffic crossing. The system starts in a STOP state (RED LED + "STOP" on OLED). When the button is pressed, it transitions to a GO state (GREEN LED + "GO" on OLED) before returning to STOP after a set interval.

---

## 🚀 How to Use

1. Install the [Arduino IDE](https://www.arduino.cc/en/software)
2. Install required libraries via **Sketch → Include Library → Manage Libraries:**
   - `DHT20` by Seeed Studio
   - `Grove BMP280` by Seeed Studio
   - `U8g2` by oliver
3. Clone this repository:
```bash
   git clone https://github.com/USERNAME/arduino-grove-embedded-systems.git
```
4. Open the `.ino` file for the task you want to run (e.g. `task1-weather-station/weather_station.ino`)
5. Connect your Grove Beginner Kit via USB
6. Select **Tools → Board → Arduino Uno** and the correct port
7. Click **Upload**

## 📂 Repository Structure
```
├── task1-weather-station/       # Weather monitoring with sensor alerts
├── task2-night-light/           # Ambient light-triggered LED system
├── task3-pedestrian-crossing/   # Button-controlled crossing simulation
├── docs/                        # Full project report (PDF)
└── README.md
```

## 👥 Team — Group 26
- Mohammed Qasem
- Huyen Khanh Nung
- Victor Emerem
- Japheth Mayeye
- Josheph Hughes

## 🎓 Context
**Module:** DG1FDS — Fundamentals of Digital Systems
**Institution:** Aston University
**Year:** 1st Year BSc Computer Science
