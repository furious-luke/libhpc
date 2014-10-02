// Copyright 2012 Luke Hodkinson

// This file is part of libhpc.
// 
// libhpc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// libhpc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with libhpc.  If not, see <http://www.gnu.org/licenses/>.

#ifndef NLOG

#include <iostream>
#include "stdout.hh"

namespace hpc {
   namespace log {

      stdout::stdout( unsigned min_level,
		      const std::string& tag )
         : logger( min_level, tag )
      {
      }

      stdout::~stdout()
      {
      }

      void
      stdout::write()
      {
         std::cout << buffer().str();
	 std::cout.flush();
      }

   }
}

#endif
