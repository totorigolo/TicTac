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

Motor::Motor(char name, uint8_t pin1, uint8_t pin2, uint8_t pinEnable)
        : Object(ObjectID_t::MOTOR),
          c_name(name), m_pin1(pin1), m_pin2(pin2), m_pinEn(pinEnable), last_power(9999)
{
    Init();
}

void Motor::Init()
{
    pinMode(m_pin1, OUTPUT);
    pinMode(m_pin2, OUTPUT);
    pinMode(m_pinEn, OUTPUT);

    setPower(0);
}

void Motor::setPower(int power)
{
    power = constrain(power, -255, 255);
    if (last_power == power) return;
    last_power = power;
    spin(uint8_t(power > 0 ? HIGH : LOW), uint8_t(power >= 0 ? LOW : HIGH));

    analogWrite(m_pinEn, abs(power));
}

void Motor::spin(uint8_t pin1, uint8_t pin2)
{
    if (pin1 == HIGH)
        last_power = abs(last_power);
    else
        last_power = -abs(last_power);
    digitalWrite(m_pin1, pin1);
    digitalWrite(m_pin2, pin2);
}

void Motor::help() const
{
    Serial << F("MOT ") << c_name << ':' << endl;
    Serial << F(" + * - : change spin") << endl;
    Serial << F(" nnn   : change speed") << endl;
}

bool Motor::parseInput(char c)
{
    if (c != c_name) return false;

    while ((c = Input::getChar()))
    {
        if (c == '+')
            spin(HIGH, LOW);
        else if (c == '-')
            spin(LOW, HIGH);
        else if (c == '*')
            spin(LOW, LOW);
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
    switch(msg)
    {
    case Message::VIEW:
        Serial << c_name << ',' << last_power << endl;
        break;

    case Message::HELP:
        help();
        break;

    case Message::PARSE_INPUT:
        return parseInput(c);

    default:
        return false;
    }
}
