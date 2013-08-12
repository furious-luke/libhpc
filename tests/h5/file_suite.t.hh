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

#include <cstdlib>
#include <boost/range/algorithm_ext.hpp>
#include <cxxtest/TestSuite.h>
#include <cxxtest/GlobalFixture.h>
#include "libhpc/h5/file.hh"

using namespace hpc;

#include "mpi_fixture.hh"

class file_suite : public CxxTest::TestSuite {
public:

   void test_write_value_world()
   {
      {
	 h5::file file(this->filename, H5F_ACC_TRUNC, mpi::comm::world);
	 file.write<int>("value", 10);
      }
      {
	 h5::file file(this->filename, H5F_ACC_RDONLY, mpi::comm::world);
	 int value = file.read<int>("value");
	 TS_ASSERT_EQUALS(value, 10);
      }
   }

   void test_write_value_self()
   {
      if(mpi::comm::world.rank() == 0) {
	 h5::file file(this->filename, H5F_ACC_TRUNC, mpi::comm::self);
	 file.write<int>("value", 10);
      }
      if(mpi::comm::world.rank() == 0) {
	 h5::file file(this->filename, H5F_ACC_RDONLY, mpi::comm::self);
	 int value = file.read<int>("value");
	 TS_ASSERT_EQUALS(value, 10);
      }
      mpi::comm::world.barrier();
   }

   void test_write_vector_world()
   {
      {
	 vector<int> vec(10);
	 boost::iota(vec, 10*mpi::comm::world.rank());
	 h5::file file(this->filename, H5F_ACC_TRUNC, mpi::comm::world);
	 file.write<int>("vector", vec);
      }
      {
	 vector<int> vec(10);
	 h5::file file(this->filename, H5F_ACC_RDONLY, mpi::comm::world);
	 file.read<int>("vector", vec);
	 for(int ii = 0; ii < vec.size(); ++ii) {
	    TS_ASSERT_EQUALS(vec[ii], 10*mpi::comm::world.rank() + ii);
	 }
      }
   }

   void test_write_vector_self()
   {
      if(mpi::comm::world.rank() == 0) {
	 vector<int> vec(10);
	 boost::iota(vec, 10*mpi::comm::self.rank());
	 h5::file file(this->filename, H5F_ACC_TRUNC, mpi::comm::self);
	 file.write<int>("vector", vec);
      }
      if(mpi::comm::world.rank() == 0) {
	 vector<int> vec(10);
	 h5::file file(this->filename, H5F_ACC_RDONLY, mpi::comm::self);
	 file.read<int>("vector", vec);
	 for(int ii = 0; ii < vec.size(); ++ii) {
	    TS_ASSERT_EQUALS(vec[ii], 10*mpi::comm::self.rank() + ii);
	 }
      }
      mpi::comm::world.barrier();
   }

   void test_write_vector_compression()
   {
      if(mpi::comm::world.rank() == 0) {
	 vector<int> vec(10000);
	 boost::iota(vec, 10000*mpi::comm::self.rank());
	 vector<hsize_t> chunk_size(1);
	 chunk_size[0] = 1000;
	 h5::file file(this->filename, H5F_ACC_TRUNC, mpi::comm::self);
	 file.write<int>("vector", vec, vector<hsize_t>::view(chunk_size), true);
      }
      if(mpi::comm::world.rank() == 0) {
	 vector<int> vec(10000);
	 h5::file file(this->filename, H5F_ACC_RDONLY, mpi::comm::self);
	 file.read<int>("vector", vec);
	 for(int ii = 0; ii < vec.size(); ++ii) {
	    TS_ASSERT_EQUALS(vec[ii], 10000*mpi::comm::self.rank() + ii);
	 }
      }
      mpi::comm::world.barrier();
   }

