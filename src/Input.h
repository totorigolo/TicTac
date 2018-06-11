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

const int MaxInputSize = 16;
class Input
{
  public:
    
    static void begin()
    {
      head=buff;
      queue=buff;
    }
    
    static add(char c)
    {
      inc(head);
      *head = c;
    }

    static char getChar()
    {
      if (head == queue)
        return 0;
      inc(head);
      return *head;  
    }

    static int getInt()
    {
      int result=0;
      bool negative = false;
      while(getChar())
    }

  private:
    Input() {}

    static void inc(char* &ptr)
    {
      ptr++;
      if (ptr == buff+MaxInputSize)
        ptr = buff;
    }

    static char* buff;
    static char* head;
    static char* queue;     
};

