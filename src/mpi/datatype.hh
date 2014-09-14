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

#ifndef hpc_mpi_datatype_hh
#define hpc_mpi_datatype_hh

#include <vector>
#include "libhpc/debug/assert.hh"
#include "init.hh"

namespace hpc {
   namespace mpi {

      class datatype
      {
      public:

	 static mpi::datatype null;
	 static mpi::datatype boolean;
	 static mpi::datatype byte;
	 static mpi::datatype character;
	 static mpi::datatype integer;
	 static mpi::datatype unsigned_integer;
	 static mpi::datatype long_integer;
	 static mpi::datatype unsigned_long;
	 static mpi::datatype long_long;
	 static mpi::datatype unsigned_long_long;
	 static mpi::datatype floating;
	 static mpi::datatype double_floating;

	 datatype( MPI_Datatype type = MPI_DATATYPE_NULL );

	 datatype( datatype&& src );

	 ~datatype();

	 void
	 clear();

	 bool
	 is_primitive() const;

	 void
	 mpi_datatype( MPI_Datatype type );

	 const MPI_Datatype&
	 mpi_datatype() const;

	 void
	 contiguous( size_t size,
		     const datatype& base,
		     size_t block_size=1,
		     size_t offs=0 );

	 template< class IndexVec >
	 mpi::datatype
	 indexed( IndexVec const& idxs ) const
	 {
	    MPI_Datatype new_type;
	    MPI_Type_create_indexed_block( idxs.size(), 1, (int*)idxs.data(), _type, &new_type );
	    MPI_Type_commit( &new_type );
	    return mpi::datatype( new_type );
	 }

	 template< class DisplVec,
		   class IndexVec >
	 void
	 indexed_csr( DisplVec const& displs,
		      IndexVec const& idxs,
		      datatype const& base,
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

	 size_t
	 size() const;

	 bool
	 operator==( const datatype& op ) const;

	 bool
	 operator==( const MPI_Datatype& op ) const;

	 bool
	 operator!=( const datatype& op ) const;

	 bool
	 operator!=( const MPI_Datatype& op ) const;

	 friend std::ostream&
	 operator<<( std::ostream& strm,
		     const datatype& obj );

      private:

	 void
	 calc_size();

	 MPI_Datatype _type;
	 size_t _size;
      };

   }
}

#endif
