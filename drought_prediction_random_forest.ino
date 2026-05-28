/*
Drought Prediction with Random Forest (Simulated Input)

***
The model is a decision tree, which is a flowchart-like 
structure where decisions are made based on conditions 
on input variables. Here, the inputs are:
    Rainfall (in mm)
    Humidity (in %)

The output is a binary classification:
    Drought = YES (1)
    Drought = NO (0)
***

OLED wire connecction
 Wiring: I2C OLED
OLED Pin	Arduino Uno / Mega
VCC	5V
GND	GND
SDA	A4 (Uno) / 20 (Mega)
SCL	A5 (Uno) / 21 (Mega)
*/


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Function to simulate sensor inputs
int getRandomRainfall() { return random(0, 100); }    // mm
int getRandomTemp() { return random(15, 40); }        // °C
int getRandomHumidity() { return random(10, 90); }    // %

// Simple Decision Tree for drought prediction
bool predictDrought(int rainfall, int humidity) {
  if (rainfall <= 15) {
    if (humidity <= 25) {
      return true;
    }
  }
  return false;
}

// Draw thermometer-style vertical bar on right side
void drawThermometerBar(int x, int y, int width, int height, int levelPercent) {
  // Draw outline rectangle
  display.drawRect(x, y, width, height, WHITE);

  // Calculate fill height (in pixels)
  int fillHeight = (height * levelPercent) / 100;

  // Draw filled rectangle (bottom up)
  display.fillRect(x + 1, y + height - fillHeight + 1, width - 2, fillHeight - 2, WHITE);
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));  // Seed random generator with analog noise

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while(true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("ML Drought Predictor");
  display.display();
  delay(2000);
}

void loop() {
  int rainfall = getRandomRainfall();
  int temperature = getRandomTemp();
  int humidity = getRandomHumidity();

  bool drought = predictDrought(rainfall, humidity);

  // Clear display
  display.clearDisplay();

  // Print sensor data and prediction
  display.setCursor(0,0);
  display.println("ML Drought Predictor");

  display.setCursor(0, 15);
  display.print("Rain(mm): ");
  display.println(rainfall);

  display.setCursor(0, 25);
  display.print("Temp(C): ");
  display.println(temperature);

  display.setCursor(0, 35);
  display.print("Humidity(%): ");
  display.println(humidity);

  display.setCursor(0, 50);
  display.print("Drought: ");
  if (drought) display.println("YES");
  else display.println("NO");

  // Draw thermometer bar on right side
  // Position: x=115 (right side), y=10, width=10, height=50
  int droughtLevel = drought ? 100 : 0;  // binary level
  drawThermometerBar(115, 10, 10, 50, droughtLevel);

  // Label below bar
  display.setCursor(100, 62);
  display.setTextSize(1);
  display.print("Level");

  display.display();

  // Debug serial output
  Serial.print("Rain: "); Serial.print(rainfall);
  Serial.print(", Temp: "); Serial.print(temperature);
  Serial.print(", Humidity: "); Serial.print(humidity);
  Serial.print(" => Drought: "); Serial.println(drought ? "YES" : "NO");

  delay(3000);
}

