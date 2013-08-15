#ifndef libhpc_interactive_command_context_hh
#define libhpc_interactive_command_context_hh

#include "libhpc/containers/vector.hh"
#include "libhpc/regexp/multimatch.hh"
#include "command.hh"

namespace hpc {
   namespace command {

      class context
      {
      public:

         void
         add( const string& re,
              command::function_type action );

         bool
         operator()( const string& line );

      protected:

         vector<command> _cmds;
      };

   }
}

#endif
