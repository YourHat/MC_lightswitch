cmake_minimum_required(VERSION 3.13.4)

include(pico_sdk_import.cmake)

project(servo_t C CXX ASM)
set(CMAKE_C STANDARD 11)
set(CMAKE_CXX STANDARD 17)
set(PICO_BOARD pico_w)
pico_sdk_init()

add_executable(${PROJECT_NAME} main.c)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

pico_add_extra_outputs(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR} )

target_link_libraries(${PROJECT_NAME}  pico_stdlib hardware_pwm)