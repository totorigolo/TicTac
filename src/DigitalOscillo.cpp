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
  m_pin1(pin1), m_pin2(pin2), m_x(WIDTH), m_last_pin1(false)
{
  m_next_ech = timer();
}

bool DigitalOscillo::parseInput(char c)
{
  if (c != 'o') return false;
  c = Input::getChar();
  switch(c)
  {
    case 'p':
      m_data.period_ech = Input::getInt();
      return true;

    case 't':
      c = Input::getChar();
      if (c==FallDown || c==Raise || c==High || c==Low)
      {
        m_data.trigger=static_cast<TriggerType>(c);
        return true;
      }
      break;
  }
  return false;
}

void DigitalOscillo::help() const
{
  Serial << F(" ot# : set trigger") << endl;
  Serial << F(" op# : set ech period") << endl;
}

void DigitalOscillo::view()
{
  Serial << "Ech : " << m_data.period_ech << endl;
  Serial << "Trig: " << (char)m_data.trigger << endl;
  Serial << "Per : " << m_period << endl;
}

bool DigitalOscillo::triggered(bool pin1)
{
  bool ret = false;
  switch(m_data.trigger)
  {
    case FallDown:
      ret = m_last_pin1 && !pin1;
      break;
    case Raise:
      ret = !m_last_pin1 && pin1;
      break;
    case Low:
      ret = !pin1;
      break;
    case High:
      ret = pin1;
      break;
  }

  m_last_pin1 = pin1;
  return ret;
}

void DigitalOscillo::loop()
{
  auto current_time = timer();
  if (current_time < m_next_ech) return;

  m_next_ech += abs(m_data.period_ech);

  bool pin1 = digitalRead(m_pin1) == HIGH;
  bool pin2 = digitalRead(m_pin2) == HIGH;

  if (triggered(pin1))
  {
    m_period = current_time - m_last_trigger_time;
    if (m_data.period_ech < 0 && (m_period>0))
    {
        m_data.period_ech = - 3 * (int32_t)m_period / (int32_t)WIDTH;
        if (m_data.period_ech == 0)
            m_data.period_ech = -1;
    }
    m_last_trigger_time = current_time;
    if (m_x == 0)
    {
      m_x = WIDTH;
      Serial << "Period : " << m_period << ' ';
      Serial << (float)timer_resolution/(float)m_period/(float)holes_count << "Hz" << endl;
    }
  }

  tft.drawPixel(pin1 ? LOW_1 : HIGH_1, m_x, BLACK);
  tft.drawPixel(pin2 ? LOW_2 : HIGH_2, m_x, BLACK);

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

    case Message::GET_PERSIST_INFO:
      c = static_cast<Message>(sizeof(m_data));
      return reinterpret_cast<uint16_t>(&m_data);

    default:
      return uint16_t(false);
  }
  return uint16_t(true);
}
