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

#include <Arduino.h>

#include "Streaming.h"
#include "Store.h"

void setup()
{
	console.Init(57600);
	Input::begin();
	blinkingLed.Init();
	leftMotor.Init();
	rightMotor.Init();

	Serial << "[TicTac] I'm ready!" << endl;
}

void loop()
{
	console.Update();
	blinkingLed.Update();
}
