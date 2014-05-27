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
#include <libhpc/numerics/clip.hh>

TEST_CASE( "/hpc/numerics/inner_product" )
{
   std::array<int,3> vec1{ 1, 2, 3 }, vec2{ 2, 3, 4 };
   TEST( hpc::inner_product( vec1.begin(), vec1.end(), vec2.begin() ) == 20 );
}

TEST_CASE( "/hpc/numerics/inside" )
{
   std::array<double,3> vec1{ 2, 1, 1 }, vec2{ 0.5, 1, 1 };
   std::array<double,4> hsp{ 1.0/sqrt( 3.0 ), 1/sqrt( 3.0 ), 1/sqrt( 3.0 ), 2 };
   TEST( hpc::inside( vec1.begin(), vec1.end(), hsp.begin() ) == true );
   TEST( hpc::inside( vec2.begin(), vec2.end(), hsp.begin() ) == false );
}

   // void test_half_space_eval()
   // {
   //    array<double,3> vec( 2, 3, 4 );
   //    array<double,4> hsp( 1.0/sqrt( 3.0 ), 1/sqrt( 3.0 ), 1/sqrt( 3.0 ), 2 );
   //    TS_ASSERT_DELTA( half_space_eval( vec.begin(), vec.end(), hsp.begin() ), 3.196, 1e-3 );
   // }

   // ///
   // ///
   // ///
   // void test_line_half_space_intersection()
   // {
   //    array<double,3> vec1( -1, -1, -1 ), vec2( 1, 1, 1 ), vec3;
   //    array<double,4> hsp( 1.0/sqrt( 3.0 ), 1/sqrt( 3.0 ), 1/sqrt( 3.0 ), 2 );
   //    line_half_space_intersection( vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), hsp.begin(), vec3.begin() );
   //    for( auto x : vec3 )
   // 	TS_ASSERT_DELTA( x, 1.1547, 1e-3 );
   // }

   // ///
   // ///
   // ///
   // void test_box_half_space_overlap()
   // {
   //    array<double,3> min( -1, -1, -1 ), max( 1, 1, 1 );
   //    array<double,4> hsp( 1.0/sqrt( 3.0 ), 1/sqrt( 3.0 ), 1/sqrt( 3.0 ), 2 );
   //    bool res = box_half_space_overlap(
   //       min.begin(), min.end(),
   //       max.begin(),
   //       hsp.begin()
   //       );
   //    TS_ASSERT( !res );
   //    hsp[3] = 1;
   //    res = box_half_space_overlap(
   //       min.begin(), min.end(),
   //       max.begin(),
   //       hsp.begin()
   //       );
   //    TS_ASSERT( res );
   //    hsp[3] = -2;
   //    res = box_half_space_overlap(
   //       min.begin(), min.end(),
   //       max.begin(),
   //       hsp.begin()
   //       );
   //    TS_ASSERT( res );
   //    hsp[3] = -1;
   //    res = box_half_space_overlap(
   //       min.begin(), min.end(),
   //       max.begin(),
   //       hsp.begin()
   //       );
   //    TS_ASSERT( res );
   // }

   // ///
   // ///
   // ///
   // void test_box_half_space_intersection()
   // {
   //    array<double,3> min( -1, -1, -1 ), max( 1, 1, 1 );
   //    array<double,4> hsp( 1.0/sqrt( 3.0 ), 1/sqrt( 3.0 ), 1/sqrt( 3.0 ), 2 );
   //    bool res = box_half_space_intersection(
   //       min.begin(), min.end(),
   //       max.begin(),
   //       hsp.begin()
   //       );
   //    TS_ASSERT( !res );
   //    hsp[3] = 1;
   //    res = box_half_space_intersection(
   //       min.begin(), min.end(),
   //       max.begin(),
   //       hsp.begin()
   //       );
   //    TS_ASSERT( res );
   //    hsp[3] = -2;
   //    res = box_half_space_intersection(
   //       min.begin(), min.end(),
   //       max.begin(),
   //       hsp.begin()
   //       );
   //    TS_ASSERT( !res );
   //    hsp[3] = -1;
   //    res = box_half_space_intersection(
   //       min.begin(), min.end(),
   //       max.begin(),
   //       hsp.begin()
   //       );
   //    TS_ASSERT( res );
   // }

   // ///
   // ///
   // ///
   // void test_polygon_area()
   // {
   //    list<array<double,2> > shape;
   //    shape.emplace_back( 0.0, 0.0 );
   //    shape.emplace_back( 2.0, 0.0 );
   //    shape.emplace_back( 2.0, 2.0 );
   //    shape.emplace_back( 0.0, 2.0 );
   //    auto area = polygon_area( shape.begin(), shape.end() );
   //    TS_ASSERT_DELTA( area, 4.0, 1e-6 );
   // }

   // ///
   // ///
   // ///
   // void test_clip_edge()
   // {
   //    list<array<double,2> > shape, result;
   //    array<double,3> left_edge( 1.0, 0.0, 0.0 );
   //    shape.emplace_back( -1.0, 0.0 );
   //    shape.emplace_back( 1.0, 1.0 );
   //    shape.emplace_back( 1.0, 2.0 );
   //    shape.emplace_back( -1.0, 2.0 );
   //    clip_edge(
   // 	 left_edge.begin(),
   // 	 shape.begin(), shape.end(),
   // 	 std::insert_iterator<list<array<double,2> > >( result, result.begin() )
   // 	 );
   // }

   // ///
   // ///
   // ///
   // void test_clip_edge_final_edge()
   // {
   //    list<array<double,2> > shape, result;
   //    array<double,3> left_edge( 1.0, 0.0, 0.0 );
   //    shape.emplace_back( 1.0, 1.0 );
   //    shape.emplace_back( 1.0, 2.0 );
   //    shape.emplace_back( -1.0, 2.0 );
   //    shape.emplace_back( -1.0, 0.0 );
   //    clip_edge(
   // 	 left_edge.begin(),
   // 	 shape.begin(), shape.end(),
   // 	 std::insert_iterator<list<array<double,2> > >( result, result.begin() )
   // 	 );
   // }

   // ///
   // ///
   // ///
   // void test_clip_polygon()
   // {
   //    list<array<double,2> > poly, result;
   //    poly.emplace_back( 1.0, 1.0 );
   //    poly.emplace_back( 1.0, 2.0 );
   //    poly.emplace_back( -1.0, 2.0 );
   //    poly.emplace_back( -1.0, 0.0 );
   //    clip_polygon(
   // 	 1.0,
   // 	 poly.begin(), poly.end(),
   // 	 std::insert_iterator<list<array<double,2> > >( result, result.begin() )
   // 	 );
   // }

   // ///
   // ///
   // ///
   // void test_clip_polygon_full()
   // {
   //    list<array<double,2> > poly, result;
   //    poly.emplace_back( -10.0, -10.0 );
   //    poly.emplace_back( 10.0, -10.0 );
   //    poly.emplace_back( 10.0, 10.0 );
   //    poly.emplace_back( -10.0, 10.0 );
   //    clip_polygon(
   // 	 1.0,
   // 	 poly.begin(), poly.end(),
   // 	 std::insert_iterator<list<array<double,2> > >( result, result.begin() )
   // 	 );
   // }

