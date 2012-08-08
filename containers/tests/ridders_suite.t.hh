#include <cxxtest/TestSuite.h>
#include "libhpc/containers/ridders.hh"

using namespace hpc;

double
f_sq( double x )
{
   return (x + 1.0)*(x + 1.0) - 1.0;
}

class ridders_suite : public CxxTest::TestSuite {
public:

   void test_initial_bracketing()
   {
#ifndef NDEBUG
      TS_ASSERT_THROWS_ANYTHING(num::ridders(f_sq, -10.0, 2.0));
#endif
   }

   void test_quadratic()
   {
      double root = num::ridders(f_sq, -10.0, -0.5);
      TS_ASSERT(root >= -2.0 - num::default_xtol && root <= -2.0 + num::default_xtol);
   }

   void test_tolerance()
   {
      unsigned num_base_its, num_its;
      double cur_xtol = num::default_xtol/100.0;
      double root = num::ridders(f_sq, -10.0, -0.5, cur_xtol, num_base_its);
      TS_ASSERT(root >= -2.0 - cur_xtol && root <= -2.0 + cur_xtol);

      cur_xtol = 10000.0*num::default_xtol;
      root = num::ridders(f_sq, -10.0, -0.5, cur_xtol, num_its);
      TS_ASSERT(root >= -2.0 - cur_xtol && root <= -2.0 + cur_xtol);
      TS_ASSERT(num_its < num_base_its);
   }
};
