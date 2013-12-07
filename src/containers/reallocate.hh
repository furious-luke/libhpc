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

#ifndef libhpc_containers_reallocate_hh
#define libhpc_containers_reallocate_hh

namespace hpc {

   template< class T,
             class ...Args >
   void
   reallocate( std::vector<T,Args...>& obj,
               typename std::vector<T,Args...>::size_type size )
   {
      std::vector<T,Args...> tmp;
      obj.swap( tmp );
      obj.resize( size );
   }

}

#endif
