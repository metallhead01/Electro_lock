#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h> // подключаем библиотеку для работы с сервоприводом

LiquidCrystal_I2C lcd(0x3F, 16, 2); // инициализируем дисплей

Servo servo;
char key;
String str_pass = "";
int i, int_pass, my_array[2] = {1234, 4321};
unsigned long pass_timer, time;

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

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); //инициализируем клавиатуру

void setup() {
  Serial.begin(9600);
  servo.attach(11);// привязываем сервопривод к аналоговому выходу 0
  lcd.init();
  lcd.backlight();// Включаем подсветку дисплея
  servo.write(7);
}

int arrayIncludeElement(int my_array[], int int_pass) {
  for (i = 0; i < 2; i++) {
    if (my_array[i] == int_pass) {   //проверяем, что ввели
      return my_array[i];
    }
  }
  return 0;
}
void pass_check() {
  lcd.setCursor(0, 0);
  lcd.print("Press * for strt");
  lcd.setCursor(0, 1);
  lcd.print("Press # for end");
  key = keypad.getKey();                 // обработка нажатия
  if (key == '*') {                      // если была нажата *
    lcd.clear();                         // отчищаем дисплей
    pass_timer = millis();               // сбросить таймер
    while (1) {                          // бесконечный цикл
      key = keypad.getKey();             // обработка нажатия
      if (key != NO_KEY) {               // если была нажата
        pass_timer = millis();           // сбросить таймер
        if (key == '#') {
          int_pass = str_pass.toInt();   // если нажата *
          break;                         // выйти из цикла
        }
        else if (key == '*') {           // если нажата #
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
    if (arrayIncludeElement(my_array, int_pass) == 1234) { // кладем в функцию массив и результат ввода
      lcd.setCursor(6, 0);
      lcd.print("Open");
      Serial.println("Open");
      servo.write(111);
      delay(3000);
    }
    else if (arrayIncludeElement(my_array, int_pass) == 4321) {
      lcd.setCursor(6, 0);
      lcd.print("Close");
      Serial.println("Close");
      servo.write(5);
      delay(2000);
    }
    else {
      lcd.setCursor(5, 0);
      lcd.print("Error!");
      Serial.println("Error!");
      lcd.setCursor(3, 1);
      //lcd.print(int_pass);
      lcd.print("Wrong code");
      delay(2000);
    }
    int_pass = 0;                        // очищаем переменные перед следующим вводом
    str_pass = "";
  }
}
void loop() {
  pass_check();
}
