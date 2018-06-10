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

class BlinkingLed {
public:
    constexpr BlinkingLed(uint8_t pin, long intervalMs)
            : m_pin(pin), m_state(LOW), m_enabled(true),
              m_intervalMs(intervalMs), m_previousMs(0L) { }

    void Init();
    void Update();

    void Enable();
    void Disable();

private:
    uint8_t m_pin;
    bool m_state;
    bool m_enabled;
    long m_intervalMs;
    long m_previousMs;
};
