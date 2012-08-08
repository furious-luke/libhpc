#include <numeric>
#include <cxxtest/TestSuite.h>
#include "libhpc/containers/range_map.hh"

using namespace hpc;

class range_map_suite : public CxxTest::TestSuite {
public:

   void test_default_ctor()
   {
      range_map<int, int> map;
      TS_ASSERT_EQUALS( map.size(), 0 );
      TS_ASSERT_EQUALS( map.empty(), true );
   }

   void test_insert_upper_bound()
   {
      range_map<int, int> map;
      map.insert( range<int>( 0, 20 ), 0 );
      map.insert( range<int>( 0, 5 ), 1 );
      map.insert( range<int>( 5, 10 ), 2 );
      map.insert( range<int>( 10, 15 ), 3 );
      map.insert( range<int>( 20, 25 ), 4 );
      map.insert( range<int>( 8, 18 ), 5 );
   }

   void test_invert()
   {
      range_map<int, int> mapping;
      mapping.insert( range<int>( 0, 20 ), 0 );
      mapping.insert( range<int>( 0, 5 ), 1 );
      mapping.insert( range<int>( 5, 10 ), 2 );
      mapping.insert( range<int>( 10, 15 ), 3 );
      mapping.insert( range<int>( 20, 25 ), 4 );
      mapping.insert( range<int>( 8, 18 ), 5 );

      map<int, set<range<int>>> inv;
      invert_mapping( mapping, inv );
   }

   void test_medium()
   {
      range_map<int, int> mapping;
      for( unsigned ii = 0; ii < 200; ++ii )
         mapping.insert( range<int>( 0, 10*(ii + 1) ), ii );

      map<int, set<range<int>>> inv;
      invert_mapping( mapping, inv );
   }

   void setUp()
   {
   }
};
