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

#include <ostream>
#include <map>
#include "libhpc/debug/debug.hh"
#include "libhpc/memory/memory.hh"

namespace hpc {

   template< class Key,
	     class Value >
   class map
      : public std::map<Key,Value>
   {
   public:

      typedef std::map<Key,Value> super_type;
      typedef typename std::map<Key,Value>::iterator iterator;
      typedef typename std::map<Key,Value>::const_iterator const_iterator;

      map()
	 : std::map<Key,Value>()
      {
      }

      ~map()
      {
      }

      std::pair<iterator, bool>
      insert( const Key& key )
      {
         Value val; // TODO: Is this efficient enough?
	 return insert( key, val );
      }

      std::pair<iterator, bool>
      insert( const Key& key,
              const Value& val )
      {
	 return insert( std::pair<Key,Value>( key, val ) );
      }

      std::pair<iterator, bool>
      insert( const typename std::map<Key,Value>::value_type& pair )
      {
         return std::map<Key,Value>::insert( pair );
      }

      iterator
      insert( typename std::map<Key,Value>::iterator position,
	      const typename std::map<Key,Value>::value_type& pair )
      {
	 return std::map<Key,Value>::insert(position, pair);
      }

      iterator
      set_default( const Key& key,
                   const Value& val )
      {
	 iterator it = this->find( key );
	 if( it == this->end() )
	    it = insert( key, val ).first;
	 return it;
      }

      const Value&
      get( const Key& key ) const
      {
	 const_iterator it = this->find( key );
	 ASSERT( it != this->end(), "Map does not contain requested key." );
	 return it->second;
      }

      const Value&
      get_default( const Key& key,
                   const Value& def )
      {
	 const_iterator it = this->find( key );
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
