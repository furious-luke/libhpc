#include "command_chain.hh"

namespace tao {
   using namespace hpc;

   void
   chain::add( context& ctx )
   {
      _ctxs.push_back( &ctx );
   }

   bool
   chain::operator()( const string& line )
   {
      for( auto& ctx : _ctxs )
      {
         if( (*ctx)( line ) )
            return true;
      }
      return false;
   }

}
