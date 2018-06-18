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
#include <Streaming.h>
#include "Input.h"
#include "Object.h"
#include "Setup.h"

class Motor : public Object
{
	public:
		Motor(char name, uint8_t pin1, uint8_t pin2, uint8_t pinEnable)
			: Object(Setup::MOTOR),
			c_name(name), m_pin1(pin1), m_pin2(pin2), m_pinEn(pinEnable), last_power(9999)
	{
		Init();
	}

		void Init();

		void setPower(int power);
		int getPower() const { return last_power; }

		void spin(uint8_t pin1, uint8_t pin2);
		void help() const;
		bool parseInput(char c);

		// Object virtuals
		bool message(Message msg, char c) override
		{
			switch(msg)
			{
				case VIEW:
					Serial << c_name << ',' << last_power << endl;
					break;

				case HELP:
					help();
					break;

				case PARSE_INPUT:
					return parseInput(c);

				default:
					return false;
			}
		}

	private:
		char c_name;
		uint8_t m_pin1;
		uint8_t m_pin2;
		uint8_t m_pinEn;
		int last_power;
};
