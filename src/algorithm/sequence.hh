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

#ifndef hpc_containers_algorithms_hh
#define hpc_containers_algorithms_hh

namespace hpc {

   template< class InputIter,
	     class OutputMap >
   void
   sequence_to_map( InputIter begin,
		    const InputIter& end,
		    OutputMap& out)
   {
      int ii = 0;
      while(begin != end) {
	 out.insert(std::make_pair(ii, *begin));
	 ++begin;
	 ++ii;
      }
   }

   template< class InputIter,
	     class OutputMap >
   void
   sequence_to_reverse_map( InputIter begin,
			    const InputIter& end,
			    OutputMap& out)
   {
      int ii = 0;
      while(begin != end) {
	 out.insert(std::make_pair(*begin, ii));
	 ++begin;
	 ++ii;
      }
   }
}

#endif
