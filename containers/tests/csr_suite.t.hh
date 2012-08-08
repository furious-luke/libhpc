#include <cxxtest/TestSuite.h>
#include "libhpc/containers/csr.hh"
#include "libhpc/containers/map.hh"
#include "libhpc/containers/multimap.hh"
#include "libhpc/containers/vector.hh"
#include "libhpc/containers/functors.hh"
#include "libhpc/containers/set.hh"

class csr_suite : public CxxTest::TestSuite {
public:

   void test_default_ctor()
   {
      hpc::csr<int> csr;
      this->check_empty(csr);
   }

   void test_copy_ctor()
   {
      hpc::csr<int> csr((const hpc::csr<int>&)this->src);
      this->check_contents(csr);
      this->check_contents(this->src);
      TS_ASSERT_DIFFERS(csr.displs().data(), this->src.displs().data());
      TS_ASSERT_DIFFERS(csr.array().data(), this->src.array().data());
   }

   void test_deallocate()
   {
      hpc::csr<int> csr((const hpc::csr<int>&)this->src);
      csr.deallocate();
      this->check_empty(csr);
   }

   void test_empty()
   {
      TS_ASSERT(!this->src.empty());
      this->src.deallocate();
      TS_ASSERT(this->src.empty());
   }

   void test_get_num_rows()
   {
      TS_ASSERT_EQUALS(this->src.num_rows(), this->num_rows);
   }

   void test_set_num_rows()
   {
      hpc::csr<int> csr;
      csr.num_rows(this->num_rows);
      TS_ASSERT_EQUALS(csr.num_rows(), this->num_rows);
      TS_ASSERT_EQUALS(csr.displs().size(), this->num_rows + 1);
      TS_ASSERT(csr.array().empty());
   }

   void test_copy_counts()
   {
      hpc::csr<int> csr;
      csr.copy_counts(this->cnts.begin(), this->num_rows);
      csr.setup_array(true);
      TS_ASSERT_EQUALS(csr.num_rows(), this->num_rows);
      this->check_displs(csr.displs());
      TS_ASSERT_EQUALS(csr.array().size(), 6);
      TS_ASSERT_DIFFERS(csr.displs().data(), this->cnts.data());
   }

   void test_copy_displs()
   {
      hpc::csr<int> csr;
      csr.copy_displs(this->displs.begin(), this->displs.size());
      csr.setup_array();
      TS_ASSERT_EQUALS(csr.num_rows(), this->num_rows);
      this->check_displs(csr.displs());
      TS_ASSERT_EQUALS(csr.array().size(), 6);
      TS_ASSERT_DIFFERS(csr.displs().data(), this->displs.data());
   }

   void test_take_displs()
   {
      hpc::csr<int> csr;
      csr.take_displs(this->displs);
      csr.setup_array();
      TS_ASSERT_EQUALS(csr.num_rows(), this->num_rows);
      this->check_displs(csr.displs());
      TS_ASSERT_EQUALS(csr.array().size(), 6);
      TS_ASSERT(this->displs.empty());
   }

   void test_take_array()
   {
      hpc::vector<int> array;
#ifndef NDEBUG
      TS_ASSERT_THROWS_ANYTHING(this->src.take_array(array));
#endif
      array.resize(this->src.array().size());
#ifndef NDEBUG
      TS_ASSERT_THROWS_NOTHING(this->src.take_array(array));
#endif
   }

   void test_setup_array()
   {
      hpc::csr<int> csr;
      csr.num_rows(3);
      std::fill(csr.mod_displs().begin(), csr.mod_displs().end(), 0);
      csr.setup_array();
      TS_ASSERT(csr.array().empty());

      csr.mod_displs()[0] = 2; csr.mod_displs()[1] = 1; csr.mod_displs()[2] = 3;
      csr.setup_array(true);
      TS_ASSERT_EQUALS(csr.array().size(), 6);

      std::fill(csr.mod_displs().begin(), csr.mod_displs().end(), 0);
      csr.setup_array(true);
      TS_ASSERT(csr.array().empty());
      csr.mod_displs()[0] = 0; csr.mod_displs()[1] = 2; csr.mod_displs()[2] = 3; csr.mod_displs()[3] = 6;
      csr.setup_array();
      TS_ASSERT_EQUALS(csr.array().size(), 6);
   }

