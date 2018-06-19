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

#include "Input.h"

namespace Input {

//////////////////////////////////////////////////////////////////
// Attributes

static char* head = nullptr;
static char* queue = nullptr;
static char* buff = nullptr;

//////////////////////////////////////////////////////////////////
// Private functions

static void inc(char*& ptr)
{
    ptr++;
    if (ptr == buff + MAX_INPUT_SIZE)
        ptr = buff;
}

static void dec(char*& ptr)
{
    ptr--;
    if (ptr < buff)
        ptr = buff + MAX_INPUT_SIZE - 1;
}

//////////////////////////////////////////////////////////////////
// Public functions

void begin()
{
    delete[] buff;
    buff = new char[MAX_INPUT_SIZE];
    clear();
}

void clear()
{
    head = buff;
    queue = buff;
}

void addChar(char c)
{
    if (full())
        return;
    inc(head);
    *head = c;
}

bool full()
{
    char* tst = head;
    inc(tst);
    return tst == queue;
}

bool delLast()
{
    if (empty()) return false;
    dec(head);
    return true;
}

char getChar()
{
    if (head == queue)
        return 0;
    inc(queue);
    return *queue;
}

void unget(char c)
{
    *queue = c;
    dec(queue);
}

bool empty()
{
    return (head == queue);
}

int getInt(char c)
{
    return int(getFloat(c));
}

float getFloat(char c)
{
    float result = 0.0;
    bool negative = false;
    bool decimals = false;
    float decimal = 0.1;
    if (c == 0) c = getChar();
    if (c == '-')
    {
        negative = true;
        c = getChar();
    }

    while (c == '.' || c == '+' || isDigit(c))
    {
        if (c == '.')
        {
            if (decimals)
                break;
            decimals = true;
        }
        else if (decimals)
        {
            result += decimal * static_cast<float>(c - '0');
            decimal *= 0.1;
        }
        else
        {
            result *= 10.0;
            result += c - '0';
        }
        c = getChar();
    }
    if (c) unget(c);

    return result * (negative ? -1.0 : 1.0);
}

} // namespace Input
