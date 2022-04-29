#line 1 "/home/dene/projects/ESP32/sample/src/sample/ir_data.h"
#pragma once

#include <stdint.h>

enum IrFormat {
    UNKNOWN = 0,
    NEC = 1,
    AEHA = 2,
    SONY = 3
};

class IrData {
    public:
        static const uint16_t MAX_LEN = 800;
        uint16_t length = 0;
        uint32_t buffer[MAX_LEN];
        void interrupt_handler(uint32_t now_micros);
        bool check_end(uint32_t now_micros);
        IrFormat get_format();
        void reset();
        // vector<uint8_t> read();
        void send();

    private:
        uint32_t prev_micros = 0;
        bool isEof = false;
};
