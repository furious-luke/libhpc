#ifndef containers_list_hh
#define containers_list_hh

#include "libhpc/memory/memory.hh"
#include <ostream>
#include <list>

namespace hpc {

   template< class T >
   class list
      : public std::list<T>
   {
   public:

      list()
	 : std::list<T>()
      {
      }

      ~list()
      {
      }

      friend std::ostream&
      operator<<( std::ostream& strm,
		  const list& obj )
      {
	 strm << "[";
	 if( obj.size() ) {
	    typename list<T>::const_iterator it = obj.begin();
	    strm << *it;
	    ++it;
	    for( ; it != obj.end(); ++it )
	       strm << ", " << *it;
	 }
	 strm << "]";
	 return strm;
      }
   };
};

#endif
