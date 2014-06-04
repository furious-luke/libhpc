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

#ifndef hpc_h5_derive_hh
#define hpc_h5_derive_hh

#include <boost/tuple/tuple.hpp>
#include "datatype.hh"

namespace hpc {
   namespace h5 {

      class derive
      {
      public:

	 typedef boost::tuple< h5::datatype const*,
			       hsize_t,
			       h5::datatype const*,
			       std::string > entry_type;

      public:

	 derive( size_t mem_size = 0 );

	 void
	 add( const h5::datatype& mem_type,
	      hsize_t mem_offs,
	      const h5::datatype& file_type,
	      const std::string& desc = "" );

	 void
	 commit( h5::datatype& mem_type,
		 h5::datatype& file_type );

      protected:

	 hsize_t
	 _calc_mem_size();

	 hsize_t
	 _calc_file_size();

      protected:

	 size_t _mem_size;
	 std::list<entry_type> _cache;
      };

   }
}

#endif
