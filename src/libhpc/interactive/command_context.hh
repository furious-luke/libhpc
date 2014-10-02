#ifndef libhpc_interactive_command_context_hh
#define libhpc_interactive_command_context_hh

#include "command.hh"

namespace hpc {
   namespace command {

      class context
      {
      public:

         void
         add( std::string const& re,
              command::function_type action );

         bool
         operator()( std::string const& line );

      protected:

         std::vector<command> _cmds;
      };

   }
}

#endif
