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

#include <boost/algorithm/string.hpp>
#include "dataset.hh"
#include "group.hh"

namespace hpc {
   namespace h5 {

      dataset::dataset( hid_t id )
	 : _id(id)
      {
      }

      dataset::dataset( h5::location& loc,
			const std::string& name )
	 : _id(-1)
      {
	 this->open(loc, name);
      }

      dataset::dataset( h5::location& loc,
                        const std::string& name,
                        const h5::datatype& datatype,
                        const h5::dataspace& dataspace,
                        boost::optional<const vector<hsize_t>::view&> chunk_size,
                        bool deflate )
         : _id( -1 )
      {
         create( loc, name, datatype, dataspace, chunk_size, deflate );
      }

      dataset::~dataset() {
	 if(this->_id != -1)
	    H5Dclose(this->_id);
      }

      void
      dataset::open( h5::location& loc,
		     const std::string& name )
      {
	 this->close();
	 this->_id = H5Dopen1(loc.id(), name.c_str());
	 ASSERT(this->_id >= 0, 10);
      }

      void
      dataset::create( h5::location& loc,
		       const std::string& name,
		       const h5::datatype& datatype,
		       const h5::dataspace& dataspace,
		       boost::optional<const vector<hsize_t>::view&> chunk_size,
		       bool deflate )
      {
	 this->close();

	 // Need to create any groups specified in name if they do not exist.
	 this->create_groups(loc, name);

	 hid_t dcpl;
	 if(chunk_size) {
	    dcpl = H5Pcreate(H5P_DATASET_CREATE);
	    ASSERT(dcpl >= 0);
	    H5Pset_chunk(dcpl, dataspace.simple_extent_num_dims(), *chunk_size);
	 }
	 else
	    dcpl = H5P_DEFAULT;

	 if(deflate)
	    H5Pset_deflate(dcpl, 9);

	 this->_id = H5Dcreate1(loc.id(), name.c_str(), datatype.id(), dataspace.id(), dcpl);
	 ASSERT(this->_id >= 0);

	 if(chunk_size)
	    H5Pclose(dcpl);
      }

      void
      dataset::close()
      {
	 if(this->_id != -1)
	    INSIST(H5Dclose(this->_id), >= 0);
      }

      H5T_class_t
      dataset::type_class() const
      {
	 hid_t type = H5Dget_type(this->_id);
	 ASSERT(type >= 0);
	 H5T_class_t type_class = H5Tget_class(type);
	 H5Tclose(type);
	 return type_class;
      }

      void
      dataset::space( h5::dataspace& space ) const
      {
#ifndef NDEBUG
	 hid_t id = H5Dget_space(this->_id);
	 ASSERT(id >= 0, 10);
	 space.set_id(id);
#else
	 space.set_id(H5Dget_space(this->_id));
#endif
      }

      void
      dataset::read( void* buf,
		     const h5::datatype& mem_type,
		     const h5::dataspace& mem_space,
		     const h5::dataspace& file_space,
		     const mpi::comm& comm )
      {
	 hid_t plist_id;
	 if(comm != mpi::comm::null && comm.size() != 1) {
	    plist_id = H5Pcreate(H5P_DATASET_XFER);
	    H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
	 }
	 else
	    plist_id = H5P_DEFAULT;

	 INSIST(H5Dread(this->_id, mem_type.id(), mem_space.id(), file_space.id(), plist_id, buf), >= 0);

	 if(comm != mpi::comm::null && comm.size() != 1)
	    H5Pclose(plist_id);
      }

      void
      dataset::write( const void* buf,
		      const h5::datatype& mem_type,
		      const h5::dataspace& mem_space,
		      const h5::dataspace& file_space,
		      const mpi::comm& comm )
      {
	 hssize_t mem_size = H5Sget_select_npoints(mem_space.id());
	 hssize_t file_size = H5Sget_select_npoints(file_space.id());

	 hid_t plist_id;
	 if(comm != mpi::comm::null && comm.size() != 1) {
	    plist_id = H5Pcreate(H5P_DATASET_XFER);
	    H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
	 }
	 else
	    plist_id = H5P_DEFAULT;

	 INSIST(H5Dwrite(this->_id, mem_type.id(), mem_space.id(), file_space.id(), plist_id, buf), >= 0);

	 if(comm != mpi::comm::null && comm.size() != 1)
	    H5Pclose(plist_id);
      }

      void
      dataset::create_groups( h5::location& loc,
			      const std::string& name ) const
      {
	 vector<std::string> grp_names;
	 boost::split(grp_names, name, boost::is_any_of("/"));

	 // The final name in the list will be the name of the dataset, so don't
	 // try and make a group out of it.
	 if(grp_names.size() > 1) {
	    h5::group grp;
	    int ii = 0;
	    if(!grp_names[ii].size())
	       ++ii;

	    // If we've traversed to the last name don't bother creating it as it
	    // is the name of the new dataset.
	    if(ii != grp_names.size() - 1) {

	       if(!loc.has_link(grp_names[ii]))
		  loc.create_group(grp_names[ii]);
	       loc.open_group(grp_names[ii], grp);
	       for(++ii; ii < grp_names.size() - 1; ++ii) {
		  if(!grp.has_link(grp_names[ii]))
		     grp.create_group(grp_names[ii]);
		  grp.open_group(grp_names[ii], grp);
	       }
	    }
	 }
      }
   }
}
