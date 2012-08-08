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

#ifndef hpc_h5_dataset_hh
#define hpc_h5_dataset_hh

#include "libhpc/hpcmpi/mpi.hh"
#include "location.hh"
#include "datatype.hh"
#include "dataspace.hh"

namespace hpc {
   namespace h5 {

      class dataset
      {
      public:

	 dataset( hid_t id=-1 );

	 dataset( h5::location& loc,
		  const std::string& name );

	 ~dataset();

	 void
	 open( h5::location& loc,
	       const std::string& name );

	 void
	 create( h5::location& loc,
		 const std::string& name,
		 const h5::datatype& datatype,
		 const h5::dataspace& dataspace,
		 boost::optional<const vector<hsize_t>::view&> chunk_size=boost::optional<const vector<hsize_t>::view&>(),
		 bool deflate=false );

	 void
	 close();

	 H5T_class_t
	 type_class() const;

	 void
	 space( h5::dataspace& space ) const;

	 void
	 read( void* buf,
	       const h5::datatype& mem_type,
	       const h5::dataspace& mem_space=h5::dataspace::all,
	       const h5::dataspace& file_space=h5::dataspace::all,
	       const mpi::comm& comm=mpi::comm::self );

	 void
	 write( const void* buf,
		const h5::datatype& mem_type,
		const h5::dataspace& mem_space=h5::dataspace::all,
		const h5::dataspace& file_space=h5::dataspace::all,
		const mpi::comm& comm=mpi::comm::self );

      protected:

	 void
	 create_groups( h5::location& loc,
			const std::string& name ) const;

	 hid_t _id;
      };
   }
}

#endif
