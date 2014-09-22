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

#ifndef libhpc_h5_property_list_hh
#define libhpc_h5_property_list_hh

#include <boost/move/move.hpp>
#include "libhpc/mpi.hh"
#include "dataspace.hh"

namespace hpc {
   namespace h5 {

      class property_list
      {
         BOOST_COPYABLE_AND_MOVABLE( property_list );

      public:

         property_list( hid_tag tag = hid_tag(),
                        hid_t id = H5P_DEFAULT );

	 ///
	 /// @param[in] id One of H5P_FILE_ACCESS, H5P_DATASET_CREATE,
         ///               H5P_DATASET_XFER
	 ///
	 property_list( hid_t class_id );

#ifdef PARALLELHDF5

         property_list( hid_t class_id,
                        mpi::comm const& comm );

#endif

         property_list( property_list const& src );

         inline
         property_list( BOOST_RV_REF( property_list ) src )
            : _id( src._id )
         {
            src._id = H5P_DEFAULT;
         }

	 ~property_list();

         inline
         property_list&
         operator=( BOOST_COPY_ASSIGN_REF( property_list ) src )
         {
            close();
            if( src._id >= 0 && src._id != H5P_DEFAULT )
               _id = H5Pcopy( src._id );
            else
               _id = src._id;
            return *this;
         }

         inline
         property_list&
         operator=( BOOST_RV_REF( property_list ) src )
         {
            close();
            _id = src._id;
            src._id = H5P_DEFAULT;
            return *this;
         }

	 hid_t
	 id() const;

	 void
	 create( hid_t class_id );

	 void
	 close();

	 void
	 set_external( std::string const& name,
		       hsize_t size = (hsize_t)1 << 31,
		       hsize_t offset = 0 );

	 void
	 set_chunk_size( hsize_t size );

	 void
	 set_deflate( bool state = true );

	 void
	 set_family( hsize_t size = (hsize_t)1 << 31 );

         void
         set_preserve( bool state = true );

#ifdef PARALLELHDF5

	 void
	 set_parallel( mpi::comm const& comm = mpi::comm::world );

         void
         set_collective();

#endif

         operator bool() const;

      protected:

	 hid_t _id;
      };

   }
}

#endif
