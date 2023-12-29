#include "LiquidCrystal_I2C.h"
#include <Keypad.h>

#define dirPin 2
#define stepPin 3
#define ledPin 13

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROW_NUM    = 4;
const byte COLUMN_NUM = 4;

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pin_rows[ROW_NUM]    = {12, 11, 10, 9};
byte pin_column[COLUMN_NUM] = {8, 7, 6, 5};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

String inputString = "";
int currentPosition = -2;
int minSpeed = 10;
int maxSpeed = 5000;
int speed = "";  // Kecepatan motor stepper (langkah per detik)

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  lcd.init();
  lcd.backlight();
  moveStepper(-currentPosition);
  lcd.setCursor(2, 0); lcd.print("SatSet Team");
  lcd.setCursor(0, 1); lcd.print("Stechoq Robotik");
  delay(2000);
  lcd.clear();
}

void moveStepper(int steps) {
  currentPosition += steps;
  digitalWrite(dirPin, HIGH);

  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speed);
  }

  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
}

void Tekan(char button) {
  lcd.clear();

  if (button == 'A' || button == 'B' || button == 'C') {
    int cairanML = 0;
    if (button == 'A' || button == 'B') {
      cairanML = (button == 'A') ? 50 : 100;
    } else if (button == 'C') {
      lcd.setCursor(2, 0); lcd.print("Atur Kecepatan");
      lcd.setCursor(0, 1); lcd.print("Kecepatan: ");

      // Minta input untuk kecepatan
      inputString = "";
      while (true) {
        char key = keypad.getKey();
        if (key && (key >= '0' && key <= '9')) {
          inputString += key;
          lcd.clear();
          lcd.setCursor(2, 0); lcd.print("Atur Kecepatan");
          lcd.setCursor(0, 1); lcd.print("Kecepatan: " + inputString);
        } else if (key == '#') {
          speed = constrain(inputString.toInt(), minSpeed, maxSpeed);
          lcd.clear();
          inputString = "";
          lcd.setCursor(2, 0); lcd.print("Pompa Suntik");
          lcd.setCursor(0, 1); lcd.print("Cairan (ml):");
          break;
        } else if (key == '*') {
          // Tombol "*" ditekan, kembali ke mode input cairan
          lcd.clear();
          lcd.setCursor(2, 0); lcd.print("Pompa Suntik");
          lcd.setCursor(0, 1); lcd.print("Cairan (ml):");
          inputString = "";
          break;
        }
      }
      lcd.clear();
      return;
    }

    lcd.setCursor(2, 0); lcd.print("Pompa Suntik");
    lcd.setCursor(0, 1); lcd.print("Cairan (ml): " + String(cairanML));
    moveStepper(cairanML);
  } else if (button == 'D') {
    inputString = "";
    lcd.clear();
    lcd.setCursor(2, 0); lcd.print("Pompa Suntik");
    lcd.setCursor(0, 1); lcd.print("Cairan (ml):");
  } else if (button == '*') {
    if (currentPosition > 0) {
      digitalWrite(dirPin, LOW);
      for (int i = 0; i < currentPosition; i++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(speed);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(speed);
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("Reset");
      }
      delay(1000);
      lcd.clear();
      currentPosition = 0;
    }
  }
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    if (key == '#') {
      int steps = inputString.toInt();
      moveStepper(steps);
      inputString = "";
      lcd.clear();
      lcd.setCursor(2, 0); lcd.print("Pompa Suntik");
      lcd.setCursor(0, 1); lcd.print("Cairan (ml): ");
    } else if (key >= '0' && key <= '9') {
      inputString += key;
      lcd.clear();
      lcd.setCursor(2, 0); lcd.print("Pompa Suntik");
      lcd.setCursor(0, 1); lcd.print("Cairan (ml): ");
      lcd.setCursor(13,1); lcd.print(inputString);
    } else {
      Tekan(key);
    }
  }
} #include "LiquidCrystal_I2C.h"
#include <Keypad.h>

