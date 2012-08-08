#ifndef hpc_containers_predicates_hh
#define hpc_containers_predicates_hh

namespace hpc {

   template< class T >
   struct always
   {

      bool
      operator()( const T& first, const T& second )
      {
	 return true;
      }

      bool
      operator()( const T& first )
      {
	 return true;
      }
   };
}

#endif
