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
#include "Setup.h"

using namespace ObjectID;

Motor::Motor(char name, uint8_t dir, uint8_t ir, uint8_t pinEnable)
        : Object(ObjectID_t::MOTOR),
          c_name(name), m_dir(dir), m_ir(ir), m_pinEn(pinEnable), current_power(9999)
{
    Init();
}

void Motor::Init()
{
    pinMode(m_dir, OUTPUT);
    pinMode(m_ir, INPUT);
    pinMode(m_pinEn, OUTPUT);

    setPower(0);
}

void Motor::setPower(int power)
{
    power = constrain(power, -255, 255);
    if (current_power == power) return;
    current_power = power;

    analogWrite(m_pinEn, abs(power));
    spin(power > 0);
}

void Motor::spin(bool forward)
{
    if (forward)
        current_power = abs(current_power);
    else
        current_power = -abs(current_power);
    digitalWrite(m_dir, forward ? HIGH : LOW);
}

void Motor::help() const
{
    Serial << F(" [Motor ") << c_name << ']' << endl;
    Serial << F(" + * - : change spin") << endl;
    Serial << F(" nnn   : change speed") << endl;
}

bool Motor::parseInput(char c)
{
    if (c != c_name) return false;

    while ((c = Input::getChar()))
    {
        if (c == '+')
            spin(true);
        else if (c == '-')
            spin(false);
        else if (c == '*')
            setPower(0);
        else if (c >= '0' && c <= '9')
            setPower(Input::getInt(c));
        else
        {
            Input::unget(c);
            break;
        }
    }
    return true;
}

uint16_t Motor::message(Object::Message msg, uint8_t& c)
{
    switch (msg)
    {
    case Message::VIEW:
        Setup::dumpName(getFlag());
        Serial << ' ' << c_name << ':' << current_power << endl;
        break;

    case Message::HELP:
        help();
        break;

    case Message::PARSE_INPUT:
        return uint16_t(parseInput(c));

    case LOOP:break;
    case GET_PERSIST_INFO:break;
    }
    return uint16_t(false);
}
