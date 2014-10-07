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

#ifndef libhpc_h5_location_hh
#define libhpc_h5_location_hh

#include <vector>
#include <boost/optional.hpp>
#include "libhpc/mpi.hh"
#include <hdf5.h>
#include "types.hh"

namespace hpc {
   namespace h5 {

      class dataset;

      class location
      {
      public:

	 location( hid_t id = -1 );

	 ~location();

	 hid_t
	 id() const;

	 bool
	 has_link( const std::string& name ) const;

	 void
	 open_group( const std::string& name,
		     h5::group& group ) const;

	 void
	 create_group( const std::string& name );

	 hsize_t
	 extent( const std::string& name ) const;

	 h5::dataset
	 dataset( std::string const& name ) const;

	 template< class T >
	 void
	 write( std::string const& name,
		T const& value );

	 template< class Buffer >
	 void
         write( std::string const& name,
                typename type_traits<Buffer>::const_reference buf,
                mpi::comm const& comm = mpi::comm::self );

      protected:

	 hid_t _id;
      };


      void
      copy( location const& src,
            std::string const& src_name,
            location const& dst,
            std::string const& dst_name = std::string() );
   }
}

#endif
