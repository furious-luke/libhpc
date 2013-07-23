#ifndef tao_zen_command_context_hh
#define tao_zen_command_context_hh

#include <libhpc/containers/vector.hh>
#include <libhpc/regexp/multimatch.hh>
#include "command.hh"

namespace tao {
   using namespace hpc;

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

#endif
