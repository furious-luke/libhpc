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

#include <boost/range/algorithm/fill.hpp>
#include "scatter.hh"
#include "surjection.hh"

namespace hpc {
   namespace mpi {

      scatter::scatter()
	 : _comm( &mpi::comm::world )
      {
      }

      scatter::scatter( surjection const& srj,
			datatype const& type )
      {
	 construct( srj, type );
      }

      scatter::scatter( scatter&& src )
	 : _comm( src._comm ),
	   _inc_types( std::move( src._inc_types ) ),
	   _out_types( std::move( src._out_types ) ),
	   _zeros( std::move( src._zeros ) ),
	   _ones( std::move( src._ones ) )
      {
      }

      void
      scatter::clear()
      {
	 hpc::deallocate( _inc_types );
	 hpc::deallocate( _out_types );
	 hpc::deallocate( _zeros );
	 hpc::deallocate( _ones );
	 _comm = &mpi::comm::world;
      }

      void
      scatter::construct( surjection const& srj,
			  datatype const& type )
      {
	 _comm = &srj.comm();
	 int n_ranks = _comm->size();

	 clear();

	 // Create datatypes for outgoing information.
	 _out_types.resize( n_ranks );
	 for( unsigned ii = 0; ii < n_ranks; ++ii )
	    _out_types[ii] = type.indexed( srj.out_indices( ii ) );

	 // Create incoming datatypes to put information in the
	 // correct positions.
	 _inc_types.resize( n_ranks );
	 for( unsigned ii = 0; ii < n_ranks; ++ii )
	    _inc_types[ii] = type.indexed( srj.inc_indices( ii ) );

	 // Need some dummy values.
	 _zeros.resize( n_ranks );
	 boost::fill( _zeros, 0 );
	 _ones.resize( n_ranks );
	 boost::fill( _ones, 1 );
      }

   }
}
