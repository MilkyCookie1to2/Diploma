#include <SPI.h>
#include "RF24.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <GyverOLED.h>
#include <Wire.h>
#include "images.h"

// Commands
// Define command codes
#define NULL_COM_PAR    0x00
#define MOVE_LEFT       0x01
#define MOVE_RIGHT      0x02
#define HIT_BALL        0x03
#define RESURRECTION    0x04
#define UPDATE_LIFES    0x05

#define RESURRECTION_DELAY 10000
#define UPDATE_CHARGE_DELAY 50000
unsigned int resurrection_timer = 0;
unsigned int update_charge_timer = 0;

bool show_low_level_battery = false;

// Structure representing a command and its parameters
struct Command {
    byte code; // Command code (1 byte)
    byte parameter; // Command parameter (1 byte)
};

static Command command;

#define left_button 5
#define right_button 6
#define solenoid_button_pin 7
#define y_pin A1
#define buzzer_pin 3
#define power_control_pin A3

// NRF24L01
#define NRF_CE 10
#define NRF_CSN 9
#define PIPE 0x6464646464LL

RF24 nrf(NRF_CE, NRF_CSN);  

// Display
GyverOLED<SSD1306_128x64, OLED_BUFFER> display;     // Объявление класса, представляющего интерфейс для работы с дисплеем

int sensorValue;
const float minVoltage = 3.0;       // Минимальное напряжение (разряженный аккумулятор)
const float maxVoltage = 4.2;       // Максимальное напряжение (полностью заряженный аккумулятор)

// Global Variables
byte score=11;
byte new_score=11;

void update_display_lifes() {  
  display.setScale(3);
  if(score >= 10)       
    display.setCursor(80,3);
  else
    display.setCursor(93,3);
  display.print(score);
  display.drawBitmap(15, 6, image_heart_59x52, 59, 52);
  display.update();
}

void update_percent_charge() {
  sensorValue = analogRead(power_control_pin );
  float voltage = sensorValue * (5.0 / 1023.0);
  float percent = ((voltage - minVoltage) / (maxVoltage - minVoltage)) * 100;
  if(percent < 0)
    percent = 0;
  if(voltage < 3.24 && !show_low_level_battery) {
    display.clear(); 
    display.setScale(1);
    display.setCursor(1, 6);
    display.print("Низкий уровень заряда");
    display.setCursor(25, 7);
    display.print("аккумулятора!!!");
    display.drawBitmap(40, 0, image_battery_48x48, 48, 48);
    display.update();
    for(int i=0; i<3; i++) {
      tone(buzzer_pin, 500);
      delay(200);
      noTone(buzzer_pin);
      delay(200);
    }
    show_low_level_battery = true;
  }
  if(voltage > 3.5)
    show_low_level_battery = false;
  display.clear(); 
  display.setScale(1);
  display.setCursor(105, 0);
  display.print((int)percent);
  display.print("%");
  display.rect(89, 0, 101, 7, OLED_STROKE);
  display.rect(87, 2, 88, 5, OLED_FILL); 

  if(percent <= 33) {
    display.rect(97, 2, 98, 5, OLED_FILL);
  } else {
    if(percent <= 66){
      display.rect(97, 2, 98, 5, OLED_FILL);
      display.rect(94, 2, 95, 5, OLED_FILL); 
    } else {
      display.rect(97, 2, 98, 5, OLED_FILL);
      display.rect(94, 2, 95, 5, OLED_FILL); 
      display.rect(91, 2, 92, 5, OLED_FILL); 
    }
  } 
  
  if(score == 0) {
    display.clear(); 
    display.drawBitmap(0, 0, arrow_128x29, 128, 29);
    display.setScale(1);
    display.setCursor(12, 4);
    display.print("Осталось 0 жизней(");
    display.setCursor(23, 5);
    display.print("Зажмите кнопки");
    display.setCursor(20, 6);
    display.print("перемещения для");
    display.setCursor(0, 7);
    display.print("восстановления жизней");
    display.update();
  } else {
    update_display_lifes();
  }
}

