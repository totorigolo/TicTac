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

class Message
{
public:

        enum Type : uint8_t
        {
            View,       // reply with answer
            Loop,       // reply with answer
            Help,       // reply with answer
            ParseInput, // reply with answer, 1st char in c
            PersistInfo // reply with size and data_ptr (not answer)
        };

        enum Answer : uint8_t
        {
            Unprocessed = 0,    // Do not change this
            Processed   = 1,
            Propagate   = 2,

            ProcessedAndPropagate = Processed | Propagate,
        };

    public:
        Message() = delete;
        explicit Message(Type t) : type(t), answer(Unprocessed)  {}

        inline bool isProcessed() const
        {
            return answer & Processed;
        }

        inline bool mustBePropagated() const
        {
            return answer & Propagate;
        }

        void* data_ptr = nullptr;
        Type type;
        Answer answer;

        union
        {
            char c;
            uint8_t size = 0;
        };
};

class Object {
    static const int MAX_OBJECTS = 10;
public:
    enum Id: uint8_t
    {
        NONE = 0U,
        PID = 1U << 0U,
        FPS = 1U << 1U,
        REGULATION = 1U << 2U,
        MOTOR = 1U << 3U,
        CAMERA = 1U << 4U,
        SETUP = 1U << 5U,
        OSCILLO = 1U << 6U,
        ALL = 0xFF,
    };

    explicit Object(Id flag);

    virtual ~Object();

    virtual void message(Message& msg) = 0;

    static void add(Object* object);

    static void showHelp();

    static bool parse(char c);

    static void loopAll();

    static void viewAll();

    static void listAll();

    static void persistAll();
	static void restoreAll();

	Id getFlag() const { return m_flag; }

private:
	void restore();

private:
    static uint16_t restorePtr;
    Id m_flag;

    static uint8_t m_count;    // -1 => not initialized
    static Object* m_objects[MAX_OBJECTS];
};
