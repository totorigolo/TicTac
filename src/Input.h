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

const int MaxInputSize = 16;
class Input {
public:

    static void begin()
    {
        delete[] buff;
        buff = new char[MaxInputSize];
        clear();
    }

    static void clear()
    {
        head = buff;
        queue = buff;
    }

    static void addChar(char c)
    {
        if (full()) return;
        inc(head);
        *head = c;
    }

    static bool full()
    {
        char* tst = head;
        inc(tst);
        return tst == queue;
    }

    static bool delLast()
    {
        if (empty()) return false;
        dec(head);
        return true;
    }

    static char getChar()
    {
        if (empty()) return 0;
        inc(queue);
        return *queue;
    }

    static void unget(char c)
    {
        *queue = c;
        dec(queue);
    }

    static bool empty()
    {
        return (head == queue);
    }

    static int getInt(char c = 0) { return static_cast<int>(getFloat(c)); }

    static float getFloat(char c = 0)
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

private:
    Input() = default;

    static void inc(char*& ptr)
    {
        ptr++;
        if (ptr == buff + MaxInputSize)
            ptr = buff;
    }

    static void dec(char*& ptr)
    {
        ptr--;
        if (ptr < buff)
            ptr = buff + MaxInputSize - 1;
    }

    static char* buff;
    static char* head;
    static char* queue;
};

