#include <stdio.h>
#include "pico/stdlib.h"
#include "rotary_encoder.hpp"

#define ROTARY_ENCODER_CLK  2 // dt must be pin next to clk
#define ROTARY_ENCODER_SW   4

static RotaryEncoder rotary_encoder(ROTARY_ENCODER_CLK, ROTARY_ENCODER_SW);
static int rotary_encoder_position = 0;

void rotary_event(RotaryEncoderEvent event) {
    switch (event) {
        case RotaryEncoderEvent::CW:
            printf("CW: %d\n", ++rotary_encoder_position);
            break;
        case RotaryEncoderEvent::CCW:
            printf("CCW: %d\n", --rotary_encoder_position);
            break;
        case RotaryEncoderEvent::PRESS:
            printf("PRESS\n");
            break;
        case RotaryEncoderEvent::RELEASE:
            printf("RELEASE\n");
            break;
    }
}

int main() {
    stdio_init_all();
    printf("Pico C/C++ Rotary Encoder Library Example\n");
    rotary_encoder.set_callback(rotary_event);
    while (1) {
        sleep_ms(100);
    }
    return 0;
}