   void test_set_from_multimap()
   {
      hpc::map<int, int> row_map;
      row_map.insert(10, 0);
      row_map.insert(20, 1);
      row_map.insert(30, 2);

      hpc::multimap<int, int> entry_map;
      entry_map.insert(10, 100);
      entry_map.insert(10, 101);
      entry_map.insert(20, 200);
      entry_map.insert(20, 201);
      entry_map.insert(30, 300);
      entry_map.insert(30, 301);

      hpc::csr<int> csr;
      csr.set_from_multimap(entry_map, row_map);

      TS_ASSERT_EQUALS(csr.num_rows(), 3);
      TS_ASSERT_EQUALS(csr.row_size(0), 2);
      TS_ASSERT_EQUALS(csr.row_size(1), 2);
      TS_ASSERT_EQUALS(csr.row_size(2), 2);
      TS_ASSERT_EQUALS(csr(0, 0), 100);
      TS_ASSERT_EQUALS(csr(0, 1), 101);
      TS_ASSERT_EQUALS(csr(1, 0), 200);
      TS_ASSERT_EQUALS(csr(1, 1), 201);
      TS_ASSERT_EQUALS(csr(2, 0), 300);
      TS_ASSERT_EQUALS(csr(2, 1), 301);
   }

   void test_duplicate()
   {
      hpc::csr<int> csr;
      csr.duplicate(this->src);
      this->check_contents(csr);
      this->check_contents(this->src);
      TS_ASSERT_DIFFERS(csr.displs().data(), this->src.displs().data());
      TS_ASSERT_DIFFERS(csr.array().data(), this->src.array().data());
   }

   void test_take()
   {
      hpc::csr<int> csr;
      csr.take(this->src);
      this->check_contents(csr);
      this->check_empty(this->src);
   }

   void test_take_components()
   {
      hpc::csr<int> csr;
      hpc::vector<int> array(this->displs[this->displs.size() -1]);
      boost::copy(this->src.array(), array.begin());
      csr.take(this->displs, array);
      this->check_contents(csr);
      TS_ASSERT(this->displs.empty());
      TS_ASSERT(array.empty());
   }

   // TODO: test the count retrieval methods.

   void test_get_displs()
   {
      const hpc::vector<hpc::index>& displs = this->src.displs();
      TS_ASSERT(displs == this->displs);
   }

   void test_mod_displs()
   {
      hpc::vector<hpc::index>::view displs = this->src.mod_displs();
      TS_ASSERT(displs == this->displs);

      // Check that modifications propagate to the object.
      displs[1] = 0;
      TS_ASSERT_EQUALS(this->src.displs()[1], 0);
   }

   void test_get_array()
   {
      const hpc::vector<int>& array = this->src.array();
      TS_ASSERT(array == this->src.array());
   }

   void test_mod_array()
   {
      hpc::vector<int>::view array = this->src.mod_array();
      TS_ASSERT(array == this->src.array());

      // Check that modifications propagate to the object.
      array[1] = 0;
      TS_ASSERT_EQUALS(this->src.array()[1], 0);
   }

   // TODO:: test row size retrieval methods.

   void test_const_deref()
   {
      TS_ASSERT_EQUALS(((const hpc::csr<int>&)this->src)[0].size(), 2);
      TS_ASSERT_EQUALS(((const hpc::csr<int>&)this->src)[1].size(), 1);
      TS_ASSERT_EQUALS(((const hpc::csr<int>&)this->src)[2].size(), 3);
      TS_ASSERT_EQUALS(((const hpc::csr<int>&)this->src)[0][0], 10);
      TS_ASSERT_EQUALS(((const hpc::csr<int>&)this->src)[0][1], 11);
      TS_ASSERT_EQUALS(((const hpc::csr<int>&)this->src)[1][0], 20);
      TS_ASSERT_EQUALS(((const hpc::csr<int>&)this->src)[2][0], 30);
      TS_ASSERT_EQUALS(((const hpc::csr<int>&)this->src)[2][1], 31);
      TS_ASSERT_EQUALS(((const hpc::csr<int>&)this->src)[2][2], 32);
   }

