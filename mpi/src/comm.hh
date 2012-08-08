#ifndef mpi_comm_hh
#define mpi_comm_hh

#include "libhpc/containers/containers.hh"
#include "init.hh"
#include "data_type.hh"
#include "request.hh"
#include "insist.hh"

namespace mpl = boost::mpl;

namespace hpc {
   namespace mpi {

      /// Wrapper for MPI_Comm.
      class comm {
      public:

	 static mpi::comm null;
	 static mpi::comm self;
	 static mpi::comm world;

	 comm( MPI_Comm comm=MPI_COMM_NULL );

	 comm( const comm& comm );

	 ~comm();

	 void
	 clear();

	 void
	 mpi_comm( MPI_Comm comm );

	 const MPI_Comm
	 mpi_comm() const;

	 int
	 rank() const;

	 int
	 size() const;

         int
         translate_rank( int rank, const mpi::comm& other ) const;

         void
         create_excl( int rank,
                      mpi::comm& new_comm );

	 void
	 create_range_incl( int first,
			    int last,
			    mpi::comm& new_comm );

         void
         split( int color,
                mpi::comm& new_comm,
                int key=0 );

	 void
	 send( const void* out,
	       const data_type& type,
	       int to,
	       int count=1,
	       int tag=0 ) const;

	 void
	 isend( const void* out,
		const data_type& type,
		int to,
		request& req,
		int count=1,
		int tag=0 ) const;

	 void
	 issend(const void* out,
		const data_type& type,
		int to,
		request& req,
		int count=1,
		int tag=0) const;

