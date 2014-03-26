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

#ifndef mpi_data_type_hh
#define mpi_data_type_hh

#include <boost/mpl/map.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/at.hpp>
#include "libhpc/containers/containers.hh"
#include "init.hh"
#include "types.hh"

#if defined( MPICH ) || defined( MPICH2 )
#define MPI_MAP_TYPE( T )                                       \
   boost::mpl::at<hpc::mpi::data_type::type_map,T>::type::value
#else
#define MPI_MAP_TYPE( T )                                               \
   hpc::mpi::data_type::_type_map[boost::mpl::at<hpc::mpi::data_type::type_map,T>::type::value]
#endif

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
	 static mpi::data_type long_long;
	 static mpi::data_type unsigned_long_long;
	 static mpi::data_type floating;
	 static mpi::data_type double_floating;

#if defined( MPICH ) || defined( MPICH2 )
	 typedef boost::mpl::map< boost::mpl::pair<byte_t,             boost::mpl::int_<MPI_BYTE> >,
                                  boost::mpl::pair<bool,               boost::mpl::int_<MPIR_CXX_BOOL> >,
                                  boost::mpl::pair<char,               boost::mpl::int_<MPI_CHAR> >,
                                  boost::mpl::pair<int,                boost::mpl::int_<MPI_INT> >,
                                  boost::mpl::pair<unsigned int,       boost::mpl::int_<MPI_UNSIGNED> >,
                                  boost::mpl::pair<long,               boost::mpl::int_<MPI_LONG> >,
                                  boost::mpl::pair<unsigned long,      boost::mpl::int_<MPI_UNSIGNED_LONG> >,
                                  boost::mpl::pair<long long,          boost::mpl::int_<MPI_LONG_LONG> >,
                                  boost::mpl::pair<unsigned long long, boost::mpl::int_<MPI_UNSIGNED_LONG_LONG> >,
                                  boost::mpl::pair<float,              boost::mpl::int_<MPI_FLOAT> >,
                                  boost::mpl::pair<double,             boost::mpl::int_<MPI_DOUBLE> > > type_map;
#else
	 typedef boost::mpl::map< boost::mpl::pair<byte_t,             boost::mpl::int_<0> >,
                                  boost::mpl::pair<bool,               boost::mpl::int_<1> >,
                                  boost::mpl::pair<char,               boost::mpl::int_<2> >,
                                  boost::mpl::pair<int,                boost::mpl::int_<3> >,
                                  boost::mpl::pair<unsigned int,       boost::mpl::int_<4> >,
                                  boost::mpl::pair<long,               boost::mpl::int_<5> >,
                                  boost::mpl::pair<unsigned long,      boost::mpl::int_<6> >,
                                  boost::mpl::pair<long long,          boost::mpl::int_<7> >,
                                  boost::mpl::pair<unsigned long long, boost::mpl::int_<8> >,
                                  boost::mpl::pair<float,              boost::mpl::int_<9> >,
                                  boost::mpl::pair<double,             boost::mpl::int_<10> > > type_map;
         static MPI_Datatype _type_map[11];
#endif

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

	 template< class DisplVec,
		   class IndexVec >
	 void
	 indexed_csr( DisplVec const& displs,
		      IndexVec const& idxs,
		      data_type const& base,
		      unsigned block_size = 1 )
	 {
	    typedef typename IndexVec::value_type index_type;

	    ASSERT( block_size >= 0, "Invalid block size." );
	    clear();

	    std::vector<int> block_displs( idxs.size() );
	    std::vector<int> block_cnts( idxs.size() );

	    for( size_t ii = 0; ii < idxs.size(); ++ii )
	    {
	       index_type idx = idxs[ii];
	       block_displs[ii] = block_size*displs[idx];
	       block_cnts[ii] = block_size*displs[idx + 1] - block_displs[ii];
	    }

	    MPI_Type_indexed( idxs.size(), block_cnts.data(), block_displs.data(), base._type, &_type );
	    MPI_Type_commit( &_type );
	    calc_size();
	 }

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
