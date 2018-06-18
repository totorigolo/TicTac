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

namespace ObjectID {

enum ObjectID_t : uint8_t {
    NONE = 0U,
    PID = 1U << 0U,
    FPS = 1U << 1U,
    REGULATION = 1U << 2U,
    MOTOR = 1U << 3U,
    CAMERA = 1U << 4U,
    SETUP = 1U << 5U,
    ONCE = 1U << 6U,
    ALL = 0xF,
};

} // namespace ObjectID
