#ifndef libhpc_logging_stack_hh
#define libhpc_logging_stack_hh

#include <libhpc/memory/memory.hh>
#include <libhpc/containers/list.hh>
#include <libhpc/containers/shared_ptr.hh>
#include "logger.hh"

#ifndef NLOG

namespace hpc {
   namespace logging {

      ///
      ///
      ///
      class stack
      {
      public:

         stack();

         ~stack();

         void
         push( logger* log );

         void
         pop();

         void
         clear();

         template< class T >
         stack&
         operator<<( const T& obj )
         {
            for( auto& log : _logs )
               (*log) << obj;
            return *this;
         }

      protected:

         list<logger*> _logs;
      };
   }
}

#endif

#endif
