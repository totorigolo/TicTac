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

namespace Input {

const size_t MAX_INPUT_SIZE = 16;

void begin();

void clear();

void addChar(char c);

bool full();

bool delLast();

char getChar();

void unget(char c);

bool empty();

int getInt(char c = 0);

float getFloat(char c = 0);

} // namespace Input
