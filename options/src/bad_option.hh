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

#ifndef libhpc_options_bad_option_hh
#define libhpc_options_bad_option_hh

#include "libhpc/containers/string.hh"

namespace hpc {
   namespace options {

      class bad_option
         : public std::exception
      {
      public:

         bad_option( const hpc::string& option_name );

         virtual
         ~bad_option() throw();

         virtual const char*
         what() const throw();

      public:

         hpc::string _name;
         hpc::string _msg;
      };

   }
}

#endif
