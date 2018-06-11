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

template<class T>
class Pid
{
	public:
		Pid(T kp, T ki, T kd)
			: m_kp(kp), m_ki(ki), m_kd(kd), m_last_error(0), m_integral(0) {}

		T update(T error)
		{
			T d=m_last_error - error;
			m_last_error = error;
			m_integral += d;
			return m_kp * error + m_ki * m_integral + m_kd * d;
		}

		void dump() const
		{
			Serial << "Pid (" << m_kp << ", " << m_ki << ", " << m_kd << ") I=" << m_integral << endl;
		}

		void setKp(T kp) { m_kp = kp; dump(); }
		void setKi(T ki) { m_ki = ki; dump(); }
		void setKd(T kd) { m_kd = kd; dump(); }

	private:
		T m_kp;
		T m_ki;
		T m_kd;
		T m_last_error;
		T m_integral;
};
