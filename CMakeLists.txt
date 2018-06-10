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
project(TicTac)

set(TicTac_src
        src/BlinkingLed.cpp
        src/Console.cpp
        src/Motor.cpp
        src/Store.cpp
        src/TicTac.cpp
        )

set(TicTac_hdrs
        src/BlinkingLed.h
        src/Console.h
        src/Motor.h
        src/Store.h
        src/Streaming.h
        )

generate_arduino_firmware(${CMAKE_PROJECT_NAME}
        SRCS ${TicTac_src}
        HDRS ${TicTac_hdrs}
        PORT /dev/ttyACM1
        BOARD uno)