TEST_CASE( "/hpc/numerics/distance_to_line_2/center" )
{
   boost::array<double,2> ep0{ -1.0, 1.0 }, ep1{ 1.0, 1.0 }, pnt{ 0.0, -1.0 };
   DELTA( hpc::distance_to_line_2( ep0, ep1, pnt ), 2.0, 1e-8 );
   pnt[0] = -0.9;
   DELTA( hpc::distance_to_line_2( ep0, ep1, pnt ), 2.0, 1e-8 );
   pnt[0] = 0.9;
   DELTA( hpc::distance_to_line_2( ep0, ep1, pnt ), 2.0, 1e-8 );
}

TEST_CASE( "/hpc/numerics/distance_to_line_2/left" )
{
   boost::array<double,2> ep0{ -1.0, 1.0 }, ep1{ 1.0, 1.0 }, pnt{ -2.0, -1.0 };
   DELTA( hpc::distance_to_line_2( ep0, ep1, pnt ), sqrt( 5.0 ), 1e-8 );
   pnt[0] = -1.1;
   DELTA( hpc::distance_to_line_2( ep0, ep1, pnt ), sqrt( 4.01 ), 1e-8 );
}

TEST_CASE( "/hpc/numerics/distance_to_line_2/right" )
{
   boost::array<double,2> ep0{ -1.0, 1.0 }, ep1{ 1.0, 1.0 }, pnt{ 3.0, -1.0 };
   DELTA( hpc::distance_to_line_2( ep0, ep1, pnt ), sqrt( 8.0 ), 1e-8 );
   pnt[0] = 1.1;
   DELTA( hpc::distance_to_line_2( ep0, ep1, pnt ), sqrt( 4.01 ), 1e-8 );
}

