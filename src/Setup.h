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

    static void dumpName(ObjectID::ObjectID_t flag);

    bool parseInput(char c);

    void view();

    bool toggle(ObjectID::ObjectID_t bit);

    bool viewPid();

    bool viewFps();

    bool pidControlled();

    bool viewMotors();

    bool isFlag(ObjectID::ObjectID_t flag);

    void help();

    uint16_t message(Message msg, uint8_t& c) override;

public:
    unsigned long interval;
    float fps;
    unsigned long tm; // FIXME: Unused

private:
    ObjectID::ObjectID_t flags = ObjectID::NONE;
	ObjectID::ObjectID_t once = ObjectID::NONE;
    uint8_t pid_input_index = 0U;  // index of GY85 array use as 'vertical' input
    uint16_t pid_target_value = 0U;
};

extern Setup settings;
