#include <cxxtest/TestSuite.h>
#include "libhpc/logging/file.hh"

using namespace hpc;

class file_suite : public CxxTest::TestSuite {
public:

   void test_ctor()
   {
#ifndef NLOG
      logging::file log( "test.log" );
#endif
   }
};