   void test_write_csr_world()
   {
      {
	 csr<int> data;
	 data.num_rows(10);
	 for(hpc::index ii = 0; ii < data.mod_displs().size(); ++ii)
	    data.mod_displs()[ii] = 10*ii;
	 data.setup_array();
	 for(hpc::index ii = 0; ii < data.mod_array().size(); ++ii)
	    data.mod_array()[ii] = 100*mpi::comm::world.rank() + ii;
	 h5::file file(this->filename, H5F_ACC_TRUNC, mpi::comm::world);
	 file.write<int>("csr", data);
      }
      {
	 csr<int> data;
	 data.num_rows(10);
      	 h5::file file(this->filename, H5F_ACC_RDONLY, mpi::comm::world);
      	 file.read<int>("csr", data);
      	 for(hpc::index ii = 0; ii < data.displs().size(); ++ii) {
      	    TS_ASSERT_EQUALS(data.displs()[ii], 10*ii);
      	 }
      	 for(hpc::index ii = 0; ii < data.array().size(); ++ii) {
      	    TS_ASSERT_EQUALS(data.array()[ii], 100*mpi::comm::world.rank() + ii);
      	 }
      }
   }

   void test_read_csr_elements()
   {
      {
	 csr<int> data;
	 data.num_rows(10);
	 for(hpc::index ii = 0; ii < data.mod_displs().size(); ++ii)
	    data.mod_displs()[ii] = 10*ii;
	 data.setup_array();
	 for(hpc::index ii = 0; ii < data.mod_array().size(); ++ii)
	    data.mod_array()[ii] = 100*mpi::comm::world.rank() + ii;
	 h5::file file(this->filename, H5F_ACC_TRUNC, mpi::comm::world);
	 file.write<int>("csr", data);
      }
      {
	 csr<int> data;
	 data.num_rows(5);
	 vector<hsize_t> elements(5);
	 for(int ii = 0; ii < elements.size(); ++ii)
	    elements[ii] = 10*mpi::comm::world.rank() + 2*ii;
      	 h5::file file(this->filename, H5F_ACC_RDONLY, mpi::comm::world);
      	 file.read<int>("csr", data, elements);
      	 for(hpc::index ii = 0; ii < data.displs().size(); ++ii) {
      	    TS_ASSERT_EQUALS(data.displs()[ii], 10*ii);
      	 }
      	 for(hpc::index ii = 0; ii < 5; ++ii) {
	    for(hpc::index jj = 0; jj < 10; ++jj) {
	       TS_ASSERT_EQUALS(data(ii, jj), 100*mpi::comm::world.rank() + 20*ii + jj);
	    }
      	 }
      }
   }

   // void testReadCSRAlloc() {
   //    int myRank = MPI::Comm::WORLD.getMyRank();
   //    int numRanks = MPI::Comm::WORLD.getNumRanks();

   //    this->writeFile(filename);

   //    Vector<hsize_t> rows(2);
   //    rows[0] = 2*(myRank + 1)%(2*numRanks);
   //    rows[1] = (2*(myRank + 1) + 1)%(2*numRanks);
   //    CSR<int> data;
   //    H5::File file(filename, H5F_ACC_RDONLY, MPI::Comm::WORLD);
   //    file.reada("test", data, rows, MPI::Comm::WORLD);

   //    TS_ASSERT(data.getNumRows() == 2);
   //    for(int ii = 0; ii < 2; ++ii)
   // 	 TS_ASSERT(data.getRowSize(ii) == 2);
   //    const VectorView<int>& array = data.getArray();
   //    for(int ii = 0; ii < array.getSize(); ++ii)
   // 	 TS_ASSERT(array[ii] == (myRank + 1)%numRanks + ii);
   // }

   // void writeFile(const String& filename) {
   //    int myRank = MPI::Comm::WORLD.getMyRank();
   //    int numRanks = MPI::Comm::WORLD.getNumRanks();

   //    CSR<int> data;
   //    data.setNumRows(2);
   //    VectorView<int>& cnts = data.accDispls();
   //    for(int ii = 0; ii < 2; ++ii)
   // 	 cnts[ii] = 2;
   //    data.setupArray(true);

   //    VectorView<int>& array = data.getArray();
   //    for(int ii = 0; ii < array.getSize(); ++ii)
   // 	 array[ii] = myRank + ii;

   //    H5::File file(filename, H5F_ACC_TRUNC, MPI::Comm::WORLD);
   //    file.write("test", data, MPI::Comm::WORLD);
   // }

   void setUp() {
      this->filename = (const char*)tmpnam(NULL);
      mpi::comm::world.bcast(this->filename);
   }

   void tearDown() {
      remove(this->filename.c_str());
   }

protected:

   std::string filename;
};
