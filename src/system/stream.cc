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

#include <map>
#include <thread>
#include <mutex>
#include "libhpc/debug/assert.hh"
#include "libhpc/debug/omp_help.hh"
#include "libhpc/memory/memory.hh"
#include "stream_indent.hh"

namespace hpc {

   namespace impl {

      std::map<std::pair<std::ostream*,std::thread::id>,int> curindent;
      std::mutex indent_mutex;

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
      impl::indent_mutex.lock();
      int& val = impl::curindent[std::make_pair( &strm, std::this_thread::get_id() )];
      impl::indent_mutex.unlock();
      val += si.indent;
      ASSERT( val >= 0 );
      if( val == 0 )
      {
         impl::indent_mutex.lock();
         impl::curindent.erase( std::make_pair( &strm, std::this_thread::get_id() ) );
         impl::indent_mutex.unlock();
      }
      return strm;
   }

   std::ostream&
   indent( std::ostream& strm )
   {
      impl::indent_mutex.lock();
      std::map<std::pair<std::ostream*,std::thread::id>,int>::const_iterator it = impl::curindent.find(
         std::make_pair( &strm, std::this_thread::get_id() )
         );
      impl::indent_mutex.unlock();
      int val = 0;
      if( it != impl::curindent.end() )
         val = it->second;
      for( int ii = 0; ii < val; ++ii )
         strm << " ";
      return strm;
   }

}
