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

#include <TFT_ST7735.h>
#include "DigitalOscillo.h"

using namespace ObjectID;

extern TFT_ST7735 tft;

static const uint16_t WIDTH = 159;

DigitalOscillo::DigitalOscillo(uint8_t pin1, uint8_t pin2)
        : Object(ObjectID_t::OSCILLO),
          m_pin1(pin1), m_pin2(pin2), m_x(WIDTH), m_last_pin1(false)
{
    m_next_micro = micros();
}

bool DigitalOscillo::parseInput(char c)
{
    return false;
}

void DigitalOscillo::help() const { }

void DigitalOscillo::view() { }

void DigitalOscillo::loop()
{
    if (micros() < m_next_micro) return;
    m_next_micro += m_period;

    const uint16_t SPACE_12 = 5;
    const uint16_t LOW_1 = 10;
    const uint16_t LOW_2 = LOW_1 + SPACE_12;
    const uint16_t HIGH_1 = 100;
    const uint16_t HIGH_2 = HIGH_1 + SPACE_12;

    bool pin1 = digitalRead(m_pin1) == HIGH;
    bool pin2 = digitalRead(m_pin2) == HIGH;

    if (pin1 && !m_last_pin1 && m_x == 0)
    {
        tft.clearScreen();
        m_x = WIDTH;
    }
    m_last_pin1 = pin1;

    tft.drawPixel(pin1 ? HIGH_1 : LOW_1, m_x, RED);
    tft.drawPixel(pin2 ? HIGH_2 : LOW_2, m_x, BLUE);

    if (m_x > 0) m_x--;
//    if (m_x == 0) m_x = WIDTH;
}

uint16_t DigitalOscillo::message(Object::Message msg, uint8_t& c)
{
    switch (msg)
    {
    case Message::PARSE_INPUT:
        return uint16_t(parseInput(c));

    case Message::VIEW:
        view();
        break;

    case Message::LOOP:
        loop();
        break;

    case Message::HELP:
        help();
        break;

    default:
        return uint16_t(false);
    }
    return uint16_t(true);
}
