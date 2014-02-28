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
#include "libhpc/mpi/mpi.hh"
#include "location.hh"
#include "datatype.hh"
#include "dataspace.hh"
#include "property_list.hh"

namespace hpc {
   namespace h5 {

      class dataset
      {
      public:

	 dataset( hid_t id = -1 );

	 dataset( h5::location& loc,
		  const std::string& name );

	 dataset( h5::location& loc,
                  const std::string& name,
                  const h5::datatype& datatype,
                  const h5::dataspace& dataspace,
                  optional<const vector<hsize_t>::view&> chunk_size = optional<const vector<hsize_t>::view&>(),
                  bool deflate=false,
		  optional<const property_list&> props = optional<const property_list&>() );

	 dataset( h5::location& loc,
                  std::string const& name,
                  h5::datatype const& dtype,
                  hsize_t size,
		  optional<property_list const&> props = optional<property_list const&>() );

	 ~dataset();

	 void
	 open( const h5::location& loc,
	       const std::string& name );

	 void
	 create( h5::location& loc,
		 const std::string& name,
		 const h5::datatype& datatype,
		 const h5::dataspace& dataspace,
		 optional<const vector<hsize_t>::view&> chunk_size = optional<const vector<hsize_t>::view&>(),
		 bool deflate=false,
		 optional<const property_list&> props = optional<const property_list&>() );

	 void
	 create( h5::location& loc,
		 std::string const& name,
		 h5::datatype const& dtype,
                 hsize_t size,
		 optional<property_list const&> props = optional<property_list const&>() );

	 void
	 close();

         h5::datatype
         datatype() const;

	 H5T_class_t
	 type_class() const;

	 void
	 space( h5::dataspace& space ) const;

	 hsize_t
	 extent() const;

	 void
	 set_extent( hsize_t size );

	 void
	 read( void* buf,
	       const h5::datatype& mem_type,
	       const h5::dataspace& mem_space=h5::dataspace::all,
	       const h5::dataspace& file_space=h5::dataspace::all,
	       const mpi::comm& comm=mpi::comm::self );

         void
         read( void* buf,
               hsize_t size,
               h5::datatype const& dtype,
               hsize_t offset,
               mpi::comm& comm = mpi::comm::self );

	 template< class T >
         T
	 read( hsize_t elem )
         {
	    BOOST_MPL_ASSERT( (mpl::has_key<h5::datatype::type_map,T>) );
	    h5::datatype dtype( mpl::at<h5::datatype::type_map,T>::type::value );

	    h5::dataspace file_space;
	    space( file_space );
	    file_space.select_one( elem );

	    h5::dataspace mem_space;
            mem_space.create( 1 );
	    mem_space.select_all();

	    T data;
	    read( &data, dtype, mem_space, file_space, mpi::comm::self );
	    return data;
         }

	 template< class T >
	 void
	 read( typename hpc::view<std::vector<T>>::type buf,
               mpi::comm& comm = mpi::comm::self )
	 {
            // Load the HDF5 datatype from the dataset.
            h5::datatype dtype = this->datatype();
	    ASSERT( dtype.size() == sizeof(T) );

            // Get the filespace and set elements.
	    h5::dataspace file_space;
	    space( file_space );
	    file_space.select_all();

            // Create the memory space.
            vector<hsize_t> dims( 1 );
	    dims[0] = buf.size();
	    h5::dataspace mem_space( dims );
	    mem_space.select_all();

            // Read from the dataset.
	    read( buf.data(), dtype, mem_space, file_space, comm );
	 }

	 template< class T >
	 void
	 read( typename hpc::view<std::vector<T>>::type buf,
	       hpc::view<std::vector<hsize_t>>::type const& elems,
               mpi::comm& comm = mpi::comm::self )
	 {
            ASSERT( buf.size() == elems.size(), "Buffer size mismatch." );

            // // Map to a HDF5 datatype.
	    // BOOST_MPL_ASSERT( (mpl::has_key<h5::datatype::type_map,T>) );
	    // h5::datatype dtype( mpl::at<h5::datatype::type_map, T>::type::value );

            // Load the HDF5 datatype from the dataset.
            h5::datatype dtype = this->datatype();

            // Get the filespace and set elements.
	    h5::dataspace file_space;
	    space( file_space );
	    file_space.select_elements2( elems );

            // Create the memory space.
            vector<hsize_t> dims( 1 );
	    dims[0] = buf.size();
	    h5::dataspace mem_space( dims );
	    mem_space.select_all();

            // Read from the dataset.
	    read( buf.data(), dtype, mem_space, file_space, comm );
	 }

	 void
	 write( const void* buf,
		const h5::datatype& mem_type,
		const h5::dataspace& mem_space=h5::dataspace::all,
		const h5::dataspace& file_space=h5::dataspace::all,
		const mpi::comm& comm=mpi::comm::self );

         void
         write( void* buf,
                hsize_t size,
                h5::datatype const& dtype,
                hsize_t offset,
                mpi::comm& comm = mpi::comm::self );

	 template< class T >
	 void
	 write( typename view<std::vector<T>>::type buf,
                hsize_t offset = 0,
                mpi::comm& comm = mpi::comm::self )
	 {
	    BOOST_MPL_ASSERT( (mpl::has_key<h5::datatype::type_map,T>) );
	    h5::datatype dtype( mpl::at<h5::datatype::type_map,T>::type::value );
            write( buf.data(), buf.size(), dtype, offset, comm );
	 }

	 void
	 extend( hsize_t size );

      protected:

	 void
	 create_groups( h5::location& loc,
			const std::string& name ) const;

	 hid_t _id;
      };

      ///
      ///
      ///
      template< class T >
      void
      location::write( const std::string& name,
                       const T& value,
                       const mpi::comm& comm )
      {
         BOOST_MPL_ASSERT( (boost::mpl::has_key<h5::datatype::type_map, T>) );
         h5::datatype datatype( boost::mpl::at<h5::datatype::type_map, T>::type::value );

         vector<hsize_t> dims( 1 );
         dims[0] = 1;

         h5::dataspace file_space( dims );
         h5::dataset file_set;
         file_set.create( *this, name, datatype, file_space );
         file_space.select_all();

         h5::dataspace mem_space( dims );
         mem_space.select_all();

         file_set.write( &value, datatype, mem_space, file_space, comm );
      }

      ///
      ///
      ///
      template< class T >
      void
      location::write( const std::string& name,
                       const typename vector<T>::view& data,
                       const mpi::comm& comm,
                       boost::optional<const vector<hsize_t>::view&> chunk_size,
                       bool deflate )
      {
         BOOST_MPL_ASSERT( (boost::mpl::has_key<h5::datatype::type_map, T>) );
         h5::datatype datatype( boost::mpl::at<h5::datatype::type_map, T>::type::value );

         vector<hsize_t> dims(1), count(1), offset(1);
         dims[0] = comm.all_reduce(data.size(), MPI_SUM);
         count[0] = data.size();
         offset[0] = comm.scan(data.size(), MPI_SUM, true);

         h5::dataspace file_space(dims);
         h5::dataset file_set;
         file_set.create(*this, name, datatype, file_space, chunk_size, deflate);
         file_space.select_all();
         file_space.select_hyperslab(H5S_SELECT_SET, count, offset);

         dims[0] = data.size();
         h5::dataspace mem_space(dims);
         mem_space.select_all();

         file_set.write(data, datatype, mem_space, file_space, comm);
      }
   }
}

#endif
