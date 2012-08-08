#include "request.hh"
#include "insist.hh"

namespace hpc {
   namespace mpi {

      // void request::waitAll(VectorView<Request>& reqs) {
      //     mpi_insist(MPI_Waitall(reqs.getSize(), (MPI_Request*)(Request*)reqs, MPI_STATUSES_IGNORE), 10);
      // }

      request::request(MPI_Request req)
	 : _req(req)
      {
      }

      request::request(const request& req)
	 : _req(req._req)
      {
      }

      request::~request()
      {
      }

      void
      request::mpi_request(MPI_Request req)
      {
	 this->wait();
	 this->_req = req;
      }

      const MPI_Request&
      request::mpi_request() const
      {
	 return this->_req;
      }

      MPI_Request&
      request::mod_mpi_request()
      {
	 this->wait();
	 return this->_req;
      }

      bool
      request::test() const
      {
	 if(this->_req != MPI_REQUEST_NULL) {
	    int flag;
	    MPI_INSIST(MPI_Test((MPI_Request*)&this->_req, &flag, MPI_STATUS_IGNORE));
	    return flag;
	 }
	 return true;
      }

      void
      request::wait() {
	 if(this->_req != MPI_REQUEST_NULL) {
	    MPI_INSIST(MPI_Wait(&this->_req, MPI_STATUS_IGNORE));
	    this->_req = MPI_REQUEST_NULL;
	 }
      }

      bool
      request::operator==(const request& op) const
      {
	 return this->_req == op._req;
      }

      bool
      request::operator==(const MPI_Request op) const
      {
	 return this->_req == op;
      }

      bool
      request::operator!=(const request& op) const
      {
	 return this->_req != op._req;
      }

      bool
      request::operator!=(const MPI_Request op) const
      {
	 return this->_req != op;
      }
   }
}
