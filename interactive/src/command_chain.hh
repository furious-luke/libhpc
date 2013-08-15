#ifndef libhpc_interactive_command_chain_hh
#define libhpc_interactive_command_chain_hh

#include "command_context.hh"

namespace hpc {
   namespace command {

      class chain
      {
      public:

         void
         add( context& ctx );

         bool
         operator()( const string& line );

      protected:

         list<context*> _ctxs;
      };

   }
}

#endif
