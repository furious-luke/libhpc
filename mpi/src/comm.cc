#include <boost/range/algorithm.hpp>
#include "comm.hh"
#include "insist.hh"

namespace hpc {
   namespace mpi {

      mpi::comm comm::null(MPI_COMM_NULL);
      mpi::comm comm::self(MPI_COMM_SELF);
      mpi::comm comm::world(MPI_COMM_WORLD);

      comm::comm(MPI_Comm comm)
	 : _comm(comm)
      {
      }

      comm::comm(const comm& comm)
      {
	 if(comm._comm != MPI_COMM_NULL)
	    MPI_INSIST(MPI_Comm_dup(comm._comm, &this->_comm));
	 else
	    this->_comm = MPI_COMM_NULL;
      }

      comm::~comm()
      {
	 this->clear();
      }

      void
      comm::clear()
      {
	 if( this->_comm != MPI_COMM_WORLD &&
             this->_comm != MPI_COMM_NULL &&
             this->_comm != MPI_COMM_SELF )
	 {
	    MPI_INSIST( MPI_Comm_free( &this->_comm ) );
	 }
	 this->_comm = MPI_COMM_NULL;
      }

      void
      comm::mpi_comm( MPI_Comm comm )
      {
	 this->clear();
	 this->_comm = comm;
      }

      const MPI_Comm
      comm::mpi_comm() const
      {
	 return this->_comm;
      }

      int
      comm::rank() const
      {
	 int rank;
	 MPI_INSIST(MPI_Comm_rank(this->_comm, &rank));
	 return rank;
      }

      int
      comm::size() const
      {
	 int size;
	 MPI_INSIST( MPI_Comm_size( this->_comm, &size ) );
	 return size;
      }

      int
      comm::translate_rank( int rank, const mpi::comm& other ) const
      {
	 MPI_Group my_group, other_group;
	 MPI_INSIST( MPI_Comm_group( this->_comm, &my_group ) );
	 MPI_INSIST( MPI_Comm_group( other._comm, &other_group ) );
         int other_rank;
         MPI_INSIST( MPI_Group_translate_ranks( my_group, 1, &rank, other_group, &other_rank ) );
         MPI_INSIST( MPI_Group_free( &my_group ) );
         MPI_INSIST( MPI_Group_free( &other_group ) );
         return other_rank;
      }

      void
      comm::create_excl( int rank,
                         mpi::comm& new_comm )
      {
	 MPI_Group group, new_group;
	 MPI_INSIST( MPI_Comm_group( this->_comm, &group ) );
	 MPI_INSIST( MPI_Group_excl( group, 1, &rank, &new_group ) );
	 MPI_Comm _new_comm;
	 MPI_INSIST( MPI_Comm_create( this->_comm, new_group, &_new_comm ) );
	 if( _new_comm != MPI_COMM_NULL )
	    new_comm.mpi_comm( _new_comm );
      }

      void
      comm::create_range_incl( int first,
			       int last,
			       mpi::comm& new_comm )
      {
	 MPI_Group group, new_group;
	 MPI_INSIST( MPI_Comm_group( this->_comm, &group ) );
	 int range[3] = { first, last, 1 };
	 MPI_INSIST( MPI_Group_range_incl( group, 1, (int(*)[3])range, &new_group ) );

         // I can free the original comm's group handle now.
         MPI_INSIST( MPI_Group_free( &group ) );

	 MPI_Comm _new_comm;
	 MPI_INSIST( MPI_Comm_create( this->_comm, new_group, &_new_comm ) );

         // Free the new group handle.
         MPI_INSIST( MPI_Group_free( &new_group ) );

	 if( _new_comm != MPI_COMM_NULL )
	    new_comm.mpi_comm( _new_comm );
      }

      void
      comm::split( int color,
                   mpi::comm& new_comm,
                   int key )
      {
         MPI_Comm _new_comm;
         MPI_Comm_split(this->_comm, color, key, &_new_comm);
         new_comm.mpi_comm(_new_comm);
      }

