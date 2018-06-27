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

#include "Object.h"
#include "Input.h"
#include "ObjectID.h"

class Setup : public Object {

public:
    Setup();

    static void dumpName(const Object::Id& flag);

    Message::Answer parseInput(char c);

    void view();

    bool toggle(const Object::Id &bit);

    bool viewPid();

    bool viewFps();

    bool pidControlled();

    bool viewMotors();

    bool isFlag(const Object::Id& flag);

    void help();

    void message(Message& msg) override;

public:
    unsigned long interval;
    unsigned long tm; // FIXME: Unused

private:
    Id flags = NONE;
	Id once = NONE;
    uint8_t pid_input_index = 0U;  // index of GY85 array use as 'vertical' input
    uint16_t pid_target_value = 0U;
};

extern Setup settings;
