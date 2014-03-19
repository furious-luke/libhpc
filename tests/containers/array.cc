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
#include <libhpc/containers/array.hh>
#include <libhpc/containers/vector.hh>
#include <libhpc/system/stream_output.hh>

using namespace hpc::test;

namespace {

  test_case<> ANON(
      "/containers/array/static_initialisation",
      "Static initilaisers need to be able to work.",
      []()
      {
         hpc::array<int,2> arr = { 1, 2 };
         TEST( arr[0] == 1 );
         TEST( arr[1] == 2 );
      }
      );

  test_case<> ANON(
      "/containers/array/stream_output_operator",
      "",
      []()
      {
         hpc::array<int,2> arr = { 1, 2 };
         std::stringstream ss;
         ss << arr;
         TEST( ss.str() == "[1, 2]" );
      }
      );

  test_case<> ANON(
      "/containers/array/emplacement",
      "",
      []()
      {
         hpc::vector<hpc::array<int,2>> vec;
         vec.emplace_back( hpc::array<int,2>{ { 1, 2 } } );
         TEST( vec.size() == 1 );
         TEST( vec[0][0] == 1 );
         TEST( vec[0][1] == 2 );
      }
      );

}
