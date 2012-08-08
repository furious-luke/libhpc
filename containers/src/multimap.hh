#ifndef containers_multimap_hh
#define containers_multimap_hh

#include "libhpc/memory/memory.hh"
#include <map>

namespace hpc {

   template< class Key,
             class Val >
   class multimap
      : public std::multimap<Key, Val>
   {
   public:

      typedef typename std::multimap<Key,Val>::const_iterator const_iterator;

      multimap()
      {
      }

      ~multimap()
      {
      }

      typename std::multimap<Key,Val>::iterator
      insert( const Key& key, const Val& val )
      {
         return this->insert( std::pair<Key,Val>( key, val ) );
      }

      typename std::multimap<Key,Val>::iterator
      insert( const typename std::multimap<Key,Val>::value_type& pair )
      {
         return std::multimap<Key, Val>::insert( pair );
      }

      void
      insert( const_iterator first,
              const_iterator last )
      {
         std::multimap<Key,Val>::insert( first, last );
      }

      friend std::ostream&
      operator<<( std::ostream& strm,
		  const multimap& obj )
      {
	 strm << "{";
	 if( obj.size() )
         {
	    auto it = obj.begin();
	    strm << it->first << ": " << it->second;
	    ++it;
	    for( ; it != obj.end(); ++it )
	       strm << ", " << it->first << ": " << it->second;
	 }
	 strm << "}";
	 return strm;
      }
   };
};

#endif
