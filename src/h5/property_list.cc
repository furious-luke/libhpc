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

#include "property_list.hh"

namespace hpc {
   namespace h5 {

      property_list::property_list( hid_t id )
	 : _id( -1 )
      {
	 create( id );
      }

      property_list::~property_list()
      {
	 close();
      }

      hid_t
      property_list::id() const
      {
	 return _id;
      }

      void
      property_list::create( hid_t class_id )
      {
	 close();
	 _id = H5Pcreate( class_id );
	 INSIST( _id, >= 0 );
      }

      void
      property_list::close()
      {
	 if( _id >= 0 )
	    INSIST( H5Pclose( _id ), >= 0 );
      }

      void
      property_list::set_external( const string& name,
				   hsize_t size,
				   hsize_t offset )
      {
	 INSIST( H5Pset_external( _id, name.c_str(), offset, size ), >= 0 );
      }

      void
      property_list::set_chunk_size( hsize_t size )
      {
	 INSIST( H5Pset_chunk( _id, 1, &size ), >= 0 );
      }

      void
      property_list::set_deflate( bool state )
      {
	 INSIST( H5Pset_deflate( _id, 9 ), >= 0 );
      }

      void
      property_list::set_family( hsize_t size )
      {
	 INSIST( H5Pset_fapl_family( _id, size, H5P_DEFAULT ), >= 0 );
      }

      void
      property_list::set_parallel( const mpi::comm& comm )
      {
	 INSIST( H5Pset_fapl_mpio( _id, comm.mpi_comm(), MPI_INFO_NULL ), >= 0 );
      }

   }
}
