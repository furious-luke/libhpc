#include "globals.hh"

#ifndef NLOG

namespace hpc {
   namespace logging {

      stack _stack;

      void
      push( logger* log )
      {
         _stack.push( log );
      }

      void
      pop()
      {
         _stack.pop();
      }

      void
      clear()
      {
         _stack.clear();
      }
   }
}

#endif
