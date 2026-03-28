
// ===== Mini Pedestrian Crossing System (Grove Beginner Kit) =====
// TEAM GROUP: 26
//
// This program simulates a simple pedestrian crossing system using the
// Grove Beginner Kit. The system starts in the STOP state, indicated by a
// red LED and a "STOP" message on the OLED display. When the push button is
// pressed, the system enters a WAIT countdown shown on the OLED. After the
// wait period ends, the system transitions into the GO state where the buzzer
// provides audible feedback and a "GO" countdown is displayed. After the
// crossing period finishes, the system returns automatically to the STOP state.
//
// FEATURES INCLUDED:
// - STOP → WAIT → GO state-machine behaviour
// - OLED display feedback for each phase (STOP, WAIT, GO)
// - Countdown timers for both WAIT and GO phases
// - Buzzer alerts for transition and end-of-crossing warnings
// - Red LED indicator for STOP state
//
// ---------------------------------------------------------------------------
//   LIBRARIES USED
//   These libraries allow the Arduino to:
//   - Communicate via I2C using Wire
//   - Display text on the SSD1306 OLED using the SSD1306Ascii library
// ---------------------------------------------------------------------------
// Include the Wire library needed for I2C communication
#include <Wire.h>

// Include libraries for writing text to the SSD1306 OLED display
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

// Create an OLED display object (using I2C via the Wire interface)
SSD1306AsciiWire display;

// Grove Beginner Kit pins
const int buttonPin = 6;   // Push button used by pedestrians to request crossing
const int ledPin = 4;      // Red LED used to signal STOP
const int buzzerPin = 5;   // Buzzer for beeps during crossing phases

// Timings (in seconds)
int waitTime = 5;          // Duration of WAIT countdown after button is pressed
int crossTime = 10;        // Duration of GO countdown (time allowed to cross)

void setup() {

  // Configure pins for input/output
  pinMode(buttonPin, INPUT);     // Button as an input
  pinMode(ledPin, OUTPUT);       // LED as an output
  pinMode(buzzerPin, OUTPUT);    // Buzzer as an output

  // Start I2C communication for the OLED display
  Wire.begin();

  // Initialize the OLED display with correct resolution and I2C address
  display.begin(&Adafruit128x64, 0x3C);

  // Choose a readable default font
  display.setFont(System5x7);
}

// Function to display the STOP message
void showStop() {
  display.clear();   // Wipe the screen
  display.set2X();   // Double-size text for visibility
  display.println("STOP");
}

// Function to display countdown before GO phase
void countdown(int time) {

  // Loop from the given time down to 1
  for(int i = time; i > 0; i--) {
    display.clear();          // Clear screen for updated number
    display.set2X();          // Large text
    display.print("WAIT ");   // Display WAIT label
    display.println(i);       // Display countdown number
    delay(1000);              // Wait one second per step
  }
}

// Function to display GO countdown during crossing phase
void crossing(int time) {

  for(int i = time; i > 0; i--) {
    display.clear();        // Clear display for the new value
    display.set2X();        // Large text for visibility
    display.print("GO ");   // Indicate GO state
    display.println(i);     // Show countdown
    delay(1000);            // 1 second delay
  }
}

void loop() {

  // Default state: STOP state active
  digitalWrite(ledPin, HIGH);  // Turn ON the RED LED (STOP)
  showStop();                  // Display STOP on OLED

  // Check if the pedestrian has pressed the button
  if(digitalRead(buttonPin) == HIGH) {

    delay(200); // Small debounce delay to avoid multiple triggers

    // Show WAIT countdown on OLED
    countdown(waitTime);

    // Give a brief beep to indicate transition to GO
    tone(buzzerPin, 1000);   // Play beep at 1 kHz
    delay(500);              // Beep duration
    noTone(buzzerPin);       // Stop the buzzer

    // Turn OFF the red LED since crossing will start
    digitalWrite(ledPin, LOW);

    // Display GO countdown while pedestrians can cross
    crossing(crossTime);

    // After GO period ends, give three warning beeps before returning to STOP
    for(int i = 0; i < 3; i++){
      tone(buzzerPin, 2000);  // Short high-pitched warning beep
      delay(200);
      noTone(buzzerPin);
      delay(300);             // Pause between beeps
    }
  }
}