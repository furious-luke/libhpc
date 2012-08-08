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
