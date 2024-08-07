#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define the screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// Create an instance of the SSD1306 display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Buzzer / beeping vars
int buzzerPin = 12;
unsigned long startTime;
int remainingTime;

void setup() {
  // Setup hardware
  setupLED();
  setupBuzzer();
  setupDisplay();

  // Initialize start time
  startTime = millis();
}

void loop() {
  // Handle the bomb beeping logic
  bomb_beeping();

  // Update the display with the remaining time or "BOOM!"
  updateDisplay();

  // Blink the LED
  //blinkLED();
}

void setupLED() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void setupBuzzer() {
  pinMode(buzzerPin, OUTPUT);
}

void setupDisplay() {
  // Start the I2C communication
  Wire.begin();

  // Initialize the display with the I2C address 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  // Clear the buffer
  display.clearDisplay();

  // Set text size and color
  display.setTextSize(4); // Set a larger text size for the timer
  display.setTextColor(SSD1306_WHITE);
}

void bomb_beeping() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;

  if (elapsedTime <= 45000) {  // Increase beep frequency over 45 seconds
    remainingTime = 45 - (elapsedTime / 1000); // Calculate the remaining time in seconds
    int delayTime = map(elapsedTime, 0, 45000, 1000, 50);  // Maps the elapsed time to a delay range of 1000ms to 100ms
    tone(buzzerPin, 1500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(150);
    noTone(buzzerPin);
    digitalWrite(LED_BUILTIN, LOW);
    delay(delayTime);
  } else {  // After 45 seconds, beep rapidly 10 times
    remainingTime = 0; // Timer is finished
    for (int i = 0; i < 5; i++) {
      tone(buzzerPin, 1500);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(50);
      noTone(buzzerPin);
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(50);
    }
    // After the 10 rapid beeps, reset the start time to repeat the cycle
    startTime = millis();
  }
}

void updateDisplay() {
  // Clear the display
  display.clearDisplay();

  // Set the cursor location
  display.setCursor(40, 5);

  if (remainingTime > 0) {
    // Print the remaining time in seconds
    display.println(remainingTime);
  } else {
    // Print "BOOM!" when the timer finishes
    display.println("BOOM!");
  }

  // Display the buffer
  display.display();
}

void blinkLED() {
  // Turn the LED on
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500); // Wait for half a second

  // Turn the LED off
  digitalWrite(LED_BUILTIN, LOW);
  delay(500); // Wait for half a second
}