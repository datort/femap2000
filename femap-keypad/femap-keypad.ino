/**
 * 
 * Keymap D1 Mini Clone: 
 * 
 * D0 - C1
 * D5 - R1
 * D6 - C2
 * D7 - R2
 * D8 - R3
 * D1 - R4
 * D2 - C3
 *************
 * Keymap Pico: 
 * 
 * D6 - C1
 * D10 - R1
 * D7 - C2
 * D11 - R2
 * D12 - R3
 * D13 - R4
 * D8 - C3
 */


#define NUM_COLUMNS 3  
#define NUM_ROWS 4

/* D1 Mini Clone: 
int columns[NUM_COLUMNS] = { D0, D6, D2 };
int rows[NUM_ROWS] = { D5, D7, D8, D1 };*/

int columns[NUM_COLUMNS] = { D6, D7, D8 };
int rows[NUM_ROWS] = { D10, D11, D12, D13 };

int key[NUM_COLUMNS][NUM_ROWS] = {
  { 1, 4, 7, -1 },
  { 2, 5, 8, 0 },
  { 3, 6, 9, -2 },
};

int reading = -99;

void setup() {
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);

  pinMode(D10, INPUT);
  pinMode(D11, INPUT);
  pinMode(D12, INPUT);
  pinMode(D13, INPUT);

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
