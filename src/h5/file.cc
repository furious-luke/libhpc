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

#include "file.hh"
//#include "Proxy.hh"

namespace hpc {
   namespace h5 {

      file::file()
	 : location(),
	   _comm(mpi::comm::self)
      {
      }

      file::file( const std::string& filename,
		  unsigned int flags,
		  const mpi::comm& comm )
	 : location(),
	   _comm(mpi::comm::self)
      {
	 this->open(filename, flags, comm);
      }

      file::~file()
      {
	 this->close();
      }

      void
      file::open( const std::string& filename,
		  unsigned int flags,
		  const mpi::comm& comm,
		  optional<property_list&> props )
      {
	 this->_comm = &comm;

	 h5::property_list local_props( H5P_FILE_ACCESS );
	 if(*this->_comm != mpi::comm::null && this->_comm->size() != 1) {
	    if( !props )
	       props = local_props;
	    (*props).set_parallel( comm );
	 }

	 hid_t props_id;
	 if( props )
	    props_id = (*props).id();
	 else
	    props_id = H5P_DEFAULT;

	 if(flags == H5F_ACC_TRUNC || flags == H5F_ACC_EXCL)
	    this->_id = H5Fcreate(filename.c_str(), flags, H5P_DEFAULT, props_id);
	 else
	    this->_id = H5Fopen(filename.c_str(), flags, props_id);
      }

      void
      file::close()
      {
	 if( _id != -1 )
	 {
	    INSIST( H5Fclose( _id ), >= 0 );  // is collective
            _id = -1;
         }
	 _comm = mpi::comm::self;
      }

      template<>
      void
      file::write<string>( const std::string& name,
			   const vector<string>::view& data,
			   boost::optional<const vector<hsize_t>::view&> chunk_size,
			   bool deflate )
      {
	 BOOST_MPL_ASSERT( (mpl::has_key<h5::datatype::type_map,char>) );
	 h5::datatype datatype( mpl::at<h5::datatype::type_map,char>::type::value );

	 // Count total size of strings.
	 size_t size = 0;
         for( unsigned ii = 0; ii < data.size(); ++ii )
	    size += data[ii].size() + 1;
	 size_t net_size = this->_comm->all_reduce( size, MPI_SUM );

	 vector<hsize_t> dims(1), count(1), offset(1);
	 dims[0] = net_size;
	 offset[0] = this->_comm->scan( size, MPI_SUM, true );

	 h5::dataspace file_space( dims );
	 h5::dataset file_set;
	 file_set.create( *this, name, datatype, file_space, chunk_size, deflate );

	 vector<hsize_t> mem_dims( 1 ), mem_offs( 1 ), mem_count( 1 );
	 mem_dims[0] = net_size;
	 mem_offs[0] = 0;
	 h5::dataspace mem_space( mem_dims );

         for( unsigned ii = 0; ii < data.size(); ++ii )
	 {
	    count[0] = data[ii].size() + 1;
	    file_space.select_hyperslab( H5S_SELECT_SET, count, offset );
	    offset[0] += data[ii].size() + 1;

	    mem_count[0] = data[ii].size() + 1;
	    mem_space.select_hyperslab( H5S_SELECT_SET, mem_count, mem_offs );
	    file_set.write( data[ii].c_str(), datatype, mem_space, file_space, *this->_comm );
	 }
      }

