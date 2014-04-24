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

#include <libhpc/unit_test/main_mpi.hh>
#include <libhpc/mpi/comm.hh>

///
/// Compute circuit of ranks.
///
std::pair<int,int>
rank_circuit( hpc::mpi::comm const& comm )
{
   int rank = comm.rank();
   int size = comm.size();
   int to = (rank + 1)%size;
   int from = (rank ? rank : size) - 1;
   return std::make_pair( from, to );
}

TEST_CASE( "/libhpc/mpi/comm/constructor/default" )
{
   hpc::mpi::comm comm;
   TEST( comm.mpi_comm() == MPI_COMM_NULL );
}

TEST_CASE( "/libhpc/mpi/comm/send" )
{
   hpc::mpi::comm comm( MPI_COMM_WORLD );
   std::pair<int,int> circ = rank_circuit( comm );
   int inc = -1, out = comm.rank();
   hpc::mpi::request req;
   comm.irecv( &inc, hpc::mpi::datatype::integer, circ.first, req );
   comm.send( &out, hpc::mpi::datatype::integer, circ.second );
   req.wait();
   TEST( inc == circ.first );
}
