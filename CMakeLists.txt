cmake_minimum_required(VERSION 3.13)

set(NAME pico_rotary_encoder)
set(URL "https://github.com/dcooperdalrymple/pico-rotary-encoder")

add_library(${NAME} INTERFACE)

pico_generate_pio_header(${NAME} ${CMAKE_CURRENT_LIST_DIR}/src/rotary_encoder.pio)

target_sources(${NAME} INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/rotary_encoder.cpp
)

target_include_directories(${NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

# Pull in pico libraries that we need
target_link_libraries(${NAME} INTERFACE pico_stdlib hardware_gpio hardware_pio hardware_irq)