TEST_CASE( "/hpc/numerics/distance_to_line_2/parallel" )
{
   boost::array<double,2> ep0{ -1.0, 1.0 }, ep1{ 1.0, 1.0 }, pnt{ 3.0, 1.0 };
   DELTA( hpc::distance_to_line_2( ep0, ep1, pnt ), 2.0, 1e-8 );
   pnt[0] = -10;
   DELTA( hpc::distance_to_line_2( ep0, ep1, pnt ), 9.0, 1e-8 );
}

TEST_CASE( "/hpc/numerics/sphere_box_collision" )
{
   std::array<double,3> sph = { 3.0, 3.0, 1.9 };
   std::array<double,2> min, max;

   // Left.
   min[0] = -1.0; min[1] = 2.0; max[0] = 1.0; max[1] = 4.0;
   TEST( hpc::sphere_box_collision( min.begin(), min.end(), max.begin(), sph.begin() ) == false );
   min[0] += 0.5; max[0] += 0.5;
   TEST( hpc::sphere_box_collision( min.begin(), min.end(), max.begin(), sph.begin() ) == true );

   // Left-down.
   min[1] -= 2.5; max[1] -= 2.5;
   TEST( hpc::sphere_box_collision( min.begin(), min.end(), max.begin(), sph.begin() ) == false );
   min[0] += 2.5; max[0] += 2.5;
   TEST( hpc::sphere_box_collision( min.begin(), min.end(), max.begin(), sph.begin() ) == true );

   // Down.
   min[1] -= 0.5; max[1] -= 0.5;
   TEST( hpc::sphere_box_collision( min.begin(), min.end(), max.begin(), sph.begin() ) == false );
   min[1] += 0.5; max[1] += 0.5;
   TEST( hpc::sphere_box_collision( min.begin(), min.end(), max.begin(), sph.begin() ) == true );

   // Down-right.
   min[0] += 2.5; max[0] += 2.5;
   TEST( hpc::sphere_box_collision( min.begin(), min.end(), max.begin(), sph.begin() ) == false );
   min[1] += 2.5; max[1] += 2.5;
   TEST( hpc::sphere_box_collision( min.begin(), min.end(), max.begin(), sph.begin() ) == true );

   // Right.
   min[0] += 0.5; max[0] += 0.5;
   TEST( hpc::sphere_box_collision( min.begin(), min.end(), max.begin(), sph.begin() ) == false );
   min[0] -= 0.5; max[0] -= 0.5;
   TEST( hpc::sphere_box_collision( min.begin(), min.end(), max.begin(), sph.begin() ) == true );

   // Right-up.
   min[1] += 2.5; max[1] += 2.5;
   TEST( hpc::sphere_box_collision( min.begin(), min.end(), max.begin(), sph.begin() ) == false );
   min[0] -= 2.5; max[0] -= 2.5;
   TEST( hpc::sphere_box_collision( min.begin(), min.end(), max.begin(), sph.begin() ) == true );

   // Up.
   min[1] += 0.5; max[1] += 0.5;
   TEST( hpc::sphere_box_collision( min.begin(), min.end(), max.begin(), sph.begin() ) == false );
   min[1] -= 0.5; max[1] -= 0.5;
   TEST( hpc::sphere_box_collision( min.begin(), min.end(), max.begin(), sph.begin() ) == true );

   // Up-left.
   min[0] -= 2.5; max[0] -= 2.5;
   TEST( hpc::sphere_box_collision( min.begin(), min.end(), max.begin(), sph.begin() ) == false );
   min[1] -= 2.5; max[1] -= 2.5;
   TEST( hpc::sphere_box_collision( min.begin(), min.end(), max.begin(), sph.begin() ) == true );
}

