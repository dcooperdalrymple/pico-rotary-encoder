#pragma once
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/irq.h"

#include "rotary_encoder.pio.h"

#define MAX_ENCODERS 2

enum class RotaryEncoderEvent : uint8_t {
    ERROR = 0,
    CW,
    CCW,
    PRESS,
    RELEASE
};

typedef void(* rotary_encoder_callback_t) (RotaryEncoderEvent event);

class RotaryEncoder {

public:

    RotaryEncoder(int clk, int sw); // dt must be pin next to clk
    RotaryEncoder(int clk);

    void set_callback(rotary_encoder_callback_t callback);
    void clear_callback();

private:

    uint8_t index;
    uint sm; // pio state machine

    static uint8_t num_encoders;

    static PIO pio[MAX_ENCODERS];

    static int pin_clk[MAX_ENCODERS];
    static int pin_dt[MAX_ENCODERS];
    static int pin_sw[MAX_ENCODERS];

    static uint8_t sw_state[MAX_ENCODERS];

    static rotary_encoder_callback_t cb[MAX_ENCODERS];

    static void process_rotary();
    static void process_switch(uint gpio, uint32_t events);

};
