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

#ifndef hpc_h5_file_hh
#define hpc_h5_file_hh

#include "libhpc/mpi/comm.hh"
#include "location.hh"
#include "datatype.hh"
#include "dataspace.hh"
#include "dataset.hh"
#include "group.hh"

namespace hpc {
   namespace h5 {

      class file
	 : public location
      {
      public:

	 file();

         ///
         /// @param flags  Can be H5F_ACC_EXCL, H5F_ACC_TRUNC, H5F_ACC_RDONLY
         ///               or H5F_ACC_RDWR.
         ///
	 file( std::string const& filename,
	       unsigned flags,
	       mpi::comm const& comm = mpi::comm::self,
	       property_list const& props = property_list() );

	 ~file();

	 void
	 open( std::string const& filename,
	       unsigned flags,
	       mpi::comm const& comm = mpi::comm::self,
	       property_list const& props = property_list() );

	 void
	 close();

         h5::dataset const
         dataset( std::string const& name ) const;

	 template< class BufferT >
         typename boost::disable_if<random_access_trait<BufferT> >::type
	 write( std::string const& name,
		typename type_traits<BufferT>::const_reference value )
	 {
	    typedef typename type_traits<BufferT>::value value_type;

	    BOOST_MPL_ASSERT( (boost::mpl::has_key<h5::datatype::type_map,value_type>) );
	    h5::datatype type( boost::mpl::at<h5::datatype::type_map,value_type>::type::value );

	    h5::dataspace file_space( 1 );
	    h5::dataset file_set( *this, name, type, file_space );
	    file_space.select_all();

	    h5::dataspace mem_space( 1 );
	    mem_space.select_all();

	    // What to do when calling to write a single element from
	    // many ranks? I'll just take one of them for now.
	    if( _comm->rank() == 0 )
	       file_set.write( &value, type, mem_space, file_space );
	 }

	 template< class T >
	 void
	 write_serial( std::string const& name,
		       std::vector<T> const& buf )
	 {
	    write_serial<std::vector<T> >( name, buf );
	 }

	 template< class Buffer >
         typename boost::enable_if<random_access_trait<Buffer> >::type
	 write_serial( std::string const& name,
                       typename type_traits<Buffer>::const_reference buf )
	 {
            typedef typename Buffer::value_type value_type;

	    BOOST_MPL_ASSERT( (boost::mpl::has_key<h5::datatype::type_map,value_type>) );
	    h5::datatype type( boost::mpl::at<h5::datatype::type_map,value_type>::type::value );

	    h5::dataspace file_space( buf.size() );
	    h5::dataset file_set( *this, name, type, file_space );
	    file_space.select_all();
	    file_space.select_hyperslab( H5S_SELECT_SET, buf.size(), 0 );

	    h5::dataspace mem_space( buf.size() );
	    mem_space.select_all();

	    // Only one rank writes.
	    if( _comm->rank() == 0 )
	       file_set.write( buf.data(), type, mem_space, file_space );
	 }

	 template< class Buffer >
         typename boost::enable_if<random_access_trait<Buffer> >::type
	 write( std::string const& name,
                typename type_traits<Buffer>::const_reference buf )
	 {
            typedef typename Buffer::value_type value_type;

	    BOOST_MPL_ASSERT( (boost::mpl::has_key<h5::datatype::type_map,value_type>) );
	    h5::datatype type( boost::mpl::at<h5::datatype::type_map,value_type>::type::value );

            hsize_t size = _comm->all_reduce( buf.size(), MPI_SUM );
            hsize_t cnt = buf.size();
            hsize_t offs = _comm->scan( cnt, MPI_SUM, true );

	    h5::dataspace file_space( size );
	    h5::dataset file_set( *this, name, type, file_space );
	    file_space.select_hyperslab( H5S_SELECT_SET, cnt, offs );

	    h5::dataspace mem_space( cnt );
	    mem_space.select_all();

	    file_set.write( buf.data(), type, mem_space, file_space, *_comm );
	 }

	 // void
	 // read_data_dims( const std::string& name,
         //                 std::vector<hsize_t>& dims );

	 // hsize_t
	 // read_data_size( const std::string& name );

	 // hsize_t
	 // read_local_data_size( const std::string& name );

	 void
	 read( std::string const& name,
               void* buf,
               h5::datatype const& type,
               hsize_t size,
               hsize_t offs,
               mpi::comm const& comm = mpi::comm::self );

	 template< class T >
         typename boost::disable_if<random_access_trait<T>,T>::type
	 read( std::string const& name,
	       hsize_t elem = 0 )
	 {
	    h5::dataset file_set( *this, name );
            return file_set.read<T>( elem );
	 }

	 template< class Buffer >
         typename boost::enable_if<random_access_trait<Buffer> >::type
	 read( const std::string& name,
	       typename type_traits<Buffer>::reference buf,
               hsize_t offs = 0 )
	 {
            h5::dataset file_set( *this, name );
            file_set.read<Buffer>( buf, offs, *_comm );
	 }

	 template< class Buffer,
                   class Elements >
	 void
	 read( const std::string& name,
	       typename type_traits<Buffer>::reference buf,
	       typename type_traits<Buffer>::const_reference elems )
	 {
	    h5::dataset file_set( *this, name );
            file_set.read<Buffer>( buf, elems, *_comm );
	 }

	 // template< class Buffer >
	 // void
	 // reada( const std::string& name,
         //        typename type_traits<Buffer>::reference buf,
	 // 	mpi::comm const& comm = mpi::comm::self )
	 // {
	 //    // TODO: Needs to be parallel.
	 //    hsize_t size = read_local_data_size( name );
	 //    buf.resize( size );
	 //    this->read<T>( name, data );
	 // }

	 // template< class T >
	 // void
	 // read_elements( const std::string& name,
	 // 		vector<T>::view data,
	 // 		const vector<hsize_t>::view& elems,
	 // 		const mpi::comm& comm=mpi::comm::self );

	 // template< class T >
	 // void
	 // reada_elements( const std::string& name,
	 // 		 vector<T>& data,
	 // 		 const vector<hsize_t>::view& elems,
	 // 		 const mpi::comm& comm=mpi::comm::self );

	 // template< class T >
	 // void reada(const std::string& name, Vector<T,2>& data, const MPI::Comm& comm=MPI::Comm::SELF, int splitDim=0);

	 // template<class T>
	 // void reada(const std::string& name, Vector<T,2>& data, const VectorView<hsize_t>& elems, const MPI::Comm& comm=MPI::Comm::SELF);

	 // void read(
	 //    const std::string& name,
	 //    void* data,
	 //    DataType& dataType,
	 //    const MPI::Comm& comm=MPI::Comm::SELF,
	 //    int splitDim=0
	 //    );

	 // void read(
	 //    const std::string& name,
	 //    void* data,
	 //    DataType& dataType,
	 //    const VectorView<hsize_t>& elems,
	 //    const MPI::Comm& comm=MPI::Comm::SELF
	 //    );

	 // template< class T >
	 // void
	 // read( const std::string& name,
	 //       csr<T>& data )
	 // {
	 //    this->read<index>(name + "_counts", data.counts());
	 //    data.setup_array(true);

	 //    this->read<T>(name + "_array", data.mod_array());
	 // }

	 // /// Number of rows must be set to indicate which chunks to read.
	 // template< class T >
	 // void
	 // read( const std::string& name,
	 //       csr<T>& data,
	 //       const vector<hsize_t>::view& elements )
	 // {
	 //    this->read<index>(name + "_counts", data.counts(), elements);
	 //    data.setup_array(true);

	 //    vector<index> displs(data.num_rows());
	 //    this->read<index>(name + "_displs", displs, elements);

	 //    // TODO: Try and handle the CSR one row at a time. Not sure if this is a very good idea or not.
	 //    // I may need to set this up to handle a bunch of rows at a time.
	 //    std::string displs_name = name + "_displs";
	 //    for(hpc::index ii = 0; ii < data.num_rows(); ++ii)
	 //       this->read<T>(name + "_array", data[ii], displs[ii]);
	 // }

         // template< class T >
         // void
         // read( const string& name,
         //       const datatype& type,
         //       typename vector<T>::view data,
         //       hsize_t offset=0,
         //       const mpi::comm& comm=mpi::comm::self )
         // {
         //    h5::dataset dset( *this, name );
         //    h5::dataspace mem_space, file_space;
         //    mem_space.create( data.size() );
         //    dset.space( file_space );
         //    vector<hsize_t> count( 1 ), start( 1 );
         //    count[0] = data.size();
         //    start[0] = offset;
         //    file_space.select_hyperslab( H5S_SELECT_SET, count, start );
         //    dset.read( data, type, mem_space, file_space, comm );
         // }

	 // template< class T >
	 // void
	 // reada( const std::string& name,
	 // 	csr<T>& data,
	 // 	const mpi::comm& comm=mpi::comm::self )
	 // {
	 //    // TODO: Needs to be parallel.
	 //    size_t num_rows = read_data_size( name + "_counts" );
	 //    data.num_rows( num_rows );
	 //    this->read<T>( name, data );
	 // }

      protected:

         mpi::comm const* _comm;
      };

      // template<>
      // void
      // file::write<string>( const std::string& name,
      // 			   const vector<string>::view& data,
      // 			   boost::optional<const vector<hsize_t>::view&> chunk_size,
      // 			   bool deflate );

      // template<>
      // void
      // file::reada<string>( const std::string& name,
      //   		   vector<string>& data,
      //   		   const mpi::comm& comm );
   }
}

#endif
