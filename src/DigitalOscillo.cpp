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
        : Object(OSCILLO),
          chan_1(pin1, BLUE), chan_2(pin2, RED), m_x(WIDTH)
{
    m_next_ech = timer();
}

void DigitalOscillo::reset()
{
    m_x = 0;
    tft.fillScreen(BLACK);
    m_next_ech = timer();
}

Message::Answer DigitalOscillo::parseInput(char c)
{
    if (c != 'o') return Message::Unprocessed;

    c = Input::getChar();
    reset();

    switch (c)
    {
    case 'p':
        {
            int32_t p=Input::getInt();
            if (p) m_data.period_ech = p;
            return Message::Processed;
        }

    case 't':
        c = Input::getChar();
        if (c == FallDown || c == Raise || c == High || c == Low)
        {
            m_data.trigger = (Channel_2 & 0x80) | c;
            return Message::Processed;
        }
        else if (c == '1')
        {
            m_data.trigger &= ~Channel_2;
            return Message::Processed;
        }
        else if (c == '2')
        {
            m_data.trigger |= Channel_2;
            return Message::Processed;
        }
        break;

    case 'l':   // Assign to left motor
        chan_1.pin = 2; // TODO magics (should be taken from Ir
        chan_2.pin = A1;
            return Message::Processed;

    case 'r':
        chan_1.pin = 3;
        chan_2.pin = A2;
            return Message::Processed;

    default:
        Input::unget(c);
    }
    return Message::Unprocessed;
}

void DigitalOscillo::help() const
{
    Serial << F(" ot# : set trigger") << endl;
    Serial << F(" op# : set ech period") << endl;
    Serial << F(" opl / opr : switch to motor") << endl;
}

DigitalOscillo::Channel::Channel(uint8_t channel_pin, uint16_t col)
    : pin(channel_pin), color(col),
    last_freq(0), last_cyclic(0)
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
    Serial << F(", p ") << period << F(", ")  << cyclic_period << F("% ") << frequency() << ("Hz ");
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

bool DigitalOscillo::Channel::triggered(DigitalOscillo::TriggerType trigger, uint32_t current_time, uint8_t x)
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

    tft.drawPixel(x, (last_state ? LOW_1 : HIGH_1) + pin, BLACK);
    tft.drawPixel(x, (last_state ? HIGH_1 : LOW_1) + pin, color);

    last_state = current_state;
    if (cyclic_trigger)
    {
        cyclic_period = 100*(current_time-m_last_trigger_time) / period;
    }
    if (ret)
    {
        period = current_time - m_last_trigger_time;
        m_last_trigger_time = current_time;
    }
    return ret;
}

void DigitalOscillo::Channel::display()
{
    float f = frequency();
    tft.setTextScale(2);

    if (f != last_freq)
    {
        tft.setTextColor(BLACK);
		tft.setCursor(WIDTH-12*8, pin*2);
		tft.print(last_freq);
		tft.print(F("Hz"));

		tft.setTextColor(color);
		tft.setCursor(WIDTH-12*8, pin*2);
		tft.print(f);
		tft.print(F("Hz"));
		last_freq = f;
    }
}

void DigitalOscillo::loop()
{
    static int16_t over=0;
    uint32_t current_time = timer();
    if (current_time < m_next_ech) return;
    m_next_ech += abs(m_data.period_ech);
    if (current_time < m_next_ech) over++;

    Channel* trigger_channel;

    TriggerType trigger = m_data.trigger & ~Channel_2;

    if (m_data.trigger & Channel_2)
    {
        chan_1.triggered(trigger, current_time, m_x);
        trigger_channel = &chan_2;
    }
    else
    {
        trigger_channel = &chan_1;
        chan_2.triggered(trigger, current_time, m_x);
    }

    if (trigger_channel->triggered(trigger, current_time, m_x))
    {
        if (m_data.period_ech < 0 && (trigger_channel->period > 0))
        {
            const int ONSCREEN_PERIODS = 3;
            m_data.period_ech = -ONSCREEN_PERIODS * (int32_t) trigger_channel->period / (int32_t) WIDTH;
            if (m_data.period_ech == 0)
                m_data.period_ech = -1;
        }
        if (m_x >= WIDTH)
        {
            m_x = 0;
            view();
        }

        if (m_data.period_ech > timer_resolution/128)
            m_data.period_ech = timer_resolution/128;

    }

    if (m_x < WIDTH)
        m_x++;
    else
    {
        if (over)
        {
            Serial << "over " << over << '/' << m_data.period_ech << endl;
            over = 0;
            if (m_data.period_ech>0)
                m_data.period_ech++;
            else
                m_data.period_ech--;
        }
        return;
        chan_1.display();
        chan_2.display();
    }
}

void DigitalOscillo::message(Message& msg)
{
    switch (msg.type)
    {
    case Message::ParseInput:
        msg.answer = parseInput(msg.c);
        return;

    case Message::View:
        view();
        break;

    case Message::Loop:
        loop();
        break;

    case Message::Help:
        help();
        break;

    case Message::PersistInfo:
        msg.size = (uint8_t)(sizeof(m_data));
        msg.data_ptr = &m_data;
        return;

    default:
        msg.answer = Message::Unprocessed;
        return;
    }
    msg.answer = Message::Processed;
}
