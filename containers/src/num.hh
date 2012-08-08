#ifndef hpc_containers_hh
#define hpc_containers_hh

#include <limits>

namespace hpc {
   namespace num {

      template< class T >
      T
      approx( T x1,
              T x2,
              T delta = std::numeric_limits<T>::epsilon() )
      {
	 return (x1 > x2 - delta && x1 < x2 + delta);
      }

      template< class T >
      bool
      is_zero( T x )
      {
	 return approx( x, 0.0 );
      }

      template< class T >
      T
      distribute( T x,
                  T rank,
                  T num_ranks )
      {
         return x/num_ranks + ((rank < (x%num_ranks)) ? 1 : 0);
      }
   }
}

#endif
