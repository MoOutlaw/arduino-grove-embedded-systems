/* 
  Smart Night-Light System ( Grove Beginner Kit)
  Team Group : 26

  Task Description: 
   this is an automatic night-light system that can turn on a 
  LED when ambient light falls below a set threshold.

  Components : 
   light Sensor (LDR)- A6
   Potentiometer - A0 (threshold adjustment)
   LED - D4 
   OLED Display-I2C (SSD1306,128*64)

  Extension (optional):
   PWM brightness control ( LED brightness varies with light level).
   OLED display showing real- time values and bar graph.
   Threshold marker : visual indicator on bar graph 
   Dynamic Icons : Sun ( day)/ Moon ( night)
*/

//===== Libraries =====
#include <Wire.h>     //I2C comunication for OLED 
#include <U8g2lib.h>    // OLED display library

//==== Pin =====

const int ledPin = 4;  // select the pin for the LED
const int light_sensor = A6;  // Light sensor

int light_Level = 0;   // Light value from sensor

const int potPin = A0; // potentiometer (threshold adjust)
int threshold = 0;

int ledBrightness = 0;  // PWM brightness value (0-255)
bool isDark= false;    // true when lightlevel < threshold

//==== OLED DISPLAY SETUP ======

// Using PAGE BUFFER MODE (_1_) to save precious RAM on Arduino Uno
// U8G2_R2 rotates screen 180° for correct viewing orientation on Grove Kit

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(
  U8G2_R2,  // rote 180 degrees
  /* reset = */ U8X8_PIN_NONE // no reset pin needed
);

//==== display scaling ====
const int light_min = 0; // minimum light value
const int light_max = 1023 ; // maximum light value
const int threshold_min = 0; // minimum threshold value
const int threshold_max = 1023; // maximum threshold value  

// Display buffer (reusable to save memory)
char lineBuffer[30];                    // For formatting text lines
char numberBuffer[10];                  // For formatting numbers

//===== funtion ====
void setup() {
  // declare the ledPin as an OUTPUT
  pinMode(ledPin, OUTPUT);
  analogWrite (ledPin,0);
  Serial.begin(9600);
  // initializes the serial port with a baud rate of 9600

  u8g2.begin();  // initializes OLED display
  displaySplashScreen();
  delay (2000);

}

void loop() {

  //  Read all sensors
  readSensors();
  
  // Process light control logic (with PWM)
  processLightControl();
  
  //Update OLED display
  updateDisplay();
  
  //Print debug information to Serial Monitor
  printDebugInfo();

  delay(200);  // delay in between reads for stability
}

void readSensors() {
  light_Level = analogRead(light_sensor); 
  // read light levl from LDR sensor ( 0 = dark , 1023 = bright)
  threshold = analogRead(potPin);
  // read threshold value from potentiomenter
}

void processLightControl() {

  if (light_Level < threshold) {

    isDark = true;

    // Calculate brightness based on darkness level
    int darknessLevel = threshold - light_Level; 
    // larger = darker
    ledBrightness = map(darknessLevel, 0, threshold, 0, 255);
    // this code  makes LED brighter if it's gets darker
    ledBrightness = constrain(ledBrightness, 0, 255);
    // this code ensure brightness stays within valid range (0-255)
    
  } else {
      isDark = false;
      ledBrightness = 0;
      // turn LED off - bright condition
  }

  analogWrite(ledPin, ledBrightness);
}

void updateDisplay() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x12_tf);
    // set font for all text
    
    // ----- Title Line -----
    u8g2.drawStr(0, 10, "Smart Night Light");
    
    // ----- Light Level Display -----
    formatNumber(light_Level, numberBuffer, sizeof(numberBuffer), "---");
    snprintf(lineBuffer, sizeof(lineBuffer), "Light: %s", numberBuffer);
    u8g2.drawStr(0, 24, lineBuffer);

    // ----- Threshold Display -----
    formatNumber(threshold, numberBuffer, sizeof(numberBuffer), "---");
    snprintf(lineBuffer, sizeof(lineBuffer), "Thres: %s", numberBuffer);
    u8g2.drawStr(0, 36, lineBuffer);
    
    // ----- LED Status & Brightness -----
    if (isDark) {
      formatNumber(ledBrightness, numberBuffer, sizeof(numberBuffer), "---");
      snprintf(lineBuffer, sizeof(lineBuffer), "LED: ON (%s)", numberBuffer);
    } else {
      snprintf(lineBuffer, sizeof(lineBuffer), "LED: OFF");
    }
    u8g2.drawStr(0, 48, lineBuffer);
    

    // ----- Status Icon (Top-right corner) -----
    // Sun icon when bright, Moon icon when dark
    if (isDark) {
      drawMoonIcon(100, 5);  // Moon icon for night mode
    } else {
      drawSunIcon(108, 5);    // Sun icon for day mode
    }
    
    // ===== BAR GRAPH VISUALIZATION (Optional Extension) =====
    // Draw bar graph showing light level relative to min/max
    drawLightBarGraph(0, 52, 128, 12);
    
    // Draw threshold marker on the bar graph
    drawThresholdMarker(0, 52, 128, 12);
    
  } while (u8g2.nextPage());
}

