#line 1 "/home/dene/projects/ESP32/sample/src/sample/sample.cpp"
#include "stdafx.h"

#include <stdint.h>
#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "sample.h"
#include "ir_data.h"

hw_timer_t *timer = NULL;
volatile bool led_state = false;
bool button_state = false;
volatile bool state = false;

volatile uint64_t count = 0;
volatile uint32_t buffer_index = 0;

IrData ir_data;

void setup()
{
  Serial.begin(115200);

  pinMode(PORT::LED, OUTPUT);
  pinMode(PORT::IR, INPUT);
  pinMode(PORT::BUTTON, INPUT_PULLUP);

  attachInterrupt(PORT::IR, ir_interrupt_handler, CHANGE);

  timer = timerBegin(0, getApbFrequency() / 1000000, true);
  timerAttachInterrupt(timer, &on_timer, true);
  timerAlarmWrite(timer, 1, true);
}

void loop()
{
  button_state = digitalRead(PORT::BUTTON);

  if (button_state)
  {
    state = true;
  }

  auto now = micros();
  if (ir_data.check_end(now))
  {
    detachInterrupt(PORT::IR);

    Serial.println("Length:" + String(ir_data.length));
    Serial.println("Format:" + String(ir_data.get_format()));
    ir_data.reset();

    attachInterrupt(PORT::IR, ir_interrupt_handler, CHANGE);
  }
  delay(100);
}

void ir_interrupt_handler()
{
  auto now = micros();
  ir_data.interrupt_handler(now);
}

void IRAM_ATTR on_timer()
{
  if (state)
  {
    if (buffer_index > (ir_data.length - 2)) {
      state = false;
      buffer_index = 0;
      count = 0;
      return;
    }

    count++;
    if (ir_data.buffer[buffer_index] < count) {
      led_state = !led_state;
      digitalWrite(PORT::LED, led_state);
      buffer_index++;
      count = 0;
    }

  }
}
