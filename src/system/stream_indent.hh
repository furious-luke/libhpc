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

#ifndef libhpc_system_stream_indent_hh
#define libhpc_system_stream_indent_hh

#include <iostream>
#include <fstream>
#include "types.hh"

namespace hpc {

   struct setindent_t {
      int indent;
   };

   setindent_t
   setindent( int indent );

   std::ostream&
   operator<<( std::ostream& strm,
               setindent_t si );

   std::ostream&
   indent( std::ostream& strm );
}

#endif
