#include "command_chain.hh"

namespace hpc {
   namespace command {

      void
      chain::add( context& ctx )
      {
         _ctxs.push_back( &ctx );
      }

      bool
      chain::operator()( std::string const& line )
      {
         for( std::list<context*>::const_iterator it = _ctxs.begin(); it != _ctxs.end(); ++it )
         {
            if( (**it)( line ) )
               return true;
         }
         return false;
      }

   }
}
