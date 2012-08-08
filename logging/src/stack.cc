#include "stack.hh"

#ifndef NLOG

namespace hpc {
   namespace logging {

      stack::stack()
      {
      }

      stack::~stack()
      {
      }

      void
      stack::push( logger* log )
      {
         log->open();
         _logs.push_back( log );
      }

      void
      stack::pop()
      {
         _logs.back()->close();
         delete _logs.back();
         _logs.pop_back();
      }

      void
      stack::clear()
      {
         for( auto log : _logs )
         {
            log->close();
            delete log;
         }
         _logs.clear();
      }
   }
}

#endif
