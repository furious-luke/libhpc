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

#ifndef containers_scan_hh
#define containers_scan_hh

#include <iterator>
#include "libhpc/system/type_traits.hh"

namespace hpc {

   template< class InputIter,
	     class OutputIter>
   OutputIter
   counts_to_displs( InputIter first,
		     InputIter last,
		     OutputIter result )
   {
      typedef typename std::iterator_traits<InputIter>::value_type value_type;

      if(first == last)
	 return result;
      *result = 0;
      value_type val;
      while(first != last) {
	 val = *result;
	 ASSERT(*first >= 0, "Counts must be >= 0.");
	 *++result = val + *first++;
      }
      return ++result;
   }

   template< class InputSeq,
	     class OutputSeq >
   void
   counts_to_displs_resize( const InputSeq& input,
			    OutputSeq& output )
   {
      if(input.size()) {
	 output.resize(input.size() + 1);
	 counts_to_displs(input.begin(), input.end(), output.begin());
      }
      else
	 output.deallocate();
   }

   template<class Iter>
   Iter
   counts_to_displs(Iter first,
		    index size)
   {
      typedef typename std::iterator_traits<Iter>::value_type value_type;

      ASSERT(size >= 0, "Invalid size.");
      if(!size)
	 return first;
      value_type tmp, val, prev = *first;
      *first = 0;
      while(size--) {
	 val = *first++;
	 ASSERT(val >= 0, "Counts must be >= 0.");
	 tmp = *first;
	 *first = prev + val;
	 prev = tmp;
      }
      return first;
   }

   template< class Sequence >
   void
   counts_to_displs( typename type_traits<Sequence>::reference seq )
   {
      if(seq.size() > 0)
	 counts_to_displs(seq.begin(), seq.size() - 1);
   }

   template<class InputIter,
	    class OutputIter>
   OutputIter
   displs_to_counts(InputIter first,
		    InputIter last,
		    OutputIter result)
   {
      typedef typename std::iterator_traits<InputIter>::value_type value_type;

      if(first == last)
	 return result;
      value_type val = *first++;
      while(first != last) {
	 ASSERT(*first >= val, "Invalid displacements.");
	 *result++ = *first - val;
	 val = *first++;
      }
      return result;
   }

   template< class InputSeq,
	     class OutputSeq >
   void
   displs_to_counts_resize( const InputSeq& input,
			    OutputSeq& output )
   {
      if(input.size() > 1) {
	 output.resize(input.size() - 1);
	 displs_to_counts(input.begin(), input.end(), output.begin());
      }
      else
	 output.deallocate();
   }

   template<class Iter>
   Iter
   displs_to_counts(Iter first,
		    index size)
   {
      typedef typename std::iterator_traits<Iter>::value_type value_type;

      ASSERT(size >= 0, "Invalid size.");
      if(size < 2)
	 return first;
      Iter cur = first++;
      while(--size) {
	 ASSERT(*first >= *cur, "Invalid displacements.");
	 *cur = *first++ - *cur++;
      }
      return cur;
   }

   template< class Sequence >
   void
   displs_to_counts( typename type_traits<Sequence>::reference seq )
   {
      displs_to_counts(seq.begin(), seq.size());
   }
}

#endif
