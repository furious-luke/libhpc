#ifndef containers_multiset_hh
#define containers_multiset_hh

#include "libhpc/memory/memory.hh"
#include <set>

namespace hpc {

   ///
   ///
   ///
   template< class T,
             class Impl = std::multiset<T> >
   class multiset
      : public Impl
   {
   public:

      typedef Impl super_type;
      typedef T mapped_type;

      ///
      ///
      ///
      multiset()
         : super_type()
      {
      }

      ///
      ///
      ///
      template< class Iter >
      multiset( Iter first,
                Iter last )
	 : super_type( first, last )
      {
      }

      ///
      ///
      ///
      ~multiset()
      {
      }

      ///
      ///
      ///
      const T&
      front() const
      {
         return *this->begin();
      }

      ///
      ///
      ///
      const T&
      back() const
      {
         return *(--this->end());
      }

      ///
      ///
      ///
      bool
      has( const T& elem ) const
      {
	 return find( elem ) != this->end();
      }

      ///
      ///
      ///
      friend std::ostream&
      operator<<( std::ostream& strm,
		  const multiset& obj )
      {
	 strm << "[";
	 if( obj.size() )
         {
            auto it = obj.begin();
	    strm << *it;
	    ++it;
            while( it != obj.end() )
	       strm << ", " << *it++;
	 }
	 strm << "]";
	 return strm;
      }
   };
};

#endif