      void
      comm::send(const void* out,
		 const data_type& type,
		 int to,
		 int count,
		 int tag) const
      {
	 ASSERT(count >= 0);
	 MPI_INSIST(MPI_Send((void*)out, count, type.mpi_data_type(), to, tag, this->_comm));
      }

      void
      comm::isend(const void* out,
		  const data_type& type,
		  int to,
		  request& req,
		  int count,
		  int tag) const
      {
	 ASSERT(count >= 0);
	 MPI_INSIST(MPI_Isend((void*)out, count, type.mpi_data_type(), to, tag, this->_comm, &req.mod_mpi_request()));
      }

      void
      comm::issend(const void* out,
		   const data_type& type,
		   int to,
		   request& req,
		   int count,
		   int tag) const
      {
	 ASSERT(count >= 0);
	 MPI_INSIST(MPI_Issend((void*)out, count, type.mpi_data_type(), to, tag, this->_comm, &req.mod_mpi_request()));
      }

      void
      comm::recv(void* inc,
		 const data_type& type,
		 int from,
		 int count,
		 int tag) const
      {
	 ASSERT(count >= 0);
	 MPI_INSIST(MPI_Recv(inc, count, type.mpi_data_type(), from, tag, this->_comm, MPI_STATUS_IGNORE));
      }

      void
      comm::irecv(void* inc,
		  const data_type& type,
		  int from,
		  request& req,
		  int count,
		  int tag) const
      {
	 ASSERT(count >= 0);
	 MPI_INSIST(MPI_Irecv(inc, count, type.mpi_data_type(), from, tag, this->_comm, &req.mod_mpi_request()));
      }

      void
      comm::bcast(void* data,
		  const data_type& type,
		  int root,
		  int count) const
      {
	 ASSERT(count >= 0);
	 MPI_INSIST(MPI_Bcast(data, count, type.mpi_data_type(), root, this->_comm));
      }

      void
      comm::bcast( std::string& str,
		   int root ) const
      {
	 int size = str.size();
	 this->bcast(size, root);
	 if(this->rank() != root)
	    str.resize(size);
	 MPI_INSIST(MPI_Bcast((void*)str.c_str(), size, MPI_CHAR, root, this->_comm));
      }

      void
      comm::barrier() const
      {
	 MPI_INSIST(MPI_Barrier(this->_comm));
      }

      template<>
      void
      comm::reduce<int>(vector<int>::view data,
			MPI_Op op,
			int root) const
      {
	 if(this->rank() == root) {
	    // MPICH2 won't allow the use of aliased buffers.
	    vector<int> inc_data(data.size());
	    MPI_INSIST(MPI_Reduce(data.data(), inc_data.data(), data.size(), MPI_INT, op, root, this->_comm));
	    boost::copy(inc_data, data.begin());
	 }
	 else {
	    MPI_INSIST(MPI_Reduce(data.data(), NULL, data.size(), MPI_INT, op, root, this->_comm));
	 }
      }

      bool
      comm::iprobe(MPI_Status& stat,
		   int from,
		   int tag) const
      {
	 int flag;
	 MPI_INSIST(MPI_Iprobe(from, tag, this->_comm, &flag, &stat));
	 return flag ? true : false;
      }

      bool
      comm::iprobe(int from,
		   int tag) const
      {
	 MPI_Status stat;
	 return this->iprobe(stat, from, tag);
      }

      bool
      comm::operator==(const comm& comm) const
      {
	 return this->_comm == comm._comm;
      }

      bool
      comm::operator==(MPI_Comm comm) const
      {
	 return this->_comm == comm;
      }

      bool
      comm::operator!=(const comm& comm) const
      {
	 return this->_comm != comm._comm;
      }

      bool
      comm::operator!=(MPI_Comm comm) const
      {
	 return this->_comm != comm;
      }
   }
}
