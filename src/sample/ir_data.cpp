#include "stdafx.h"
#include "ir_data.h"

void IrData::interrupt_handler(uint32_t now_micros) {
    if (len > (MAX_LEN + 1)) {
        return;
    } else if (len > 0) {
        if (now_micros > prev_micros) {
            buffer[len - 1] = now_micros - prev_micros;
        } else {
            buffer[len - 1] = UINT32_MAX - prev_micros + now_micros;
        }
    }
    prev_micros = now_micros;
    len++;
}

bool IrData::check_end(uint32_t now_micros) {
    if (length() > 0) {
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
    len = 0;
    prev_micros = 0;
    isEof = false;
}

void IrData::send() {

}