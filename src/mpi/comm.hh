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

#ifndef hpc_mpi_comm_hh
#define hpc_mpi_comm_hh

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/move/move.hpp>
#include "libhpc/logging.hh"
#include "libhpc/system/view.hh"
#include "libhpc/algorithm/counts.hh"
#include "init.hh"
#include "type_map.hh"
#include "datatype.hh"
#include "request.hh"
#include "insist.hh"
#include "comm_proxies.hh"

namespace hpc {
   namespace mpi {

      class comm
      {
         BOOST_COPYABLE_AND_MOVABLE( comm );

      public:

	 static mpi::comm const null;
	 static mpi::comm const self;
	 static mpi::comm const world;

	 comm( MPI_Comm comm = MPI_COMM_NULL );

	 comm( comm const& src );

         inline
         comm( BOOST_RV_REF( comm ) src )
            : _comm( src._comm )
         {
            if( src._comm != MPI_COMM_WORLD &&
                src._comm != MPI_COMM_NULL &&
                src._comm != MPI_COMM_SELF )
	    {
	       src._comm = MPI_COMM_NULL;
	    }
         }

	 ~comm();

         inline
	 comm&
	 operator=( BOOST_COPY_ASSIGN_REF( comm ) src )
         {
            clear();
            if( src._comm != MPI_COMM_WORLD &&
                src._comm != MPI_COMM_NULL &&
                src._comm != MPI_COMM_SELF )
            {
               MPI_INSIST( MPI_Comm_dup( src._comm, &_comm ) );
            }
            else
               _comm = src._comm;
         }

         inline
	 comm&
	 operator=( BOOST_RV_REF( comm ) src )
         {
            clear();
            _comm = src._comm;
            if( src._comm != MPI_COMM_WORLD &&
                src._comm != MPI_COMM_NULL &&
                src._comm != MPI_COMM_SELF )
	    {
	       src._comm = MPI_COMM_NULL;
	    }
            return *this;
         }

	 void
	 clear();

	 void
	 mpi_comm( MPI_Comm comm );

	 MPI_Comm const&
	 mpi_comm() const;

	 int
	 rank() const;

	 int
	 size() const;

         int
         translate_rank( int rank,
                         mpi::comm const& other ) const;

         mpi::comm
         create_excl( int rank ) const;

	 void
	 create_range_incl( int first,
			    int last,
			    mpi::comm& new_comm );

	 mpi::comm
         split( int color,
                int key = 0 ) const;

	 void
	 send( const void* out,
	       const datatype& type,
	       int to,
	       int count=1,
	       int tag=0 ) const;

	 void
	 isend( const void* out,
		const datatype& type,
		int to,
		request& req,
		int count=1,
		int tag=0 ) const;

	 void
	 issend(const void* out,
		const datatype& type,
		int to,
		request& req,
		int count=1,
		int tag=0) const;

	 template< class T >
	 void
	 send( view<std::vector<T> > const& out,
	       int to,
               unsigned block_size = 1,
	       int tag = 0 ) const
	 {
	    MPI_INSIST( MPI_Send( out.data(),
                                  out.size()*block_size,
                                  MPI_MAP_TYPE( T ),
                                  to,
                                  tag,
                                  _comm ) );
	 }

	 template< class T >
	 void
	 isend( view<std::vector<T> > const& out,
		int to,
		request& req,
                unsigned block_size = 1,
		int tag = 0 ) const
	 {
	    MPI_INSIST( MPI_Isend( out.data(),
                                   out.size()*block_size,
                                   MPI_MAP_TYPE( T ),
                                   to,
                                   tag,
                                   _comm,
                                   &req.mod_mpi_request() ) );
	 }

	 // template< class T >
	 // void
	 // issend( const typename vector<T>::view& out,
	 //         int to,
	 //         request& req,
	 //         int tag=0 ) const
	 // {
	 //    MPI_INSIST(MPI_Issend(
	 //        	  out.data(),
	 //        	  out.size(),
         //                  MPI_MAP_TYPE(T),
	 //        	  to,
	 //        	  tag,
	 //        	  this->_comm,
	 //        	  &req.mod_mpi_request()
	 //        	  ));
	 // }

