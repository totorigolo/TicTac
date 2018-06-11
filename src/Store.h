// This file is part of TicTac.
//
// TicTac is free software: you can redistribute it and/or modify
//         it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// TicTac is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with TicTac.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <Arduino.h>

#include "Motor.h"
#include "Console.h"
#include "BlinkingLed.h"
#include "Pid.h"
#include "Setup.h"

// Console
extern Console console;

// Blinking LED_BUILTIN
extern BlinkingLed blinkingLed;

// Motors
extern Motor leftMotor;
extern Motor rightMotor;
extern Pid<float> pid;

extern unsigned long interval;
extern unsigned long tm;
extern float fps;
extern uint8_t pid_input_index;
extern int16_t pid_target_value;

