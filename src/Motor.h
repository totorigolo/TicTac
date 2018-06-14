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
#include "Input.h"

class Motor {
	public:
		constexpr Motor(char name, uint8_t pin1, uint8_t pin2, uint8_t pinEnable)
			: m_name(name), m_pin1(pin1), m_pin2(pin2), m_pinEn(pinEnable), last_power(9999) { }

		void Init();

		void setPower(int power);
		int getPower() const { return last_power; }

		void parseInput();

		void spin(uint8_t pin1, uint8_t pin2);

		constexpr int Get4() { return 4; }

		static void help();

	private:
		char m_name;
		uint8_t m_pin1;
		uint8_t m_pin2;
		uint8_t m_pinEn;
		int last_power;
};
