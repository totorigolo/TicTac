#include "Object.h"
#include "Setup.h"

uint8_t Object::m_count=0;
Object* Object::m_objects[Object::MAX_OBJECTS];

void Object::viewAll()
{
    {
      for(uint8_t i=0; i<m_count; i++)
        if (settings.isFlag(m_objects[i]->m_flag))
          m_objects[i]->message(VIEW);
    }
}

void Object::add(Object* object)
{
    if (m_count < MAX_OBJECTS)
    {
      m_objects[m_count++] = object;
    }
    else
      Serial << F("ERR: Cannot add object") << endl;
}

void Object::listAll()
{
  for(int i=0; i<m_count; i++)
    {
        Setup::dumpName(m_objects[i]->m_flag);
        Serial << endl;
    }
  
}

void Object::showHelp()
{
	for(uint8_t i=0; i<m_count; i++)
	{
		Serial << F("Help for ");
		Setup::dumpName(m_objects[i]->m_flag);
		Serial << endl;
		m_objects[i]->message(HELP);
	}
}
