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



Motor::Motor(char name, uint8_t dir, uint8_t ir, uint8_t pinEnable)
        : Object(MOTOR),
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

Message::Answer Motor::parseInput(char c)
{
    if (c != c_name) return Message::Unprocessed;

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
    return Message::Processed;
}

void Motor::message(Message& msg)
{
    switch (msg.type)
    {
        case Message::View:
            Setup::dumpName(getFlag());
            Serial << ' ' << c_name << ':' << current_power << endl;
            msg.answer = Message::Processed;
            break;

        case Message::Help:
            help();
            msg.answer = Message::Processed;
            break;

        case Message::ParseInput:
            msg.answer = parseInput(msg.c);
            break;

        default:
            msg.answer = Message::Unprocessed;
            break;
    }
}