	 template< class T >
	 void
	 send( T const& out,
	       int to,
	       int tag = 0 ) const
	 {
	    MPI_INSIST( MPI_Send( (void*)&out, MPI_MAP_TYPE_SIZE( T ), MPI_MAP_TYPE( T ),
                                  to, tag,
                                  _comm ) );
	 }

	 template< class T >
	 void
	 isend( const T& out,
		int      to,
		request& req,
		int      tag = 0 ) const
	 {
	    MPI_INSIST( MPI_Isend( (void*)&out, MPI_MAP_TYPE_SIZE( T ), MPI_MAP_TYPE(T),
                                   to, tag,
                                   this->_comm, &req.mod_mpi_request() ) );
	 }

	 template< class T >
	 void
	 issend( const T& out,
		 int to,
		 request& req,
		 int tag=0 ) const
	 {
	    MPI_INSIST(MPI_Issend(
			  (void*)&out,
			  1,
                          MPI_MAP_TYPE(T),
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
	    MPI_INSIST(MPI_Send(
			  (void*)&out,
			  2,
                          MPI_MAP_TYPE(T),
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
	    MPI_INSIST(MPI_Isend(
			  (void*)&out,
                          2,
                          MPI_MAP_TYPE(T),
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
	    MPI_INSIST( MPI_Issend(
                           (void*)&out,
                           2,
                           MPI_MAP_TYPE(T),
                           to,
                           tag,
                           this->_comm,
                           &req.mod_mpi_request()
                           ) );
	 }

	 void
	 recv(void* inc,
	      const datatype& type,
	      int from,
	      int count=1,
	      int tag=0) const;

	 void
	 irecv(void* inc,
	       const datatype& type,
	       int from,
	       request& req,
	       int count=1,
	       int tag=0) const;

	 // template< class T >
	 // void
	 // recv( typename vector<T>::view inc,
	 //       int from,
	 //       int tag=0 ) const
	 // {
	 //    MPI_INSIST(MPI_Recv(
	 //        	  inc.data(),
	 //        	  inc.size(),
         //                  MPI_MAP_TYPE(T),
	 //        	  from,
	 //        	  tag,
	 //        	  this->_comm,
	 //        	  MPI_STATUS_IGNORE
	 //        	  ));
	 // }

	 template< class T >
	 void
	 irecv( view<std::vector<T> > inc,
		int from,
		request& req,
                unsigned block_size = 1,
		int tag = 0 ) const
	 {
	    MPI_INSIST( MPI_Irecv( inc.data(),
                                   inc.size(),
                                   MPI_MAP_TYPE( T ),
                                   from,
                                   tag,
                                   _comm,
                                   &req.mod_mpi_request() ) );
	 }

	 template< class T >
	 void
	 recv( T& value,
	       int from,
	       int tag = 0 ) const
	 {
	    MPI_INSIST( MPI_Recv( (void*)&value, MPI_MAP_TYPE_SIZE( T ), MPI_MAP_TYPE(T),
                                  from, tag,
                                  _comm, MPI_STATUS_IGNORE ) );
	 }

	 template< class T >
         T
	 recv( MPI_Status const& stat ) const
	 {
            T val;
	    MPI_INSIST( MPI_Recv( (void*)&val, 1, MPI_MAP_TYPE( T ),
                                  stat.MPI_SOURCE, stat.MPI_TAG,
                                  _comm, MPI_STATUS_IGNORE ) );
            return val;
	 }

	 template< class T >
	 void
	 irecv( T& value,
		int from,
		request& req,
		int tag=0 ) const
	 {
	    MPI_INSIST(MPI_Irecv(
			  (void*)&value,
			  1,
                          MPI_MAP_TYPE(T),
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
	    MPI_INSIST( MPI_Recv(
			  (void*)&value,
			  2,
                          MPI_MAP_TYPE(T),
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
	    MPI_INSIST( MPI_Irecv(
			  (void*)&value,
                          2,
                          MPI_MAP_TYPE(T),
			  from,
			  tag,
			  this->_comm,
			  &req.mod_mpi_request()
			  ) );
	 }

	 void
	 sendrecv( void* out_data,
		   int out_cnt,
		   mpi::datatype const& out_type,
		   int dst,
		   int out_tag,
		   void* inc_data,
		   int inc_cnt,
		   mpi::datatype const& inc_type,
		   int src,
		   int inc_tag ) const;

	 void
	 exchange( void* out_data,
		   int out_cnt,
		   mpi::datatype const& out_type,
		   void* inc_data,
		   int inc_cnt,
		   mpi::datatype const& inc_type,
		   int partner,
		   int tag = 0 ) const;

	 void
	 bcast(void* data,
	       const datatype& type,
	       int root,
	       int count=1) const;

	 template< class T >
	 void
	 bcast( T& value,
		int root,
                typename boost::disable_if<boost::is_class<T>,int>::type = 0 ) const
	 {
	    MPI_INSIST(MPI_Bcast(
			  &value,
			  1,
                          MPI_MAP_TYPE(T),
			  root,
			  this->_comm
			  ));
	 }

	 template< class T >
         T
	 bcast2( T const& value,
                 int root = 0 ) const
	 {
            if( rank() == root )
            {
               MPI_INSIST( MPI_Bcast( (void*)&value,
                                      1,
                                      MPI_MAP_TYPE( T ),
                                      root,
                                      _comm ) );
               return value;
            }
            else
            {
               T res;
               MPI_INSIST( MPI_Bcast( &res,
                                      1,
                                      MPI_MAP_TYPE( T ),
                                      root,
                                      _comm ) );
               return res;
            }
	 }

// 	 template< class T >
// 	 void
// 	 bcast( typename vector<T>::view data,
// 		int root ) const
// 	 {
// #ifndef NDEBUG
// 	    // Must have enougn space to recieve.
// 	    typename vector<T>::size_type size = data.size();
// 	    this->bcast(size, root);
// 	    ASSERT(data.size() == size, "Not all broadcast arrays share the same size.");
// #endif
// 	    MPI_INSIST(MPI_Bcast(
// 			  data.data(),
// 			  data.size(),
//                           MPI_MAP_TYPE(T),
// 			  root,
// 			  this->_comm
// 			  ));
// 	 }

	 template< class T >
         inline
         std::vector<T>
	 bcast( std::vector<T> const& data,
                int root = 0 ) const
         {
            return bcast<T>( hpc::view<std::vector<T> >( data ), root );
         }

	 template< class T >
         std::vector<T>
	 bcast( view<std::vector<T> > const& data,
                int root = 0 ) const
	 {
            typedef typename view<std::vector<T> >::size_type size_type;

            // Get size for resulting vector.
            size_type size = bcast2<size_type>( data.size(), root );

            if( rank() == root )
            {
               if( size )
               {
                  MPI_INSIST( MPI_Bcast( (void*)data.data(),
                                         size,
                                         MPI_MAP_TYPE( T ),
                                         root,
                                         _comm ) );
               }
               return std::vector<T>( data.begin(), data.end() );
            }
            else
            {
               std::vector<T> res( size );
               if( size )
               {
                  MPI_INSIST( MPI_Bcast( res.data(),
                                         size,
                                         MPI_MAP_TYPE( T ),
                                         root,
                                         _comm ) );
               }
               return res;
            }
	 }

	 // /// To be called from root (combined with below).
	 // template< class T >
	 // void
	 // bcasta( const typename vector<T>::view& data ) const
	 // {
	 //    int root = this->rank();
	 //    typename vector<T>::size_type size = data.size();
	 //    this->bcast(size, root);
	 //    MPI_INSIST(MPI_Bcast(
	 //        	  data.data(),
	 //        	  data.size(),
         //                  MPI_MAP_TYPE(T),
	 //        	  root,
	 //        	  this->_comm
	 //        	  ));
	 // }

	 template< class T >
	 void
	 bcasta_root( view<std::vector<T> > data ) const
	 {
	    int root = this->rank();
	    typename std::vector<T>::size_type size = data.size();
	    this->bcast( size, root );
	    MPI_INSIST( MPI_Bcast(
			   data.data(),
			   data.size(),
			   MPI_MAP_TYPE(T),
			   root,
			   this->_comm
			   ) );
	 }

	 // /// To be called from recievers (combined with above).
	 // template< class T >
	 // void
	 // bcasta( vector<T>& data,
	 //         int root = 0 ) const
	 // {
	 //    typename vector<T>::size_type size = data.size();
	 //    this->bcast( size, root );
	 //    data.resize( size );
	 //    MPI_INSIST( MPI_Bcast( data.data(),
         //                           data.size(),
         //                           MPI_MAP_TYPE( T ),
         //                           root,
         //                           this->_comm ) );
	 // }

	 template< class T >
	 void
	 bcasta( std::vector<T>& data,
		 int root = 0 ) const
	 {
	    typename std::vector<T>::size_type size = data.size();
	    this->bcast( size, root );
	    data.resize( size );
	    MPI_INSIST( MPI_Bcast( data.data(),
                                   data.size(),
                                   MPI_MAP_TYPE( T ),
                                   root,
                                   this->_comm ) );
	 }

	 void
	 bcast( std::string& str,
		int root=0 ) const;

	 void
	 barrier() const;

         template< class T,
		   typename boost::disable_if<boost::is_class<T>,int>::type = 0 >
	 void
         all_gather( T const& data,
		     std::vector<T>& out ) const
         {
            typedef typename std::vector<T>::size_type size_type;
	    ASSERT( out.size() == size(), "Allgather buffer wrong size." );
	    MPI_INSIST( MPI_Allgather( (void*)&data, 1, MPI_MAP_TYPE( T ),
                                       out.data(),   1, MPI_MAP_TYPE( T ),
                                       _comm ) );
         }

         template< class T,
		   typename boost::disable_if<boost::is_class<T>,int>::type = 0 >
         std::vector<T>
         all_gather( T const& data ) const
         {
            typedef typename std::vector<T>::size_type size_type;
            std::vector<T> res( size() );
	    all_gather( data, res );
            return res;
         }

         template< class T >
         std::vector<T>
         all_gather( std::vector<T> const& data ) const
         {
            typedef typename std::vector<T>::size_type size_type;

            ASSERT( data.size() == bcast2( data.size() ),
                    "all_gather must have the same data sizes." );

            // Get size of resulting vector.
            size_type size = all_reduce( data.size() );
            std::vector<T> res( size );

            // Call MPI routine.
	    MPI_INSIST( MPI_Allgather( (void*)data.data(),
                                       data.size(),
                                       MPI_MAP_TYPE( T ),
                                       res.data(),
                                       size,
                                       MPI_MAP_TYPE( T ),
                                       _comm ) );

            return res;
         }

         template< class T >
         std::vector<T>
         all_gatherv( std::vector<T> const& data ) const
         {
            typedef typename std::vector<T>::size_type size_type;

            LOGBLOCKT( "all_gatherv" );

            // Get counts and displacements.
            int              size   = MPI_MAP_TYPE_SIZE( T );
            std::vector<int> cnts   = all_gather<int>( data.size() );
            std::vector<int> displs = hpc::counts_to_displs( cnts );

            // Resize vector based on number of elements.
            std::vector<T> res( displs[this->size()] );

            // Multiply displacements/counts to get result in blocks.
            if( size != 1 )
            {
               std::transform( cnts.begin(), cnts.end(), cnts.begin(),
                               std::bind1st( std::multiplies<int>(), size ) );
               std::transform( displs.begin(), displs.end(), displs.begin(),
                               std::bind1st( std::multiplies<int>(), size ) );
            }

            // Get final size.
            size *= data.size();

            LOGTLN( "Counts: ", cnts );
            LOGTLN( "Displs: ", displs );

            // Call MPI routine.
	    MPI_INSIST( MPI_Allgatherv( (void*)data.data(), size, MPI_MAP_TYPE( T ),
                                        res.data(), cnts.data(), displs.data(), MPI_MAP_TYPE( T ),
                                        _comm ) );

            return res;
         }

	 // template< class T >
	 // void
	 // all_gather( const T& value,
	 //             typename vector<T>::view inc ) const
	 // {
	 //    ASSERT(inc.size() == this->size());
	 //    MPI_INSIST(MPI_Allgather(
	 //        	  (void*)&value,
	 //        	  1,
         //                  MPI_MAP_TYPE(T),
	 //        	  inc.data(),
	 //        	  1,
         //                  MPI_MAP_TYPE(T),
	 //        	  this->_comm
	 //        	  ));
	 // }

	 // template< class T >
	 // void
	 // all_gather( const typename vector<T>::view& out,
         //             fibre<T>& inc ) const
	 // {
         //    ASSERT( out.size() == inc.fibre_size() );
	 //    ASSERT( inc.size() == this->size() );
	 //    MPI_INSIST( MPI_Allgather(
         //                   (void*)out.data(),
         //                   out.size(),
         //                   MPI_MAP_TYPE(T),
         //                   inc.data(),
         //                   inc.fibre_size(),
         //                   MPI_MAP_TYPE(T),
         //                   this->_comm
         //                   ) );
	 // }

	 // template< class T >
	 // void
	 // all_to_all( const typename vector<T>::view& out,
         //             typename vector<T>::view inc ) const
	 // {
	 //    ASSERT( out.size() == this->size() );
	 //    ASSERT( inc.size() == this->size() );
	 //    MPI_INSIST( MPI_Alltoall(
         //                   (void*)out.data(),
         //                   1,
         //                   MPI_MAP_TYPE(T),
         //                   inc.data(),
         //                   1,
         //                   MPI_MAP_TYPE(T),
         //                   this->_comm
         //                   ) );
	 // }

	 // template< class T >
	 // void
	 // reduce( typename vector<T>::view data,
	 //         MPI_Op op,
	 //         int root ) const
	 // {
	 // }

	 template< class T >
	 T
	 all_reduce( T const& out,
		     MPI_Op op = MPI_SUM ) const
	 {
	    T inc;
	    MPI_INSIST( MPI_Allreduce( (void*)&out,
                                       &inc,
                                       1,
                                       MPI_MAP_TYPE( T ),
                                       op,
                                       _comm ) );
	    return inc;
	 }

	 // /// Reduce an array of values.
	 // template< class T >
	 // void
	 // all_reduce( const typename vector<T>::view out,
	 //             typename vector<T>::view inc,
	 //             MPI_Op op=MPI_SUM ) const
	 // {
	 //    ASSERT( out.size() == inc.size() );
	 //    MPI_INSIST( MPI_Allreduce(
	 //        	  (void*)out.data(),
	 //        	  (void*)inc.data(),
	 //        	  out.size(),
         //                  MPI_MAP_TYPE(T),
	 //        	  op,
	 //        	  this->_comm
	 //        	  ) );
	 // }

	 /// Reduce an array of values.
	 template< class T >
	 void
	 all_reduce( view<std::vector<T> > buf,
		     MPI_Op op = MPI_SUM ) const
	 {
	    std::vector<T> inc( buf.size() );
	    MPI_INSIST( MPI_Allreduce( (void*)buf.data(),
                                       (void*)inc.data(),
                                       buf.size(),
                                       MPI_MAP_TYPE( T ),
                                       op,
                                       _comm ) );
	    std::copy( inc.begin(), inc.end(), buf.begin() );
	 }

         void
	 probe( MPI_Status& stat,
		int from = MPI_ANY_SOURCE,
		int tag = MPI_ANY_TAG ) const;

	 bool
	 iprobe(MPI_Status& stat,
		int from=MPI_ANY_SOURCE,
		int tag=MPI_ANY_TAG) const;

	 bool
	 iprobe(int from=MPI_ANY_SOURCE,
		int tag=MPI_ANY_TAG) const;

	 template< class T,
		   typename boost::enable_if<hpc::is_fundamental_r<T>,int>::type = 0 >
	 typename type_traits<T>::value
	 _scan( typename type_traits<T>::const_reference out,
		MPI_Op op = MPI_SUM,
		bool exclusive = true ) const
	 {
	    typename type_traits<T>::value inc;
	    MPI_INSIST( MPI_Scan( (void*)&out, &inc, 1,
                                  MPI_MAP_TYPE( typename type_traits<T>::value ), op, _comm ) );
	    if( exclusive )
	       inc -= out;
	    return inc;
	 }

	 template< class T,
		   typename boost::disable_if<hpc::is_fundamental_r<T>,int>::type = 0 >
	 std::vector<typename type_traits<T>::value::value_type>
	 _scan( typename type_traits<T>::const_reference out,
		MPI_Op op = MPI_SUM,
		bool exclusive = true ) const
	 {
	    std::vector<typename type_traits<T>::value::value_type> inc( out.size() );
	    MPI_INSIST( MPI_Scan( (void*)out.data(), inc.data(), out.size(),
                                  MPI_MAP_TYPE( typename type_traits<T>::value::value_type ),
				  op, _comm ) );
	    if( exclusive )
	    {
	       auto inc_it = inc.begin();
	       auto out_it = out.cbegin();
	       for( ; inc_it != inc.end(); ++inc_it, ++out_it )
		  *inc_it -= *out_it;
	    }
	    return inc;
	 }

	 template< class T >
	 typename scan_proxy<T>::result_type
	 scan( T&& out,
	       MPI_Op op = MPI_SUM,
	       bool exclusive = true ) const
	 {
	    return _scan<T>( std::forward<T>( out ), op, exclusive );
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

	 template< class OutBufT,
		   class IncBufT >
	 void
	 alltoall( OutBufT const& out,
		   IncBufT&& inc ) const
	 {
	    _alltoall<OutBufT,IncBufT>( out, std::forward<IncBufT>( inc ) );
	 }

	 template< class OutBufT,
		   class OutCntsT,
		   class OutDisplsT,
		   class IncBufT,
		   class IncCntsT,
		   class IncDisplsT >
	 void
	 alltoallv( OutBufT const& out_buf,
		    OutCntsT const& out_cnts,
		    OutDisplsT const& out_displs,
		    IncBufT&& inc_buf,
		    IncCntsT&& inc_cnts,
		    IncDisplsT&& inc_displs ) const
	 {
	    _alltoallv<OutBufT,OutCntsT,OutDisplsT,IncBufT,IncCntsT,IncDisplsT>(
	       out_buf, out_cnts, out_displs,
	       std::forward<IncBufT>( inc_buf ),
	       std::forward<IncCntsT>( inc_cnts ),
	       std::forward<IncDisplsT>( inc_displs )
	       );
	 }

	 void
	 alltoallw( void const* out_data,
		    int const* out_cnts,
		    int const* out_displs,
		    datatype const* out_types,
		    void* inc_data,
		    int const* inc_cnts,
		    int const* inc_displs,
		    datatype const* inc_types ) const;

	 void
	 abort( int ec = 1 ) const;

	 bool
	 operator==(const comm& comm) const;

	 bool
	 operator==(MPI_Comm comm) const;

	 bool
	 operator!=(const comm& comm) const;

	 bool
	 operator!=(MPI_Comm comm) const;

      protected:

	 template< class OutBufT,
		   class IncBufT >
	 void
	 _alltoall( OutBufT const& out,
		    typename type_traits<IncBufT>::reference inc ) const
	 {
	    typedef typename OutBufT::value_type out_type;
	    typedef typename type_traits<IncBufT>::value::value_type inc_type;
	    MPI_INSIST( MPI_Alltoall( (void*)out.data(), 1, MPI_MAP_TYPE( out_type ),
				      (void*)inc.data(), 1, MPI_MAP_TYPE( inc_type ),
				      _comm ) );
	 }

	 template< class OutBufT,
		   class OutCntsT,
		   class OutDisplsT,
		   class IncBufT,
		   class IncCntsT,
		   class IncDisplsT >
	 void
	 _alltoallv( OutBufT const& out_buf,
		     OutCntsT const& out_cnts,
		     OutDisplsT const& out_displs,
		     typename type_traits<IncBufT>::reference inc_buf,
		     typename type_traits<IncCntsT>::reference inc_cnts,
		     typename type_traits<IncDisplsT>::reference inc_displs ) const
	 {
	    typedef typename OutBufT::value_type out_type;
	    typedef typename type_traits<IncBufT>::value::value_type inc_type;
	    MPI_INSIST(
	       MPI_Alltoallv( (void*)out_buf.data(), (int*)out_cnts.data(), (int*)out_displs.data(), MPI_MAP_TYPE( out_type ),
			      (void*)inc_buf.data(), (int*)inc_cnts.data(), (int*)inc_displs.data(), MPI_MAP_TYPE( inc_type ),
			      _comm )
	       );
	 }

      protected:

	 MPI_Comm _comm;
      };

      std::ostream&
      operator<<( std::ostream& strm,
                  mpi::comm const& obj );

   }
}

#endif
