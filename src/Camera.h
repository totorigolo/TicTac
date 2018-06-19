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

#define FAST_CAMERA 0

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#if FAST_CAMERA

#ifdef __AVR_ATmega2560__
#define Pin2HIGH PORTE |= _BV (4); // digitalWrite (2, HIGH);
#define Pin3HIGH PORTE |= _BV (5); // digitalWrite (3, HIGH);

#define Pin2LOW  PORTE &= ~_BV (4); // digitalWrite (2, LOW);
#define Pin3LOW  PORTE &= ~_BV (5); // digitalWrite (3, LOW);
#else
#error Define above for 368 also
#endif

#define SI_HIGH Pin3HIGH
#define SI_LOW  Pin3LOW
#define CLK_HIGH Pin2HIGH
#define CLK_LOW  Pin2LOW

#else

#define SI_HIGH  digitalWrite(m_si, HIGH);
#define SI_LOW   digitalWrite(m_si, LOW);
#define CLK_HIGH digitalWrite(m_clk, HIGH);
#define CLK_LOW  digitalWrite(m_clk, LOW);

#endif

class TSL1401 : public Object {
public:
    TSL1401(uint8_t clk, uint8_t si, uint8_t aout);

    // Object virtuals
    bool parseInput(char c);

    void help() const;

    void view();

    uint16_t message(Message msg, uint8_t& c) override;

    const uint8_t* readData();

private:
    uint8_t m_time;
    uint8_t m_si;
    uint8_t m_clk;
    uint8_t m_aout;
};

using Camera = TSL1401;