      template<>
      void
      file::reada<string>( const std::string& name,
			   vector<string>& data,
			   const mpi::comm& comm )
      {
	 // Read in the full vector of bytes.
	 vector<char> full_str;
	 this->reada<char>( name, full_str );

	 // Count how many strings there are and allocate.
	 unsigned num_strs = std::count( full_str.begin(), full_str.end(), 0 );
	 data.reallocate( num_strs );

	 // Insert each string.
	 const char* first = full_str.data(), *last = full_str.data() + full_str.size();
	 const char* mark = first;
	 unsigned idx = 0;
	 while( first != last )
	 {
	    if( *first == 0 )
	    {
	       data[idx++] = mark;
	       mark = first + 1;
	    }
	    ++first;
	 }
      }

// template<>
// void file::writeItem(const std::string& name, const int& itm, const MPI::Comm& comm) {
//     this->writeItem(name, &itm, DataType::NATIVE_INT, comm);
// }

// void file::writeItem(const std::string& name, const void* itm, DataType& dataType, const MPI::Comm& comm) {
//     Vector<int> dataSize(1);
//     if(comm.getMyRank() == 0) {
// 	dataSize[0] = 1;
// 	this->write(name, dataSize, itm, dataType, comm);
//     }
//     else {
// 	dataSize[0] = 0;
// 	this->write(name, dataSize, NULL, dataType, comm);
//     }
// }

// template<>
// void file::write(const std::string& name, const VectorView<int>& data, const Comm& comm) {
//     Vector<int> dataSize(1);
//     dataSize[0] = data.getSize();
//     this->write(name, dataSize, data, DataType::NATIVE_INT, comm);
// }

// template<>
// void file::write(const std::string& name, const VectorView<int,2>& data, const Comm& comm, int splitDim) {
//     Vector<int> dataSize(2);
//     dataSize[0] = data.getSize(0);
//     dataSize[1] = data.getSize(1);
//     this->write(name, dataSize, data, DataType::NATIVE_INT, comm, splitDim);
// }

// template<>
// void file::write(const std::string& name, const VectorView<int>& data, const VectorView<hsize_t>& chunkSize, const Comm& comm, bool deflate) {
//     Vector<int> dataSize(1);
//     dataSize[0] = data.getSize();
//     this->write(name, dataSize, data, DataType::NATIVE_INT, chunkSize, comm, deflate);
// }

// template<>
// void file::write(const std::string& name, const VectorView<int,2>& data, const VectorView<hsize_t>& chunkSize, const Comm& comm, int splitDim, bool deflate) {
//     Vector<int> dataSize(2);
//     dataSize[0] = data.getSize(0);
//     dataSize[1] = data.getSize(1);
//     this->write(name, dataSize, data, DataType::NATIVE_INT, chunkSize, comm, splitDim, deflate);
// }

// template<>
// void file::write(const std::string& name, const VectorView<float>& data, const Comm& comm) {
//     Vector<int> dataSize(1);
//     dataSize[0] = data.getSize();
//     this->write(name, dataSize, data, DataType::NATIVE_FLOAT, comm);
// }

// template<>
// void file::write(const std::string& name, const VectorView<float,2>& data, const Comm& comm, int splitDim) {
//     Vector<int> dataSize(2);
//     dataSize[0] = data.getSize(0);
//     dataSize[1] = data.getSize(1);
//     this->write(name, dataSize, data, DataType::NATIVE_FLOAT, comm, splitDim);
// }

// template<>
// void file::write(const std::string& name, const VectorView<float>& data, const VectorView<hsize_t>& chunkSize, const Comm& comm, bool deflate) {
//     Vector<int> dataSize(1);
//     dataSize[0] = data.getSize();
//     this->write(name, dataSize, data, DataType::NATIVE_FLOAT, chunkSize, comm, deflate);
// }

// template<>
// void file::write(const std::string& name, const VectorView<float,2>& data, const VectorView<hsize_t>& chunkSize, const Comm& comm, int splitDim, bool deflate) {
//     Vector<int> dataSize(2);
//     dataSize[0] = data.getSize(0);
//     dataSize[1] = data.getSize(1);
//     this->write(name, dataSize, data, DataType::NATIVE_FLOAT, chunkSize, comm, splitDim, deflate);
// }

// template<>
// void file::write(const std::string& name, const VectorView<double>& data, const Comm& comm) {
//     Vector<int> dataSize(1);
//     dataSize[0] = data.getSize();
//     this->write(name, dataSize, data, DataType::NATIVE_DOUBLE, comm);
// }

// template<>
// void file::write(const std::string& name, const VectorView<double,2>& data, const Comm& comm, int splitDim) {
//     Vector<int> dataSize(2);
//     dataSize[0] = data.getSize(0);
//     dataSize[1] = data.getSize(1);
//     this->write(name, dataSize, data, DataType::NATIVE_DOUBLE, comm, splitDim);
// }

// template<>
// void file::write(const std::string& name, const VectorView<double>& data, const VectorView<hsize_t>& chunkSize, const Comm& comm, bool deflate) {
//     Vector<int> dataSize(1);
//     dataSize[0] = data.getSize();
//     this->write(name, dataSize, data, DataType::NATIVE_DOUBLE, chunkSize, comm, deflate);
// }

// template<>
// void file::write(const std::string& name, const VectorView<double,2>& data, const VectorView<hsize_t>& chunkSize, const Comm& comm, int splitDim, bool deflate) {
//     Vector<int> dataSize(2);
//     dataSize[0] = data.getSize(0);
//     dataSize[1] = data.getSize(1);
//     this->write(name, dataSize, data, DataType::NATIVE_DOUBLE, chunkSize, comm, splitDim, deflate);
// }

// void file::write(const std::string& name, const VectorView<int>& dataSize, const void* data, DataType& dataType, const Comm& comm, int splitDim) {
//     this->write(name, dataSize, data, dataType, VectorView<hsize_t>(), comm, splitDim, false);
// }

// void file::write(const std::string& name, const VectorView<int>& dataSize, const void* data, DataType& dataType, const VectorView<hsize_t>& chunkSize, const Comm& comm, int splitDim, bool deflate) {
//     assert_throw(dataSize.prod() >= 0,
// 	   "Data size cannot be negative.");
//     assert_throw(chunkSize.isEmpty() || chunkSize.getSize() == dataSize.getSize(),
// 	   "Chunk size must be either not specified, indicating no chunking to "
// 	   "be performed, or must be the same dimension as the data size.");
//     assert_throw(!deflate || !chunkSize.isEmpty(),
// 	   "Cannot request deflation without specifying a chunk size.");
//     assert_throw(splitDim >= 0 && splitDim < dataSize.getSize(),
// 	   "Trying to write split data using an invalid dimension index.");

//     // Reduce the global data size.
//     Vector<int> globalDataSize(dataSize);
//     globalDataSize[splitDim] = comm.allReduce<int>(globalDataSize[splitDim], MPI_SUM);
//     int rankOffs = comm.chainRecv<int>();
//     comm.chainSend<int>(rankOffs + dataSize[splitDim]);

//     // Copy the data size to hsize_t types.
//     Vector<hsize_t> dims(globalDataSize.getSize());
//     for(int ii = 0; ii < globalDataSize.getSize(); ++ii)
// 	dims[ii] = globalDataSize[ii];

//     DataSpace fileSpace(dims);
//     DataSet fileSet;
//     fileSet.create(*this, name, dataType, fileSpace, chunkSize, deflate);
//     fileSet.getSpace(fileSpace);
//     {
//     	Vector<hsize_t> count(dataSize.getSize()), offset(dataSize.getSize());
//     	for(int ii = 0; ii < dataSize.getSize(); ++ii) {
//     	    count[ii] = globalDataSize[ii];
//     	    offset[ii] = 0;
//     	}
//     	count[splitDim] = dataSize[splitDim];
//     	offset[splitDim] = rankOffs;
//     	fileSpace.selectHyperslab(H5S_SELECT_SET, count, offset);
//     }

//     dims[splitDim] = dataSize[splitDim];
//     DataSpace memSpace(dims);
//     if(dims[splitDim])
//     	memSpace.selectAll();
//     else
//     	memSpace.selectNone();

//     fileSet.write(data, dataType, memSpace, fileSpace, comm);
// }

// template<>
// void file::write(const std::string& name, const CSR<int>& data, const Comm& comm) {
//     this->writeCSR(name, data.getDispls(), data, DataType::NATIVE_INT, comm);
// }

// template<>
// void file::write(const std::string& name, const CSR<int>& data, const VectorView<hsize_t>& chunkSize, const Comm& comm, bool deflate) {
//     this->writeCSR(name, data.getDispls(), data, DataType::NATIVE_INT, chunkSize, comm, deflate);
// }

// template<>
// void file::write(const std::string& name, const CSR<float>& data, const Comm& comm) {
//     this->writeCSR(name, data.getDispls(), data, DataType::NATIVE_FLOAT, comm);
// }

// template<>
// void file::write(const std::string& name, const CSR<float>& data, const VectorView<hsize_t>& chunkSize, const Comm& comm, bool deflate) {
//     this->writeCSR(name, data.getDispls(), data, DataType::NATIVE_FLOAT, chunkSize, comm, deflate);
// }

// template<>
// void file::write(const std::string& name, const CSR<double>& data, const Comm& comm) {
//     this->writeCSR(name, data.getDispls(), data, DataType::NATIVE_DOUBLE, comm);
// }

// template<>
// void file::write(const std::string& name, const CSR<double>& data, const VectorView<hsize_t>& chunkSize, const Comm& comm, bool deflate) {
//     this->writeCSR(name, data.getDispls(), data, DataType::NATIVE_DOUBLE, chunkSize, comm, deflate);
// }

// void file::writeCSR(const std::string& name, const VectorView<int>& displs, const void* data, DataType& dataType, const Comm& comm) {
//     this->writeCSR(name, displs, data, dataType, VectorView<hsize_t>(), comm, false);
// }

// void file::writeCSR(const std::string& name, const VectorView<int>& displs, const void* data, DataType& dataType, const VectorView<hsize_t>& chunkSize, const Comm& comm, bool deflate) {
//     assert_throw(chunkSize.isEmpty() || chunkSize.getSize() == 2, 10);
//     assert_throw(!deflate || !chunkSize.isEmpty(), 20);

//     Vector<int> cnts;
//     cnts.setCntsFromDispls(displs);
//     std::string subName(name);
//     subName += "_counts";
//     this->write<int>(subName, cnts, chunkSize, comm, deflate);

//     // Collect displacement offsets from all ranks involved.
//     int rankOffs = comm.chainRecv<int>();
//     comm.chainSend<int>(rankOffs + displs[displs.getSize() - 1]);
//     VectorView<int> offsDispls = (VectorView<int>&)displs;
//     offsDispls.add(rankOffs);
//     this->write<int>(name + "_displs", VectorView<int>(displs, displs.getSize() - 1), chunkSize, comm, deflate);
//     offsDispls.sub(rankOffs);

//     subName = name;
//     subName += "_array";
//     Vector<int> writeSize(1);
//     writeSize[0] = displs.isEmpty() ? 0 : displs[displs.getSize() - 1];
//     this->write(subName, writeSize, data, dataType, chunkSize, comm, deflate);
// }

// template<>
// void file::writeInterlaced(const std::string& name, const CSR<int>& data, hsize_t size, const VectorView<int>& offsets, const MPI::Comm& comm) {
//     this->writeInterlaced(name, data.getDispls(), data, DataType::NATIVE_INT, size, offsets, comm);
// }

// void file::writeInterlaced(const std::string& name, const VectorView<int>& dataDispls, const void* data, const DataType& dataType, hsize_t size, const VectorView<int>& offsets, const MPI::Comm& comm) {
//     assert_throw(size >= 0,
// 	   "Invalid dataset size.");
// #ifndef NDEBUG
//     check_displs(dataDispls);
//     check_offsets(offsets);
// #endif
//     assert_throw((dataDispls.getSize() == 0 && offsets.getSize() == 0) || (dataDispls.getSize() - 1 == offsets.getSize()),
// 	   "There must be the same number of rows in the data CSR and the file displacements.");
// #ifndef NDEBUG
//     // Check that no element of data is to be written beyond the end of our dataset.
//     for(int ii = 0; ii < dataDispls.getSize() - 1; ++ii) {
// 	assert_throw(offsets[ii] + dataDispls[ii + 1] - dataDispls[ii] <= size,
// 	       "Attempting to write data beyond the limits of destination dataset.");
//     }
// #endif

//     // Check if the requested dataset exists. If so, be sure it is the same size. If not,
//     // create it with the appropriate size.
//     DataSet fileSet;
//     DataSpace fileSpace;
//     if(this->hasLink(name)) {
// 	fileSet.open(*this, name);
// 	fileSet.getSpace(fileSpace);
// 	assert_throw(fileSpace.getSimpleExtentNumDims() == 1,
// 	       "Cannot write an interlaced array to dataspace of dimension other than 1.");
// #ifndef NDEBUG
// 	Vector<hsize_t> dims;
// 	fileSpace.getSimpleExtentDims(dims);
// 	assert_throw(dims.prod() == size,
// 	       "Attempting to write an interlaced set of data to an existing dataset, but the "
// 	       "dataset is not matching the requested size.");
// #endif
//     }
//     else {
// 	Vector<hsize_t> dims(1);
// 	dims[0] = size;
// 	fileSpace.create(dims);
// 	fileSet.create(*this, name, H5::DataType::NATIVE_INT, fileSpace);
//     }

//     // Select the appropriate elements in the filespace in the right order.
//     {
// 	Vector<hsize_t> elems(dataDispls[dataDispls.getSize() - 1]);
// 	int pos = 0;
// 	for(int ii = 0; ii < offsets.getSize(); ++ii) {
// 	    int rowSize = dataDispls[ii + 1] - dataDispls[ii];
// 	    for(int jj = 0; jj < rowSize; ++jj)
// 		elems[pos++] = offsets[ii] + jj;
// 	}
// 	fileSpace.selectElements(elems);
//     }

//     // Create the memory dataspace.
//     Vector<hsize_t> dims(1);
//     dims[0] = dataDispls[dataDispls.getSize() - 1];
//     DataSpace memSpace(dims);

//     // Perform the transfer.
//     fileSet.write(data, dataType, memSpace, fileSpace, comm);
// }