void setup() {
  Serial.begin(115200);   

  pinMode(left_button, INPUT);
  pinMode(right_button, INPUT);
  pinMode(solenoid_button_pin, INPUT);
  pinMode(y_pin, INPUT);
  pinMode(power_control_pin, INPUT);
  pinMode(buzzer_pin, OUTPUT);
  
  // I2C/TWI
  Wire.begin();   // Инициализация шины I2C/TWI
  display.init();

  display.clear(); 
  display.setScale(1);
  display.setCursor(20, 3);
  display.print("Инициализация...");
  display.setCursor(0, 4);
  display.print("Пожалуйста, подождите!");
  display.update();

  // NRF24L01
  nrf.begin();                              // активировать модуль
  nrf.setAutoAck(1);                        // режим подтверждения приёма, 1 вкл 0 выкл
  nrf.setRetries(0, 5);                    // (время между попыткой достучаться, число попыток)
  nrf.enableAckPayload();                   // разрешить отсылку данных в ответ на входящий сигнал
  nrf.setPayloadSize(sizeof(Command));      // размер пакета, в байтах

  // Установка адреса приемника и передатчика
  nrf.openWritingPipe(PIPE);      // устанавливаем адрес передатчика
  nrf.openReadingPipe(1, PIPE);   // устанавливаем адрес приемника, используя трубу 1

  nrf.setChannel(0x44);                     // выбираем канал (в котором нет шумов!)
  nrf.setPALevel(RF24_PA_MAX);              // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  nrf.setDataRate(RF24_1MBPS);              // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
                                            // должна быть одинакова на приёмнике и передатчике!
  nrf.powerUp();                            // начать работу
  nrf.startListening();                     // переводим модуль в режим приема данных

  display.clear(); 
  display.drawBitmap(40, 0, check_45x45, 45, 45);
  display.setScale(1);
  display.setCursor(3, 6);
  display.print("Инициализация прошла");
  display.setCursor(43, 7);
  display.print("успешно!");
  display.update();

  tone(buzzer_pin, 900);
  delay(100);
  noTone(buzzer_pin);
  delay(900);

  update_percent_charge();
}

void nrf_write(){
    nrf.stopListening();               // Переключаемся в режим передачи
    nrf.write(&command, sizeof(Command));  // Отправляем команду
    delay(100);  // Небольшая пауза для передачи данных
    nrf.startListening();
}

void loop() {
  command.code = NULL_COM_PAR;
  command.parameter = NULL_COM_PAR;

  update_charge_timer++;
  if(update_charge_timer >= UPDATE_CHARGE_DELAY) {
    update_percent_charge();
    update_charge_timer = 0;
  }

  // Read NRF
  if (nrf.available()) {          
    //Serial.println("00");        
    nrf.read(&command, sizeof(Command));
    //Serial.println("01");
    if(command.code == UPDATE_LIFES) {
      //Serial.println("UPDATE_LIFES");
      if(score != command.parameter) {
        if(score > command.parameter) {
          tone(buzzer_pin, 900);
        }
        score = command.parameter;
        if(score == 0) {
          display.clear(); 
          display.drawBitmap(0, 0, arrow_128x29, 128, 29);
          display.setScale(1);
          display.setCursor(12, 4);
          display.print("Осталось 0 жизней(");
          display.setCursor(23, 5);
          display.print("Зажмите кнопки");
          display.setCursor(20, 6);
          display.print("перемещения для");
          display.setCursor(0, 7);
          display.print("восстановления жизней");
          display.update();
        } else {
          update_percent_charge();
        }
        delay(250);
        noTone(buzzer_pin);
      }
    }
  }

  // Joystic
  unsigned int xValue = map(analogRead(y_pin), 0, 1023, 0, 255);
  if ((xValue < 100) || (xValue > 130)) {  
    command.code = xValue < 100 ? MOVE_LEFT : MOVE_RIGHT;
    command.parameter = xValue;
    nrf_write();
  }

    // Solenoid button
  if (digitalRead(solenoid_button_pin) == HIGH){
    command.code = HIT_BALL;
    command.parameter = NULL_COM_PAR;
    nrf_write();
  //  Serial.println("PUSH");  
  }

  //Left and Right buttons
  if(digitalRead(left_button) == HIGH || digitalRead(right_button) == HIGH){
    //delay(100);
    if(digitalRead(left_button) == HIGH && digitalRead(right_button) == HIGH){
      resurrection_timer++;
      if(resurrection_timer == RESURRECTION_DELAY){
        command.code = RESURRECTION;
        command.parameter = NULL_COM_PAR;
        resurrection_timer = 0;
        nrf_write();
        command.code = NULL_COM_PAR;
        command.parameter = NULL_COM_PAR;
      }
      return;
    } else {
      resurrection_timer = 0;
      if(digitalRead(left_button) == HIGH){
        command.code = MOVE_LEFT;
        command.parameter = 0;
      } 
      if(digitalRead(right_button) == HIGH){
        command.code = MOVE_RIGHT;
        command.parameter = 255;
      } 
      if(command.code != NULL_COM_PAR) {
        nrf_write();
        command.code = NULL_COM_PAR;
        command.parameter = NULL_COM_PAR;
      }
    }
  }
  resurrection_timer = 0;
}
