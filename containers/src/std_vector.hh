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