void drawLightBarGraph(int x, int y, int w, int h) {
  // Draw frame
  u8g2.drawFrame(x, y, w, h);
  
  // light level 
  int displayLight = constrain(light_Level, light_min, light_max);
  
  // Calculate fill width 
  int fillWidth = map(displayLight,light_min, light_max, 0, w - 2);
  
  // Draw bar chart
  if (fillWidth > 0) {
    u8g2.drawBox(x + 1, y + 1, fillWidth, h - 2);
  }
}

// Draw threshold marker on the bar graph
void drawThresholdMarker(int x, int y, int w, int h) {
  // Map threshold value 
  int markerX = map(threshold, threshold_min, threshold_max, x, x + w - 1);
  
  // Draw vertical line as marker
  u8g2.drawLine(markerX, y - 3, markerX, y + h + 2);
  
}

// ============================ ICON DRAWING FUNCTIONS ============================

// Draw sun icon (for bright conditions)
void drawSunIcon(int x, int y) {

  int cx = x + 10, cy = y + 10;

  // center of the Sun
  u8g2.drawDisc(cx, cy, 6, U8G2_DRAW_ALL);
  
  // Circle Face
  u8g2.setDrawColor(0);  // 0 = black
  
  // Draw black eye
  u8g2.drawDisc(cx - 3, cy - 3, 1, U8G2_DRAW_ALL);  // left eye
  u8g2.drawDisc(cx + 3, cy - 3, 1, U8G2_DRAW_ALL);  // right eye
  
  // draw smiling mouth (black)
    u8g2.drawLine(cx - 4, cy + 3, cx, cy + 5);
  u8g2.drawLine(cx, cy + 5, cx + 4, cy + 3);
  
  // change colour to white for sun rays 
  u8g2.setDrawColor(1);  // 1 = white

  // Draw 4 main sun rays (top, bottom, left, right)
  u8g2.drawLine(cx, cy - 10, cx, cy - 7);  // Top ray
  u8g2.drawLine(cx, cy + 7, cx, cy + 10);  // Bottom ray
  u8g2.drawLine(cx - 10, cy, cx - 7, cy);  // left ray
  u8g2.drawLine(cx + 7, cy, cx + 10, cy);  // Right ray
  
  // Draw 4 rays
  u8g2.drawLine(cx - 7, cy - 7, cx - 5, cy - 5);  // top left
  u8g2.drawLine(cx + 5, cy + 5, cx + 7, cy + 7);  // bottom right
  u8g2.drawLine(cx - 7, cy + 7, cx - 5, cy + 5);  // bottom left
  u8g2.drawLine(cx + 5, cy - 5, cx + 7, cy - 7);  // top right
}
// Draw moon icon (for dark conditions)
void drawMoonIcon(int x, int y) {

  // draw full moon
  u8g2.drawDisc(x + 10, y + 10, 7, U8G2_DRAW_ALL); 

  u8g2.setDrawColor(0);   // Set color to black 
  u8g2.drawDisc(x + 14, y + 8, 5, U8G2_DRAW_ALL);
  u8g2.setDrawColor(1);   // Restore to white
}

// ===== Helper =====

// Format numbers for display (handles invalid values)
void formatNumber(int value, char* buffer, size_t bufferSize, const char* invalidText) {
  if (value >= 0 && value <= 1023) {
    snprintf(buffer, bufferSize, "%d", value);
  } else {
    snprintf(buffer, bufferSize, "%s", invalidText);
  }
}

// Print debug information to Serial Monitor
void printDebugInfo() {
  Serial.print(F("Light: "));
  Serial.print(light_Level);
  Serial.print(F(" | Threshold: "));
  Serial.print(threshold);
  Serial.print(F(" | LED: "));
  if (isDark) {
    Serial.print(F("ON (PWM="));
    Serial.print(ledBrightness);
    Serial.println(F(")"));
  } else {
    Serial.println(F("OFF"));
  }
}

// Display splash screen on startup
void displaySplashScreen() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.drawStr(20, 20, "Smart Night");
    u8g2.drawStr(25, 35, "Light System");
    u8g2.drawStr(10, 50, "Task 2 - Group 26");
  } while (u8g2.nextPage());
}

// Thank You//





