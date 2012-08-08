#ifndef containers_map_hh
#define containers_map_hh

#include "libhpc/memory/memory.hh"
#include <map>

namespace hpc {

   template< class Key,
	     class Val >
   class map
      : public std::map<Key,Val>
   {
   public:

      map()
	 : std::map<Key,Val>()
      {
      }

      ~map()
      {
      }

      std::pair<typename std::map<Key,Val>::iterator, bool>
      insert( const Key& key,
              const Val& val )
      {
	 return this->insert(std::pair<Key,Val>(key, val));
      }

      std::pair<typename std::map<Key,Val>::iterator, bool>
      insert( const typename std::map<Key,Val>::value_type& pair )
      {
	 return std::map<Key,Val>::insert(pair);
      }

      typename std::map<Key,Val>::iterator
      insert( typename std::map<Key,Val>::iterator position,
	      const typename std::map<Key,Val>::value_type& pair )
      {
	 return std::map<Key,Val>::insert(position, pair);
      }

      typename std::map<Key,Val>::iterator
      set_default( const Key& key,
                   const Val& val )
      {
	 typename map<Key,Val>::iterator it = this->find( key );
	 if( it == this->end() )
	    it = this->insert( key, val ).first;
	 return it;
      }

      Val
      get( const Key& key ) const
      {
	 typename map::const_iterator it = this->find(key);
	 ASSERT( it != this->end(), "Map does not contain requested key." );
	 return it->second;
      }

      Val
      get_default( const Key& key,
                   const Val& def )
      {
	 typename map::const_iterator it = this->find( key );
         if( it != this->end() )
            return it->second;
         else
            return def;
      }

      bool
      has( const Key& key ) const
      {
	 return this->find( key ) != this->end();
      }

      friend std::ostream&
      operator<<( std::ostream& strm,
		  const map& obj )
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
