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

#ifndef hpc_algorithm_permute_hh
#define hpc_algorithm_permute_hh

#include <boost/range/algorithm/fill.hpp>

namespace hpc {

   template< class SeqT,
             class IdxT >
   void
   permute( SeqT const& seq_begin,
            SeqT const& seq_end,
            IdxT const& idx_begin )
   {
      typedef typename SeqT::value_type seq_type;
      typedef typename IdxT::value_type idx_type;

      // Need to track which indices have moved.
      size_t size = seq_end - seq_begin;
      std::vector<bool> done( size );
      boost::fill( done, false );

      // Process each position, skipping any that have
      // already been done due to the cycles.
      for( size_t ii = 0; ii < size; ++ii )
      {
         if( done[ii] || *(idx_begin + ii) == ii )
            continue;

         // Move the cycle.
         seq_type tmp[2];
         idx_type idx = ii;
         ASSERT( idx < size, "Invalid index." );
         tmp[0] = *(seq_begin + idx);
         do
         {
            idx = *(idx_begin + idx);
            tmp[1] = *(seq_begin + idx);
            ASSERT( idx < size, "Invalid index." );
            *(seq_begin + idx) = tmp[0];
            done[idx] = true;
            tmp[0] = tmp[1];
         }
         while( idx != ii );
      }
   }

}

#endif
