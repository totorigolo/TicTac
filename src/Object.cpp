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

#include <EEPROM.h>

#include "Object.h"
#include "Setup.h"

using namespace ObjectID;

uint8_t Object::m_count = 0;
Object* Object::m_objects[Object::MAX_OBJECTS];

Object::Object(ObjectID_t flag)
        : m_flag(flag)
{
    add(this);
}

Object::~Object()
{
    for (auto& m_object : m_objects)
        m_object = nullptr;
}

void Object::add(Object* object)
{
    if (m_count < MAX_OBJECTS)
    {
        m_objects[m_count++] = object;
    }
    else
        Serial << F("ERR: Too many Objects") << endl;
}

void Object::showHelp()
{
    uint8_t unused;
    for (uint8_t i = 0; i < m_count; i++)
    {
        Serial << F("Help for ");
        Setup::dumpName(m_objects[i]->m_flag);
        Serial << endl;
        m_objects[i]->message(Message::HELP, unused);
    }
}

void Object::viewAll()
{
    uint8_t unused;
    for (uint8_t i = 0; i < m_count; i++)
        if (settings.isFlag(m_objects[i]->m_flag))
            m_objects[i]->message(Message::VIEW, unused);
}

void Object::listAll()
{
    for (int i = 0; i < m_count; i++)
    {
        Setup::dumpName(m_objects[i]->m_flag);
        Serial << endl;
    }

}

void Object::loopAll()
{
    uint8_t unused;
    for (uint8_t i = 0; i < m_count; i++)
        m_objects[i]->message(Message::LOOP, unused);
}

bool Object::parse(char c)
{
    Serial << "parseo " << c << endl;
    for (uint8_t i = 0; i < m_count; i++)
        if (m_objects[i]->message(Message::PARSE_INPUT, reinterpret_cast<uint8_t&>(c)))
        {
            // Serial << c << " Handled by " << m_objects[i]->m_flag << endl;
            return true;
        }
    return false;
}

void Object::persistAll()
{
    uint16_t lastIdx = 0;
    uint8_t size = 0;
    for (uint8_t i = 0; i < m_count; i++)
    {
        uint16_t data_ptr = m_objects[i]->message(Message::GET_PERSIST_INFO, size);

        for (uint8_t offset = 0; offset < size; ++offset)
        {
            EEPROM.write(lastIdx + offset, *reinterpret_cast<uint8_t*>(data_ptr + offset));
        }
        lastIdx += size;
    }
}
