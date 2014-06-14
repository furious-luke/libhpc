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

#include "dataspace.hh"
#include "dataset.hh"

namespace hpc {
   namespace h5 {

      dataspace dataspace::all( H5S_ALL );

      dataspace::dataspace()
         : _id( -1 )
      {
      }

      dataspace::dataspace( hid_tag tag,
			    hid_t id )
	 : _id( id )
      {
      }

      dataspace::dataspace( hsize_t size,
                            bool unlimited )
         : _id( -1 )
      {
         create( size, unlimited );
      }

      dataspace::dataspace( dataset const& dset )
	 : _id( dset.dataspace().id() )
      {
      }

      template< class Dims >
      dataspace::dataspace( typename type_traits<Dims>::const_reference dims )
	 : _id( -1 )
      {
	 create( dims );
      }

      dataspace::dataspace( dataspace const& src )
         : _id( -1 )
      {
         *this = src;
      }

      dataspace::~dataspace()
      {
	 close();
      }

      void
      dataspace::set_id( hid_t id )
      {
	 this->close();
	 this->_id = id;
      }

      hid_t
      dataspace::id() const
      {
	 return this->_id;
      }

      void
      dataspace::create( hsize_t size,
			 bool unlimited )
      {
         close();

	 // Prepare the maximum dimensions.
	 hsize_t max_dims = unlimited ? H5S_UNLIMITED : size;

         // if( size )
            _id = H5Screate_simple( 1, &size, &max_dims );
         // else
         //    _id = H5Screate( H5S_NULL );
         ASSERT( _id >= 0 );
      }

      void
      dataspace::close()
      {
	 if( _id != -1 && _id != H5S_ALL && _id != H5S_NULL )
	 {
	    INSIST( H5Sclose( _id ), >= 0 );
	    _id = -1;
	 }
      }

      hsize_t
      dataspace::size() const
      {
#ifndef NDEBUG
	 hssize_t size = H5Sget_simple_extent_npoints( _id );
	 ASSERT( size >= 0 );
	 return (hsize_t)size;
#else
	 return (hsize_t)H5Sget_simple_extent_npoints( _id );
#endif
      }

      hsize_t
      dataspace::simple_extent_num_dims() const
      {
#ifndef NDEBUG
	 return H5Sget_simple_extent_ndims(this->_id);
#else
	 hsize_t num_dims = H5Sget_simple_extent_ndims(this->_id);
	 ASSERT(numDims >= 0);
	 return num_dims;
#endif
      }

      void
      dataspace::select_all()
      {
	 INSIST(H5Sselect_all(this->_id), >= 0);
      }

      void
      dataspace::select_none()
      {
	 INSIST(H5Sselect_none(this->_id), >= 0);
      }

      void
      dataspace::select_one( hsize_t element,
                             H5S_seloper_t op )
      {
         INSIST( H5Sselect_elements( _id, op, 1, &element ), >= 0 );
      }

      void
      dataspace::select_hyperslab( H5S_seloper_t op,
                                   hsize_t count,
                                   hsize_t start,
				   boost::optional<hsize_t> stride,
				   boost::optional<hsize_t> block )
      {
	 ASSERT( simple_extent_num_dims() == 1 );

	 // Don't call the HDF5 routine if there is nothing to select.
	 if( count )
         {
	    INSIST( H5Sselect_hyperslab(
                       _id,
                       op,
                       &start,
                       stride ? &*stride : NULL,
                       &count,
                       block ? &*block : NULL
                       ), >= 0 );
	 }
	 else
	    INSIST( H5Sselect_none( _id ), >= 0 );
      }

      void
      dataspace::select_range( hsize_t start,
			       hsize_t finish )
      {
	 hsize_t cnt = finish - start;
	 INSIST( H5Sselect_hyperslab( _id, H5S_SELECT_SET, &start, NULL, &cnt, NULL ), >= 0 );
      }

   }
}
