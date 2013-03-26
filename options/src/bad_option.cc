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

#include <sstream>
#include "bad_option.hh"

namespace hpc {
   namespace options {

      bad_option::bad_option( const hpc::string& option_name )
         : std::exception(),
           _name( option_name )
      {
         std::stringstream ss;
         ss << "Invalid option requested from dictionary.\n";
         ss << "  Option name: " << option_name << "\n";
         _msg = ss.str();
      }

      bad_option::~bad_option() throw()
      {
      }

      const char*
      bad_option::what() const throw()
      {
         return _msg.c_str();
      }

   }
}
