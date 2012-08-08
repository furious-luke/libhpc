#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <cxxtest/TestSuite.h>
#include <cxxtest/GlobalFixture.h>
#include "libhpc/hpcmpi/comm.hh"

using namespace hpc;

#include "mpi_fixture.hh"

class comm_suite : public CxxTest::TestSuite {
public:

   void test_default_ctor()
   {
      mpi::comm comm;
      TS_ASSERT_EQUALS(comm.mpi_comm(), MPI_COMM_NULL);
   }

   void test_mpi_comm_ctor()
   {
      mpi::comm comm(MPI_COMM_WORLD);
      TS_ASSERT_EQUALS(comm.mpi_comm(), MPI_COMM_WORLD);
   }

   void test_copy_ctor()
   {
      mpi::comm comm(MPI_COMM_WORLD);
      mpi::comm copy(comm);

      // Duplicates the communicator, so can't check for equality.
      TS_ASSERT_DIFFERS(copy.mpi_comm(), MPI_COMM_WORLD);
   }

   void test_clear()
   {

      // Must clear without delete when using WORLD, NULL or SELF.
      mpi::comm comm(MPI_COMM_WORLD);
      comm.clear();
      TS_ASSERT_EQUALS(comm.mpi_comm(), MPI_COMM_NULL);
      comm.mpi_comm(MPI_COMM_NULL);
      comm.clear();
      TS_ASSERT_EQUALS(comm.mpi_comm(), MPI_COMM_NULL);
      comm.mpi_comm(MPI_COMM_SELF);
      comm.clear();
      TS_ASSERT_EQUALS(comm.mpi_comm(), MPI_COMM_NULL);

      // Must clear with delete when using a custom communicator.
      MPI_Comm mpi_comm;
      MPI_Comm_dup(MPI_COMM_WORLD, &mpi_comm);
      comm.mpi_comm(mpi_comm);
      comm.clear();
      TS_ASSERT_EQUALS(comm.mpi_comm(), MPI_COMM_NULL);
   }

   void test_set_mpi_comm()
   {
      mpi::comm comm;
      comm.mpi_comm(MPI_COMM_WORLD);
      TS_ASSERT_EQUALS(comm.mpi_comm(), MPI_COMM_WORLD);
   }

   void test_get_mpi_comm()
   {
      mpi::comm comm(MPI_COMM_WORLD);
      TS_ASSERT_EQUALS(comm.mpi_comm(), MPI_COMM_WORLD);
   }

   void test_get_rank()
   {
      mpi::comm comm(MPI_COMM_WORLD);
      int my_rank;
      MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
      TS_ASSERT_EQUALS(comm.rank(), my_rank);
   }

   void test_get_size()
   {
      mpi::comm comm(MPI_COMM_WORLD);
      int size;
      MPI_Comm_size(MPI_COMM_WORLD, &size);
      TS_ASSERT_EQUALS(comm.size(), size);
   }

   void test_split()
   {
      mpi::comm comm(MPI_COMM_WORLD), new_comm;
      comm.split(comm.rank()%2, new_comm);

      // Correct number of ranks per communicator.
      TS_ASSERT_EQUALS(new_comm.size(), comm.size()/2 + ((comm.rank()%2 == 0) ? comm.size()%2 : 0));
   }

   void test_raw_send()
   {
      mpi::comm comm(MPI_COMM_WORLD);
      int to, from;
      this->setup_ring(comm, to, from);

      int inc = -1, out = comm.rank();
      mpi::request req;
      comm.irecv(&inc, mpi::data_type::integer, from, req);
      comm.send(&out, mpi::data_type::integer, to);
      req.wait();
      TS_ASSERT_EQUALS(inc, from);
   }

