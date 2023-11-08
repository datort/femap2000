/**
 * 
 * Keymap: 
 * 
 * D0 - C1
 * D5 - R1
 * D6 - C2
 * D7 - R2
 * D8 - R3
 * D1 - R4
 * D2 - C3
 */


#define NUM_COLUMNS 3
#define NUM_ROWS 4

int columns[NUM_COLUMNS] = { D0, D6, D2 };
int rows[NUM_ROWS] = { D5, D7, D8, D1 };

int key[NUM_COLUMNS][NUM_ROWS] = {
  { 1, 4, 7, -1 },
  { 2, 5, 8, 0 },
  { 3, 6, 9, -2 },
};

int reading = -99;

void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D2, OUTPUT);

  pinMode(D5, INPUT);
  pinMode(D7, INPUT);
  pinMode(D8, INPUT);
  pinMode(D1, INPUT);

  Serial.begin(115200);
}

void loop() {
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
      }
      delay(2);
    }

    digitalWrite(columns[i], LOW);
    delay(2);
  }

  delay(10);
}
