#ifndef hpc_h5_location_hh
#define hpc_h5_location_hh

#include "libhpc/hpcmpi/mpi.hh"
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

      protected:
	 hid_t _id;
      };
   }
}

#endif
