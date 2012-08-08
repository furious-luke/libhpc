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

#ifndef libhpc_containers_shuffle_mapping_hh
#define libhpc_containers_shuffle_mapping_hh

#include "vector.hh"

namespace hpc {

   class shuffle_mapping
   {
   public:

      enum shuffle_type {
	 front,
	 back
      };

      shuffle_mapping();

      ~shuffle_mapping();

      void
      setup( vector<index>& mobile_indices,
	     shuffle_type type );

      template< class Container >
      void
      operator()( Container& ctr )
      {
	 if(this->_type == front)
	    this->shuffle_to_front(ctr);
	 else
	    this->shuffle_to_back(ctr);
      }

      template< class Container >
      void
      shuffle_to_front( Container& ctr )
      {
	 index pos = 0;
	 for(index ii = 0; ii < this->_mobile_idxs.size(); ++ii)
	    ctr[pos++] = ctr[this->_mobile_idxs[ii]];
      }

      template< class Container >
      void
      shuffle_to_back( Container& ctr )
      {
	 index pos = ctr.size() - 1;
	 for(index ii = this->_mobile_idxs.size(); ii > 0; --ii)
	    ctr[pos--] = ctr[this->_mobile_idxs[ii - 1]];
      }

      // 	 index start=0, end=ctr.size() - 1;
      // 	 for(set<index>::const_iterator it = this->_mobile_idxs.begin(); it != this->_mobile_idxs.end(); ++it) {
      // 	    index ii = *it;

      // 	    // If the index is already in the right range, leave it.
      // 	    if(ii >= ctr.size() - this->_mobile_idxs.size())
      // 	       continue;

      // 	    // Scan until we find a good spot for this one.
      // 	    while(this->_mobile_idxs.find(end) != this->_mobile_idxs.end())
      // 	       --end;

      // 	    // Put it in.
      // 	    typename Container::value_type tmp = ctr[end];
      // 	    ctr[end] = ctr[ii];
      // 	    ctr[ii] = tmp;
      // 	 }
      // }

   protected:

      vector<index> _mobile_idxs;
      shuffle_type _type;
   };
}

#endif
