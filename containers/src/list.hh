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

      bool
      has( const T& elem )
      {
         return std::find( this->begin(), this->end(), elem ) != this->end();
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