	 template< class T >
	 void
	 send( const typename vector<T>::view& out,
	       int to,
	       int tag=0 ) const
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<mpi::data_type::type_map, T>));
	    MPI_INSIST(MPI_Send(
			  out.data(),
			  out.size(),
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  to,
			  tag,
			  this->_comm
			  ));
	 }

	 template< class T >
	 void
	 isend( const typename vector<T>::view& out,
		int to,
		request& req,
		int tag=0 ) const
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<mpi::data_type::type_map, T>));
	    MPI_INSIST(MPI_Isend(
			  out.data(),
			  out.size(),
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  to,
			  tag,
			  this->_comm,
			  &req.mod_mpi_request()
			  ));
	 }

	 template< class T >
	 void
	 issend( const typename vector<T>::view& out,
		 int to,
		 request& req,
		 int tag=0 ) const
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<mpi::data_type::type_map, T>));
	    MPI_INSIST(MPI_Issend(
			  out.data(),
			  out.size(),
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  to,
			  tag,
			  this->_comm,
			  &req.mod_mpi_request()
			  ));
	 }

	 template< class T >
	 void
	 send( const T& out,
	       int to,
	       int tag=0 ) const
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<mpi::data_type::type_map, T>));
	    MPI_INSIST(MPI_Send(
			  (void*)&out,
			  1,
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  to,
			  tag,
			  this->_comm
			  ));
	 }

	 template< class T >
	 void
	 isend( const T& out,
		int to,
		request& req,
		int tag=0 ) const
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<mpi::data_type::type_map, T>));
	    MPI_INSIST(MPI_Isend(
			  (void*)&out,
			  1,
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  to,
			  tag,
			  this->_comm,
			  &req.mod_mpi_request()
			  ));
	 }

	 template< class T >
	 void
	 issend( const T& out,
		 int to,
		 request& req,
		 int tag=0 ) const
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<mpi::data_type::type_map, T>));
	    MPI_INSIST(MPI_Issend(
			  (void*)&out,
			  1,
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  to,
			  tag,
			  this->_comm,
			  &req.mod_mpi_request()
			  ));
	 }

	 template< class T >
	 void
	 send( const std::pair<T, T>& out,
	       int to,
	       int tag=0 ) const
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<mpi::data_type::type_map, T>));
	    MPI_INSIST(MPI_Send(
			  (void*)&out,
			  2,
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  to,
			  tag,
			  this->_comm
			  ));
	 }

	 template< class T >
	 void
	 isend( const std::pair<T, T>& out,
		int to,
		request& req,
		int tag=0 ) const
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<mpi::data_type::type_map, T>));
	    MPI_INSIST(MPI_Isend(
			  (void*)&out,
                          2,
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  to,
			  tag,
			  this->_comm,
			  &req.mod_mpi_request()
			  ));
	 }

	 template< class T >
	 void
	 issend( const std::pair<T, T>& out,
                 int to,
                 request& req,
                 int tag=0 ) const
	 {
	    BOOST_MPL_ASSERT( (mpl::has_key<mpi::data_type::type_map, T>) );
	    MPI_INSIST( MPI_Issend(
                           (void*)&out,
                           2,
                           mpl::at<mpi::data_type::type_map, T>::type::value,
                           to,
                           tag,
                           this->_comm,
                           &req.mod_mpi_request()
                           ) );
	 }

	 void
	 recv(void* inc,
	      const data_type& type,
	      int from,
	      int count=1,
	      int tag=0) const;

	 void
	 irecv(void* inc,
	       const data_type& type,
	       int from,
	       request& req,
	       int count=1,
	       int tag=0) const;

	 template< class T >
	 void
	 recv( typename vector<T>::view inc,
	       int from,
	       int tag=0 ) const
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<mpi::data_type::type_map, T>));
	    MPI_INSIST(MPI_Recv(
			  inc.data(),
			  inc.size(),
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  from,
			  tag,
			  this->_comm,
			  MPI_STATUS_IGNORE
			  ));
	 }

	 template< class T >
	 void
	 irecv( typename vector<T>::view inc,
		int from,
		request& req,
		int tag=0 ) const
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<mpi::data_type::type_map, T>));
	    MPI_INSIST(MPI_Irecv(
			  inc.data(),
			  inc.size(),
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  from,
			  tag,
			  this->_comm,
			  &req.mod_mpi_request()
			  ));
	 }

	 template< class T >
	 void
	 recv( T& value,
	       int from,
	       int tag=0 ) const
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<mpi::data_type::type_map, T>));
	    MPI_INSIST(MPI_Recv(
			  (void*)&value,
			  1,
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  from,
			  tag,
			  this->_comm,
			  MPI_STATUS_IGNORE
			  ));
	 }

	 template< class T >
	 void
	 irecv( T& value,
		int from,
		request& req,
		int tag=0 ) const
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<mpi::data_type::type_map, T>));
	    MPI_INSIST(MPI_Irecv(
			  (void*)&value,
			  1,
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  from,
			  tag,
			  this->_comm,
			  &req.mod_mpi_request()
			  ));
	 }

	 template< class T >
	 void
	 recv( std::pair<T, T>& value,
	       int from,
	       int tag=0 ) const
	 {
	    BOOST_MPL_ASSERT( (mpl::has_key<mpi::data_type::type_map, T>) );
	    MPI_INSIST( MPI_Recv(
			  (void*)&value,
			  2,
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  from,
			  tag,
			  this->_comm,
			  MPI_STATUS_IGNORE
			  ) );
	 }

	 template< class T >
	 void
	 irecv( std::pair<T, T>& value,
		int from,
		request& req,
		int tag=0 ) const
	 {
	    BOOST_MPL_ASSERT( (mpl::has_key<mpi::data_type::type_map, T>) );
	    MPI_INSIST( MPI_Irecv(
			  (void*)&value,
                          2,
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  from,
			  tag,
			  this->_comm,
			  &req.mod_mpi_request()
			  ) );
	 }

	 void
	 bcast(void* data,
	       const data_type& type,
	       int root,
	       int count=1) const;

	 template< class T >
	 void
	 bcast( T& value,
		int root ) const
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<mpi::data_type::type_map, T>));
	    MPI_INSIST(MPI_Bcast(
			  &value,
			  1,
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  root,
			  this->_comm
			  ));
	 }

	 template< class T >
	 void
	 bcast( typename vector<T>::view data,
		int root ) const
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<mpi::data_type::type_map, T>));
#ifndef NDEBUG
	    // Must have enougn space to recieve.
	    typename vector<T>::size_type size = data.size();
	    this->bcast(size, root);
	    ASSERT(data.size() == size, "Not all broadcast arrays share the same size.");
