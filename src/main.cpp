#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2); // Устанавливаем дисплей

char key;
String str_pass = "";
int i, int_pass, my_array[2] = {1234, 4321};
unsigned long pass_timer;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '4', '7', '*'},
  {'2', '5', '8', '0'},
  {'3', '6', '9', '#'},
  {'A', 'B', 'C', 'D'}
};
byte rowPins[ROWS] = {38, 40, 42, 44};
byte colPins[COLS] = {46, 48, 50, 52};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();// Включаем подсветку дисплея
}

boolean arrayIncludeElement(int my_array[], int int_pass) {
  for (i = 0; i < 2; i++) {
    if (my_array[i] == int_pass) {   //проверяем, что ввели
      return true;
    }
  }
  return false;
}
void pass_check() {
  key = keypad.getKey();                 // обработка нажатия
  if (key == '*') {                      // если была нажата *
    lcd.clear();                         // отчищаем дисплей
    pass_timer = millis();               // сбросить таймер
    while (1) {                          // бесконечный цикл
      key = keypad.getKey();             // обработка нажатия
      if (key != NO_KEY) {               // если была нажата
        pass_timer = millis();           // сбросить таймер
        if (key == '*') {
          int_pass = str_pass.toInt();   // если нажата *
          break;                         // выйти из цикла
        }
        else if (key == '#') {           // если нажата #
          str_pass = "";                 // начать ввод сначала
        }
        else {                           // если * не нажата
          str_pass += key;               // прибавить нажатую цифру к паролю

        }
      }
      if (millis() - pass_timer > 10000) {     // если сработал таймер
        str_pass = String(int_pass, DEC);      // сбросить ввод и выйти из цикла
        break;
      }
    }
    Serial.println(int_pass);

    if (arrayIncludeElement(my_array, int_pass) == true) { // кладем в функцию
      lcd.setCursor(6, 0);                                 //массив и результат ввода
      lcd.print("Open");
      Serial.println("Open");
    }
    else {
      lcd.setCursor(5, 0);
      lcd.print("Error!");
      Serial.println("Error!");
      lcd.setCursor(6, 1);
      lcd.print(int_pass);
    }
    int_pass = 0;                        // очищаем переменные перед следующим
    str_pass = "";                       //вводом
  }
}
void loop() {
  pass_check();
}
