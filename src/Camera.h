#pragma once
#include <Arduino.h>
#include "Object.h"
#include "Setup.h"

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif


 
#define Pin2HIGH PORTE |= _BV (4); // digitalWrite (2, HIGH);
#define Pin3HIGH PORTE |= _BV (5); // digitalWrite (3, HIGH);

#define Pin2LOW  PORTE &= ~_BV (4); // digitalWrite (2, LOW);
#define Pin3LOW  PORTE &= ~_BV (5); // digitalWrite (3, LOW);

class TSL1401 : public Object
{
	public:
		TSL1401(uint8_t clk, uint8_t si, uint8_t aout)
			: Object(Setup::CAMERA),
			m_si(si), m_clk(clk), m_aout(aout)
	{
		pinMode(m_si, OUTPUT);
		pinMode(m_clk, OUTPUT);
		pinMode(m_aout, INPUT);	// analog input
		 sbi(ADCSRA,ADPS2) ;
 cbi(ADCSRA,ADPS1) ;
 cbi(ADCSRA,ADPS0) ;
		m_time = 170;
	}

	#if 1 // FAST 2560
		#define SI_HIGH Pin3HIGH
		#define SI_LOW  Pin3LOW
		#define CLK_HIGH Pin2HIGH
		#define CLK_LOW  Pin2LOW
	#else
		#define SI_HIGH  digitalWrite(m_si, HIGH);
		#define SI_LOW   digitalWrite(m_si, LOW);
		#define CLK_HIGH digitalWrite(m_clk, HIGH);
		#define CLK_LOW  digitalWrite(m_clk, LOW);
	#endif

		// Object virtuals
		bool parseInput(char c)
		{
			if (c=='s')
			{
				m_time=Input::getInt();
				return true;
			}
			return false;
		}

		void help() const
		{
			Serial << " s# : Change sample speed" << endl;
		}

		void view();

		bool message(Message msg, char c)
		{
			switch(msg)
			{
				case PARSE_INPUT:
					return parseInput(c);
					break;

				case VIEW:
					view();
					break;

				case HELP:
					help();
					break;

				default:
					return false;
			}
			return true;
		}

		const uint8_t* readData(void)  
		{
			SI_HIGH
			delayMicroseconds(m_time/2);
			CLK_HIGH
			delayMicroseconds(m_time/2);
			SI_LOW
			delayMicroseconds(m_time/2);
			CLK_LOW
			delayMicroseconds(m_time);

			for(int pixel=0; pixel<128; pixel++)
			{ 
				CLK_HIGH
				m_data[pixel]=analogRead(m_aout);
				delayMicroseconds(m_time);
				CLK_LOW
				delayMicroseconds(m_time);
			}
			// delay(20);
			return &m_data[0];
		}

	private:
		uint8_t m_si;
		uint8_t m_clk;
		uint8_t m_aout;
		uint8_t m_data[128];
		uint8_t m_time;
};

typedef TSL1401 Camera;
