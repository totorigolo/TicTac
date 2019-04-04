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



uint16_t Object::restorePtr = 0;
uint8_t Object::m_count = 0;
Object* Object::m_objects[Object::MAX_OBJECTS];

Object::Object(Id flag)
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
    Message msg(Message::Help);

    for (uint8_t i = 0; i < m_count; i++)
    {
        Object* object(m_objects[i]);

        Serial << F("Help for ");
        Setup::dumpName(object->m_flag);
        Serial << endl;
        object->message(msg);
    }
}

void Object::viewAll()
{
    Message view(Message::View);

    for (uint8_t i = 0; i < m_count; i++)
        if (settings.isFlag(m_objects[i]->m_flag))
            m_objects[i]->message(view);
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
    Message loop(Message::Loop);

    for (uint8_t i = 0; i < m_count; i++)
        m_objects[i]->message(loop);
}

bool Object::parse(char c)
{
    bool ret = false;

    Message parseMsg(Message::ParseInput);
    parseMsg.c = c;

    for (uint8_t i = 0; i < m_count; i++)
    {
        Object* object = m_objects[i];

        object->message(parseMsg);
        if (parseMsg.isProcessed())
        {
            ret = true;
            Message viewMsg(Message::View);
            object->message(viewMsg);

            Message persistInfoMsg(Message::PersistInfo);
            object->message(persistInfoMsg);
            if (persistInfoMsg.isProcessed()) persistAll();
            if (!parseMsg.mustBePropagated()) break;
        }
    }
    return ret;
}

void Object::persistAll()
{
    uint16_t ptr = 0;
    for (uint8_t i = 0; i < m_count; i++)
    {
        Object* object = m_objects[i];

        Message persistMsg(Message::PersistInfo);
        object->message(persistMsg);
        if (!persistMsg.isProcessed())
            continue;

        uint8_t* data_ptr = (uint8_t*) persistMsg.data_ptr;

        Serial << F("Persist ");
        Setup::dumpName(object->m_flag);

        // TODO: Enclose in #if guards
        //static_assert(sizeof(size) == 1);
        //static_assert(sizeof(object->m_flag) == 1);

        auto& size = persistMsg.size;
        EEPROM.update(ptr++, object->m_flag);
        EEPROM.update(ptr++, size);

        while (size--) EEPROM.update(ptr++, *(data_ptr++));

        Serial << endl;
    }
}

void Object::restoreAll()
{
    restorePtr = 0;
    for (uint8_t i = 0; i < m_count; i++)
    {
        m_objects[i]->restore();
    }
}

void Object::restore()
{
    Message restore(Message::PersistInfo);
    message(restore);
    if (!restore.isProcessed()) return;

    uint8_t* data_ptr = (uint8_t*) restore.data_ptr;

    Serial << F("Restoring ");
    Setup::dumpName(getFlag());

    auto& size = restore.size;
    uint8_t size_check;
    Id id_check;
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
        Serial << F(" Bad data: ") << size << F(" ") << m_flag << endl;
    }
}
