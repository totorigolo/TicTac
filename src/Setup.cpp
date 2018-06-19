#include "Setup.h"

using namespace ObjectID;

Setup settings;

Setup::Setup()
        : Object(ObjectID_t::SETUP) { }

void Setup::dumpName(ObjectID_t flag)
{
    switch (flag)
    {
    case ObjectID_t::PID:
        Serial << F("PID");
        break;
    case ObjectID_t::CAMERA:
        Serial << F("CAMERA");
        break;
    case ObjectID_t::MOTOR:
        Serial << F("MOTOR");
        break;
    case ObjectID_t::SETUP:
        Serial << F("SETUP");
        break;
    default:
        Serial << '?';
    }
}

bool Setup::parseInput(char c)
{
    switch (c)
    {
    case 'x':
    {
        pid_input_index = Input::getInt();
        view();
    }
        break;
    case 't':
    {
        pid_target_value = Input::getInt();
        view();
    }
    case 'I':
    {
        interval = Input::getInt();
        fps = interval;
        extern float fps;
        tm = millis() + interval;
        Serial << F("Interval: ") << interval << endl;
    }
        break;

    case 'C':
        toggle(ObjectID_t::CAMERA);
        break;

    case 'F':
        toggle(ObjectID_t::FPS);
        break;

    case 'M':
        toggle(ObjectID_t::MOTOR);
        break;

    case 'P':
        toggle(ObjectID_t::PID);
        break;

    case '!':
        Serial << F("Regulation : ") << (toggle(ObjectID_t::REGULATION) ? "ON" : "OFF") << endl;
        break;

    case '0':
        flags = ObjectID_t::NONE;
        break;

    case '*':
        once = ObjectID_t::ALL;
        break;

    case 'V':
        view();
        break;

    default:
        return false;
    }
    return true;
}

void Setup::view()
{
    static const char* flag_name = "PFRM---1";
    uint8_t f = 0U;
    Serial << "Setup : ";
    for (uint8_t i = 0; i < 8; i++)
    {
        char c;
        if (f & 1U)
        {
            c = flag_name[i];
        }
        else
        {
            c = '-';
        }
        f >>= 1U;
        Serial << c;
    }
    Serial << endl;
}

bool Setup::toggle(ObjectID_t bit)
{
    if (flags & bit)
        flags &= ~bit;
    else
        flags |= bit;
    return isFlag(bit);
}

bool Setup::viewPid() { return isFlag(ObjectID_t::PID); }

bool Setup::viewFps() { return isFlag(ObjectID_t::FPS); }

bool Setup::pidControlled() { return isFlag(ObjectID_t::REGULATION); }

bool Setup::viewMotors() { return isFlag(ObjectID_t::MOTOR); }

bool Setup::isFlag(ObjectID_t flag)
{
    auto result = bool((flags | once) & flag);
    if (result)
    {
		once &= ~flag;
    }
    return result;
}

void Setup::help()
{
    Serial << F(" P : toggle view pid") << endl;
    Serial << F(" M : toggle view motors") << endl;
    Serial << F(" F : toggle fps view") << endl;
    Serial << F(" V : view flags") << endl;
    Serial << F(" 0 : stop all views") << endl;
    Serial << F(" * : view all once") << endl;
    Serial << F(" ! : toggle pid control") << endl;
    Serial << F(" I#: set update interval") << endl << endl;
}

uint16_t Setup::message(Object::Message msg, uint8_t& c)
{
    switch (msg)
    {
    case Message::VIEW:
        Serial << F("Pid input : ") << pid_input_index << endl;
        Serial << F("Pid target: ") << pid_target_value << endl;
        break;

    case Message::PARSE_INPUT:
        return parseInput(c);

    case Message::HELP:
        help();
        break;

    default:
        return false;
    }
}
