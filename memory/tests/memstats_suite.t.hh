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

#include <numeric>
#include <cxxtest/TestSuite.h>
#include "libhpc/memory/new.hh"
#include "libhpc/memory/group_context.hh"
#include "libhpc/memory/state.hh"
#include "libhpc/memory/globals.hh"

using namespace hpc;

#ifndef NMEMSTATS
namespace hpc {
   namespace memory {
      extern group_context<state_t> ctx;
   }
}
#endif

class memstats_suite : public CxxTest::TestSuite {
public:

   void test_add()
   {
#ifndef NMEMSTATS
      auto cur_size = memory::ctx.find_group( "/" ).data().size;
      auto cur_peak = memory::ctx.find_group( "/" ).data().peak;
      int* buf = new int[100];
      TS_ASSERT_EQUALS( memory::ctx.find_group( "/" ).data().size - cur_size, 100*sizeof(int) + padding );
      TS_ASSERT_EQUALS( memory::ctx.find_group( "/" ).data().peak - cur_peak, 100*sizeof(int) + padding );
      delete buf;
#endif
   }

   void test_remove()
   {
#ifndef NMEMSTATS
      auto cur_size = memory::ctx.find_group( "/" ).data().size;
      auto cur_peak = memory::ctx.find_group( "/" ).data().peak;
      int* buf = new int[100];
      delete buf;
      std::cout << "\n" << memory::ctx.find_group( "/" ).data().size << ", " << cur_size << "\n";
      TS_ASSERT_EQUALS( memory::ctx.find_group( "/" ).data().size - cur_size, 0 );
      TS_ASSERT_EQUALS( memory::ctx.find_group( "/" ).data().peak - cur_peak, 100*sizeof(int) + padding );
#endif
   }

   void test_threaded()
   {
#if !defined(NMEMSTATS) && defined(_OPENMP)
      auto cur_size = memory::ctx.find_group( "/" ).data().size;
      auto cur_peak = memory::ctx.find_group( "/" ).data().peak;
      int* buf[10];
#pragma omp parallel num_threads(10)
      {
	 int tid = OMP_TID;
	 if( tid == 0 )
	    memory::select( "/zero" );
	 else if( tid == 5 )
	 {
	    memory::select( "/zero" );
	    memory::select( "/five" );
	 }
	 buf[tid] = new int[100];
      }
      TS_ASSERT_EQUALS( memory::ctx.find_group( "/" ).data().size - cur_size, 10*(100*sizeof(int) + padding) );
      TS_ASSERT_EQUALS( memory::ctx.find_group( "/" ).data().peak - cur_peak, 10*(100*sizeof(int) + padding) );
      TS_ASSERT_EQUALS( memory::ctx.find_group( "/zero" ).data().size, 2*(100*sizeof(int) + padding) );
      TS_ASSERT_EQUALS( memory::ctx.find_group( "/five" ).data().size, 1*(100*sizeof(int) + padding) );
#pragma omp parallel num_threads(10)
      {
	 int tid = OMP_TID;
	 delete buf[tid];
      }
      TS_ASSERT_EQUALS( memory::ctx.find_group( "/" ).data().size - cur_size, 0 );
      TS_ASSERT_EQUALS( memory::ctx.find_group( "/" ).data().peak - cur_peak, 10*(100*sizeof(int) + padding) );
      TS_ASSERT_EQUALS( memory::ctx.find_group( "/zero" ).data().size, 0 );
      TS_ASSERT_EQUALS( memory::ctx.find_group( "/five" ).data().size, 0 );
#endif
   }

   void setUp()
   {
#ifndef NMEMSTATS
      for( auto& grp : memory::ctx )
      {
	 memory::state_t& state = grp.data();
	 state.size = 0;
	 state.peak = 0;
      }
#endif
   }

#ifndef NDEBUG
   static const size_t padding = 12;
#else
   static const size_t padding = 8;
#endif
};
