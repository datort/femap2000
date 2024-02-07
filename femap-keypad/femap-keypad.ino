#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <arduino-timer.h>
#include "pitches.h"

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

String serviceProvider = String("FeMAp 2000");
bool isRinging = false;
String callerId;

int sMeter = -1;
int previousReading = -99;  // storing last pressed button to avoid bouncing/repeated input
String dialedNumber;
bool isPickedUp = false;
bool isActiveCall = false;
unsigned long callStartedAt;
unsigned long callEndedAt;

unsigned long headsetInterruptTriggeredAt;
#define HEADSET_INTERRUPT_BOUNCE_TIME 50

bool snakeMode = false;

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
  oled.setCursor(3, 18);
  oled.println("FeMAp 2000");
  oled.display();

  delay(2500);

  oled.setCursor(0, 20);

  isPickedUp = digitalRead(D22);
  attachInterrupt(D22, handleHeadsetInterrupt, CHANGE);

  oled.setTextSize(1);
  refreshDisplay();

  timer.in(5000, requestStatus);
  timer.in(10000, requestBoardInfo);
  timer.in(13000, requestServiceProvider);
  timer.every(15000, requestSignalStatus);

  sMeter = -1;
}

// Start up the second core
void setup1() {}

void loop() {
  timer.tick();

  int value = getUserInput();
  
  if (snakeMode) {
    if (value > 0) {
      handleSnakeInput(value);
    }
    snakeIteration();
  } else {
    if (value >= 0) {
      dialedNumber += value;
      refreshDisplay();
    } else if (value == -1 && dialedNumber.length() > 0) {
      dialedNumber = "";
      refreshDisplay();
    } else if (value == -1 && dialedNumber.length() == 0) {
      Serial.println("STARTING SNAKE MODE");
      startSnake();
    }
  }

  while (Serial.available()) {
    Serial1.write(Serial.read());
  }

  String message;
  while (Serial1.available()) {
    message = Serial1.readStringUntil('\n');
    Serial.println(message);

    if (handleModemMessage(message)) {
      refreshDisplay();
    }

    delay(10);
  }

  delay(10);
}

int getUserInput() {
  int detectedKeyPress = -99;

  for (int i = 0; i < NUM_COLUMNS; i++) {
    digitalWrite(columns[i], HIGH);
    delay(2);

    for (int j = 0; j < NUM_ROWS; j++) {
      int buttonPressed = digitalRead(rows[j]);
      int value = key[i][j];

      if (!buttonPressed && value == previousReading) {
        previousReading = -99;
      }

      if (buttonPressed && value == previousReading) {
        continue;
      }

      if (buttonPressed) {
        previousReading = value;
        detectedKeyPress = value;
        Serial.println(value);
      }

      delay(2);
    }

    digitalWrite(columns[i], LOW);
    delay(2);
  }

  return detectedKeyPress;
}

void loop1() {
  unsigned int taskId = rp2040.fifo.pop();

  if (taskId == 1) playMelody();
}

void refreshDisplay() {
  oled.clearDisplay();
  drawServiceProvider();
  drawSMeter();

  drawIncomingCall();
  drawActiveCall();
  drawDialedNumber();

  drawHeadsetState();

  oled.display();
}



void handleHeadsetInterrupt() {
  // debouncing logic
  if (headsetInterruptTriggeredAt > (millis() - HEADSET_INTERRUPT_BOUNCE_TIME)) {
    return;
  }

  headsetInterruptTriggeredAt = millis();
  timer.in(HEADSET_INTERRUPT_BOUNCE_TIME + 1, handleHeadset);
}

bool handleHeadset(void *) {
  bool state = digitalRead(D22);
  Serial.println(state ? "Headset picked up" : "Headset hung up");
  isPickedUp = state;

  if (isRinging && isPickedUp && !isActiveCall) {
    Serial1.println("ATA");

    isRinging = false;
    isActiveCall = true;
    callStartedAt = millis();
  }

  else if (isPickedUp && dialedNumber.length() > 0 && !isActiveCall) {
    Serial.print("Calling: ");
    Serial.println(dialedNumber);

    Serial1.print("ATD+ +49");
    Serial1.print(dialedNumber.startsWith("0") ? dialedNumber.substring(1) : dialedNumber);
    Serial1.println(";");

    isActiveCall = true;
  }

  else if (!isPickedUp && isActiveCall) {
    Serial1.println("ATH");

    isActiveCall = false;
    callEndedAt = millis();
    dialedNumber = "";
  }

  refreshDisplay();

  return false;
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
  if (isRinging) return true;

  Serial.println("Requesting AT+CSQ");
  Serial1.println("AT+CSQ");

  return true;
}

bool requestServiceProvider(void *) {
  Serial.println("Requesting AT+CSPN?");
  Serial1.println("AT+CSPN?");

  return false;
}

void playMelody() {
  Serial.println("Playing melody");

  int melody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
  };

  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4
  };

  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(D21, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(D21);
  }
}
