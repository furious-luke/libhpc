#include <boost/regex.hpp>
#include "libhpc/logging.hh"
#include "command_context.hh"

namespace hpc {
   namespace command {

      void
      context::add( std::string const& expr,
                    command::function_type action )
      {
         _cmds.push_back( command( expr, action ) );
      }

      bool
      context::operator()( std::string const& line )
      {
         boost::smatch match;
         for( std::vector<command>::const_iterator it = _cmds.begin(); it != _cmds.end(); ++it )
         {
            if( boost::regex_match( line, match, it->re() ) )
            {
               (*it)( match );
               return true;
            }
         }
         return false;
      }

   }
}
