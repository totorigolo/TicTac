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
#include <Streaming.h>

#include "Input.h"
#include "ObjectID.h"

class Object {
    static const int MAX_OBJECTS = 10;
public:

    enum Message: uint8_t {
        LOOP,
        VIEW,
        HELP,
        PARSE_INPUT,
        GET_PERSIST_INFO
    };

    explicit Object(ObjectID::ObjectID_t flag);

    virtual ~Object();

    virtual uint16_t message(Message msg, uint8_t& c) = 0;

    static void add(Object* object);

    static void showHelp();

    static bool parse(char c);

    static void loopAll();

    static void viewAll();

    static void listAll();

    static void persistAll();
	static void restoreAll();

	ObjectID::ObjectID_t getFlag() const { return m_flag; }

private:
	void restore();

private:
    ObjectID::ObjectID_t m_flag;

    static uint8_t m_count;    // -1 => not initialized
    static Object* m_objects[MAX_OBJECTS];
};
