#pragma once

class PORT {
    public:
        static const uint8_t LED = 32;
        static const uint8_t IR = 33;
        static const uint8_t BUTTON = 25;
        static const uint8_t LED_STATUS = 26;

};

void ir_interrupt_handler();
void IRAM_ATTR on_timer();
