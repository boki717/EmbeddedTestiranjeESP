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
void IRAM_ATTR changeFlagInterupt();

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  xTaskCreate(ledLoop, "Naziv", 2048, NULL, 5, &ledLoopTaskHandle);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), changeFlagInterupt, RISING);
  // timer
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &changeFlagInterupt, true);
  timerAlarmWrite(timer, 2000000, true);
  timerAlarmEnable(timer);
}

void loop() {
  vTaskDelete(nullptr);
}

// put function definitions here:
void ledLoop(void *args){
  while (1){
      if (flag){
      led_state = !led_state;
      digitalWrite(LED_PIN, led_state);
      vTaskDelay(pdMS_TO_TICKS(200));
      flag = 0;
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void IRAM_ATTR changeFlagInterupt(){
  flag = 1;
}

// https://cdn.shopify.com/s/files/1/0870/0021/9940/files/ESP32-38_PIN-DEVBOARD-PINOUT.png?v=1772243497
