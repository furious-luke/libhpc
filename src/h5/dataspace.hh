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

#ifndef libhpc_h5_dataspace_hh
#define libhpc_h5_dataspace_hh

#include <boost/optional.hpp>
#include "libhpc/mpi.hh"
#include <hdf5.h>
#include "libhpc/system/view.hh"

namespace hpc {
   namespace h5 {

      class dataset;

      class dataspace
      {
      public:

	 static dataspace all;

	 dataspace();

	 dataspace( hid_t id,
                    bool dummy );

         dataspace( hsize_t size,
                    bool unlimited = false );

	 dataspace( h5::dataset const& dset );

         template< class Dims >
	 dataspace( typename type_traits<Dims>::const_reference dims );

	 dataspace( dataspace&& src );

	 ~dataspace();

	 void
	 set_id( hid_t id );

	 hid_t
	 id() const;

         void
         create( hsize_t size,
		 bool unlimited = false );

         template< class Dims >
	 void
	 create( typename type_traits<Dims>::const_reference dims )
         {
            static_assert( sizeof(typename Dims::value_type) == sizeof(hsize_t),
                           "Incompatible hsize_t type." );

            close();

            if( std::accumulate( dims.begin(), dims.end(), 0 ) )
               _id = H5Screate_simple( dims.size(), dims.data(), 0 );
            else
               _id = H5Screate( H5S_NULL );

            ASSERT( _id >= 0 );
         }


	 void
	 close();

	 hsize_t
	 size() const;

	 hsize_t
	 simple_extent_num_dims() const;

         template< class Dims >
         void
	 simple_extent_dims( typename type_traits<Dims>::reference dims ) const
         {
            static_assert( sizeof(typename Dims::value_type) == sizeof(hsize_t), "Incompatible hsize_t type." );
            INSIST( H5Sget_simple_extent_dims( _id, dims.data(), 0 ), >= 0 );
         }

	 void
	 select_all();

	 void
	 select_none();

	 void
	 select_one( hsize_t element,
                     H5S_seloper_t op = H5S_SELECT_SET );

         template< class Buffer >
         typename boost::enable_if<random_access_trait<Buffer>>::type
	 select_hyperslab( H5S_seloper_t op,
			   typename type_traits<Buffer>::const_reference count,
			   typename type_traits<Buffer>::const_reference start,
                           typename type_traits<Buffer>::const_reference stride = typename type_traits<Buffer>::const_reference(),
			   typename type_traits<Buffer>::const_reference block = typename type_traits<Buffer>::const_reference() )
         {
            static_assert( sizeof(typename Buffer::value_type) == sizeof(hsize_t), "Incompatible hsize_t type." );
            ASSERT( simple_extent_num_dims() == count.size() && count.size() == start.size() );
            ASSERT( stride.size() == 0 || stride.size() == count.size() );
            ASSERT( block.size() == 0 || block.size() == count.size() );

            // Don't call the HDF5 routine if there is nothing to select.
            if( std::accumulate( count.begin(), count.end(), 0 ) )
            {
               INSIST( H5Sselect_hyperslab( _id,
                                            op,
                                            start.data(),
                                            stride.empty() ? 0 : stride.data(),
                                            count.data(),
                                            block.empty() ? 0 : block.data() ), >= 0 );
            }
            else
               INSIST( H5Sselect_none( _id ), >= 0 );
         }

	 void
	 select_hyperslab( H5S_seloper_t op,
			   hsize_t count,
			   hsize_t start,
			   boost::optional<hsize_t> stride = boost::optional<hsize_t>(),
                           boost::optional<hsize_t> block = boost::optional<hsize_t>() );

	 void
	 select_range( hsize_t start,
		       hsize_t finish );

         template< class Buffer >
	 void
	 select_elements( typename type_traits<Buffer>::const_reference elems,
                          H5S_seloper_t op = H5S_SELECT_SET )
         {
            static_assert( sizeof(typename Buffer::value_type) == sizeof(hsize_t), "Incompatible hsize_t type." );

            if( !elems.empty() )
            {
               std::vector<hsize_t> dims( simple_extent_num_dims() );
               simple_extent_dims<std::vector<hsize_t>>( dims );
               ASSERT( elems.size()%dims.size() == 0,
                      "Flattened element selection array does not divide evenly between the "
                      "number of dimensions in the dataset.");
               INSIST( H5Sselect_elements( _id, op, elems.size()/dims.size(), elems ), >= 0 );
            }
            else if( op == H5S_SELECT_SET )
               select_none();
         }

      protected:

	 hid_t _id;
      };
   }
}

#endif
