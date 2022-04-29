#include "stdafx.h"

#include <stdint.h>
#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "sample.h"
#include "ir_data.h"

hw_timer_t *timer = NULL;
volatile bool led_state = false;
bool button_state = false;
bool prev_button_state = false;
volatile bool state = false;
volatile bool prev_state = false;

bool view_state = false;

volatile uint32_t count = 0;
volatile uint32_t buffer_index = 0;

IrData ir_data;

void setup()
{
  Serial.begin(115200);

  pinMode(PORT::LED, OUTPUT);
  pinMode(PORT::LED_STATUS, OUTPUT);
  pinMode(PORT::IR, INPUT);
  pinMode(PORT::BUTTON, INPUT_PULLUP);

  attachInterrupt(PORT::IR, ir_interrupt_handler, CHANGE);

  Serial.println("getApbFrequency:" + String(getApbFrequency()));

  timer = timerBegin(0, getApbFrequency() / 1000000, true);
  timerAttachInterrupt(timer, &on_timer, true);
  timerAlarmWrite(timer, 10, true);
  timerAlarmEnable(timer);
}

void loop()
{
  button_state = digitalRead(PORT::BUTTON);
  digitalWrite(PORT::LED_STATUS, button_state);

  if (button_state && !prev_button_state) {
    led_state = false;
    state = true;
    view_state = false;
  }
  if (!state && prev_state) {
    ir_data.reset();
  }
  prev_button_state = button_state;
  prev_state = state;

  auto now = micros();
  if (ir_data.check_end(now))
  {
    detachInterrupt(PORT::IR);

    Serial.println("Length:" + String(ir_data.length()));
    Serial.println("Format:" + String(ir_data.get_format()));
    // Serial.print("  Data: [");
    // for (size_t i = 0; i < (ir_data.length()); i++)
    // {
    //   Serial.print(" " + String(ir_data.buffer[i]) + ",");
    // }
    // Serial.println("]");
    // view_state = true;
    attachInterrupt(PORT::IR, ir_interrupt_handler, CHANGE);
  }
  Serial.println("buffer_index:" + String(buffer_index) + ",count:" + String(count));
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
    if (buffer_index >= (ir_data.length() - 1)) {
      state = false;
      buffer_index = 0;
      count = 0;
      led_state = false;
      digitalWrite(PORT::LED, led_state);
      return;
    }

    count +=10;
    if (ir_data.buffer[buffer_index] < count) {
      led_state = !led_state;
      digitalWrite(PORT::LED, led_state);
      buffer_index++;
      count = 0;
    }

  }
}
