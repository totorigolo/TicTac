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

#include <Streaming.h>
#include "Object.h"
#include "Input.h"
#include "Setup.h"

using PidType = float;

class Pid : public Object {
public:
    Pid();

    PidType update(PidType error);

    void resetIntegral();

    void setKp(PidType kp);

    void setKi(PidType ki);

    void setKd(PidType kd);

    // Object virtuals
    bool parseInput(char c);

    void view() const;

    void help();

    uint16_t message(Message msg, uint8_t& c);

private:
    struct {
        PidType m_kp = 0;
        PidType m_ki = 0;
        PidType m_kd = 0;
    } m_data;

private:
    PidType m_last_error = 0;
    PidType m_integral = 0;
};
