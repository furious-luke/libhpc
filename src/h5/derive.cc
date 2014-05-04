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
	 _cache.push_back( boost::make_tuple( &mem_type, mem_offs, &file_type, desc ) );
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
	 for( std::list<entry_type>::const_iterator it = _cache.begin(); it != _cache.end(); ++it )
	 {
	    std::string const& desc = boost::get<3>( *it );
	    mem_type.insert( *boost::get<0>( *it ), desc, boost::get<1>( *it ) );
	    file_type.insert( *boost::get<2>( *it ), desc, offs );
	    offs += boost::get<2>( *it )->size();
	 }
      }

      hsize_t
      derive::_calc_mem_size()
      {
	 hsize_t max_offs = 0, max_size = 0;
	 for( std::list<entry_type>::const_iterator it = _cache.begin(); it != _cache.end(); ++it )
	 {
	    if( boost::get<1>( *it ) > max_offs )
	    {
	       max_offs = boost::get<1>( *it );
	       max_size = boost::get<0>( *it )->size();
	    }
	 }
	 return max_offs + max_size;
      }

      hsize_t
      derive::_calc_file_size()
      {
	 hsize_t size = 0;
	 for( std::list<entry_type>::const_iterator it = _cache.begin(); it != _cache.end(); ++it )
	    size += boost::get<2>( *it )->size();
	 return size;
      }

   }
}
