#ifndef hpc_containers_global_hh
#define hpc_containers_global_hh

#include <tuple>
#include <boost/lexical_cast.hpp>

namespace hpc {

   template< class T,
	     class U >
   std::ostream&
   operator<<( std::ostream& strm,
	       const std::pair<T, U>& obj )
   {
      strm << "(" << obj.first << ", " << obj.second << ")";
      return strm;
   }

   template< class T,
	     class U,
             class V >
   std::ostream&
   operator<<( std::ostream& strm,
               const std::tuple<T, U, V>& obj )
   {
      strm << "(" << std::get<0>( obj ) << ", " << std::get<1>( obj ) << ", " << std::get<2>( obj ) << ")";
      return strm;
   }

   ///
   /// Convert anything to a string.
   ///
   template< class Source >
   std::string
   to_string( Source src )
   {
      return boost::lexical_cast<std::string>( src );
   }
}

#endif
