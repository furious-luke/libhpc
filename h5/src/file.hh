#ifndef hpc_h5_file_hh
#define hpc_h5_file_hh

#include "libhpc/containers/containers.hh"
#include "libhpc/hpcmpi/mpi.hh"
#include "location.hh"
#include "datatype.hh"
#include "dataspace.hh"
#include "dataset.hh"
#include "group.hh"

namespace mpl = boost::mpl;

namespace hpc {
   namespace h5 {

      class file
	 : public location
      {
      public:

         ///
         /// @param flags  Can be H5F_ACC_EXCL, H5F_ACC_TRUNC, H5F_ACC_RDONLY
         ///               or H5F_ACC_RDWR.
         ///
	 file();

	 file( const std::string& filename,
	       unsigned int flags,
	       const mpi::comm& comm=mpi::comm::self );

	 ~file();

	 void
	 open( const std::string& filename,
	       unsigned int flags,
	       const mpi::comm& comm=mpi::comm::self );

	 void
	 close();

	 template< class T >
	 void
	 write( const std::string& name,
		const T& value )
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<h5::datatype::type_map, T>));
	    h5::datatype datatype(mpl::at<h5::datatype::type_map, T>::type::value);

	    vector<hsize_t> dims(1);
	    dims[0] = 1;

	    h5::dataspace file_space(dims);
	    h5::dataset file_set;
	    file_set.create(*this, name, datatype, file_space);
	    file_space.select_all();

	    h5::dataspace mem_space(dims);
	    mem_space.select_all();

