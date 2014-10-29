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

      dataset::dataset()
         : _id( -1 )
      {
      }

      dataset::dataset( hid_t id,
                        bool dummy )
	 : _id( id )
      {
      }

      dataset::dataset( h5::location const& loc,
			std::string const& name )
	 : _id( -1 )
      {
	 open( loc, name );
      }

      dataset::dataset( h5::location& loc,
                        std::string const& name,
                        h5::datatype const& type,
                        h5::dataspace const& space,
                        property_list const& props )
         : _id( -1 )
      {
         create( loc, name, type, space, props );
      }

      dataset::dataset( h5::location& loc,
                        std::string const& name,
                        h5::datatype const& type,
                        hsize_t size,
                        property_list const& props )
         : _id( -1 )
      {
         create( loc, name, type, size, props );
      }

      dataset::~dataset()
      {
	 close();
      }

      void
      dataset::open( const h5::location& loc,
		     const std::string& name )
      {
	 this->close();
	 this->_id = H5Dopen1(loc.id(), name.c_str());
	 ASSERT( this->_id >= 0, "Failed to open HDF5 dataset." );
      }

      void
      dataset::create( h5::location& loc,
                       std::string const& name,
                       h5::datatype const& type,
                       h5::dataspace const& space,
                       property_list const& props )
      {
	 close();
	 _create_groups( loc, name );
	 _id = H5Dcreate1( loc.id(), name.c_str(), type.id(), space.id(), props.id() );
         ASSERT( _id >= 0, "Failed to create HDF5 dataset." );
      }

      void
      dataset::create( h5::location& loc,
                       std::string const& name,
                       h5::datatype const& type,
                       hsize_t size,
                       property_list const& props )
      {
	 close();
	 _create_groups( loc, name );
         h5::dataspace space( size );
	 _id = H5Dcreate1( loc.id(), name.c_str(), type.id(), space.id(), props.id() );
         ASSERT( _id >= 0, "Failed to create HDF5 dataset." );
      }

      bool
      dataset::is_open() const
      {
         return _id >= 0;
      }

      void
      dataset::close()
      {
	 if( _id != -1 )
	 {
	    INSIST( H5Dclose(this->_id), >= 0 );
	    _id = -1;
	 }
      }

      h5::datatype
      dataset::datatype() const
      {
	 hid_t type = H5Dget_type( _id );
	 ASSERT( type >= 0 );
	 return h5::datatype( type );
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

      h5::dataspace
      dataset::dataspace() const
      {
	 hid_t id = H5Dget_space( _id );
	 ASSERT( id >= 0, "Failed to retrieve HDF5 dataspace." );
         return h5::dataspace( hid_tag(), id );
      }

      hsize_t
      dataset::extent() const
      {
         return this->dataspace().size();
      }

      void
      dataset::set_extent( hsize_t size )
      {
         ASSERT( this->dataspace().simple_extent_num_dims() == 1 );
	 hsize_t size_vec[1];
	 size_vec[0] = size;
	 INSIST( H5Dset_extent( _id, size_vec ), >= 0 );
      }

      void
      dataset::read( void* buf,
		     const h5::datatype& mem_type,
		     const h5::dataspace& mem_space,
		     const h5::dataspace& file_space,
		     const mpi::comm& comm ) const
      {
	 hid_t plist_id;
#ifdef PARALLELHDF5
	 if(comm != mpi::comm::null && comm.size() != 1) {
	    plist_id = H5Pcreate(H5P_DATASET_XFER);
	    H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
	 }
	 else
#endif
	    plist_id = H5P_DEFAULT;

	 INSIST(H5Dread(this->_id, mem_type.id(), mem_space.id(), file_space.id(), plist_id, buf), >= 0);

	 if(comm != mpi::comm::null && comm.size() != 1)
	    H5Pclose(plist_id);
      }

      void
      dataset::read( void* buf,
                     h5::datatype const& dtype,
                     hsize_t size,
                     hsize_t offset,
                     mpi::comm const& comm ) const
      {
         h5::dataspace file_space( this->dataspace() );
         file_space.select_hyperslab( H5S_SELECT_SET, size, offset );

         h5::dataspace mem_space( size );
         mem_space.select_all();

         read( buf, dtype, mem_space, file_space, comm );
      }

      void
      dataset::write( void const* buf,
		      h5::datatype const& mem_type,
		      h5::dataspace const& mem_space,
		      h5::dataspace const& file_space,
		      mpi::comm const& comm,
                      h5::property_list const& props ) const
      {
#ifndef NDEBUG
	 hssize_t mem_size = H5Sget_select_npoints(mem_space.id());
	 hssize_t file_size = H5Sget_select_npoints(file_space.id());
         ASSERT( mem_size == file_size, "Dataspace sizes don't match: ", mem_size, ", ", file_size );
#endif

	 h5::property_list local_props( props );
#ifdef PARALLELHDF5
	 if( comm != mpi::comm::null && comm.size() != 1 )
         {
            if( !props )
               local_props.create( H5P_DATASET_XFER );
            local_props.set_collective();
	 }
#else
         ASSERT( *_comm == mpi::comm::null || _comm->size() == 1,
                 "Attempting to create parallel HDF5 file without parallel extensions." );
#endif

	 INSIST( H5Dwrite( _id, mem_type.id(), mem_space.id(), file_space.id(), local_props.id(), buf ), >= 0 );
      }

      void
      dataset::write( void const* buf,
                      h5::datatype const& type,
                      hsize_t size,
                      hsize_t offset,
                      mpi::comm const& comm,
                      h5::property_list const& props ) const
      {
         h5::dataspace file_space( dataspace() );
#ifndef NDEBUG
	 hssize_t file_size = H5Sget_select_npoints( file_space.id() );
         ASSERT( offset + size <= file_size, "Trying to write out of range: offset=",
                 offset, ", size=", size, ", dataset size=", file_size );
#endif
         file_space.select_hyperslab( H5S_SELECT_SET, size, offset );

         h5::dataspace mem_space( size );
         mem_space.select_all();

         write( buf, type, mem_space, file_space, comm, props );
      }

      void
      dataset::_create_groups( h5::location& loc,
                               std::string const& name ) const
      {
         std::vector<std::string> grp_names;
	 boost::split( grp_names, name, boost::is_any_of( "/" ) );

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