TEST_CASE( "/hpc/numerics/ecs_box_collision/max_distance" )
{
   std::array<double,3> ecs_min, ecs_max;
   ecs_min[0] = 0.0;      ecs_min[1] = -0.5*M_PI; ecs_min[2] = 0.0;
   ecs_max[0] = 2.0*M_PI; ecs_max[1] = 0.5*M_PI;  ecs_max[2] = 7050.0;
   std::array<double,3> min, max;
   for( double ra = 0.0; ra <= 2.0*M_PI; ra += 2.0*M_PI/10.0 )
   {
      for( double dec = -0.5*M_PI; dec <= 0.5*M_PI; dec += M_PI/10.0 )
      {
         // Outside.
         hpc::num::ecs_to_cartesian<double>( ra, dec, min[0], min[1], min[2], 7060.0 );
         max[0] = min[0] + 1.0;
         max[1] = min[1] + 1.0;
         max[2] = min[2] + 1.0;
         TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );

         // On rim.
         hpc::num::ecs_to_cartesian<double>( ra, dec, min[0], min[1], min[2], 7040.0 );
         max[0] = min[0] + 20.0;
         max[1] = min[1] + 20.0;
         max[2] = min[2] + 20.0;
         TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == true );

         // Inside.
         hpc::num::ecs_to_cartesian<double>( ra, dec, min[0], min[1], min[2], 7040.0 );
         max[0] = min[0] + 1.0;
         max[1] = min[1] + 1.0;
         max[2] = min[2] + 1.0;
         TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == true );
      }
   }
}

TEST_CASE( "/hpc/numerics/ecs_box_collision/min_distance" )
{
   std::array<double,3> ecs_min, ecs_max;
   ecs_min[0] = 0.0;      ecs_min[1] = -0.5*M_PI; ecs_min[2] = 7050.0;
   ecs_max[0] = 2.0*M_PI; ecs_max[1] = 0.5*M_PI;  ecs_max[2] = 20000.0;
   std::array<double,3> min, max;
   for( double ra = 0.0; ra <= 2.0*M_PI; ra += 2.0*M_PI/10.0 )
   {
      for( double dec = -0.5*M_PI; dec <= 0.5*M_PI; dec += M_PI/10.0 )
      {
         // Outside.
         hpc::num::ecs_to_cartesian<double>( ra, dec, min[0], min[1], min[2], 7040.0 );
         max[0] = min[0] + 1.0;
         max[1] = min[1] + 1.0;
         max[2] = min[2] + 1.0;
         TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );

         // Inside.
         hpc::num::ecs_to_cartesian<double>( ra, dec, min[0], min[1], min[2], 7060.0 );
         max[0] = min[0] + 1.0;
         max[1] = min[1] + 1.0;
         max[2] = min[2] + 1.0;
         TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == true );
      }
   }
}

