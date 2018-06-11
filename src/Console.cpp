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

void pidView()
{
	Serial << "Pid input : " << pid_input_index << endl;
	Serial << "Pid target: " << pid_target_value << endl;
}

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

		char c;

		while(c = Input::getChar())
		{
			/////////////////////////////////////////////////////////////////////////
			/// Blinking LED

			// Enabled / disabled
			if (c == 'e') blinkingLed.Enable();
			else if (c == 'd') blinkingLed.Disable();


			/////////////////////////////////////////////////////////////////////////
			/// Motors

			else if (c == 'l' || c == 'm')
			{
				leftMotor.parseInput();
				if (c == 'm')
					rightMotor.setPower(leftMotor.getPower());
			}
			else if (c == 'r')
			{
				rightMotor.parseInput();
			}

			/////////////////////////////////////////////////////////////////////////
			/// PIDs

			else if (c == 'h')
			{
				Serial << "---[ console minihelp ]----" << endl;
				Serial << "p# i# d# setup pid kx to #" << endl;
				Serial << "x# : Change pid input index" << endl;
				Serial << "t# : Change pid target value" << endl;
				Serial << "l... r... m... setup motor:" << endl;
				Motor::help();
				Setup::help();
				Serial << "I# set update interval to #" << endl << endl;
			}
			else if (c == 'x')
			{
				pid_input_index = Input::getInt();
				pidView();
			}
			else if (c == 't')
			{
				pid_target_value = Input::getInt();
				pidView();
			}
			else if (c == 'p')
				pid.setKp(Input::getFloat());
			else if (c == 'i')
				pid.setKi(Input::getFloat());
			else if (c == 'd')
				pid.setKd(Input::getFloat());
			else if (Setup::use(c))
				return;
			else if (c == 'I')
			{
				interval = Input::getInt();
				fps = interval;
				tm = millis() + interval;
				Serial << "Interval set to " << interval << endl;
			}
			else
				Serial << "Unknown command:" << c << endl;
		}
	}
}
