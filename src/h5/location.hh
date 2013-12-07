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

#ifndef hpc_h5_location_hh
#define hpc_h5_location_hh

#include "libhpc/mpi/mpi.hh"
#include <hdf5.h>
#include "libhpc/containers/containers.hh"
#include "types.hh"

namespace hpc {
   namespace h5 {

      class location
      {
      public:

	 location( hid_t id=-1 );

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
	 extent( const string& name ) const;

	 template< class T >
	 void
	 write( const std::string& name,
		const T& value,
                const mpi::comm& comm=mpi::comm::self );

	 template< class T >
	 void
	 write( const std::string& name,
		const typename vector<T>::view& data,
                const mpi::comm& comm=mpi::comm::self,
		boost::optional<const vector<hsize_t>::view&> chunk_size=boost::optional<const vector<hsize_t>::view&>(),
		bool deflate=false );

      protected:

	 hid_t _id;
      };
   }
}

#endif
