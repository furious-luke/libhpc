#include <cxxtest/TestSuite.h>
#include "libhpc/logging/globals.hh"
#include "libhpc/logging/file.hh"

using namespace hpc;

class globals_suite : public CxxTest::TestSuite {
public:

   void test_push_pop()
   {
#ifndef NLOG
      logging::file* log = new logging::file( "test.log" );
      logging::push( log );
      logging::pop();
#endif
   }

   // void test_clear()
   // {
   //    logging::file log( "test.log" );
   //    logging::push( log );
   //    logging::clear();
   // }

   void test_log()
   {
#ifndef NLOG
      LOG_PUSH( new logging::file( "test1.log" ) );
      LOG_PUSH( new logging::file( "test2.log" ) );
      LOG( "Testing.", logging::endl );
      LOG( "This is on" );
      LOG( " one line." );
      LOG( logging::endl );
      LOG( setindent( 2 ), indent, "Indented.", logging::endl, setindent( -2 ) );
      LOG( indent, "Not indented.", logging::endl );
      LOG( "Also not indented...", logging::endl, setindent( 2 ) );
      LOG( "... but this one is.", logging::endl );
      LOG( setindent( -2 ) );
      LOG_POP();
      LOG_POP();
#endif
   }
};
