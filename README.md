# Arduino Embedded Systems Project — Group 26

A three-task embedded systems project built using the Grove Beginner Kit for Arduino, completed as part of the DG1FDS (Fundamentals of Digital Systems) module at Aston University. The project demonstrates practical application of sensor integration, real-time data processing, and hardware control using C++ and the Arduino framework.

---

## Module Context

**Module:** DG1FDS — Fundamentals of Digital Systems
**Institution:** Aston University
**Year:** 1st Year, BSc Computer Science
**Team:** Group 26

---

## Hardware & Tech Stack

**Board:** Arduino Uno (Grove Beginner Kit)
**Language:** C++ (Arduino)

**Sensors:**
- DHT20 — temperature and humidity
- BMP280 — atmospheric air pressure
- Light sensor — ambient light level detection

**Output Components:**
- SSD1306 OLED display
- LEDs (red and green)
- Buzzer
- Potentiometer

**Libraries:**
- `Wire.h` — I2C communication
- `DHT20` — temperature and humidity sensor driver
- `Seeed_BMP280` — air pressure sensor driver
- `U8g2lib` — OLED display rendering

---

## Project Tasks

### Task 1 — Mini Weather Station
**Folder:** `Task 1 Weather Station/`

Monitors temperature, humidity, and air pressure in real time using the DHT20 and BMP280 sensors. All readings are displayed on the OLED screen and updated continuously. A buzzer triggers an alert when the temperature exceeds a user-defined threshold, which can be adjusted live using the onboard potentiometer.

**Extensions implemented:**
- Temperature bar graph rendered on the OLED display
- Dynamic weather icons (sun, cloud, droplet) that update based on current conditions
- Potentiometer-controlled alarm threshold, adjustable between 15°C and 30°C
- Memory-optimised OLED rendering using U8g2 page-buffer mode to reduce RAM usage

---

### Task 2 — Smart Night-Light System
**Folder:** `Task 2 Night Light/`

An automatic lighting system that reads ambient light levels from the onboard light sensor. When the detected light level drops below a defined threshold, the LED activates automatically to simulate a real-world night-light. The current light level and system status are shown on the OLED display, giving clear feedback on the system state at all times.

---

### Task 3 — Mini Pedestrian Crossing System
**Folder:** `Task 3 pedestrian crossing/`

Simulates the logic of a pedestrian traffic crossing. The system initialises in a STOP state, indicated by a red LED and a "STOP" message on the OLED display. When the button is pressed, the system transitions to a GO state, activating the green LED and displaying "GO". After a fixed interval, the system automatically returns to the STOP state, completing the crossing cycle.

---

## Repository Structure

```
├── task1-weather-station/       # Weather monitoring with sensor alerts
├── task2-night-light/           # Ambient light-triggered LED system
├── task3-pedestrian-crossing/   # Button-controlled crossing simulation
├── docs/                        # Full project report (PDF)
└── README.md
```

---

## How to Use

**Prerequisites:**
- Arduino IDE installed on your machine
- Grove Beginner Kit for Arduino connected via USB

**Step 1 — Install required libraries**

Open Arduino IDE and go to Sketch → Include Library → Manage Libraries, then search for and install:
- `DHT20` by Seeed Studio
- `Grove BMP280` by Seeed Studio
- `U8g2` by oliver

**Step 2 — Clone the repository**

```bash
git clone https://github.com/MoOutlaw/arduino-grove-embedded-systems.git
```

**Step 3 — Open the task**

Navigate to the folder for the task you want to run and open the `.ino` file, for example:
`task1-weather-station/weather_station.ino`

**Step 4 — Configure and upload**

1. Go to Tools → Board and select Arduino Uno
2. Go to Tools → Port and select the correct COM port
3. Click Upload

The sketch will compile and upload to the board. Open the Serial Monitor if you want to observe debug output.

---

## Team — Group 26

- Mohammed Qasem
- Huyen Khanh Nung
- Victor Emerem
- Japheth Mayeye
- Josheph Hughes
