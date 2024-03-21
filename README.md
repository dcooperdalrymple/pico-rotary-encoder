# pico-rotary-encoder
PIO rotary encoder driver for the Raspberry Pi Pico C/C++ SDK.

## Installation
Make sure that [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk) is installed correctly before following the instructions below.

### Example Compilation
Clone the library using `git clone https://github.com/dcooperdalrymple/pico-rotary-encoder` into a suitable location. Run the following commands to build the library example and ensure that your machine is capable of building this software.

````
cd pico-rotary-encoder/example
mkdir build && cd build
cmake ..
make
````

### Install Extension into Your Project
In order to add this library as an extension of your project, insert it as a submodule using `git submodule add https://github.com/dcooperdalrymple/pico-rotary-encoder.git` into the desired location. In the `CMakeLists.txt` file, insert `add_subdirectory(./{PATH_TO_SUBMODULE}/pico-rotary-encoder)` below your source files (ie: `add_executable(...)`). Then add `pico_rotary_encoder` to your list of _target_link_libraries_ such as `target_link_libraries(... pico_rotary_encoder ...)`.

## Usage
Because the rotary encoder CLK (A) GPIO pin must prepend the DT (B) GPIO pin, only the CLK pin needs to be provided during initialization. An optional SW pin can be added in the constructor to handle encoder switch press and release events.

Currently, only 2 encoders are allowed to be initialized within a program. Any additional RotaryEncoder objects will not be active.

The event callback assigned by the `.set_callback(...)` method must be in the format `void ...(RotaryEncoderEvent)` and is interrupt-driven.

````
#include "rotary_encoder.hpp"
void rotary_encoder_event(RotaryEncoderEvent event) {
    // Available options within RotaryEncoderEvent: CW, CCW, Press, Release
}
int main() {
    int clk_pin = 0; // DT pin is 1
    //RotaryEncoder rotary_encoder(clk_pin); // If not using switch
    int sw_pin = 2;
    RotaryEncoder rotary_encoder(clk_pin, sw_pin);
    rotary_encoder.set_callback(rotary_encoder_event);
    while (1) {
        sleep_ms(10);
    }
    return 0;
}
````

See `./example/pico-rotary-encoder-example.cpp` for full implementation.

## Citations

* [GitJer/Some_RPI-Pico_stuff/Rotary_encoder](https://github.com/GitJer/Some_RPI-Pico_stuff/tree/main/Rotary_encoder) _- PIO code is taken from this repository. Class modified to be event-based and support multiple encoders and GPIO switches.
