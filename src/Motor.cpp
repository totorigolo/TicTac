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

#include "Motor.h"
#include "Streaming.h"

void Motor::Init()
{
    pinMode(m_pin1, OUTPUT);
    pinMode(m_pin2, OUTPUT);
    pinMode(m_pinEn, OUTPUT);

    SetPower(0);
}

void Motor::SetPower(int power)
{
  spin(power>0 ? HIGH : LOW , power>=0 ? LOW : HIGH);

    analogWrite(m_pinEn, abs(power));

//    Serial << "Motor " << m_name << ": " << power << endl;
}

void Motor::spin(uint8_t pin1, uint8_t pin2)
{
  digitalWrite(m_pin1, pin1);
  digitalWrite(m_pin2, pin2);
}

void Motor::parseInput()
{
  char c=Input::getChar();
  if (c == '+')
    spin(HIGH, LOW);
  else if (c == '-')
    spin(LOW, HIGH);
  else if (c == '*')
    spin(LOW, LOW);
  else if (c>='0' && c<='9' || c=='-')
    SetPower(Input::getInt());
}

