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

#ifndef containers_std_vector_hh
#define containers_std_vector_hh

#include "libhpc/memory/memory.hh"
#include <vector>
#include "vector_view.hh"

namespace hpc {
   namespace impl {
      namespace std {

         template<class T>
         class vector
            : public ::std::vector<T>
         {
         public:

            typedef typename ::std::vector<T>::size_type size_type;
            typedef typename ::std::vector<T>::const_reference const_reference;
            typedef typename ::std::vector<T>::reference reference;

            typedef vector_view<vector<T> > view;

            vector();

            explicit vector( size_type size );

            template< class Iter >
            vector( Iter first,
                    Iter last )
               : ::std::vector<T>(first, last)
            {
            }

            ~vector();

            void
            deallocate();

            void
            reallocate(size_type size);

            // void
            // duplicate(const vector<T>& src);

            void
            take(vector<T>& src);

            /// Dereference element at position 'idx'. I am overriding the STL version of
            /// this method because it does not check for out of bounds by default. I would
            /// very much like it to.
            const_reference
            operator[](size_type idx) const;

            /// Dereference element at position 'idx'. I am overriding the STL version of
            /// this method because it does not check for out of bounds by default. I would
            /// very much like it to.
            reference
            operator[](size_type idx);
         };
      }
   }
}

#include "std_vector.tcc"

#endif
