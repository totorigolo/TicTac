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

#include "Console.h"

#include "Store.h"

void Console::Init() { }

void Console::Update()
{
    while (Serial.available() > 0)
    {
        int incomingByte = Serial.read();

        /////////////////////////////////////////////////////////////////////////
        /// Blinking LED

        // Enabled / disabled
        if (incomingByte == 'e') blinkingLed.Enable();
        else if (incomingByte == 'd') blinkingLed.Disable();


        /////////////////////////////////////////////////////////////////////////
        /// Motors

        // Power
        if (incomingByte >= '0' && incomingByte <= '9')
            m_motorPower = 25 * (incomingByte - '0');

        // Direction
        if (incomingByte == '+') m_motorDir = 1;
        else if (incomingByte == '-') m_motorDir = -1;
        else if (incomingByte == '0') m_motorDir = 0;

        // Setup
        if (incomingByte == 'l')
        {
            leftMotor.SetPower(m_motorPower * m_motorDir);
        }
        else if (incomingByte == 'r')
        {
            rightMotor.SetPower(m_motorPower * m_motorDir);
        }
    }
}
