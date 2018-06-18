#pragma once

#include <Arduino.h>
#include <Streaming.h>
#include "Object.h"
#include "Input.h"

class Setup : public Object
{

	public:
		enum Flag
		{
			PID=1,
			FPS=2,
			REGULATION=4,
			MOTOR=8,
			CAMERA=16,
			SETUP=32,
			ONCE=128
		};

	public:
		Setup() : Object(SETUP) {}

		static void dumpName(Flag flag)
		{
			switch(flag)
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
				default:
					Serial << '?';
			}

		}

		bool parseInput(char c)
		{
			switch(c)
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

				case '!':
					Serial << F("Regulation : ") << (toggle(REGULATION) ? "ON" : "OFF") << endl;
					break;

				case '0':
					flags = 0;
					break;

				case '1':
					if (toggle(ONCE))
						flags = 255;
					break;

				case 'V':
					view();
					break;

				default:
					return false;
			}
			return true;
		}

		void view()
		{
			static const char* flag_name="PFRM---1";
			uint8_t f;
			Serial << "Setup : ";
			for(uint8_t i=0; i<8; i++)
			{
				char c='-';
				if (f & 1)
				{
					c = flag_name[i];
				}
				else
				{
					c = '-';
				}
				f >>=1;
				Serial << c;
			}
			Serial << endl;
		}

		bool toggle(uint8_t bit)
		{
			if (flags & bit)
				flags &= ~bit;
			else
				flags |= bit;
			return isFlag(bit);
		}

		bool viewPid() { return isFlag(PID); }
		bool viewFps() { return isFlag(FPS); }
		bool pidControled() { return isFlag(REGULATION); }
		bool viewMotors() { return isFlag(MOTOR); }

		bool isFlag(uint8_t flag)
		{
			bool result = flags & flag;
			if (result && (flags & ONCE))
			{
				toggle(flag);
			}
			return result;
		}

		void help()
		{
			Serial << F(" P : toggle view pid") << endl;
			Serial << F(" M : toggle view motors") << endl;
			Serial << F(" F : toggle fps view") << endl;
			Serial << F(" V : view flags") << endl;
			Serial << F(" 0 : stop all views") << endl;
			Serial << F(" 1 : view all once") << endl;
			Serial << F(" ! : toggle pid control") << endl;
			Serial << F(" I#: set update interval") << endl << endl;
		}

		bool message(Message msg, char c) override
		{
			switch(msg)
			{
				case VIEW:
					Serial << F("Pid input : ") << pid_input_index << endl;
					Serial << F("Pid target: ") << pid_target_value << endl;   
					break;

				case PARSE_INPUT:
					return parseInput(c);

				case HELP:
					help();
					break;

				default:
					return false;
			}
		}

	public:
		unsigned long interval;
		float fps;
		unsigned long tm;


	private:
		uint8_t flags;
		uint8_t pid_input_index;  // index of GY85 arrray use as 'vertical' input
		uint16_t pid_target_value;

};

extern Setup settings;

