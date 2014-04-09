#include <boost/regex.hpp>
#include "libhpc/logging.hh"
#include "command_context.hh"

namespace hpc {
   namespace command {

      void
      context::add( std::string const& expr,
                    command::function_type action )
      {
         _cmds.emplace_back( expr, action );
      }

      bool
      context::operator()( std::string const& line )
      {
         boost::smatch match;
         for( auto const& cmd : _cmds )
         {
            if( boost::regex_match( line, match, cmd.re() ) )
            {
               cmd( match );
               return true;
            }
         }
         return false;
      }

   }
}
