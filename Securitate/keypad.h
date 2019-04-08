#define BUZZER 29
#define ROWS 4
#define COLS 4

char pressedKey = '\0';
int rowPins[ROWS] = { 1, 4, 5, 6 };
int colPins[COLS] = { 12, 3, 2, 0 };

char keys[ROWS][COLS] = { { '1', '2', '3', 'A' }, { '4', '5', '6', 'B' }, { '7',
		'8', '9', 'C' }, { '*', '0', '#', 'D' } };

void init_keypad() {
	for (int c = 0; c < COLS; c++) {
		pinMode(colPins[c], OUTPUT);
		digitalWrite(colPins[c], HIGH);
	}
	for (int r = 0; r < ROWS; r++) {
		pinMode(rowPins[0], INPUT);
		pullUpDnControl(rowPins[r], PUD_UP);
	}
}

int findLowRow() {
	for (int r = 0; r < ROWS; r++) {
		if (digitalRead(rowPins[r]) == LOW) {
//            printf("Row %d LOW\n", r);
			return r;
		} else {
//         printf("Row %d HIGH\n", r);
		}
	}
	return -1;
}


char get_key() {
	int rowIndex;
	for (int c = 0; c < COLS; c++) {
		digitalWrite(colPins[c], LOW);
		//    printf("Col %d\n", c);
		// delay(1000);
		rowIndex = findLowRow();
		if (rowIndex > -1) {
			if (!pressedKey) {
				pressedKey = keys[rowIndex][c];
				digitalWrite(BUZZER, LOW);
				delay(100);
				digitalWrite(BUZZER, HIGH);
			}
			digitalWrite(colPins[c], HIGH);
			return pressedKey;
		}
		digitalWrite(colPins[c], HIGH);
	}
	pressedKey = '\0';
	return pressedKey;
}