#endif
	    MPI_INSIST(MPI_Bcast(
			  data.data(),
			  data.size(),
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  root,
			  this->_comm
			  ));
	 }

	 /// To be called from root (combined with below).
	 template< class T >
	 void
	 bcasta( const typename vector<T>::view& data ) const
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<mpi::data_type::type_map, T>));
	    int root = this->rank();
	    typename vector<T>::size_type size = data.size();
	    this->bcast(size, root);
	    MPI_INSIST(MPI_Bcast(
			  data.data(),
			  data.size(),
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  root,
			  this->_comm
			  ));
	 }

	 /// To be called from recievers (combined with above).
	 template< class T >
	 void
	 bcasta( vector<T>& data,
		 int root ) const
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<mpi::data_type::type_map, T>));
	    typename vector<T>::size_type size = data.size();
	    this->bcast(size, root);
	    data.resize(size);
	    MPI_INSIST(MPI_Bcast(
			  data.data(),
			  data.size(),
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  root,
			  this->_comm
			  ));
	 }

	 void
	 bcast( std::string& str,
		int root=0 ) const;

	 void
	 barrier() const;

	 template< class T >
	 void
	 all_gather( const T& value,
		     typename vector<T>::view inc ) const
	 {
	    BOOST_MPL_ASSERT((mpl::has_key<mpi::data_type::type_map, T>));
	    ASSERT(inc.size() == this->size());
	    MPI_INSIST(MPI_Allgather(
			  (void*)&value,
			  1,
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  inc.data(),
			  1,
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  this->_comm
			  ));
	 }

	 template< class T >
	 void
	 all_gather( const typename vector<T>::view& out,
                     fibre<T>& inc ) const
	 {
	    BOOST_MPL_ASSERT( (mpl::has_key<mpi::data_type::type_map, T>) );
            ASSERT( out.size() == inc.fibre_size() );
	    ASSERT( inc.size() == this->size() );
	    MPI_INSIST( MPI_Allgather(
                           (void*)out.data(),
                           out.size(),
                           mpl::at<mpi::data_type::type_map, T>::type::value,
                           inc.data(),
                           inc.fibre_size(),
                           mpl::at<mpi::data_type::type_map, T>::type::value,
                           this->_comm
                           ) );
	 }

	 template< class T >
	 void
	 all_to_all( const typename vector<T>::view& out,
                     typename vector<T>::view inc ) const
	 {
	    BOOST_MPL_ASSERT( (mpl::has_key<mpi::data_type::type_map, T>) );
	    ASSERT( out.size() == this->size() );
	    ASSERT( inc.size() == this->size() );
	    MPI_INSIST( MPI_Alltoall(
                           (void*)out.data(),
                           1,
                           mpl::at<mpi::data_type::type_map, T>::type::value,
                           inc.data(),
                           1,
                           mpl::at<mpi::data_type::type_map, T>::type::value,
                           this->_comm
                           ) );
	 }

	 template< class T >
	 void
	 reduce( typename vector<T>::view data,
		 MPI_Op op,
		 int root ) const
	 {
	 }

	 template< class T >
	 T
	 all_reduce( const T& out,
		     MPI_Op op=MPI_SUM ) const
	 {
	    BOOST_MPL_ASSERT( (mpl::has_key<mpi::data_type::type_map, T>) );
	    T inc;
	    MPI_INSIST( MPI_Allreduce(
			  (void*)&out,
			  &inc,
			  1,
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  op,
			  this->_comm
			  ) );
	    return inc;
	 }

	 /// Reduce an array of values.
	 template< class T >
	 void
	 all_reduce( const typename vector<T>::view out,
		     typename vector<T>::view inc,
		     MPI_Op op=MPI_SUM ) const
	 {
	    ASSERT( out.size() == inc.size() );
	    BOOST_MPL_ASSERT( (mpl::has_key<mpi::data_type::type_map, T>) );
	    MPI_INSIST( MPI_Allreduce(
			  (void*)out.data(),
			  (void*)inc.data(),
			  out.size(),
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  op,
			  this->_comm
			  ) );
	 }

	 bool
	 iprobe(MPI_Status& stat,
		int from=MPI_ANY_SOURCE,
		int tag=MPI_ANY_TAG) const;

	 bool
	 iprobe(int from=MPI_ANY_SOURCE,
		int tag=MPI_ANY_TAG) const;

	 template< class T >
	 T
	 scan( const T& out,
	       MPI_Op op = MPI_SUM,
	       bool exclusive=false ) const
	 {
	    BOOST_MPL_ASSERT( (mpl::has_key<mpi::data_type::type_map, T>) );
	    T inc;
	    MPI_INSIST( MPI_Scan(
			  (void*)&out,
			  &inc,
			  1,
			  mpl::at<mpi::data_type::type_map, T>::type::value,
			  op,
			  this->_comm
			  ) );
	    if( exclusive )
	       inc -= out;
	    return inc;
	 }

	 template<class T>
	 void
	 chain_send(const T& out,
		    int tag=0) const
	 {
	    int rank = this->rank();
	    if(rank < this->size() - 1)
	       this->send(out, rank + 1, tag);
	 }

	 template<class T>
	 T
	 chain_recv(int tag=0) const
	 {
	    int rank = this->rank();
	    T inc = 0;
	    if(rank > 0)
	       this->recv(inc, rank - 1, tag);
	    return inc;
	 }

	 bool
	 operator==(const comm& comm) const;

	 bool
	 operator==(MPI_Comm comm) const;

	 bool
	 operator!=(const comm& comm) const;

	 bool
	 operator!=(MPI_Comm comm) const;

      private:
	 MPI_Comm _comm;
      };
   }
}

#endif
