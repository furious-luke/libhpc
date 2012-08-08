#ifndef libhpc_containers_array_hh
#define libhpc_containers_array_hh

#include "boost_array.hh"
#include "global.hh"

namespace hpc {

   template< class T,
             int N >
   class array
      : public impl::boost::array< T, N >
   {
   public:

      typedef typename impl::boost::array<T,N>::size_type size_type;
      typedef index key_type;
      typedef T mapped_type;

   public:

      friend std::ostream&
      operator<<( std::ostream& strm,
		  const array<T,N>& obj )
      {
	 strm << "[";
	 if( obj.size() )
         {
	    strm << obj[0];
	    for( index ii = 1; ii < obj.size(); ++ii )
	       strm << ", " << obj[ii];
	 }
	 strm << "]";
	 return strm;
      }
   };
};

#endif