#define dirPin 2
#define stepPin 3
#define ledPin 13

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROW_NUM    = 4;
const byte COLUMN_NUM = 4;

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pin_rows[ROW_NUM]    = {12, 11, 10, 9};
byte pin_column[COLUMN_NUM] = {8, 7, 6, 5};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

String inputString = "";
int currentPosition = -2;
int minSpeed = 10;
int maxSpeed = 5000;
int speed = 0;  // Kecepatan motor stepper (langkah per detik)

bool speedSelected = false;  // Menandakan apakah kecepatan sudah dipilih atau belum

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  lcd.init();
  lcd.backlight();
  moveStepper(-currentPosition);
  lcd.setCursor(2, 0); lcd.print("SatSet Team");
  lcd.setCursor(0, 1); lcd.print("Stechoq Robotik");
  delay(2000);
  lcd.clear();
}

void moveStepper(int steps) {
  currentPosition += steps;
  digitalWrite(dirPin, HIGH);

  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speed);
  }

  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
}

void Tekan(char button) {
  lcd.clear();

  if (button == 'A' || button == 'B' || button == 'C') {
    int cairanML = 0;
    if (button == 'A' || button == 'B') {
      cairanML = (button == 'A') ? 50 : 100;
    } else if (button == 'C') {
      lcd.setCursor(0, 0); lcd.print("Pilih Kecepatan: ");
      lcd.setCursor(0, 1); lcd.print("1. 100%  2. 50%");

      // Minta input untuk kecepatan
      inputString = "";
      while (true) {
        char key = keypad.getKey();
        if (key && (key == '1' || key == '2')) {
          speed = (key == '1') ? 1500 : 5000;
          lcd.clear();
          lcd.setCursor(2, 0); lcd.print("Pompa Suntik");
          lcd.setCursor(0, 1); lcd.print("Cairan (ml):");
          speedSelected = true;
          break;
        }
      }
      lcd.clear();
      return;
    }

    if (speedSelected) {
      lcd.setCursor(2, 0); lcd.print("Pompa Suntik");
      lcd.setCursor(0, 1); lcd.print("Cairan (ml): " + String(cairanML));
      moveStepper(cairanML);
    }
  } else if (button == 'D') {
    inputString = "";
    lcd.clear();
    lcd.setCursor(2, 0); lcd.print("Pompa Suntik");
    lcd.setCursor(0, 1); lcd.print("Cairan (ml):");
  } else if (button == '*') {
    if (currentPosition > 0) {
      digitalWrite(dirPin, LOW);
      for (int i = 0; i < currentPosition; i++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(speed);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(speed);
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("Reset");
      }
      delay(1000);
      lcd.clear();
      currentPosition = 0;
    }
  }
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    if (key == '#') {
      if (speedSelected) {
        int steps = inputString.toInt();
        if (steps > 100) {
          // Melebihi batas maksimal, tampilkan pesan di LCD
          lcd.clear();
          lcd.setCursor(2, 0);
          lcd.print("Input tidak ");
          lcd.setCursor(5, 1);
          lcd.print("sesuai! ");
          delay(2000);
          lcd.clear();
        } else {
          moveStepper(steps);
          inputString = "";
          lcd.clear();
          lcd.setCursor(2, 0); lcd.print("Pompa Suntik");
          lcd.setCursor(0, 1); lcd.print("Cairan (ml): ");
        }
      } else {
        // Tampilkan pesan bahwa kecepatan belum dipilih
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Pilih kecepatan ");
        lcd.setCursor(0, 1);
        lcd.print("terlebih dahulu!");
        delay(2000);
        lcd.clear();
      }
    } else if (key >= '0' && key <= '9') {
      inputString += key;
      lcd.clear();
      lcd.setCursor(2, 0); lcd.print("Pompa Suntik");
      lcd.setCursor(0, 1); lcd.print("Cairan (ml): ");
      lcd.setCursor(13,1); lcd.print(inputString);
    } else {
      Tekan(key);
    }
  }
}