   void test_raw_send_multi()
   {
      mpi::comm comm(MPI_COMM_WORLD);
      int to, from;
      this->setup_ring(comm, to, from);

      int inc[2] = {-1, -1}, out[2] = {comm.rank(), comm.rank() + 1};
      mpi::request req;
      comm.irecv(inc, mpi::data_type::integer, from, req, 2);
      comm.send(out, mpi::data_type::integer, to, 2);
      req.wait();
      TS_ASSERT_EQUALS(inc[0], from);
      TS_ASSERT_EQUALS(inc[1], from + 1);
   }

   void test_raw_isend()
   {
      mpi::comm comm(MPI_COMM_WORLD);
      int to, from;
      this->setup_ring(comm, to, from);

      int inc = -1, out = comm.rank();
      mpi::request req;
      comm.isend(&out, mpi::data_type::integer, to, req);
      comm.recv(&inc, mpi::data_type::integer, from);
      req.wait();
      TS_ASSERT_EQUALS(inc, from);
   }

   void test_raw_isend_multi()
   {
      mpi::comm comm(MPI_COMM_WORLD);
      int to, from;
      this->setup_ring(comm, to, from);

      int inc[2] = {-1, -1}, out[2] = {comm.rank(), comm.rank() + 1};
      mpi::request req;
      comm.isend(out, mpi::data_type::integer, to, req, 2);
      comm.recv(inc, mpi::data_type::integer, from, 2);
      req.wait();
      TS_ASSERT_EQUALS(inc[0], from);
      TS_ASSERT_EQUALS(inc[1], from + 1);
   }

   void test_raw_issend()
   {
      mpi::comm comm(MPI_COMM_WORLD);
      int to, from;
      this->setup_ring(comm, to, from);

      int inc = -1, out = comm.rank();
      mpi::request send_req, recv_req;
      comm.irecv(&inc, mpi::data_type::integer, from, recv_req);
      comm.barrier();
      comm.issend(&out, mpi::data_type::integer, to, send_req);
      recv_req.wait();
      send_req.wait();
      TS_ASSERT_EQUALS(inc, from);
   }

   void test_raw_issend_multi()
   {
      mpi::comm comm(MPI_COMM_WORLD);
      int to, from;
      this->setup_ring(comm, to, from);

      int inc[2] = {-1, -1}, out[2] = {comm.rank(), comm.rank() + 1};
      mpi::request send_req, recv_req;
      comm.irecv(inc, mpi::data_type::integer, from, recv_req, 2);
      comm.barrier();
      comm.issend(out, mpi::data_type::integer, to, send_req, 2);
      recv_req.wait();
      send_req.wait();
      TS_ASSERT_EQUALS(inc[0], from);
      TS_ASSERT_EQUALS(inc[1], from + 1);
   }

   void test_vector_send()
   {
      mpi::comm comm(MPI_COMM_WORLD);
      int to, from;
      this->setup_ring(comm, to, from);

      mpi::request req;
      comm.irecv<int>(this->inc, from, req);
      comm.send<int>(this->out, to);
      req.wait();
      for(int ii = 0; ii < 10; ++ii) {
         TS_ASSERT_EQUALS(this->inc[ii], from + ii);
      }
   }

   void test_vector_isend()
   {
      mpi::comm comm(MPI_COMM_WORLD);
      int to, from;
      this->setup_ring(comm, to, from);

      mpi::request req;
      comm.isend<int>(this->out, to, req);
      comm.recv<int>(this->inc, from);
      req.wait();
      for(int ii = 0; ii < 10; ++ii) {
         TS_ASSERT_EQUALS(this->inc[ii], from + ii);
      }
   }

   void test_vector_issend()
   {
      mpi::comm comm(MPI_COMM_WORLD);
      int to, from;
      this->setup_ring(comm, to, from);

      mpi::request send_req, recv_req;
      comm.irecv<int>(this->inc, from, recv_req);
      comm.barrier();
      comm.issend<int>(this->out, to, send_req);
      recv_req.wait();
      send_req.wait();
      for(int ii = 0; ii < 10; ++ii) {
         TS_ASSERT_EQUALS(this->inc[ii], from + ii);
      }
   }

