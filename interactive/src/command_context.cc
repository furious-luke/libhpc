#include <libhpc/logging/logging.hh>
#include "command_context.hh"

namespace tao {
   using namespace hpc;

   void
   context::add( const string& re,
                 command::function_type action )
   {
      _cmds.emplace_back( re, action );
   }

   bool
   context::operator()( const string& line )
   {
      re::match match;
      for( const auto& cmd : _cmds )
      {
         if( cmd.re().match( line, match ) )
         {
            cmd( match );
            return true;
         }
      }
      return false;
   }

}
