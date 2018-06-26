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
#include <Streaming.h>

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
        Object* object(m_objects[i]);
        
        Serial << F("Help for ");
        Setup::dumpName(object->m_flag);
        Serial << endl;
        object->message(Message::HELP, unused);
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
    for (uint8_t i = 0; i < m_count; i++)
    {
        Object* object = m_objects[i];
        if (object->message(Message::PARSE_INPUT, reinterpret_cast<uint8_t&>(c)))
        {
            uint8_t size = 0;
            object->message(Message::VIEW, size);

            size = 0;
            object->message(Message::GET_PERSIST_INFO, size);
            if (size)
                persistAll();
            return true;
        }
    }
    return false;
}

void Object::persistAll()
{
    uint16_t ptr = 0;
    uint8_t size;
    for (uint8_t i = 0; i < m_count; i++)
    {
        Object* object = m_objects[i];

        size = 0;
        uint8_t* data_ptr = reinterpret_cast<uint8_t*>( object->message(Message::GET_PERSIST_INFO, size));
        if (size == 0)
            continue;

        Serial << F("Persist ");
        Setup::dumpName(object->m_flag);

        //static_assert(sizeof(size) == 1);
        //static_assert(sizeof(object->m_flag) == 1);

        EEPROM.update(ptr++, object->m_flag);
        EEPROM.update(ptr++, size);

        while (size--) EEPROM.update(ptr++, *(data_ptr++));

        Serial << endl;
    }
}

void Object::restoreAll()
{
    for (uint8_t i = 0; i < m_count; i++)
    {
        m_objects[i]->restore();
    }
}

void Object::restore()
{
    uint16_t restorePtr = 0;
    uint8_t size = 0;
    uint8_t* data_ptr = reinterpret_cast<uint8_t*>(message(Message::GET_PERSIST_INFO, size));
    if (size == 0)
        return;

    Serial << F("Restoring ");
    Setup::dumpName(getFlag());

    uint8_t size_check = 0;
    ObjectID::ObjectID_t id_check;
    EEPROM.get(restorePtr, id_check);
    EEPROM.get(restorePtr + 1, size_check);
    if (size_check == size && id_check == m_flag)
    {
        restorePtr += 2;
        while (size)
        {
            uint8_t b;
            EEPROM.get(restorePtr++, b);
            *data_ptr++ = b;
            size--;
        }
        Serial << endl;
    }
    else
    {
        Serial << F(" Bad data !") << endl;
    }
}