   void test_scalar_send()
   {
      mpi::comm comm(MPI_COMM_WORLD);
      int to, from;
      this->setup_ring(comm, to, from);

      mpi::request req;
      comm.irecv(this->inc[0], from, req);
      comm.send(this->out[0], to);
      req.wait();
      TS_ASSERT_EQUALS(this->inc[0], from);
   }

   void test_scalar_isend()
   {
      mpi::comm comm(MPI_COMM_WORLD);
      int to, from;
      this->setup_ring(comm, to, from);

      mpi::request req;
      comm.isend(this->out[0], to, req);
      comm.recv(this->inc[0], from);
      req.wait();
      TS_ASSERT_EQUALS(this->inc[0], from);
   }

   void test_scalar_issend()
   {
      mpi::comm comm(MPI_COMM_WORLD);
      int to, from;
      this->setup_ring(comm, to, from);

      mpi::request send_req, recv_req;
      comm.irecv(this->inc[0], from, recv_req);
      comm.barrier();
      comm.issend(this->out[0], to, send_req);
      recv_req.wait();
      send_req.wait();
      TS_ASSERT_EQUALS(this->inc[0], from);
   }

   void test_raw_bcast()
   {
      mpi::comm comm(MPI_COMM_WORLD);

      vector<int> data(this->out);
      comm.bcast(data.data(), mpi::data_type::integer, 0);
      TS_ASSERT_EQUALS(data[0], 0);

      data.duplicate(this->out);
      comm.bcast(data.data(), mpi::data_type::integer, 0, 10);
      for(int ii = 0; ii < 10; ++ii)
         TS_ASSERT_EQUALS(data[ii], ii);

      if(comm.size() > 1) {
         data.duplicate(this->out);
         comm.bcast(data.data(), mpi::data_type::integer, 1, 10);
         for(int ii = 0; ii < 10; ++ii) {
            TS_ASSERT_EQUALS(data[ii], 1 + ii);
         }
      }
   }

   void test_scalar_bcast()
   {
      mpi::comm comm(MPI_COMM_WORLD);

      int rank = comm.rank();
      comm.bcast(rank, 0);
      TS_ASSERT_EQUALS(rank, 0);
      if(comm.size() > 1) {
         rank = comm.rank();
         comm.bcast(rank, 1);
         TS_ASSERT_EQUALS(rank, 1);
      }
   }

   void test_equality()
   {
      mpi::comm comm, op;
      TS_ASSERT(comm == op);
      comm.mpi_comm(MPI_COMM_WORLD);
      TS_ASSERT(!(comm == op));
   }

   void test_mpi_comm_equality()
   {
      mpi::comm comm;
      TS_ASSERT(comm == MPI_COMM_NULL);
      comm.mpi_comm(MPI_COMM_WORLD);
      TS_ASSERT(!(comm == MPI_COMM_NULL));
   }

   void test_inequality()
   {
      mpi::comm comm, op;
      TS_ASSERT(!(comm != op));
      comm.mpi_comm(MPI_COMM_WORLD);
      TS_ASSERT(comm != op);
   }

   void test_mpi_comm_inequality()
   {
      mpi::comm comm;
      TS_ASSERT(!(comm != MPI_COMM_NULL));
      comm.mpi_comm(MPI_COMM_WORLD);
      TS_ASSERT(comm != MPI_COMM_NULL);
   }

   void setup_ring(const mpi::comm& comm, int& from, int& to)
   {
      int rank = comm.rank();
      int size = comm.size();
      to = (rank + 1)%size;
      from = (rank ? rank : size) - 1;
   }

   void setUp() {
      this->out.resize(10);
      this->inc.resize(10);
      boost::iota(this->out, mpi::comm::world.rank());
      boost::fill(this->inc, -1);
   }

private:
   vector<int> out, inc;
};
