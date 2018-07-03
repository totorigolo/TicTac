#include "Setup.h"

Setup settings;

Setup::Setup()
        : Object(Id::SETUP) { }

void Setup::dumpName(const Object::Id& flag)
{
    switch (flag)
    {
    case PID:
        Serial << F("PID");
        break;
    case CAMERA:
        Serial << F("CAMERA");
        break;
    case MOTOR:
        Serial << F("MOTOR");
        break;
    case SETUP:
        Serial << F("SETUP");
        break;
    case OSCILLO:
        Serial << F("OSCILLO");
        break;
    default:
        Serial << '?';
    }
}

Message::Answer Setup::parseInput(char c)
{
    switch (c)
    {
    case 'x':
        pid_input_index = Input::getInt();
        view();
        break;

    case 't':
        pid_target_value = Input::getInt();
        view();
        break;

    case 'I':
        // TODO, use an interrupt for PID
        interval = Input::getInt();
        tm = millis() + interval;
        Serial << F("Interval: ") << interval << endl;
        break;

    case 'C':
        toggle(CAMERA);
        break;

    case 'F':
        toggle(FPS);
        break;

    case 'M':
        toggle(MOTOR);
        break;

    case 'P':
        toggle(PID);
        break;

    case 'O':
        toggle(OSCILLO);
        break;

    case '!':
        Serial << F("Regulation : ") << (toggle(REGULATION) ? "ON" : "OFF") << endl;
        break;

    case '0':
        flags = NONE;
        break;

    case '*':
        once = ALL;
        break;

    case 'V':
        view();
        break;

    default:
        return Message::Unprocessed;
    }
    return Message::Processed;
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

bool Setup::toggle(const Id& bit)
{
    if (flags & bit)
        flags &= ~bit;
    else
        flags |= bit;
    return isFlag(bit);
}

bool Setup::viewPid() { return isFlag(PID); }

bool Setup::viewFps() { return isFlag(FPS); }

bool Setup::pidControlled() { return isFlag(REGULATION); }

bool Setup::viewMotors() { return isFlag(MOTOR); }

bool Setup::isFlag(const Id& flag)
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
    Serial << F(" I#: set update interval") << endl;
}

void Setup::message(Message& msg)
{
    switch (msg.type)
    {
    case Message::View:
        Serial << F("Pid input : ") << pid_input_index << endl;
        Serial << F("Pid target: ") << pid_target_value << endl;
        msg.answer = Message::Processed;
        break;

    case Message::ParseInput:
        msg.answer = parseInput(msg.c);
        break;

    case Message::Help:
        help();
        msg.answer = Message::Processed;
        break;

    default:
        msg.answer = Message::Unprocessed;
        break;
    }
}
