// ===== Mini Weather Station (Grove Beginner Kit) ===== 

// TEAM GROUP: 26 

// 

// This program measures temperature, humidity, and air pressure using the 

// Grove Beginner Kit. It displays live values on an OLED screen and triggers 

// a buzzer alarm if the temperature rises above a user‑adjustable threshold. 

// 

// EXTENSIONS INCLUDED: 

// - Temperature bar graph 

// - Dynamic weather icons (sun / cloud / droplet) 

// - Potentiometer‑controlled temperature alert threshold 

// - Memory‑efficient OLED output using U8g2 page mode 

 

/* --------------------------------------------------------------------------- 

   LIBRARIES USED 

--------------------------------------------------------------------------- */ 

#include <Wire.h> 

#include "DHT20.h" 

#include "Seeed_BMP280.h" 

#include <U8g2lib.h> 

 

/* --------------------------------------------------------------------------- 

   PIN DEFINITIONS   

--------------------------------------------------------------------------- */ 

#define BUZZER_PIN 5 

#define POT_PIN    A0 

 

/* --------------------------------------------------------------------------- 

   SENSOR OBJECTS 

--------------------------------------------------------------------------- */ 

DHT20 dht20;      // Correct object for Grove DHT20 

BMP280 bmp280; 

 

/* --------------------------------------------------------------------------- 

   OLED DISPLAY SETUP 

--------------------------------------------------------------------------- */ 

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2( 

  U8G2_R2, U8X8_PIN_NONE 

); 

 

/* --------------------------------------------------------------------------- 

   VARIABLES 

--------------------------------------------------------------------------- */ 

float tempC = NAN, humidity = NAN, pressure_hPa = NAN; 

float tempThresholdC = 30.0; 

 

const float THRESH_MIN = 15.0; 

const float THRESH_MAX = 30.0; 

 

const float TEMP_MIN = 0.0; 

const float TEMP_MAX = 30.0; 

 

const unsigned long BEEP_PERIOD_MS = 1000; 

const unsigned long BEEP_ON_MS     = 200; 

unsigned long t0 = 0; 

 

char lineBuf[24]; 

char numBuf[10]; 

 

/* --------------------------------------------------------------------------- 

   SETUP 

--------------------------------------------------------------------------- */ 

void setup() { 

  Serial.begin(115200); 

 

  u8g2.begin(); 

 

  dht20.begin();        // Correct start method 

 

  if (!bmp280.init()) { 

    Serial.println("BMP280 init failed."); 

  } 

 

  pinMode(BUZZER_PIN, OUTPUT); 

 

  u8g2.firstPage(); 

  do { 

    u8g2.setFont(u8g2_font_6x12_tf); 

    u8g2.drawStr(0, 14, "Grove Mini Weather"); 

    u8g2.drawStr(0, 30, "DHT20 + BMP280 + OLED"); 

    u8g2.drawStr(0, 46, "Pot sets alarm thresh"); 

  } while (u8g2.nextPage()); 

 

  delay(800); 

} 

 

/* --------------------------------------------------------------------------- 

   LOOP 

--------------------------------------------------------------------------- */ 

void loop() { 

 

  readSensors(); 

 

  int potValue = analogRead(POT_PIN); 

  tempThresholdC = mapFloat(potValue, 0, 1023, THRESH_MIN, THRESH_MAX); 

 

  updateAlarm(); 

 

  drawUI(); 

 

  delay(100); 

} 

 

/* --------------------------------------------------------------------------- 

   SENSOR READING 

--------------------------------------------------------------------------- */ 

void readSensors() { 

 

  dht20.read();                    // Correct method for DHT20 

 

  tempC = dht20.getTemperature();  // access correct values 

  humidity = dht20.getHumidity(); 

 

  float pPa = bmp280.getPressure(); 

  pressure_hPa = pPa / 100.0; 

 

  if (isnan(tempC) || isnan(humidity)) { 

    Serial.println("DHT20 read failed."); 

  } 

} 

 

/* --------------------------------------------------------------------------- 

   BUZZER LOGIC 

--------------------------------------------------------------------------- */ 

void updateAlarm() { 

 

  unsigned long now = millis(); 

  bool alarmActive = (!isnan(tempC) && tempC > tempThresholdC); 

 

  if (alarmActive) { 

 

    unsigned long phase = (now - t0) % BEEP_PERIOD_MS; 

 

    if (phase < BEEP_ON_MS) { 

      tone(BUZZER_PIN, 1000); 

    } else { 

      noTone(BUZZER_PIN); 

    } 

 

  } else { 

    noTone(BUZZER_PIN); 

    t0 = now; 

  } 

} 

 

