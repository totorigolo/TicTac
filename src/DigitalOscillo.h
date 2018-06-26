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
#include "Object.h"
#include "Setup.h"

class DigitalOscillo : public Object {
    const uint16_t holes_count = 20;    // Nbre of holes in motor disk sensor
    const uint32_t timer_resolution = 1000000; // Nbr of timer() ticks for 1 sec
public:
    enum TriggerType : char {
        FallDown = 'F',
        Raise = 'R',
        Low = '0',
        High = '1'
    };

public:
    DigitalOscillo(uint8_t pin1, uint8_t pin2);

    // Object virtuals
    bool parseInput(char c);

    void help() const;

    void view();

    void loop();

    uint16_t message(Message msg, uint8_t& c) override;

    bool triggered(bool pin);

    uint32_t getPeriod() const { return m_period; }

    static uint32_t timer() { return micros(); }

private:
    uint8_t m_pin1;
    uint8_t m_pin2;
    uint16_t m_x;
    bool m_last_pin1;
    uint32_t m_next_ech;
    uint32_t m_period;
    uint32_t m_last_trigger_time;

    struct {
        int32_t period_ech = -50;     // <0 => Auto, >0 manual
        TriggerType trigger = Raise;
    } m_data;
};
