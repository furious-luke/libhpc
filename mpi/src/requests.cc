#include "requests.hh"
#include "insist.hh"

namespace hpc {
   namespace mpi {

      requests::requests( index size )
	 : _reqs( size )
      {
         boost::fill( this->_reqs, MPI_REQUEST_NULL );
      }

      requests::~requests() {
      }

      void
      requests::clear()
      {
	 this->_reqs.clear();
      }

      bool
      requests::empty() const
      {
	 this->_reqs.empty();
      }

      void
      requests::reserve( index size )
      {
	 this->_reqs.reserve(size);
      }

      void
      requests::resize( index size )
      {
	 this->_reqs.resize(size);
      }

      vector<request>::size_type
      requests::size() const
      {
	 this->_reqs.size();
      }

      request&
      requests::append()
      {
	 this->_reqs.resize(this->_reqs.size() + 1);
	 return this->_reqs.back();
      }

      void
      requests::wait_all()
      {
	 if(this->_reqs.size())
	    MPI_INSIST(MPI_Waitall(this->_reqs.size(), (MPI_Request*)this->_reqs.data(), MPI_STATUSES_IGNORE));
      }

      bool
      requests::test_all()
      {
	 if(this->_reqs.size()) {
            int flag;
	    MPI_INSIST( MPI_Testall( this->_reqs.size(), (MPI_Request*)this->_reqs.data(), &flag, MPI_STATUSES_IGNORE ) );
            return flag;
         }
         return true;

	 // for(int ii = 0; ii < this->_reqs.size(); ++ii) {
	 //    int flag;

         //    // Note that we cannot use mod_mpi_request here, because it requires that
         //    // the communication be complete.
	 //    MPI_INSIST(MPI_Test(&this->_reqs[ii]._req, &flag, MPI_STATUS_IGNORE));
	 //    if(flag) {
	 //       this->_reqs.erase(this->_reqs.begin() + ii);
	 //       --ii;
	 //    }
	 // }
      }

      request&
      requests::operator[](int idx)
      {
	 return this->_reqs[idx];
      }
   }
}
