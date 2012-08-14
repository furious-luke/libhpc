// Copyright 2012 Luke Hodkinson

// This file is part of libhpc.
// 
// libhpc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// libhpc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with libhpc.  If not, see <http://www.gnu.org/licenses/>.

namespace hpc {
   namespace re {

      class re
      {
      public:

         void
         compile( const char* prog )
         {
            if( !prog )
               return;

            const char* cur = prog;
            while( *cur )
            {
               // Exact match.
               if( (*cur >= 65 && *cur <= 90) ||
                   (*cur >= 97 && *cur <= 122) )
               {
               }
            }
         }

         bool
         match( const char* str )
         {
            if( !str )
               return false;

            dfa* state = _start;
            const char* cur = str;
            while( *cur && !state->done() )
               state = state->next( *cur++ );

            return state->matched();
         }

      protected:

         ///
         /// Merge two DFA programs as combined
         ///
         void
         _merge()

      protected:

         list<scoped_ptr<dfa>> _states;
         dfa* _start;
      };
   }
}
