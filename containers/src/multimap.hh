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

      const Val&
      get( const Key& key ) const
      {
	 const_iterator it = this->find( key );
	 ASSERT( it != this->end(), "Map does not contain requested key." );
	 return it->second;
      }

      bool
      has( const Key& key ) const
      {
	 return this->find( key ) != this->end();
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
