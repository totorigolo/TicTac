#include "Fps.h"

Fps::Fps() : Object(FPS)
{

}

void Fps::message(Message& msg)
{
    switch(msg.type)
    {
    case Message::View:
        Serial << F("Avg fps : ") << fps << endl;
        msg.answer = Message::Processed;
        return;
    }
}
