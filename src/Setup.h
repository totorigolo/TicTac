#pragma once

#include <Arduino.h>
#include <Streaming.h>

class Setup
{
	static const uint8_t PID=1;
	static const uint8_t FPS=2;
	static const uint8_t REGULATION=4;
	static const uint8_t MOTORS=8;
	static const uint8_t ONCE=128;


	public:
		static bool use(char c)
		{
			switch(c)
			{
				case 'P':
					toggle(PID);
					break;

				case 'F':
					toggle(FPS);
					break;

				case 'M':
					toggle(MOTORS);
					break;

				case '!':
					Serial << "Regulation : " << (toggle(REGULATION) ? "ON" : "OFF") << endl;
					break;

				case '0':
					flags = 0;
					break;

				case '1':
					if (toggle(ONCE))
						flags = 255;
					Serial << endl;
					break;

				case 'V':
					viewFlags();
					break;

				default:
					return false;
			}
			return true;
		}

		static void help()
		{
			Serial << "P : toggle view pid" << endl;
			Serial << "M : toggle view motors" << endl;
			Serial << "F : toggle fps view" << endl;
			Serial << "V : view flags" << endl;
			Serial << "0 : stop all views" << endl;
			Serial << "1 : view all once" << endl;
			Serial << "! : toggle pid control" << endl;
		}

		static void viewFlags()
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

		static bool toggle(uint8_t bit)
		{
			if (flags & bit)
				flags &= ~bit;
			else
				flags |= bit;
			return isFlag(bit);
		}

		static bool viewPid() { return isFlag(PID); }
		static bool viewFps() { return isFlag(FPS); }
		static bool pidControled() { return isFlag(REGULATION); }
		static bool viewMotors() { return isFlag(MOTORS); }

		static bool isFlag(uint8_t flag)
		{
			bool result = flags & flag;
			if (result && (flags & ONCE))
			{
				toggle(flag);
			}
			return result;
		}


	private:
		static uint8_t flags;

};
