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

#include "libhpc/debug/debug.hh"
#include "libhpc/memory/memory.hh"
#include "stream_indent.hh"
#include <map>

namespace hpc {
   namespace impl {
      std::map<std::ostream*,int> curindent;
   }

   setindent_t
   setindent( int indent )
   {
      setindent_t ind;
      ind.indent = indent;
      return ind;
   }

   std::ostream&
   operator<<( std::ostream& strm,
               setindent_t si )
   {
      int& val = impl::curindent[&strm];
      val += si.indent;
      ASSERT( val >= 0 );
      if( val == 0 )
         impl::curindent.erase( &strm );
      return strm;
   }

   std::ostream&
   indent( std::ostream& strm )
   {
      auto it = impl::curindent.find( &strm );
      int val = 0;
      if( it != impl::curindent.end() )
         val = it->second;
      for( int ii = 0; ii < val; ++ii )
         strm << " ";
      return strm;
   }
}
