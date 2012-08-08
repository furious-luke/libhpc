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

#ifndef debug_tracer_hh
#define debug_tracer_hh

#include <sstream>

namespace debug {

   class tracer {
   public:

      static const int max_size = 16384;

      tracer();

      ~tracer();

      void
      clear();

      void
      push( const char* id );

      void
      pop();

      int
      buffer_size( int indent=0 ) const;

      int
      print( int indent=0 ) const;

      int
      sprint( char* buf,
	      int indent=0 ) const;

      friend std::ostream&
      operator<<( std::ostream& strm,
		  const tracer& obj );

   private:
      int _num_entries;
      int _pos;
      char _stack[max_size + 1];
   };
}

#endif
