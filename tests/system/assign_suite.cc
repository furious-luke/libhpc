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

#include <libhpc/unit_test/main.hh>
#include <libhpc/system/assign.hh>

template< class Vec >
void
forwarding( hpc::view<std::vector<int>>& tgt,
            Vec&& src )
{
   hpc::assign( tgt, std::forward<Vec>( src ) );
}

TEST_CASE( "/libhpc/system/assign/view" )
{
   std::vector<int> src_vec( 10 );
   hpc::view<std::vector<int>> src( src_vec ), tgt;
   hpc::assign( tgt, src );
   TEST( tgt.data() == src_vec.data() );
   TEST( tgt.size() == src_vec.size() );
}

TEST_CASE( "/libhpc/system/assign/view/forwarding" )
{
   std::vector<int> src_vec( 10 );
   hpc::view<std::vector<int>> src( src_vec ), tgt;
   forwarding( tgt, src );
   TEST( tgt.data() == src_vec.data() );
   TEST( tgt.size() == src_vec.size() );
}

TEST_CASE( "/libhpc/system/assign/view/vector/forwarding" )
{
   std::vector<int> src_vec( 10 );
   hpc::view<std::vector<int>> tgt;
   forwarding( tgt, src_vec );
   TEST( tgt.data() == src_vec.data() );
   TEST( tgt.size() == src_vec.size() );
}

TEST_CASE( "/libhpc/system/assign/view/const_vector" )
{
   std::vector<int> src_vec( 10 );
   hpc::view<std::vector<int>> tgt;
   hpc::assign( tgt, (std::vector<int> const&)src_vec );
   TEST( tgt.data() == src_vec.data() );
   TEST( tgt.size() == src_vec.size() );
}
