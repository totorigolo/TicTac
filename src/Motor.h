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
		Motor(char name, uint8_t dir, uint8_t ir, uint8_t pinEnable);

		void Init();

		void setPower(int power);
		int getPower() const { return current_power; }

		void spin(bool foward);
		void help() const;
		Message::Answer parseInput(char c);

		// Object virtual
        void message(Message& msg) override;

	private:
		char c_name;
		uint8_t m_dir;
		uint8_t m_ir;
		uint8_t m_pinEn;
		int current_power;
};
