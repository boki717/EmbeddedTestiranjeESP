#include <Arduino.h>
#include "freertos/FreeRTOS.h"

#define LED_PIN GPIO_NUM_12
#define BUTTON_PIN GPIO_NUM_35

TaskHandle_t ledLoopTaskHandle;
hw_timer_t *timer = nullptr;
volatile bool flag = 0;
bool led_state = 0;

// put function declarations here:
void ledLoop(void *args);
void adcLoop(void *args);
void IRAM_ATTR timerChangeFlagInterrupt();
void IRAM_ATTR buttonChangeFlagInterrupt();

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  xTaskCreate(ledLoop, "Naziv", 2048, NULL, 5, &ledLoopTaskHandle);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonChangeFlagInterrupt, RISING);
  // timer
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &timerChangeFlagInterrupt, true);
  timerAlarmWrite(timer, 2000000, true);
  timerAlarmEnable(timer);
}

void loop() {
  vTaskDelete(nullptr);
}

// put function definitions here:
void ledLoop(void *args){
  while (1){
    // react on frequency change notification form button
    // use mutex to change the frequency
    if (flag){
      led_state = !led_state;
      digitalWrite(LED_PIN, led_state);
      vTaskDelay(pdMS_TO_TICKS(200));
      flag = 0;
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void adcLoop(void *args){ // adc task
  // read adc in a loop
  // when adc reading goes from one range to another, change led frequency using mutex
  // set motor speed (duty cicle!?) with pwm according to adc reading
}

void IRAM_ATTR timerChangeFlagInterrupt(){
  flag = 1;
}

void IRAM_ATTR buttonChangeFlagInterrupt(){
  flag = 1;
  // notify frequency change
}

// https://cdn.shopify.com/s/files/1/0870/0021/9940/files/ESP32-38_PIN-DEVBOARD-PINOUT.png?v=1772243497
