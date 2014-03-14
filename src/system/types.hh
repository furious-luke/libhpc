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

#ifndef libhpc_system_types_hh
#define libhpc_system_types_hh

#include <stdlib.h>
#include <stdint.h>
#include <ostream>

typedef unsigned char byte_t;

namespace hpc {

   typedef byte_t byte;
   typedef size_t index;
   typedef int16_t int16;
   typedef uint16_t uint16;
   typedef uint32_t uint32;

   std::ostream&
   operator<<( std::ostream& strm,
               byte_t val );
}

#endif