TEST_CASE( "/hpc/numerics/ecs_box_collision/min_ra" )
{
   std::array<double,3> ecs_min, ecs_max;
   ecs_min[0] = 0.25*M_PI; ecs_min[1] = -0.5*M_PI; ecs_min[2] = 0.0;
   ecs_max[0] = 2.0*M_PI;  ecs_max[1] = 0.5*M_PI;  ecs_max[2] = 7050.0;
   std::array<double,3> min, max;

   // First quadrant.
   min[0] = 1.0; min[1] = 0.1; min[2] = 0.5;
   max[0] = 2.0; max[1] = 0.9; max[2] = 0.9;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );
   max[1] = 1.1;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == true );

   // Third quadrant.
   ecs_min[0] = 1.25*M_PI;
   min[0] = -2.0; min[1] = -0.9; min[2] = -0.9;
   max[0] = -1.0; max[1] =  0.1; max[2] = -0.5;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );
   min[1] = -1.1;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == true );
}

TEST_CASE( "/tao/base/lightcone/overlap/max_ra" )
{
   std::array<double,3> ecs_min, ecs_max;
   ecs_min[0] = 0.0;       ecs_min[1] = -0.5*M_PI; ecs_min[2] = 0.0;
   ecs_max[0] = 0.25*M_PI; ecs_max[1] = 0.5*M_PI;  ecs_max[2] = 7050.0;
   std::array<double,3> min, max;

   // First quadrant.
   min[0] = 0.0; min[1] = 1.0; min[2] = 0.5;
   max[0] = 0.9; max[1] = 2.0; max[2] = 0.9;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );
   max[0] = 1.1;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == true );

   // Third quadrant.
   ecs_max[0] = 1.25*M_PI;
   min[0] = -1.0; min[1] = -2.1; min[2] = -0.9;
   max[0] =  0.0; max[1] = -1.1; max[2] = -0.5;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );
   max[1] = -0.9;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == true );
}

TEST_CASE( "/hpc/numerics/ecs_box_collision/max_dec" )
{
   std::array<double,3> ecs_min, ecs_max;
   ecs_min[0] = 0.0;      ecs_min[1] = -0.5*M_PI; ecs_min[2] = 0.0;
   ecs_max[0] = 2.0*M_PI; ecs_max[1] = 0.25*M_PI; ecs_max[2] = 7050.0;
   std::array<double,3> min, max;

   // max_dec > 0
   min[0] = -0.9; min[1] = -0.01; min[2] = 1.1;
   max[0] =  0.9; max[1] =  0.01; max[2] = 2.1;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );
   min[2] = 0.9;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == true );

   // max_dec < 0
   ecs_max[1] = -0.25*M_PI;
   min[0] = 1.1; min[1] = -1.0; min[2] = -1.0;
   max[0] = 2.0; max[1] =  1.0; max[2] =  2.0;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );
   min[0] = 0.9;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == true );
}

TEST_CASE( "/hpc/numerics/ecs_box_collision/min_dec" )
{
   std::array<double,3> ecs_min, ecs_max;
   ecs_min[0] = 0.0;      ecs_min[1] = -0.5*M_PI; ecs_min[2] = 0.0;
   ecs_max[0] = 2.0*M_PI; ecs_max[1] = 0.25*M_PI; ecs_max[2] = 7050.0;
   std::array<double,3> min, max;

   // min_dec < 0
   ecs_min[1] = -0.25*M_PI; ecs_max[1] = 0.5*M_PI;
   min[0] = -0.9; min[1] = -0.01; min[2] = -2.1;
   max[0] =  0.9; max[1] =  0.01; max[2] = -1.1;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );
   max[2] = -0.9;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == true );

   // min_dec > 0
   ecs_min[1] = 0.25*M_PI; ecs_max[1] = 0.5*M_PI;
   min[0] = 1.1; min[1] = -1.0; min[2] = -2.0;
   max[0] = 2.0; max[1] =  1.0; max[2] =  1.0;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );
   min[0] = 0.9;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == true );
}

