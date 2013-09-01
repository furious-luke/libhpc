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

#include <limits>
#include "libhpc/debug/debug.hh"
#include "boost_match.hh"

namespace hpc {
   namespace re {
      namespace impl {
         namespace boost {

            uint16
            match::last_capture() const
            {
               uint16 last = std::numeric_limits<uint16>::max();
               for( unsigned ii = 1; ii < size(); ++ii )
               {
                  if( (*this)[ii].matched )
                     last = ii - 1;
               }
               return last;
            }

            uint16
            match::num_captures() const
            {
               return size();
            }

            match::capture_type
            match::capture( uint16 idx ) const
            {
	       unsigned pos = position( idx + 1 );
	       unsigned len = length( idx + 1 );
               return std::make_pair( pos, pos + len );
            }
         }
      }
   }
}
