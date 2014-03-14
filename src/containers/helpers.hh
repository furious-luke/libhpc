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

#ifndef libhpc_containers_helpers_hh
#define libhpc_containers_helpers_hh

#include <set>

namespace hpc {

   template< class Seq >
   bool
   has_duplicates( Seq const& seq )
   {
      std::set<typename Seq::value_type> set;
      unsigned cnt = 0;
      for( auto const& x : seq )
      {
         set.insert( x );
         ++cnt;
      }
      return set.size() == cnt;
   }

   template< class Seq >
   bool
   has_element( Seq const& seq,
                typename Seq::value_type const& elem )
   {
      return std::find( seq.begin(), seq.end(), elem ) != seq.end();
   }

   template< class Seq >
   bool
   is_ordered( Seq const& seq )
   {
      auto it = seq.begin();
      if( it != seq.end() )
      {
         auto last = it++;
         while( it != seq.end() )
         {
            if( *it++ <= *last++ )
               return false;
         }
      }
      return true;
   }

}

#endif
