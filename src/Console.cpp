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

#include "Console.h"

static const char BACK = 8;

void Console::loop()
{
    if (Serial.available())
    {
        // Feed Input
        char c = char(Serial.read());
        if (c == BACK)
        {
            if (Input::delLast())
                Serial << c << ' ' << c;
            return;
        }
        if (Input::full()) return;

        if (c != '\r' && c != '\n')
        {
            Serial << c;
            Input::addChar(c);
            return;
        }
        Serial << endl;

        // Consume from Input
        while ((c = Input::getChar()))
        {
            if (c == 'h')
            {
                Serial << endl;
                Object::showHelp();
            }
            else if (c == 'L')
                Object::listAll();
            else
            {
                if (Object::parse(c) == Message::Unprocessed)
                {
                    Serial << F("Unknown command:") << c << endl;
                }
            }
        }
        Serial << F("> ");
    }
}
