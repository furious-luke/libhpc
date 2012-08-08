// Copyright 2012 Luke Hodkinson

// This file is part of libhpc.
// 
// libhpc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// libhpc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with libhpc.  If not, see <http://www.gnu.org/licenses/>.

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
