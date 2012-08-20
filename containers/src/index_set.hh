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

#ifndef libhpc_containers_index_set_hh
#define libhpc_containers_index_set_hh

#include "vector.hh"

namespace hpc {

   class index_set
   {
   public:

      static const byte_t idx_add[8];
      static const byte_t idx_rem[8];

      index_set( index max_size=0 );

      ~index_set();

      void
      clear();

      void
      set_max_size( index max_size );

      void
      merge( index idx );

      void
      remove( index idx );

      void
      set_index( index idx,
		 bool state );

      bool
      has( index idx ) const;

      const vector<byte_t>&
      bits() const;

      int
      bit( index idx ) const;

      bool
      operator[]( index idx ) const;

   private:
      index _max_size;
      index _size;
      vector<byte_t> _bits;
   };
}

#endif
