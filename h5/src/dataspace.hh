#ifndef hpc_h5_dataspace_hh
#define hpc_h5_dataspace_hh

#include <boost/optional/optional.hpp>
#include "libhpc/hpcmpi/mpi.hh"
#include <hdf5.h>
#include "libhpc/containers/containers.hh"

namespace hpc {
   namespace h5 {

      class dataspace
      {
      public:

	 static dataspace all;

	 dataspace( hid_t id=-1 );

	 dataspace( const vector<hsize_t>::view& dims );

	 ~dataspace();

	 void
	 set_id( hid_t id );

	 hid_t
	 id() const;

	 void
	 create( const vector<hsize_t>::view& dims );

	 void
	 close();

	 hsize_t
	 simple_extent_num_dims() const;

	 hsize_t
	 simple_extent_dims( vector<hsize_t>::view dims );

	 void
	 select_all();

	 void
	 select_none();

	 void
	 select_hyperslab( H5S_seloper_t op,
			   const vector<hsize_t>::view& count,
			   const vector<hsize_t>::view& start,
			   boost::optional<const vector<hsize_t>::view&> stride=boost::optional<const vector<hsize_t>::view&>(),
			   boost::optional<const vector<hsize_t>::view&> block=boost::optional<const vector<hsize_t>::view&>() );

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
