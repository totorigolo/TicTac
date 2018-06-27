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

class DigitalOscillo : public Object
{
public:
    enum TriggerType : unsigned char {
        FallDown = 'd',
        Raise = 'u',
        Low = 'l',
        High = 'h',
        Channel_2 = 0x80
    };

    struct Channel
    {
        Channel(uint8_t chan_pin, uint16_t color);

        bool state() const { return digitalRead(pin) == HIGH; }
        float frequency() const;
        void view() const;
        bool triggered(TriggerType, uint32_t period, uint8_t x);
        void display();

        uint8_t pin;
        bool last_state;
        uint32_t period;
        uint16_t color;
        uint8_t cyclic_period;

    private:
        // for display purpose
        float last_freq;
        uint8_t last_cyclic;
        uint32_t m_last_trigger_time;
    };

    static const uint16_t holes_count = 20;    // Nbre of holes in motor disk sensor
    static const uint32_t timer_resolution = 1000000; // Nbr of timer() ticks for 1 sec


public:
    DigitalOscillo(uint8_t pin1, uint8_t pin2);

    // Object virtuals
    bool parseInput(char c);

    void help() const;

    void view();

    void loop();
    void reset();

    uint16_t message(Message msg, uint8_t& c) override;

    static uint32_t timer() { return micros(); }

    float getFrequency() const;

    float getCyclicRatio(uint8_t channel) const;

private:
    Channel chan_1;
    Channel chan_2;

    uint16_t m_x;
    uint32_t m_next_ech;
    uint32_t m_high_period;

    struct {
        int32_t period_ech = -50;     // <0 => Auto, >0 manual
        TriggerType trigger = Raise;
    } m_data;
};