TEST_CASE( "/hpc/numerics/ecs_box_collision/corner_at_origin" )
{
   std::array<double,3> ecs_min, ecs_max;
   ecs_min[0] = 0.0;       ecs_min[1] = 0.0;       ecs_min[2] = 0.0;
   ecs_max[0] = 0.25*M_PI; ecs_max[1] = 0.25*M_PI; ecs_max[2] = 100.0;
   std::array<double,3> min, max;

   // RA.
   min[0] = -1.0; min[1] = 0.0; min[2] = 0.0;
   max[0] =  0.0; max[1] = 1.0; max[2] = 1.0;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );
   max[0] = 0.001;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == true );

   // DEC.
   min[0] = -1.0; min[1] = -0.5; min[2] = 0.0;
   max[0] =  0.0; max[1] =  0.5; max[2] = 1.0;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );
   max[0] = 0.001;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == true );
}

TEST_CASE( "/hpc/numerics/ecs_box_collision/clip_min_dist" )
{
   std::array<double,3> ecs_min, ecs_max;
   ecs_min[0] = 0.0;       ecs_min[1] = -0.25*M_PI; ecs_min[2] = 10.0;
   ecs_max[0] = 0.25*M_PI; ecs_max[1] =  0.25*M_PI; ecs_max[2] = 100.0;
   std::array<double,3> min, max;

   min[0] = -20.0; min[1] = -20.0; min[2] = -2.0;
   max[0] =   2.0; max[1] =   2.0; max[2] =  2.0;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );
   max[0] = 10.0;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == true );
}

TEST_CASE( "/hpc/numerics/ecs_box_collision/clip_column" )
{
   std::array<double,3> ecs_min, ecs_max;
   ecs_min[0] = 0.0;      ecs_min[1] = -0.5*M_PI;  ecs_min[2] = 10.0;
   ecs_max[0] = 2.0*M_PI; ecs_max[1] =  0.25*M_PI; ecs_max[2] = 100.0;
   std::array<double,3> min, max;

   // Upper.
   min[0] = -1.0; min[1] = -1.0; min[2] =  -5.0;
   max[0] =  1.0; max[1] =  1.0; max[2] = 100.0;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );
   max[0] = 7.0;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );
   max[0] = 8.0;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == true );
   ecs_min[1] = -0.3*M_PI;
   max[0] = 1.0;
   min[2] = -100.0;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );
   max[0] = 7.0;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == true );

   // Lower.
   ecs_min[1] = -0.25*M_PI; ecs_max[1] = 0.5*M_PI;
   min[0] = -1.0; min[1] = -1.0; min[2] = -100.0;
   max[0] =  1.0; max[1] =  1.0; max[2] =    5.0;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );
   max[0] = 7.0;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );
   max[0] = 8.0;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == true );
   ecs_max[1] = 0.3*M_PI;
   max[0] = 1.0;
   max[2] = 100.0;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );
   max[0] = 7.0;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == true );
}

TEST_CASE( "/hpc/numerics/ecs_box_collision/huge_box" )
{
   std::array<double,3> ecs_min, ecs_max;
   ecs_min[0] = 0.0;      ecs_min[1] = -0.5*M_PI;  ecs_min[2] = 1.0;
   ecs_max[0] = 2.0*M_PI; ecs_max[1] =  0.25*M_PI; ecs_max[2] = 10.0;
   std::array<double,3> min, max;

   min[0] = -1000.0; min[1] = -1000.0; min[2] = -1000.0;
   max[0] =  1000.0; max[1] =  1000.0; max[2] = 1000.0;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == true );
   max[0] = -1100.0;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );
}

TEST_CASE( "/hpc/numerics/ecs_box_collision/special_1" )
{
   std::array<double,3> ecs_min, ecs_max;
   ecs_min[0] = 0.0;        ecs_min[1] = 0.0;        ecs_min[2] = 0.0;
   ecs_max[0] = M_PI/180.0; ecs_max[1] = M_PI/180.0; ecs_max[2] = 293.0;
   std::array<double,3> min, max;
   min[0] = 0.0;  min[1] = 0.0;  min[2] = 62.5;
   max[0] = 62.5; max[1] = 62.5; max[2] = 125.0;
   TEST( hpc::ecs_box_collision( ecs_min, ecs_max, min, max ) == false );
}
