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
const uint16_t SPACE_12 = 5;
const uint16_t LOW_1 = 10;
const uint16_t LOW_2 = LOW_1 + SPACE_12;
const uint16_t HIGH_1 = 100;
const uint16_t HIGH_2 = HIGH_1 + SPACE_12;

DigitalOscillo::DigitalOscillo(uint8_t pin1, uint8_t pin2)
        : Object(ObjectID_t::OSCILLO),
          chan_1(pin1, BLUE), chan_2(pin2, RED), m_x(WIDTH)
{
    m_next_ech = timer();
}

bool DigitalOscillo::parseInput(char c)
{
    if (c != 'o') return false;
    c = Input::getChar();
    switch (c)
    {
    case 'p':
        m_data.period_ech = Input::getInt();
        return true;

    case 't':
        c = Input::getChar();
        if (c == FallDown || c == Raise || c == High || c == Low)
        {
            m_data.trigger = (Channel_2 & 0x80) | c;
            return true;
        }
        else if (c == '1')
        {
            m_data.trigger &= ~Channel_2;
            return true;
        }
        else if (c == '2')
        {
            m_data.trigger |= Channel_2;
            return true;
        }
        break;
    default:
        Input::unget(c);
    }
    return false;
}

void DigitalOscillo::help() const
{
    Serial << F(" ot# : set trigger") << endl;
    Serial << F(" op# : set ech period") << endl;
}

DigitalOscillo::Channel::Channel(uint8_t channel_pin, uint16_t col)
    : pin(channel_pin), color(col)
{
    // pinMode(pin, INPUT);
}

float DigitalOscillo::Channel::frequency() const
{
    return (float) timer_resolution / (float) period / (float) holes_count;
}

void DigitalOscillo::Channel::view() const
{
    Serial << F("Chan.") << pin;
    if (pin < 10) Serial << ' ';
    Serial << F(", p ") << period << F(", ") << frequency() << ("Hz ");
}

void DigitalOscillo::view()
{
    chan_1.view();
    Serial << ' ';
    chan_2.view();
    Serial << F(" Ech : ") << m_data.period_ech << ' ';
    Serial << F(" Trig(") << (m_data.trigger & Channel_2 ? 2 : 1) << F(") ")
    << (char)( m_data.trigger  & ~Channel_2)<< endl;
}

bool DigitalOscillo::Channel::triggered(DigitalOscillo::TriggerType trigger, uint8_t x)
{
    bool ret = false;
    bool cyclic_trigger = false;
    bool current_state = state();

    switch (trigger)
    {
    case FallDown:
        ret = last_state && !current_state;
        cyclic_trigger = !last_state && current_state;
        break;
    case Raise:
        ret = !last_state && current_state;
        cyclic_trigger = last_state && !current_state;
        break;
    case Low:
        ret = !current_state;
        cyclic_trigger = current_state;
        break;
    case High:
        ret = current_state;
        cyclic_trigger = !current_state;
        break;
    }

    tft.drawPixel((last_state ? LOW_1 : HIGH_1) + pin, x, BLACK);
    tft.drawPixel((last_state ? HIGH_1 : LOW_1) + pin, x, color);

    last_state = current_state;
    return ret;
}

void DigitalOscillo::loop()
{
    uint32_t current_time = timer();
    if (current_time < m_next_ech) return;
    m_next_ech += abs(m_data.period_ech);

    TriggerType trigger = m_data.trigger & ~Channel_2;
    bool on_trigger;

    if (m_data.trigger & Channel_2)
    {
        chan_1.triggered(trigger, m_x);
        on_trigger = chan_2.triggered(trigger, m_x);
    }
    else
    {
        on_trigger = chan_1.triggered(trigger, m_x);
        chan_2.triggered(trigger, m_x);
    }

    if (on_trigger)
    {
        chan_1.period = current_time - m_last_trigger_time;
        if (m_data.period_ech < 0 && (chan_1.period > 0))
        {
            const int ONSCREEN_PERIODS = 3;
            m_data.period_ech = -ONSCREEN_PERIODS * (int32_t) chan_1.period / (int32_t) WIDTH;
            if (m_data.period_ech == 0)
                m_data.period_ech = -1;
        }
        m_last_trigger_time = current_time;
        if (m_x == 0)
        {
            m_x = WIDTH;
            view();
        }

        // Max 1s decl
        if (m_data.period_ech > timer_resolution)
            m_data.period_ech = timer_resolution;
    }

    if (m_x > 0) m_x--;
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

    case Message::GET_PERSIST_INFO:
        c = static_cast<Message>(sizeof(m_data));
        return &m_data;

    default:
        return uint16_t(false);
    }
    return uint16_t(true);
}
