#line 1 "/home/dene/projects/ESP32/sample/src/sample/sample.h"
#pragma once

class PORT {
    public:
        static const uint8_t LED = 32;
        static const uint8_t IR = 33;
        static const uint8_t BUTTON = 25;

};

void ir_interrupt_handler();
void IRAM_ATTR on_timer();
