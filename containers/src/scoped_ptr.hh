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

#ifndef containers_scoped_ptr_hh
#define containers_scoped_ptr_hh

#include <boost/scoped_ptr.hpp>

namespace hpc {

   template< class T >
   class scoped_ptr
      : public std::unique_ptr<T>
   {
   public:

      scoped_ptr( T* ptr=NULL )
         : std::unique_ptr<T>( ptr )
      {
      }

      // scoped_ptr&
      // operator=( T* ptr )
      // {
      //    reset( ptr );
      // }

      bool
      empty() const
      {
         return this->get() == NULL;
      }
   };
}

#endif
