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

#ifndef hpc_h5_dataset_hh
#define hpc_h5_dataset_hh

#include <boost/mpl/map.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/at.hpp>
#include <boost/move/move.hpp>
#include "libhpc/mpi.hh"
#include "location.hh"
#include "datatype.hh"
#include "dataspace.hh"
#include "property_list.hh"

namespace hpc {
   namespace h5 {

      class dataset
      {
         BOOST_MOVABLE_BUT_NOT_COPYABLE( dataset );

      public:

         dataset();

	 dataset( hid_t id,
                  bool dummy );

	 dataset( h5::location const& loc,
		  std::string const& name );

	 dataset( h5::location& loc,
                  std::string const& name,
                  h5::datatype const& type,
                  h5::dataspace const& space,
		  property_list const& props = property_list() );

	 dataset( h5::location& loc,
                  std::string const& name,
                  h5::datatype const& type,
                  hsize_t size,
		  property_list const& props = property_list() );

         inline
         dataset( BOOST_RV_REF( dataset ) src )
            : _id( src._id )
         {
            src._id = -1;
         }

	 ~dataset();

         inline
         dataset&
         operator=( BOOST_RV_REF( dataset ) src )
         {
            close();
            _id = src._id;
            src._id = -1;
            return *this;
         }

	 void
	 open( h5::location const& loc,
	       std::string const& name );

	 void
	 create( h5::location& loc,
		 std::string const& name,
                 h5::datatype const& type,
                 h5::dataspace const& space,
                 property_list const& props = property_list() );

	 void
	 create( h5::location& loc,
		 std::string const& name,
		 h5::datatype const& type,
                 hsize_t size,
                 property_list const& props = property_list() );

	 void
	 close();

         h5::datatype
         datatype() const;

	 H5T_class_t
	 type_class() const;

         h5::dataspace
	 dataspace() const;

	 void
	 set_extent( hsize_t size );

	 hsize_t
	 extent() const;

	 void
	 read( void* buf,
	       h5::datatype const& mem_type,
	       h5::dataspace const& mem_space = h5::dataspace::all,
	       h5::dataspace const& file_space = h5::dataspace::all,
	       mpi::comm const& comm = mpi::comm::self );

         void
         read( void* buf,
               h5::datatype const& type,
               hsize_t size,
               hsize_t offset,
               mpi::comm const& comm = mpi::comm::self );

	 template< class T >
         T
	 read( hsize_t elem )
         {
	    BOOST_MPL_ASSERT( (boost::mpl::has_key<h5::datatype::type_map,T>) );
	    h5::datatype type( boost::mpl::at<h5::datatype::type_map,T>::type::value );
	    ASSERT( type.size() == sizeof(T) );

	    h5::dataspace file_space( dataspace() );
	    file_space.select_one( elem );

	    h5::dataspace mem_space( 1 );
	    mem_space.select_all();

	    T data;
	    read( &data, type, mem_space, file_space );
	    return data;
         }

	 template< class Buffer >
	 void
	 read( typename type_traits<Buffer>::reference buf,
               hsize_t offs = 0,
               mpi::comm const& comm = mpi::comm::self )
	 {
            typedef typename Buffer::value_type value_type;

	    BOOST_MPL_ASSERT( (boost::mpl::has_key<h5::datatype::type_map,value_type>) );
	    h5::datatype type( boost::mpl::at<h5::datatype::type_map,value_type>::type::value );

            // Get the filespace and set elements.
	    h5::dataspace file_space( dataspace() );
            if( offs )
               file_space.select_range( offs, offs + buf.size() );
            else
               file_space.select_all();

            // Create the memory space.
	    h5::dataspace mem_space( buf.size() );
	    mem_space.select_all();

            // Read from the dataset.
	    read( buf.data(), type, mem_space, file_space, comm );
	 }

	 template< class Buffer,
                   class Elements >
	 void
	 read( typename type_traits<Buffer>::reference buf,
	       typename type_traits<Elements>::const_reference elems,
               mpi::comm& comm = mpi::comm::self )
	 {
            typedef typename Buffer::value_type value_type;

            ASSERT( buf.size() == elems.size(), "Buffer size mismatch." );

            // Map to a HDF5 datatype.
	    BOOST_MPL_ASSERT( (boost::mpl::has_key<h5::datatype::type_map,value_type>) );
	    h5::datatype type( boost::mpl::at<h5::datatype::type_map,value_type>::type::value );

            // Get the filespace and set elements.
	    h5::dataspace file_space( dataspace() );
	    file_space.select_elements( elems );

            // Create the memory space.
	    h5::dataspace mem_space( buf.size() );
	    mem_space.select_all();

            // Read from the dataset.
	    read( buf.data(), type, mem_space, file_space, comm );
	 }

	 void
	 write( void const* buf,
		h5::datatype const& mem_type,
		h5::dataspace const& mem_space = h5::dataspace::all,
		h5::dataspace const& file_space = h5::dataspace::all,
		mpi::comm const& comm = mpi::comm::self );

         void
         write( void* buf,
                h5::datatype const& type,
                hsize_t size,
                hsize_t offset,
                mpi::comm const& comm = mpi::comm::self );

	 template< class Buffer >
	 void
	 write( typename type_traits<Buffer>::const_reference buf,
                hsize_t offset = 0,
                mpi::comm const& comm = mpi::comm::self )
	 {
            typedef typename Buffer::value_type value_type;
	    BOOST_MPL_ASSERT( (boost::mpl::has_key<h5::datatype::type_map,value_type>) );
	    h5::datatype type( boost::mpl::at<h5::datatype::type_map,value_type>::type::value );
            write( buf.data(), type, buf.size(), offset, comm );
	 }

      protected:

	 void
	 _create_groups( h5::location& loc,
                         std::string const& name ) const;

	 hid_t _id;
      };

      ///
      ///
      ///
      template< class T >
      void
      location::write( std::string const& name,
                       T const& value )
      {
         BOOST_MPL_ASSERT( (boost::mpl::has_key<h5::datatype::type_map,T>) );
         h5::datatype type( boost::mpl::at<h5::datatype::type_map,T>::type::value );

         h5::dataspace file_space( 1 );
         h5::dataset file_set( *this, name, type, file_space );
         file_space.select_all();

         h5::dataspace mem_space( 1 );
         mem_space.select_all();

         file_set.write( &value, type, mem_space, file_space );
      }

      ///
      ///
      ///
      template< class Buffer >
      void
      location::write( std::string const& name,
                       typename type_traits<Buffer>::const_reference buf,
                       mpi::comm const& comm )
      {
         typedef typename Buffer::value_type value_type;

         BOOST_MPL_ASSERT( (boost::mpl::has_key<h5::datatype::type_map,value_type>) );
         h5::datatype type( boost::mpl::at<h5::datatype::type_map,value_type>::type::value );

         hsize_t size = comm.all_reduce( buf.size(), MPI_SUM );
         hsize_t cnt = buf.size();
         hsize_t offs = comm.scan( cnt, MPI_SUM, true );

         h5::dataspace file_space( size );
         h5::dataset file_set( *this, name, type, file_space );
         file_space.select_hyperslab( H5S_SELECT_SET, cnt, offs );

         h5::dataspace mem_space( cnt );
         mem_space.select_all();

         file_set.write( buf, type, mem_space, file_space, comm );
      }
   }
}

#endif