   void test_deref()
   {
      TS_ASSERT_EQUALS(((hpc::csr<int>&)this->src)[0].size(), 2);
      TS_ASSERT_EQUALS(((hpc::csr<int>&)this->src)[1].size(), 1);
      TS_ASSERT_EQUALS(((hpc::csr<int>&)this->src)[2].size(), 3);
      TS_ASSERT_EQUALS(((hpc::csr<int>&)this->src)[0][0], 10);
      TS_ASSERT_EQUALS(((hpc::csr<int>&)this->src)[0][1], 11);
      TS_ASSERT_EQUALS(((hpc::csr<int>&)this->src)[1][0], 20);
      TS_ASSERT_EQUALS(((hpc::csr<int>&)this->src)[2][0], 30);
      TS_ASSERT_EQUALS(((hpc::csr<int>&)this->src)[2][1], 31);
      TS_ASSERT_EQUALS(((hpc::csr<int>&)this->src)[2][2], 32);
   }

   void test_const_elem_deref()
   {
      TS_ASSERT_EQUALS(((const hpc::csr<int>&)this->src)(0, 0), 10);
      TS_ASSERT_EQUALS(((const hpc::csr<int>&)this->src)(0, 1), 11);
      TS_ASSERT_EQUALS(((const hpc::csr<int>&)this->src)(1, 0), 20);
      TS_ASSERT_EQUALS(((const hpc::csr<int>&)this->src)(2, 0), 30);
      TS_ASSERT_EQUALS(((const hpc::csr<int>&)this->src)(2, 1), 31);
      TS_ASSERT_EQUALS(((const hpc::csr<int>&)this->src)(2, 2), 32);
   }

   void test_elem_deref()
   {
      TS_ASSERT_EQUALS(((hpc::csr<int>&)this->src)(0, 0), 10);
      TS_ASSERT_EQUALS(((hpc::csr<int>&)this->src)(0, 1), 11);
      TS_ASSERT_EQUALS(((hpc::csr<int>&)this->src)(1, 0), 20);
      TS_ASSERT_EQUALS(((hpc::csr<int>&)this->src)(2, 0), 30);
      TS_ASSERT_EQUALS(((hpc::csr<int>&)this->src)(2, 1), 31);
      TS_ASSERT_EQUALS(((hpc::csr<int>&)this->src)(2, 2), 32);
   }

   void test_equality()
   {
      hpc::csr<int> csr;
      TS_ASSERT(!(csr == this->src));
      csr.num_rows(3);
      TS_ASSERT(!(csr == this->src));
      csr.copy_displs(this->displs.begin(), this->displs.size());
      TS_ASSERT(!(csr == this->src));
      csr.duplicate(this->src);
      TS_ASSERT(csr == this->src);
   }

   void test_inequality()
   {
      hpc::csr<int> csr;
      TS_ASSERT(csr != this->src);
      csr.num_rows(3);
      TS_ASSERT(csr != this->src);
      csr.copy_displs(this->displs.begin(), this->displs.size());
      TS_ASSERT(csr != this->src);
      csr.duplicate(this->src);
      TS_ASSERT(!(csr != this->src));
   }

   void test_3phase()
   {
      hpc::vector<int> values(1000);
      for(hpc::index ii = 0; ii < values.size(); ++ii)
	 values[ii] = 10*ii;

      hpc::set<int> pred;
      for(hpc::index ii = 0; ii < 100; ++ii)
	 pred.insert(100*ii);

      hpc::map<int, int> mapping;
      for(hpc::index ii = 0; ii < 100; ++ii)
	 mapping.insert(100*ii, ii/10);

      hpc::vector<int> displs, array;
      csr_3phase(10, displs, array, values.begin(), values.end(), hpc::map_get(mapping), hpc::set_has(pred));

      TS_ASSERT_EQUALS(displs.size(), 11);
      TS_ASSERT_EQUALS(array.size(), 100);
      for(hpc::index ii = 0; ii < displs.size(); ++ii)
	 TS_ASSERT_EQUALS(displs[ii], 10*ii);
      for(hpc::index ii = 0; ii < array.size(); ++ii)
      	 TS_ASSERT_EQUALS(array[ii], 100*ii);
   }

