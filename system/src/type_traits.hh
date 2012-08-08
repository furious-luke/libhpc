#ifndef libhpc_system_type_traits_hh
#define libhpc_system_type_traits_hh

namespace hpc {

   template< class T >
   struct type_traits
   {
      typedef T value;
      typedef const T const_value;
      typedef T& reference;
      typedef const T& const_reference;
   };
}

#endif
