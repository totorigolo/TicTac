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

#include "Pid.h"

Pid::Pid()
        : Object(PID) { }

PidType Pid::update(PidType error)
{
    PidType d = m_last_error - error;
    m_last_error = error;
    m_integral += error;
    return m_data.m_kp * error + m_data.m_ki * m_integral + m_data.m_kd * d;
}

void Pid::resetIntegral()
{
    m_integral = 0;
}

void Pid::setKp(PidType kp)
{
    m_data.m_kp = kp;
//    view();
}

void Pid::setKi(PidType ki)
{
    m_data.m_ki = ki;
//    view();
}

void Pid::setKd(PidType kd)
{
    m_data.m_kd = kd;
//    view();
}

Message::Answer Pid::parseInput(char c)
{
    if (c == 'p')
        setKp(Input::getFloat());
    else if (c == 'i')
        setKi(Input::getFloat());
    else if (c == 'd')
        setKd(Input::getFloat());
    else
        return Message::Unprocessed;

    return Message::Processed;
}

void Pid::view() const
{
    Setup::dumpName(getFlag());
    Serial << '(' << m_data.m_kp << F(", ") << m_data.m_ki << F(", ") << m_data.m_kd << F(") I=") << m_integral << endl;
}

void Pid::help()
{
    Serial << F(" p# i# d# setup kpid") << endl;
    Serial << F(" x# : input index") << endl;
    Serial << F(" t# : target value") << endl;
}

void Pid::message(Message& msg)
{
    switch (msg.type)
    {
    case Message::ParseInput:
        msg.answer = parseInput(msg.c);
        break;

    case Message::Help:
        help();
        msg.answer = Message::Processed;
        break;

    case Message::View:
        view();
        msg.answer = Message::Processed;
        break;

    case Message::PersistInfo:
        msg.size = sizeof(m_data);
        msg.data_ptr = &m_data;
        msg.answer = Message::Processed;
        break;

    default:
        msg.answer = Message::Unprocessed;
        break;
    }
}
