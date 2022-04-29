#line 1 "/home/dene/projects/ESP32/sample/src/sample/ir_data.cpp"
#include "stdafx.h"
#include "ir_data.h"

void IrData::interrupt_handler(uint32_t now_micros) {
    if (length > MAX_LEN) {
        return;
    } else if (length > 0) {
        if (now_micros > prev_micros) {
            buffer[length - 1] = now_micros - prev_micros;
        } else {
            buffer[length - 1] = UINT32_MAX - prev_micros + now_micros;
        }
    }
    prev_micros = now_micros;
    length++;
}

bool IrData::check_end(uint32_t now_micros) {
    if (length > 0) {
        isEof = (((now_micros + 50) - prev_micros) > 9000); // 9ms
    } else {
        isEof = false;
    }

    return isEof;
}

IrFormat IrData::get_format() {

    if ((562 - 75) <= (buffer[0] / 16) && (buffer[0] / 16) <= (562 + 75)) {
        return IrFormat::NEC;
    } else if (350 <= (buffer[0] / 8) && (buffer[0] / 8) <= 500) {
        return IrFormat::AEHA;
    } else if ((600 - 75) <= (buffer[0] / 4) && (buffer[0] / 4) <= (600 + 75)) {
        return IrFormat::SONY;
    } else {
        return IrFormat::UNKNOWN;
    }
}

void IrData::reset() {
    length = 0;
    prev_micros = 0;
    isEof = false;
}

void IrData::send() {

}