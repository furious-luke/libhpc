#ifndef containers_set_hh
#define containers_set_hh

#include "libhpc/memory/memory.hh"
#include <set>

namespace hpc {

   template< class T, class Impl=std::set<T> >
   class set : public Impl {
   public:

      typedef T mapped_type;

      set()
      {
      }

      template< class Iter >
      set( Iter first,
	   Iter last )
	 : Impl(first, last)
      {
      }

      ~set()
      {
      }

      const T&
      front() const
      {
         return *this->begin();
      }

      const T&
      back() const
      {
         return *(--this->end());
      }

      bool
      has( const T& elem ) const
      {
	 return this->find(elem) != this->end();
      }

      friend std::ostream&
      operator<<( std::ostream& strm,
		  const set& obj )
      {
	 strm << "[";
	 if(obj.size()) {
	    typename set<T>::const_iterator it = obj.begin();
	    strm << *it;
	    ++it;
	    for(; it != obj.end(); ++it)
	       strm << ", " << *it;
	 }
	 strm << "]";
	 return strm;
      }
   };

};

#endif
