#ifndef libhpc_containers_boost_array_hh
#define libhpc_containers_boost_array_hh

#include "libhpc/memory/memory.hh"
#include <boost/array.hpp>

namespace hpc {
   namespace impl {
      namespace boost {

         template< class T,
                   int N >
         class array
            : public ::boost::array< T, N >
         {
         public:

            typedef typename ::boost::array<T,N>::size_type size_type;
            typedef typename ::boost::array<T,N>::const_reference const_reference;
            typedef typename ::boost::array<T,N>::reference reference;
         };
      }
   }
}

#endif