/* --------------------------------------------------------------------------- 

   OLED UI 

--------------------------------------------------------------------------- */ 

void drawUI() { 

 

  u8g2.firstPage(); 

  do { 

 

    u8g2.setFont(u8g2_font_6x12_tf); 

 

    u8g2.drawStr(0, 10, "Mini Weather Station"); 

 

    formatFloat(tempC, 1, numBuf, sizeof(numBuf), "--.-"); 

    snprintf(lineBuf, sizeof(lineBuf), "Temp: %s C", numBuf); 

    u8g2.drawStr(0, 24, lineBuf); 

 

    formatFloat(humidity, 1, numBuf, sizeof(numBuf), "--.-"); 

    snprintf(lineBuf, sizeof(lineBuf), "Humidity: %s %%", numBuf); 

    u8g2.drawStr(0, 36, lineBuf); 

 

    formatFloat(pressure_hPa, 1, numBuf, sizeof(numBuf), "----"); 

    snprintf(lineBuf, sizeof(lineBuf), "Pressure: %s hPa", numBuf); 

    u8g2.drawStr(0, 48, lineBuf); 

 

    formatFloat(tempThresholdC, 1, numBuf, sizeof(numBuf), "--.-"); 

    snprintf(lineBuf, sizeof(lineBuf), "Thresh: %s C", numBuf); 

    u8g2.drawStr(0, 60, lineBuf); 

 

    if (!isnan(humidity) && humidity >= 70.0) { 

      drawDropletIcon(108, 0); 

    } else if (!isnan(tempC) && tempC >= tempThresholdC) { 

      drawSunIcon(108, 0); 

    } else { 

      drawCloudIcon(108, 2); 

    } 

 

    drawTempBar(0, 52, 128, 12); 

 

  } while (u8g2.nextPage()); 

} 

 

/* --------------------------------------------------------------------------- 

   BAR GRAPH 

--------------------------------------------------------------------------- */ 

void drawTempBar(int x, int y, int w, int h) { 

 

  u8g2.drawFrame(x, y, w, h); 

 

  float t = tempC; 

  if (isnan(t)) t = TEMP_MIN; 

  t = constrain(t, TEMP_MIN, TEMP_MAX); 

 

  int fillW = (int)((t - TEMP_MIN) * (w - 2) / (TEMP_MAX - TEMP_MIN)); 

 

  u8g2.drawBox(x + 1, y + 1, fillW, h - 2); 

 

  for (int tick = 10; tick <= 40; tick += 10) { 

    int tx = x + 1 + (tick - TEMP_MIN) * (w - 2) / (TEMP_MAX - TEMP_MIN); 

    u8g2.drawVLine(tx, y, 3); 

  } 

} 

 

/* --------------------------------------------------------------------------- 

   ICONS 

--------------------------------------------------------------------------- */ 

void drawSunIcon(int x, int y) { 

  int cx = x + 10, cy = y + 10, r = 6; 

  u8g2.drawCircle(cx, cy, r, U8G2_DRAW_ALL); 

  u8g2.drawLine(cx - 10, cy, cx - 6, cy); 

  u8g2.drawLine(cx + 6, cy, cx + 10, cy); 

  u8g2.drawLine(cx, cy - 10, cx, cy - 6); 

  u8g2.drawLine(cx, cy + 6, cx, cy + 10); 

} 

 

void drawCloudIcon(int x, int y) { 

  u8g2.drawCircle(x + 6,  y + 8, 6, U8G2_DRAW_ALL); 

  u8g2.drawCircle(x + 14, y + 6, 8, U8G2_DRAW_ALL); 

  u8g2.drawCircle(x + 22, y + 9, 5, U8G2_DRAW_ALL); 

  for (int i = 8; i <= 14; ++i) { 

    u8g2.drawHLine(x + 2, y + i, 24); 

  } 

} 

void drawDropletIcon(int x, int y) { 

  int cx = x + 10, cy = y + 12; 

  u8g2.drawLine(cx, y, cx - 6, cy - 4); 

  u8g2.drawLine(cx, y, cx + 6, cy - 4); 

} 

/* --------------------------------------------------------------------------- 

   HELPERS 

--------------------------------------------------------------------------- */ 

void formatFloat(float v, uint8_t decimals, char* out, size_t outLen, const char* fallback) { 

  if (isnan(v)) { 

    strncpy(out, fallback, outLen); 

    out[outLen - 1] = '\0'; 

  } else { 

    dtostrf(v, 0, decimals, out); 

  } 

} 


float mapFloat(long x, long in_min, long in_max, float out_min, float out_max) { 

  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min; 

} 