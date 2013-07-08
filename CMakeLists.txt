cmake_minimum_required(VERSION 2.8)

set(CMAKE_TOOLCHAIN_FILE /home/pete/code/arduino-cmake/cmake/ArduinoToolchain.cmake)
set(CMAKE_ECLIPSE_VERSION 4.2)

set(amt_SRCS amt.cpp)
set(amt_HDRS amt.h)
set(amt_BOARD uno)

generate_arduino_firmware(amt)
