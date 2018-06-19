#pragma once

#include "Input.h"
#include "Object.h"

class Console
{
	public:
		static void loop()
		{
			if (Serial.available())
			{
				{
					char c = Serial.read();

					if (c == 8)
					{
						if (Input::delLast())
							Serial << c << ' ' << c;
						return;
					}
					if (!Input::full())
						Serial << c;
					if (c != '\r' && c != '\n')
					{
						Input::addChar(c);
						return;
					}
				}

				char c;

				while(c = Input::getChar())
				{
					if (c == 'h')
					{
						Serial << F("---[ console minihelp ]----") << endl;
						Object::showHelp();
					}
					else if (c=='L')
						Object::listAll();
					else
					{
						if (!Object::parse(c))
						{
							Serial << F("Unknown command:") << c << endl;
						}
					}
				}
				Serial << F("> ");
			}
		}
};


