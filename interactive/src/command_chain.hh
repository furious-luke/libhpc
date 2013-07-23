#ifndef tao_zen_command_chain_hh
#define tao_zen_command_chain_hh

#include "command_context.hh"

namespace tao {
   using namespace hpc;

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

#endif
