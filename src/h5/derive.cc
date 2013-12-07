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

#include "derive.hh"

namespace hpc {
   namespace h5 {

      void
      derive::add( const h5::datatype& mem_type,
		   hsize_t mem_offs,
		   const h5::datatype& file_type,
		   const std::string& desc )
      {
	 _cache.push_back( std::make_tuple( &mem_type, mem_offs, &file_type, desc ) );
      }

      void
      derive::commit( h5::datatype& mem_type,
		      h5::datatype& file_type )
      {
	 hsize_t mem_size = _calc_mem_size();
	 hsize_t file_size = _calc_file_size();
	 mem_type.compound( mem_size );
	 file_type.compound( file_size );
	 hsize_t offs = 0;
	 for( auto const& entry : _cache )
	 {
	    std::string const& desc = std::get<3>( entry );
	    mem_type.insert( *std::get<0>( entry ), desc, std::get<1>( entry ) );
	    file_type.insert( *std::get<2>( entry ), desc, offs );
	    offs += std::get<2>( entry )->size();
	 }
      }

      hsize_t
      derive::_calc_mem_size()
      {
	 hsize_t max_offs = 0, max_size = 0;
	 for( auto const& entry : _cache )
	 {
	    if( std::get<1>( entry ) > max_offs )
	    {
	       max_offs = std::get<1>( entry );
	       max_size = std::get<0>( entry )->size();
	    }
	 }
	 return max_offs + max_size;
      }

      hsize_t
      derive::_calc_file_size()
      {
	 hsize_t size = 0;
	 for( auto const& entry : _cache )
	    size += std::get<2>( entry )->size();
	 return size;
      }

   }
}
