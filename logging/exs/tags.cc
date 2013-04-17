#include <stdio.h>
#include "libhpc/logging/stdout.hh"
#include "libhpc/logging/globals.hh"

using namespace hpc;

int
main( int argc,
      char** argv )
{
   logging::stdout* log = new logging::stdout( 0, "hello" );
   log->add_tag( "world" );
   LOG_PUSH( log );
   LOG_PUSH_TAG( "hello" );
   LOGLN( "Hello world!", setindent( 2 ) );
   LOGLN( "More hellos?", setindent( -2 ) );
   LOG_POP_TAG( "hello" );
   LOGLN( "That'll do." );
   LOG_PUSH_TAG( "world" );
   LOGLN( "Will it?" );
   return EXIT_SUCCESS;
}
