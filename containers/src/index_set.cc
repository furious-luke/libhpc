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

#include <boost/range/algorithm/fill.hpp>
#include "libhpc/debug/debug.hh"
#include "index_set.hh"

namespace hpc {

   const byte_t index_set::idx_add[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
   const byte_t index_set::idx_rem[8] = {0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};

   index_set::index_set( index max_size )
      : _max_size(max_size),
	_size(0),
	_bits(max_size ? ((max_size >> 3) + 1) : 0)
   {
      boost::fill(this->_bits, 0);
   }

   index_set::~index_set()
   {
   }

   void
   index_set::clear()
   {
      this->_size = 0;
      boost::fill(this->_bits, 0);
   }

   void
   index_set::set_max_size( index max_size )
   {
      this->_max_size = max_size;
      this->_size = 0;
      this->_bits.reallocate(max_size ? ((max_size >> 3) + 1) : 0);
      boost::fill(this->_bits, 0);
   }

   void
   index_set::merge( index idx )
   {
      ASSERT(idx < this->_max_size);
      this->_bits[idx >> 3] |= this->idx_add[idx&7];
      ++this->_size;
   }

   void
   index_set::remove( index idx )
   {
      ASSERT(idx < this->_max_size);
      this->_bits[idx >> 3] &= this->idx_rem[idx&7];
      --this->_size;
   }

   void
   index_set::set_index( index idx,
			 bool state )
   {
      if(state)
	 this->merge(idx);
      else
	 this->remove(idx);
   }

   const vector<byte_t>&
   index_set::bits() const
   {
      return this->_bits;
   }

   int
   index_set::bit( index idx ) const
   {
      return this->has(idx) ? 1 : 0;
   }

   bool
   index_set::has( index idx ) const
   {
      ASSERT(idx < this->_max_size);
      return this->_bits[idx >> 3]&this->idx_add[idx&7];
   }
}