   void test_3phase_indices()
   {
      hpc::vector<int> values(1000);
      for(hpc::index ii = 0; ii < values.size(); ++ii)
	 values[ii] = 10*ii;

      hpc::set<int> pred;
      for(hpc::index ii = 0; ii < 100; ++ii)
	 pred.insert(100*ii);

      hpc::map<int, int> mapping;
      for(hpc::index ii = 0; ii < 100; ++ii)
	 mapping.insert(100*ii, ii/10);

      hpc::vector<int> displs, array, indices;
      csr_3phase(10, displs, array, indices, values.begin(), values.end(), hpc::map_get(mapping), hpc::set_has(pred));

      TS_ASSERT_EQUALS(displs.size(), 11);
      TS_ASSERT_EQUALS(array.size(), 100);
      TS_ASSERT_EQUALS(indices.size(), 100);
      for(hpc::index ii = 0; ii < displs.size(); ++ii)
      	 TS_ASSERT_EQUALS(displs[ii], 10*ii);
      for(hpc::index ii = 0; ii < array.size(); ++ii)
      	 TS_ASSERT_EQUALS(array[ii], 100*ii);
      for(hpc::index ii = 0; ii < indices.size(); ++ii)
      	 TS_ASSERT_EQUALS(indices[ii], 10*ii);
   }

   void check_displs(const hpc::vector<hpc::index>::view& displs)
   {
      TS_ASSERT_EQUALS(displs.size(), 4);
      TS_ASSERT_EQUALS(displs[0], 0);
      TS_ASSERT_EQUALS(displs[1], 2);
      TS_ASSERT_EQUALS(displs[2], 3);
      TS_ASSERT_EQUALS(displs[3], 6);
   }

   void check_empty(const hpc::csr<int>& csr)
   {
      TS_ASSERT_EQUALS(csr.num_rows(), 0);
      TS_ASSERT_EQUALS(csr.displs().empty(), true);
      TS_ASSERT_EQUALS(csr.array().empty(), true);
   }

   void check_contents(const hpc::csr<int>& csr)
   {
      TS_ASSERT_EQUALS(csr.num_rows(), 3);
      TS_ASSERT_EQUALS(csr.displs().size(), 4);
      TS_ASSERT_EQUALS(csr.array().size(), 6);
      this->check_displs(csr.displs());
      int pos = 0;
      for(int ii = 0; ii < 3; ii++) {
	 for(int jj = 0; jj < this->cnts[ii]; jj++)
	    TS_ASSERT_EQUALS(csr.array()[pos++], (ii + 1)*10 + jj);
      }
   }

   void setUp()
   {
      this->num_rows = 3;
      this->cnts.resize(this->num_rows + 1);
      this->cnts[0] = 2; this->cnts[1] = 1; this->cnts[2] = 3; this->cnts[3] = 0;
      this->displs.resize(this->num_rows + 1);
      this->displs[0] = 0; this->displs[1] = 2; this->displs[2] = 3; this->displs[3] = 6;

      this->src.copy_displs(this->displs.begin(), this->displs.size());
      this->src.setup_array();
      int pos = 0;
      for(int ii = 0; ii < this->num_rows; ii++) {
	 for(int jj = 0; jj < this->cnts[ii]; jj++)
	    this->src.mod_array()[pos++] = (ii + 1)*10 + jj;
      }
   }

private:
   int num_rows;
   hpc::vector<hpc::index> cnts;
   hpc::vector<hpc::index> displs;
   hpc::csr<int> src;
};
