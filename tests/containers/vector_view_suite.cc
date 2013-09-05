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

#include <libhpc/debug/unit_test_main.hh>
#include <vector>
#include "libhpc/containers/view.hh"

using namespace hpc;
using namespace hpc::test;

namespace {

   struct fixture
   {
      fixture()
         : vec( 10 )
      {
         std::iota( vec.begin(), vec.end(), 0 );
      }

      std::vector<int> vec;
   };

   void
   check_contents( vector_view<std::vector<int>>& view,
                   size_t size = 10,
                   size_t offs = 0 )
   {
      TEST( view.size() == size );
      for( size_t ii = 0; ii < size; ++ii )
         TEST( view[ii] == offs + ii );
   }

   test_case<fixture> ANON(
      "/containers/vector_view/constructor/one_to_one",
      "",
      []( fixture& fix )
      {
         view<std::vector<int>>::type view( fix.vec );
         check_contents( view );
      }
      );

   test_case<fixture> ANON(
      "/containers/vector_view/constructor/resize",
      "",
      []( fixture& fix )
      {
         view<std::vector<int>>::type view( fix.vec, 5 );
         check_contents( view, 5 );
      }
      );

   test_case<fixture> ANON(
      "/containers/vector_view/constructor/offset",
      "",
      []( fixture& fix )
      {
         view<std::vector<int>>::type view( fix.vec, 7, 3 );
         check_contents( view, 7, 3 );
      }
      );

   test_case<fixture> ANON(
      "/containers/vector_view/constructor/copy",
      "",
      []( fixture& fix )
      {
         view<std::vector<int>>::type view( fix.vec );
         hpc::view<std::vector<int>>::type copy( view );
         check_contents( view );
         check_contents( copy );
      }
      );

   test_case<fixture> ANON(
      "/containers/vector_view/size",
      "",
      []( fixture& fix )
      {
         view<std::vector<int>>::type view( fix.vec );
         TEST( view.size() == fix.vec.size() );
      }
      );

   // TODO: Finish these

//    void test_empty()
//    {
//       {
//          vector<int>::view view;
//          TS_ASSERT(view.empty());
//       }
//       {
//          vector<int>::view view(this->vec);
//          TS_ASSERT(!view.empty());
//       }
//    }

//    void test_data()
//    {
//       vector<int>::view view(this->vec);
//       TS_ASSERT_EQUALS(view.data(), this->vec.data());
//    }

//    void test_const_dereference()
//    {
//       vector<int>::view view(this->vec);
//       TS_ASSERT_THROWS_NOTHING(((const vector<int>::view&)view)[0]);
// #ifndef NDEBUG
//       TS_ASSERT_THROWS_ANYTHING(((const vector<int>::view&)view)[-1]);
//       TS_ASSERT_THROWS_ANYTHING(((const vector<int>::view&)view)[11]);
// #else
//       TS_ASSERT_THROWS_NOTHING(((const vector<int>::view&)view)[11]);
// #endif
//    }

//    void test_dereference()
//    {
//       vector<int>::view view(this->vec);
//       TS_ASSERT_THROWS_NOTHING(((vector<int>::view&)view)[0]);
// #ifndef NDEBUG
//       TS_ASSERT_THROWS_ANYTHING(((vector<int>::view&)view)[-1]);
//       TS_ASSERT_THROWS_ANYTHING(((vector<int>::view&)view)[11]);
// #else
//       TS_ASSERT_THROWS_NOTHING(((vector<int>::view&)view)[11]);
// #endif
//    }

//    void test_assignment()
//    {
//       vector<int> tmp(10);
//       vector<int>::view view(tmp);
//       vector<int>::view src(this->vec);
//       view = src;
//       for(int ii = 0; ii < view.size(); ++ii)
// 	 TS_ASSERT_EQUALS(view[ii], src[ii]);
//    }

//    void test_const_begin()
//    {
//       const vector<int>::view view(this->vec);
//       TS_ASSERT_EQUALS(*view.begin(), 0);
//       TS_ASSERT_EQUALS(*++view.begin(), 1);
//    }

//    void test_begin()
//    {
//       vector<int>::view view(this->vec);
//       TS_ASSERT_EQUALS(*view.begin(), 0);
//       TS_ASSERT_EQUALS(*++view.begin(), 1);
//    }

//    void test_const_end()
//    {
//       const vector<int>::view view(this->vec);
//       TS_ASSERT_EQUALS(*--view.end(), 9);
//       TS_ASSERT_EQUALS(*----view.end(), 8);
//    }

//    void test_end()
//    {
//       vector<int>::view view(this->vec);
//       TS_ASSERT_EQUALS(*--view.end(), 9);
//       TS_ASSERT_EQUALS(*----view.end(), 8);
//    }

//    void test_equality()
//    {
//       vector<int>::view view(this->vec);
//       vector<int>::view copy(view);
//       vector<int>::view offs(this->vec, 7, 3);
//       vector<int> diff(10);
//       hpc::iota(diff.begin(), diff.end(), 3);
//       TS_ASSERT(view == copy);
//       TS_ASSERT(!(view == offs));
//       TS_ASSERT(!(view == diff));
//    }



//    void setUp()
//    {
//       this->vec.resize(10);
//       hpc::iota(vec.begin(), vec.end(), 0);
//    }

// private:

//    vector<int> vec;
// };

}