	    file_set.write(&value, datatype, mem_space, file_space, *this->_comm);
	 }

	 template< class T >
	 void
	 write( const std::string& name,
		const typename vector<T>::view& data,
		boost::optional<const vector<hsize_t>::view&> chunk_size=boost::optional<const vector<hsize_t>::view&>(),
		bool deflate=false )
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<h5::datatype::type_map, T>));
	    h5::datatype datatype(mpl::at<h5::datatype::type_map, T>::type::value);

	    vector<hsize_t> dims(1), count(1), offset(1);
	    dims[0] = this->_comm->all_reduce(data.size(), MPI_SUM);
	    count[0] = data.size();
	    offset[0] = this->_comm->scan(data.size(), MPI_SUM, true);

	    h5::dataspace file_space(dims);
	    h5::dataset file_set;
	    file_set.create(*this, name, datatype, file_space, chunk_size, deflate);
	    file_space.select_all();
	    file_space.select_hyperslab(H5S_SELECT_SET, count, offset);

	    dims[0] = data.size();
	    h5::dataspace mem_space(dims);
	    mem_space.select_all();

	    file_set.write(data, datatype, mem_space, file_space, *this->_comm);
	 }

	 template< class T >
	 void
	 write( const std::string& name,
		const csr<T>& data,
		boost::optional<const vector<hsize_t>::view&> chunk_size=boost::optional<const vector<hsize_t>::view&>(),
		bool deflate=false )
	 {
	    vector<index>::view displs = ((csr<T>&)data).mod_displs();
	    hpc::displs_to_counts(displs.begin(), displs.size());
	    vector<index>::view counts;
	    if(!displs.empty())
	       counts.setup(displs, displs.size() - 1);
	    this->write<index>(name + "_counts", counts, chunk_size, deflate);
	    if(!displs.empty())
	       hpc::counts_to_displs(displs.begin(), displs.size() - 1);

	    // Collect displacement offsets from all ranks involved.
	    index rank_offs = this->_comm->scan(displs.back(), MPI_SUM, true);
	    vector<index>::view offs_displs;
	    if(!displs.empty())
	       offs_displs.setup(displs, displs.size() - 1);
	    std::transform(offs_displs.begin(), offs_displs.end(), offs_displs.begin(), std::bind1st(std::plus<index>(), rank_offs));
	    this->write<index>(name + "_displs", offs_displs, chunk_size, deflate);
	    std::transform(offs_displs.begin(), offs_displs.end(), offs_displs.begin(), std::bind1st(std::minus<index>(), rank_offs));

	    this->write<T>(name + "_array", data.array(), chunk_size, deflate);
	 }

	 void
	 read_data_dims( const std::string& name,
			 vector<hsize_t>& dims );

	 hsize_t
	 read_data_size( const std::string& name );

	 template< class T >
	 T
	 read( const std::string& name )
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<h5::datatype::type_map, T>));
	    h5::datatype datatype(mpl::at<h5::datatype::type_map, T>::type::value);

	    vector<hsize_t> dims(1);
	    dims[0] = 1;

	    h5::dataset file_set(*this, name);
	    h5::dataspace file_space;
	    file_set.space(file_space);
	    file_space.select_all();

	    h5::dataspace mem_space(dims);
	    mem_space.select_all();

	    T value;
	    file_set.read(&value, datatype, mem_space, file_space, *this->_comm);
	    return value;
	 }

	 // template< class T >
	 // T
	 // read_element( const std::string& name,
	 // 	       int elem,
	 // 	       const mpi::comm& comm=mpi::comm::self );

	 template< class T >
	 void
	 read( const std::string& name,
	       typename vector<T>::view data )
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<h5::datatype::type_map, T>));
	    h5::datatype datatype(mpl::at<h5::datatype::type_map, T>::type::value);

	    vector<hsize_t> dims(1), count(1), offset(1);
	    dims[0] = this->_comm->all_reduce(data.size(), MPI_SUM);
	    count[0] = data.size();
	    offset[0] = this->_comm->scan(data.size(), MPI_SUM, true);

	    h5::dataset file_set;
	    file_set.open(*this, name);
	    h5::dataspace file_space;
	    file_set.space(file_space);
	    file_space.select_all();
	    file_space.select_hyperslab(H5S_SELECT_SET, count, offset);

	    dims[0] = data.size();
	    h5::dataspace mem_space(dims);
	    mem_space.select_all();

	    file_set.read(data, datatype, mem_space, file_space, *this->_comm);
	 }

	 template< class T >
	 void
	 read( const std::string& name,
	       typename vector<T>::view data,
	       const vector<hsize_t>::view& elements )
	 {
	    ASSERT(data.size() == elements.size());

	    BOOST_MPL_ASSERT((mpl::has_key<h5::datatype::type_map, T>));
	    h5::datatype datatype(mpl::at<h5::datatype::type_map, T>::type::value);

	    h5::dataset file_set;
	    file_set.open(*this, name);
	    h5::dataspace file_space;
	    file_set.space(file_space);
	    file_space.select_elements(elements);

	    vector<hsize_t> dims(1);
	    dims[0] = data.size();
	    h5::dataspace mem_space(dims);
	    mem_space.select_all();

	    file_set.read(data, datatype, mem_space, file_space, *this->_comm);
	 }

	 template< class T >
	 void
	 read( const std::string& name,
	       typename vector<T>::view data,
	       hsize_t offset )
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<h5::datatype::type_map, T>));
	    h5::datatype datatype(mpl::at<h5::datatype::type_map, T>::type::value);

	    vector<hsize_t> dims(1), count(1), _offset(1);
	    dims[0] = this->_comm->all_reduce(data.size(), MPI_SUM);
	    count[0] = data.size();
	    _offset[0] = offset;

	    h5::dataset file_set;
	    file_set.open(*this, name);
	    h5::dataspace file_space;
	    file_set.space(file_space);
	    file_space.select_all();
	    file_space.select_hyperslab(H5S_SELECT_SET, count, _offset);

	    dims[0] = data.size();
	    h5::dataspace mem_space(dims);
	    mem_space.select_all();

	    file_set.read(data, datatype, mem_space, file_space, *this->_comm);
	 }

	 // template< class T >
	 // void
	 // reada( const std::string& name,
	 // 	vector<T>& data,
	 // 	const mpi::comm& comm=mpi::comm::self );

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

	 template< class T >
	 void
	 read( const std::string& name,
	       csr<T>& data )
	 {
	    this->read<index>(name + "_counts", data.counts());
	    data.setup_array(true);

	    this->read<T>(name + "_array", data.mod_array());
	 }

	 /// Number of rows must be set to indicate which chunks to read.
	 template< class T >
	 void
	 read( const std::string& name,
	       csr<T>& data,
	       const vector<hsize_t>::view& elements )
	 {
	    this->read<index>(name + "_counts", data.counts(), elements);
	    data.setup_array(true);

	    vector<index> displs(data.num_rows());
	    this->read<index>(name + "_displs", displs, elements);

	    // TODO: Try and handle the CSR one row at a time. Not sure if this is a very good idea or not.
	    // I may need to set this up to handle a bunch of rows at a time.
	    std::string displs_name = name + "_displs";
	    for(hpc::index ii = 0; ii < data.num_rows(); ++ii)
	       this->read<T>(name + "_array", data[ii], displs[ii]);
	 }

	 // template< class T >
	 // void
	 // reada( const std::string& name,
	 // 	csr<T>& data,
	 // 	const vector<hsize_t>::view& rows,
	 // 	const mpi::comm& comm=mpi::comm::self );

      protected:

	 shared_ptr<mpi::comm> _comm;
      };
   }
}

#endif
