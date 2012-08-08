#ifndef mpi_data_type_hh
#define mpi_data_type_hh

#include <boost/mpl/map.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/at.hpp>
#include "libhpc/containers/containers.hh"
#include "init.hh"
#include "types.hh"

namespace mpl = boost::mpl;

namespace hpc {
   namespace mpi {

      class data_type {
      public:

	 static mpi::data_type null;
	 static mpi::data_type boolean;
	 static mpi::data_type byte;
	 static mpi::data_type character;
	 static mpi::data_type integer;
	 static mpi::data_type unsigned_integer;
	 static mpi::data_type long_integer;
	 static mpi::data_type unsigned_long;
	 static mpi::data_type floating;
	 static mpi::data_type double_floating;

	 typedef mpl::map< mpl::pair<byte_t, mpl::int_<MPI_BYTE> >,
			   mpl::pair<bool, mpl::int_<MPIR_CXX_BOOL> >,
			   mpl::pair<char, mpl::int_<MPI_CHAR> >,
			   mpl::pair<int, mpl::int_<MPI_INT> >,
			   mpl::pair<unsigned int, mpl::int_<MPI_UNSIGNED> >,
			   mpl::pair<long, mpl::int_<MPI_LONG> >,
			   mpl::pair<unsigned long, mpl::int_<MPI_UNSIGNED_LONG> >,
			   mpl::pair<float, mpl::int_<MPI_FLOAT> >,
			   mpl::pair<double, mpl::int_<MPI_DOUBLE> > > type_map;

	 data_type( MPI_Datatype type=MPI_DATATYPE_NULL );

	 ~data_type();

	 void
	 clear();

	 void
	 mpi_data_type( MPI_Datatype type );

	 const MPI_Datatype&
	 mpi_data_type() const;

	 void
	 contiguous( mpi::lindex size,
		     const data_type& base,
		     mpi::lindex block_size=1,
		     mpi::lindex offs=0 );

	 void
	 indexed( const vector<mpi::lindex>::view& idxs,
		  const data_type& base,
		  mpi::lindex block_size=1 );

	 void
	 indexed_csr( const vector<index>::view& displs,
		      const vector<mpi::lindex>::view& idxs,
		      const data_type& base,
		      mpi::lindex block_size=1 );

	 mpi::lindex
	 size() const;

	 bool
	 operator==( const data_type& op ) const;

	 bool
	 operator==( const MPI_Datatype& op ) const;

	 bool
	 operator!=( const data_type& op ) const;

	 bool
	 operator!=( const MPI_Datatype& op ) const;

	 friend std::ostream&
	 operator<<( std::ostream& strm,
		     const data_type& obj );

      private:

	 void
	 calc_size();

	 MPI_Datatype _type;
	 mpi::lindex _size;
      };
   }
}

#endif
