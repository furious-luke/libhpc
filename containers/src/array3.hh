#ifndef libhpc_containers_array3_hh
#define libhpc_containers_array3_hh

#include "array.hh"

namespace hpc {

   template< class T >
   class array< T, 3 >
      : public impl::boost::array< T, 3 >
   {
   public:

      array()
      {
      }

      array( const T& x,
             const T& y,
             const T& z )
      {
         this->elems[0] = x;
         this->elems[1] = y;
         this->elems[2] = z;
      }
   };
};

#endif
