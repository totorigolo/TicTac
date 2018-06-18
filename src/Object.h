#pragma once

#include <Arduino.h>
#include <Streaming.h>
#include "Input.h"

class Object
{
	static const int MAX_OBJECTS=10;
	public:

	enum Message
	{
		LOOP,
		VIEW,
		HELP,
		PARSE_INPUT
	};

	Object(uint8_t flag) : m_flag(flag)
	{
		add(this);
	}

	~Object()
	{
		for(uint8_t i=0; i<MAX_OBJECTS; i++)
			m_objects[i] = 0;
	}

	virtual bool message(Message msg, char c=0)=0;

	static void add(Object* object);

	static void showHelp();

	static bool parse(char c)
	{
		Serial << "parseo " << c << endl;
		for(uint8_t i=0; i<m_count; i++)
			if (m_objects[i]->message(PARSE_INPUT, c))
			{
				// Serial << c << " Handled by " << m_objects[i]->m_flag << endl;
				return true;
			}
		return false;
	}

	static bool loopAll()
	{
		for(uint8_t i=0; i<m_count; i++)
			m_objects[i]->message(LOOP);
	}

	static void viewAll();

	static void listAll();

	private:
	uint8_t m_flag;

	static uint8_t m_count;	// -1 => not initialized
	static Object* m_objects[MAX_OBJECTS];

};
