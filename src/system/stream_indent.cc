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

#include "libhpc/debug/assert.hh"
#include "libhpc/memory/memory.hh"
#include "stream_indent.hh"
#include <map>

namespace hpc {
   namespace impl {

#ifdef _OPENMP
      std::map<std::pair<std::ostream*,int>,int> curindent;
#else
      std::map<std::ostream*,int> curindent;
#endif

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
#ifdef _OPENMP
      int& val = impl::curindent[std::make_pair( &strm, OMP_TID )];
#else
      int& val = impl::curindent[&strm];
#endif
      val += si.indent;
      ASSERT( val >= 0 );
      if( val == 0 )
      {
#ifdef _OPENMP
         impl::curindent.erase( std::make_pair( &strm, OMP_TID ) );
#else
         impl::curindent.erase( &strm );
#endif
      }
      return strm;
   }

   std::ostream&
   indent( std::ostream& strm )
   {
#ifdef _OPENMP
      std::map<std::pair<std::ostream*,int>,int>::const_iterator it = impl::curindent.find( std::make_pair( &strm, OMP_TID ) );
#else
      std::map<std::ostream*,int>::const_iterator it = impl::curindent.find( &strm );
#endif
      int val = 0;
      if( it != impl::curindent.end() )
         val = it->second;
      for( int ii = 0; ii < val; ++ii )
         strm << " ";
      return strm;
   }

}
