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
#include "libhpc/mpi/mpi.hh"
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

	 dataspace( dataset&& src );

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
	 create( typename type_traits<Dims>::const_reference dims );

	 void
	 close();

	 hsize_t
	 size() const;

	 hsize_t
	 simple_extent_num_dims() const;

	 hsize_t
	 simple_extent_dims( view<std::vector<hsize_t>> dims ) const;

	 void
	 select_all();

	 void
	 select_none();

	 void
	 select_one( hsize_t element,
                     H5S_seloper_t op=H5S_SELECT_SET );

	 void
	 select_hyperslab( H5S_seloper_t op,
			   view<std::vector<hsize_t>> const& count,
			   view<std::vector<hsize_t>> const& start,
			   boost::optional<view<std::vector<hsize_t>> const&> stride = boost::optional<view<std::vector<hsize_t>> const&>(),
			   boost::optional<view<std::vector<hsize_t>> const&> block=boost::optional<view<std::vector<hsize_t>> const&>() );

	 void
	 select_hyperslab( H5S_seloper_t op,
			   hsize_t count,
			   hsize_t start,
			   boost::optional<hsize_t> stride = boost::optional<hsize_t>(),
                           boost::optional<hsize_t> block = boost::optional<hsize_t>() );

	 void
	 select_range( hsize_t start,
		       hsize_t finish );

	 void
	 select_elements( view<std::vector<hsize_t>> const& elements,
			  H5S_seloper_t op=H5S_SELECT_SET );

	 void
	 select_elements2( view<std::vector<hsize_t>> const& elems,
                           H5S_seloper_t op = H5S_SELECT_SET );

	 void
	 select_slices( int slice_dim,
			view<std::vector<hsize_t>> const& idxs,
			H5S_seloper_t op=H5S_SELECT_SET );

      protected:

	 hid_t _id;
      };
   }
}

#endif
