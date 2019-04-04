# This file is part of TicTac.
#
# TicTac is free software: you can redistribute it and/or modify
#         it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# TicTac is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with TicTac.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)

# Compiler parameters
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -felide-constructors -pedantic -Wall -Wextra")
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

# Board settings
set(ARDUINO_DEFAULT_BOARD pro)
set(ARDUINO_DEFAULT_PORT /dev/ttyACM1)
set(ARDUINO_CPU 16MHzatmega328)

# Libraries
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs)

# Source files listing
set(TicTac_src
        src/DigitalOscillo.cpp
        src/Camera.cpp
        src/Object.cpp
        src/Console.cpp
        src/Motor.cpp
        src/Input.cpp
        src/Setup.cpp
        src/TicTac.cpp
        src/Pid.cpp
        )

set(TicTac_hdrs
        src/DigitalOscillo.h
        src/Camera.h
        src/Object.h
        src/Console.h
        src/Motor.h
        src/Input.h
        src/Setup.h
        src/Pid.h
        )

# Project
project(TicTac)
generate_arduino_firmware(${CMAKE_PROJECT_NAME}
        SRCS ${TicTac_src}
        HDRS ${TicTac_hdrs}
        )