      void
      file::read_data_dims( const std::string& name,
			    vector<hsize_t>& dims )
      {
	 h5::dataset file_set;
	 file_set.open(*this, name);
	 h5::dataspace file_space;
	 file_set.space(file_space);
	 dims.resize(file_space.simple_extent_num_dims());
	 file_space.simple_extent_dims(dims);
      }

      hsize_t
      file::read_data_size( const std::string& name )
      {
	 vector<hsize_t> dims;
	 this->read_data_dims(name, dims);
	 return std::accumulate(dims.begin(), dims.end(), 1, std::multiplies<hsize_t>());
      }

      hsize_t
      file::read_local_data_size( const std::string& name )
      {
	 hsize_t global_size = read_data_size( name );
	 return ((_comm->rank() + 1)*global_size)/_comm->size() - (_comm->rank()*global_size)/_comm->size();
      }

// template<>
// void file::read(const std::string& name, VectorView<int> data, const Comm& comm) {
// #ifndef NDEBUG
//     Proxy proxy(this->readDataSize(name), comm);
//     assert_throw(data.getSize() == proxy.getNumLocals(), 10);
// #endif
//     this->read(name, data, DataType::NATIVE_INT, comm);
// }

// template<>
// void file::read(const std::string& name, VectorView<float> data, const Comm& comm) {
// #ifndef NDEBUG
//     Proxy proxy(this->readDataSize(name), comm);
//     assert_throw(data.getSize() == proxy.getNumLocals(), 10);
// #endif
//     this->read(name, data, DataType::NATIVE_FLOAT, comm);
// }

// template<>
// void file::read(const std::string& name, VectorView<double> data, const Comm& comm) {
// #ifndef NDEBUG
//     Proxy proxy(this->readDataSize(name), comm);
//     assert_throw(data.getSize() == proxy.getNumLocals(), 10);
// #endif
//     this->read(name, data, DataType::NATIVE_DOUBLE, comm);
// }

// template<>
// void file::reada(const std::string& name, Vector<int>& data, const Comm& comm) {
//     Proxy proxy(this->readDataSize(name), comm);
//     data.setSize(proxy.getNumLocals());
//     this->read(name, data, comm);
// }

// template<>
// void file::reada(const std::string& name, Vector<float>& data, const Comm& comm) {
//     Proxy proxy(this->readDataSize(name), comm);
//     data.setSize(proxy.getNumLocals());
//     this->read(name, data, comm);
// }

// template<>
// void file::reada(const std::string& name, Vector<double>& data, const Comm& comm) {
//     Proxy proxy(this->readDataSize(name), comm);
//     data.setSize(proxy.getNumLocals());
//     this->read(name, data, comm);
// }

// template<>
// void file::read(const std::string& name, VectorView<int> data, const VectorView<hsize_t>& elems, const MPI::Comm& comm) {
// #ifndef NDEBUG
//     Vector<hsize_t> dims;
//     this->readDataDims(name, dims);
//     assert_throw(elems.getSize()%dims.getSize() == 0,
// 		 "Element selection array does not divide into the number of dimensions.");
//     assert_throw(data.getSize() == elems.getSize()/dims.getSize(),
// 		 "Insufficent input data size.");
// #endif
//     this->read(name, data, H5::DataType::NATIVE_INT, elems, comm);
// }

// template<>
// void file::read(const std::string& name, VectorView<float> data, const VectorView<hsize_t>& elems, const MPI::Comm& comm) {
// #ifndef NDEBUG
//     Vector<hsize_t> dims;
//     this->readDataDims(name, dims);
//     assert_throw(elems.getSize()%dims.getSize() == 0,
// 		 "Element selection array does not divide into the number of dimensions.");
//     assert_throw(data.getSize() == elems.getSize()/dims.getSize(),
// 		 "Insufficent input data size.");
// #endif
//     this->read(name, data, H5::DataType::NATIVE_FLOAT, elems, comm);
// }

// template<>
// void file::read(const std::string& name, VectorView<double> data, const VectorView<hsize_t>& elems, const MPI::Comm& comm) {
// #ifndef NDEBUG
//     Vector<hsize_t> dims;
//     this->readDataDims(name, dims);
//     assert_throw(elems.getSize()%dims.getSize() == 0,
// 		 "Element selection array does not divide into the number of dimensions.");
//     assert_throw(data.getSize() == elems.getSize()/dims.getSize(),
// 		 "Insufficent input data size.");
// #endif
//     this->read(name, data, H5::DataType::NATIVE_DOUBLE, elems, comm);
// }

// template<>
// void file::reada(const std::string& name, Vector<int>& data, const VectorView<hsize_t>& elems, const MPI::Comm& comm) {
//     Vector<hsize_t> dims;
//     this->readDataDims(name, dims);
//     data.setSize(elems.getSize()/dims.getSize());
//     this->read(name, data, elems, comm);
// }

// template<>
// void file::reada(const std::string& name, Vector<int,2>& data, const Comm& comm, int splitDim) {
//     Vector<hsize_t> dims;
//     this->readDataDims(name, dims);
//     assert_throw(dims.getSize() == 2,
// 	   "Invalid data rank as compared to an array of rank 2.");
//     Proxy proxy(dims[splitDim], comm);
//     Vector<int> dataSize(2);
//     for(int ii = 0; ii < 2; ++ii)
// 	dataSize[ii] = dims[ii];
//     dataSize[splitDim] = proxy.getNumLocals();
//     data.setSize(dataSize[0], dataSize[1]);
//     this->read(name, data, comm);
// }

// template<>
// void file::reada(const std::string& name, Vector<float,2>& data, const Comm& comm, int splitDim) {
//     Vector<hsize_t> dims;
//     this->readDataDims(name, dims);
//     assert_throw(dims.getSize() == 2,
// 	   "Invalid data rank as compared to an array of rank 2.");
//     Proxy proxy(dims[splitDim], comm);
//     Vector<int> dataSize(2);
//     for(int ii = 0; ii < 2; ++ii)
// 	dataSize[ii] = dims[ii];
//     dataSize[splitDim] = proxy.getNumLocals();
//     data.setSize(dataSize[0], dataSize[1]);
//     this->read(name, data, comm);
// }

// template<>
// void file::reada(const std::string& name, Vector<double,2>& data, const Comm& comm, int splitDim) {
//     Vector<hsize_t> dims;
//     this->readDataDims(name, dims);
//     assert_throw(dims.getSize() == 2,
// 	   "Invalid data rank as compared to an array of rank 2.");
//     Proxy proxy(dims[splitDim], comm);
//     Vector<int> dataSize(2);
//     for(int ii = 0; ii < 2; ++ii)
// 	dataSize[ii] = dims[ii];
//     dataSize[splitDim] = proxy.getNumLocals();
//     data.setSize(dataSize[0], dataSize[1]);
//     this->read(name, data, comm);
// }

// template<>
// void file::reada(const std::string& name, Vector<double,2>& data, const VectorView<hsize_t>& elems, const Comm& comm) {
//     Vector<hsize_t> dims;
//     this->readDataDims(name, dims);
//     assert_throw(dims.getSize() == 2,
// 	   "Invalid data rank as compared to an array of rank 2.");
//     data.setSize(elems.getSize()/(dims.getSize()*dims[1]), dims[1]);
//     this->read(name, VectorView<double>(data), elems, comm);
// }

// void file::read(const std::string& name, void* data, DataType& dataType, const Comm& comm, int splitDim) {
//     DataSet fileSet;
//     fileSet.open(*this, name);
//     DataSpace fileSpace;
//     fileSet.getSpace(fileSpace);

//     Vector<hsize_t> dims;
//     fileSpace.getSimpleExtentDims(dims);
//     assert_throw(splitDim >= 0 && splitDim < dims.getSize(),
// 	   "Trying to read split data using an invalid dimension index.");
//     Proxy proxy(dims[splitDim], comm);
//     {
// 	Vector<hsize_t> count(dims.getSize()), offset(dims.getSize());
// 	count.copy(dims);
// 	offset.setEntries(0);
// 	count[splitDim] = proxy.getNumLocals();
// 	offset[splitDim] = proxy.offs;
// 	fileSpace.selectHyperslab(H5S_SELECT_SET, count, offset);
//     }

//     dims[splitDim] = proxy.getNumLocals();
//     DataSpace memSpace(dims);
//     if(dims[splitDim])
// 	memSpace.selectAll();
//     else
// 	memSpace.selectNone();

//     fileSet.read(data, dataType, memSpace, fileSpace, comm);
// }

// void file::read(const std::string& name, void* data, DataType& dataType, const VectorView<hsize_t>& elems, const Comm& comm) {
//     DataSet fileSet;
//     fileSet.open(*this, name);
//     DataSpace fileSpace;
//     fileSet.getSpace(fileSpace);
//     Vector<hsize_t> fileDims;
//     this->readDataDims(name, fileDims);

// #ifndef NDEBUG
//     {
// 	int curDim = 0;
// 	for(int ii = 0; ii < elems.getSize(); ++ii) {
// 	    assert_throw(elems[ii] < fileDims[curDim],
// 			 "Attempting to read an element not within the bounds of the dataset.");
// 	    curDim = (curDim + 1)%fileDims.getSize();
// 	}
//     }
// #endif

//     fileSpace.selectElements(elems);

//     Vector<hsize_t> memDims(1);
//     memDims[0] = elems.getSize()/fileDims.getSize();
//     DataSpace memSpace(memDims);
//     if(memDims[0])
// 	memSpace.selectAll();
//     else
// 	memSpace.selectNone();

//     fileSet.read(data, dataType, memSpace, fileSpace, comm);
// }

// template<>
// void file::reada(const std::string& name, CSR<int>& data, const Comm& comm) {
//     std::string subName(name);
//     subName += "_counts";
//     Proxy proxy(this->readDataSize(subName), comm);
//     data.setNumRows(proxy.getNumLocals());
//     this->read(subName, VectorView<int>(data.accDispls(), proxy.getNumLocals()), comm);
//     data.setupArray(true);

//     // Read the offset of the beginning of my data.
//     int offs = this->readElem<int>(name + "_displs", proxy.offs, comm);

//     subName = name;
//     subName += "_array";
//     DataSet fileSet;
//     fileSet.open(*this, subName);
//     DataSpace fileSpace;
//     fileSet.getSpace(fileSpace);
//     assert_throw(fileSpace.getSimpleExtentNumDims() == 1, 10);
//     {
// 	Vector<hsize_t> count(1), offset(1);
// 	count[0] = data.getNumEntries(), offset[0] = data.getDispls()[offs];
// 	fileSpace.selectHyperslab(H5S_SELECT_SET, count, offset);
//     }

//     Vector<hsize_t> dims(1);
//     dims[0] = data.getNumEntries();
//     DataSpace memSpace(dims);
//     memSpace.selectAll();

//     fileSet.read(data, DataType::NATIVE_INT, memSpace, fileSpace);
// }

// template<>
// void file::reada(const std::string& name, CSR<int>& data, const VectorView<hsize_t>& rows, const Comm& comm) {
//     data.setNumRows(rows.getSize());

//     Vector<int> displs;
//     VectorView<int> cnts = VectorView<int>(data.accDispls(), rows.getSize());
//     this->reada(name + "_displs", displs, rows, comm);
//     this->read(name + "_counts", cnts, rows, comm);

//     // Open the file dataset and get its dataspace.
//     DataSet fileSet;
//     fileSet.open(*this, name + "_array");
//     DataSpace fileSpace;
//     fileSet.getSpace(fileSpace);

//     // Setup the elements I'm going to read.
//     {
// 	Vector<hsize_t> elems(cnts.sum());
// 	int pos = 0;
// 	for(int ii = 0; ii < rows.getSize(); ++ii) {
// 	    for(int jj = 0; jj < cnts[ii]; ++jj)
// 		elems[pos++] = displs[ii] + jj;
// 	}
// 	fileSpace.selectElements(elems);
//     }

//     // Finish setting up the data CSR and set a memory dataspace.
//     data.setupArray(true);
//     Vector<hsize_t> dims(1);
//     dims[0] = data.getNumEntries();
//     DataSpace memSpace(dims);

//     // Read the data.
//     fileSet.read(data, DataType::NATIVE_INT, memSpace, fileSpace);
// }

   }
}
