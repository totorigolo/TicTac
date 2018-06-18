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

#include <Streaming.h>
#include "Object.h"
#include "Input.h"
#include "Setup.h"

template<class T>
class Pid : public Object
{
	public:
		Pid(T kp, T ki, T kd)
		: Object(Setup::PID),
			m_kp(kp), m_ki(ki), m_kd(kd), m_last_error(0), m_integral(0) {}

		T update(T error)
		{
			T d=m_last_error - error;
			m_last_error = error;
			m_integral += d;
			return m_kp * error + m_ki * m_integral + m_kd * d;
		}

		void setKp(T kp) { m_kp = kp; view(); }
		void setKi(T ki) { m_ki = ki; view(); }
		void setKd(T kd) { m_kd = kd; view(); }

		// Object virtuals
		bool parseInput(char c)
    {
      if (c == 'p')
        setKp(Input::getFloat());
      else if (c == 'i')
        setKi(Input::getFloat());
      else if (c == 'd')
        setKd(Input::getFloat());
      else
        return false;
    
      return true;
    }
    

		void view() const
		{
			Serial << m_kp << F(", ") << m_ki << F(", ") << m_kd << F(") I=") << m_integral << endl;
		}

   void help() 
   {
  Serial << F(" p# i# d# setup kpid") << endl;
  Serial << F(" x# : input index") << endl;
  Serial << F(" t# : target value") << endl; 
   }

   
    bool message(Message msg, char c)
    {
      switch(c)
      {
        case PARSE_INPUT:
          return parseInput(c);
          break;

        case HELP:
          help();
          break;

          case VIEW:
            view();
            break;

        default:
          return false;
      }
    }


	private:
		T m_kp;
		T m_ki;
		T m_kd;
		T m_last_error;
		T m_integral;
};
