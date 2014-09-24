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

#ifndef hpc_sort_by_key_hh
#define hpc_sort_by_key_hh

#include <algorithm>
#include <tuple>
#include <boost/iterator/iterator_facade.hpp>
#include "sort_permute_iter.hh"

namespace hpc {

   template< class KeyIter,
	     class ValIter >
   void
   sort_by_key( KeyIter const& key_begin,
		KeyIter const& key_end,
		ValIter const& val_begin,
		ValIter const& val_end )
   {
      std::sort(
	 make_sort_permute_iter( key_begin, val_begin ),
	 make_sort_permute_iter( key_end, val_end ),
	 sort_permute_iter_compare<KeyIter,ValIter>()
	 );
   }

   template< class KeySeq,
	     class ValSeq >
   void
   sort_by_key( KeySeq& keys,
		ValSeq& vals )
   {
      sort_by_key( keys.begin(), keys.end(), vals.begin(), vals.end() );
   }

}

#endif
