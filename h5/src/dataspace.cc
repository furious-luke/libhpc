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

namespace hpc {
   namespace h5 {

      dataspace dataspace::all(H5S_ALL);

      dataspace::dataspace(hid_t id)
	 : _id(id)
      {
      }

      dataspace::dataspace( const vector<hsize_t>::view& dims )
	 : _id(-1)
      {
	 this->create(dims);
      }

      dataspace::~dataspace()
      {
	 this->close();
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
      dataspace::create( const vector<hsize_t>::view& dims )
      {
	 this->close();
	 if(std::accumulate(dims.begin(), dims.end(), 0))
	    this->_id = H5Screate_simple(dims.size(), dims, NULL);
	 else
	    this->_id = H5Screate(H5S_NULL);
	 ASSERT(this->_id >= 0);
      }

      void
      dataspace::close()
      {
	 if(this->_id != -1 && this->_id != H5S_ALL && this->_id != H5S_NULL)
	    INSIST(H5Sclose(this->_id), >= 0);
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

      hsize_t
      dataspace::simple_extent_dims( vector<hsize_t>::view dims )
      {
#ifndef NDEBUG
	 hsize_t num_dims = H5Sget_simple_extent_dims(this->_id, dims, NULL);
	 ASSERT(num_dims >= 0);
	 return num_dims;
#else
	 return H5Sget_simple_extent_dims(this->_id, dims, NULL);
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
      dataspace::select_hyperslab( H5S_seloper_t op,
				   const vector<hsize_t>::view& count,
				   const vector<hsize_t>::view& start,
				   boost::optional<const vector<hsize_t>::view&> stride,
				   boost::optional<const vector<hsize_t>::view&> block )
      {
	 ASSERT(this->simple_extent_num_dims() == count.size() && count.size() == start.size());
	 ASSERT(!stride || stride->size() == count.size());
	 ASSERT(!block || block->size() == count.size());

	 // Don't call the HDF5 routine if there is nothing to select.
	 if(std::accumulate(count.begin(), count.end(), 0)) {
	    INSIST(H5Sselect_hyperslab(
		      this->_id,
		      op,
		      start,
		      stride ? (const hsize_t*)*stride : NULL,
		      count,
		      block ? (const hsize_t*)*block : NULL
		      ), >= 0);
	 }
	 else
	    INSIST(H5Sselect_none(this->_id), >= 0);
      }

      void
      dataspace::select_elements( const vector<hsize_t>::view& elements,
				  H5S_seloper_t op )
      {
	 if(elements.size()) {
	    vector<hsize_t> dims(this->simple_extent_num_dims());
	    this->simple_extent_dims(dims);
	    ASSERT(elements.size()%dims.size() == 0,
		   "Flattened element selection array does not divide evenly between the "
		   "number of dimensions in the dataset.");
	    INSIST(H5Sselect_elements(this->_id, op, elements.size()/dims.size(), elements), >= 0);
	 }
	 else if(op == H5S_SELECT_SET)
	    this->select_none();
      }

      void
      dataspace::select_slices( int slice_dim,
				const vector<hsize_t>::view& idxs,
				H5S_seloper_t op )
      {
	 ASSERT(slice_dim >= 0 && slice_dim < this->simple_extent_num_dims(),
		"Slice dimension out of data space dimension range.");

	 vector<hsize_t> dims, elems;
	 this->simple_extent_dims(dims);
	 int slice_size = 1;
	 for(int ii = 0; ii < dims.size(); ++ii) {
	    if(ii != slice_dim)
	       slice_size *= dims[ii];
	 }
	 elems.resize(idxs.size()*slice_size);
	 for(int ii = 0; ii < dims[slice_dim]; ++ii) {
	    for(int jj = 0; jj < slice_size; ++jj)
	       elems[ii*slice_size + jj] = idxs[ii]*slice_size + jj;
	 }
	 this->select_elements(elems, op);
      }
   }
}
