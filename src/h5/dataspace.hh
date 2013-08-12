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

#ifndef hpc_h5_dataspace_hh
#define hpc_h5_dataspace_hh

#include <boost/optional/optional.hpp>
#include "libhpc/mpi/mpi.hh"
#include <hdf5.h>
#include "libhpc/containers/containers.hh"

namespace hpc {
   namespace h5 {

      class dataset;

      class dataspace
      {
      public:

	 static dataspace all;

	 dataspace( hid_t id=-1 );

	 dataspace( const dataset& dset );

	 dataspace( const vector<hsize_t>::view& dims );

	 ~dataspace();

	 void
	 set_id( hid_t id );

	 hid_t
	 id() const;

         void
         create( hsize_t size,
		 bool unlimited = false );

	 void
	 create( const vector<hsize_t>::view& dims );

	 void
	 close();

	 hsize_t
	 size() const;

	 hsize_t
	 simple_extent_num_dims() const;

	 hsize_t
	 simple_extent_dims( vector<hsize_t>::view dims ) const;

	 void
	 select_all();

	 void
	 select_none();

	 void
	 select_one( hsize_t element,
                     H5S_seloper_t op=H5S_SELECT_SET );

	 void
	 select_hyperslab( H5S_seloper_t op,
			   const vector<hsize_t>::view& count,
			   const vector<hsize_t>::view& start,
			   boost::optional<const vector<hsize_t>::view&> stride=boost::optional<const vector<hsize_t>::view&>(),
			   boost::optional<const vector<hsize_t>::view&> block=boost::optional<const vector<hsize_t>::view&>() );

	 void
	 select_range( hsize_t start,
		       hsize_t finish );

	 void
	 select_elements( const vector<hsize_t>::view& elements,
			  H5S_seloper_t op=H5S_SELECT_SET );

	 void
	 select_slices( int slice_dim,
			const vector<hsize_t>::view& idxs,
			H5S_seloper_t op=H5S_SELECT_SET );

      protected:

	 hid_t _id;
      };
   }
}

#endif
