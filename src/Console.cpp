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

void Console::Update()
{
	if (Serial.available())
	{
		{
			char c = Serial.read();

			if (c != '\r' && c != '\n')
			{
				Input::addChar(c);
				return;
			}
		}

		char c = Input::getChar();
		/////////////////////////////////////////////////////////////////////////
		/// Blinking LED

		// Enabled / disabled
		if (c == 'e') blinkingLed.Enable();
		else if (c == 'd') blinkingLed.Disable();


		/////////////////////////////////////////////////////////////////////////
		/// Motors

		if (c == 'l')
		{
			leftMotor.parseInput();
		}
		else if (c == 'r')
		{
			rightMotor.parseInput();
		}

		/////////////////////////////////////////////////////////////////////////
		/// PIDs

		if (c == 'p')
			pid.setKp(Input::getFloat());
		else if (c == 'i')
			pid.setKi(Input::getFloat());
		else if (c == 'd')
			pid.setKd(Input::getFloat());
		else if (c == 'f')
		{
			Serial << "FLOAT : " << Input::getFloat() << endl;
		}
		Input::clear();
	}
}
