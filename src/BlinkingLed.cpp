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

#include "BlinkingLed.h"

void BlinkingLed::Init()
{
    pinMode(m_pin, OUTPUT);
    digitalWrite(m_pin, LOW);
}

void BlinkingLed::Update()
{
    if (!m_enabled) return;

    unsigned long currentMillis = millis();

    if (currentMillis - m_previousMs >= m_intervalMs)
    {
        m_previousMs = currentMillis;

        m_state = !m_state;

        digitalWrite(m_pin, static_cast<uint8_t>(m_state));
    }
}

void BlinkingLed::Enable()
{
    digitalWrite(m_pin, LOW);
    m_enabled = true;
}

void BlinkingLed::Disable()
{
    digitalWrite(m_pin, LOW);
    m_enabled = false;
}
