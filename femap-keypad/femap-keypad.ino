#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <arduino-timer.h>

#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3C

#define NUM_COLUMNS 3
#define NUM_ROWS 4

auto timer = timer_create_default();
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int columns[NUM_COLUMNS] = { D6, D7, D8 };
int rows[NUM_ROWS] = { D10, D11, D12, D13 };

int key[NUM_COLUMNS][NUM_ROWS] = {
  { 1, 4, 7, -1 },
  { 2, 5, 8, 0 },
  { 3, 6, 9, -2 },
};

String serviceProvider = String("FeMAp");
bool isRinging = false;
String callerId;

int sMeter = -1;
int reading = -99;  // @todo: rename, unclear
bool isPickedUp = false;

void setup() {
  pinMode(D6, OUTPUT);  // Keypad C1
  pinMode(D7, OUTPUT);  // Keypad C2
  pinMode(D8, OUTPUT);  // Keypad C3

  pinMode(D10, INPUT);  // Keypad R1
  pinMode(D11, INPUT);  // Keypad R2
  pinMode(D12, INPUT);  // Keypad R3
  pinMode(D13, INPUT);  // Keypad R4
  pinMode(D22, INPUT);  // Headset Switch

  Serial.begin(115200);
  Serial1.begin(9600);

  if (!oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }

  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(11, 18);
  oled.println("FeMAp//2k");
  oled.display();

  oled.setCursor(0, 20);

  isPickedUp = digitalRead(D22);
  attachInterrupt(D22, handleHeadset, CHANGE);

  oled.setTextSize(1);
  refreshDisplay();

  timer.in(5000, requestStatus);
  timer.in(10000, requestBoardInfo);
  timer.in(15000, requestServiceProvider);
  timer.every(20000, requestSignalStatus);

  sMeter = -1;
}

void loop() {
  timer.tick();

  for (int i = 0; i < NUM_COLUMNS; i++) {
    digitalWrite(columns[i], HIGH);
    delay(2);

    for (int j = 0; j < NUM_ROWS; j++) {
      int buttonPressed = digitalRead(rows[j]);
      int value = key[i][j];

      if (!buttonPressed && value == reading) {
        reading = -99;
      }

      if (buttonPressed && value == reading) {
        continue;
      }

      if (buttonPressed) {
        reading = value;
        Serial.println(value);
        //oled.print(value);
        //oled.display();
      }
      delay(2);
    }

    digitalWrite(columns[i], LOW);
    delay(2);
  }

  while (Serial.available()) {
    Serial1.write(Serial.read());
  }

  delay(10);
}

void refreshDisplay() {
  oled.clearDisplay();
  drawServiceProvider();
  drawSMeter();

  drawIncomingCall();

  oled.display();
}

void serialEvent1() {
  String message;

  while (Serial1.available()) {
    //message += (char)Serial1.read();
    message = Serial1.readStringUntil('\n');
    Serial.println(message);

    if (handleModemMessage(message)) {
      refreshDisplay();
    }

    delay(2);  // Allow the Serial buffer to fill up
  }
}

void handleHeadset() {
  bool state = digitalRead(D22);
  Serial.println(state ? "Headset picked up" : "Headset hung up");
  isPickedUp = state;
}

bool requestStatus(void *) {
  Serial.println("Requesting AT");
  Serial1.println("AT");

  return false;
}

bool requestBoardInfo(void *) {
  Serial.println("Requesting ATI");
  Serial1.println("ATI");

  return false;
}

bool requestSignalStatus(void *) {
  Serial.println("Requesting AT+CSQ");
  Serial1.println("AT+CSQ");

  return true;
}

bool requestServiceProvider(void *) {
  Serial.println("Requesting AT+CSPN?");
  Serial1.println("AT+CSPN?");

  return false;
}
