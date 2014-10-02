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

#include "instrumentation.hh"

#ifndef NINSTRUMENT

#include <stdlib.h>
#include <string.h>
#include <memory>
#include <list>
#include <dlfcn.h>
#include <cxxabi.h>
#include "assert.hh"
#include "omp_help.hh"
#include "func.hh"

extern "C" {

   extern int main( int argc, char** argv );
   bool __instr_ready = false;

   struct instr_enabled_node
   {
      instr_enabled_node( instr_enabled_node* next = NULL )
         : tid( OMP_TID ),
           enabled( true ),
           next( next )
      {
      }

      int tid;
      bool enabled;
      instr_enabled_node* next;
   };

   instr_enabled_node instr_enabled;

   struct instr_node
   {
      instr_node( hpc::debug::instrument* instr,
                  instr_node* next = NULL )
         : instr( instr ),
           next( next ),
           ignore( true )
      {
      }

      ~instr_node()
      {
         delete next;
      }

      hpc::debug::instrument* instr;
      instr_node* next;
      bool ignore;
   };

   instr_node* instr_head = NULL;

   void
   add_instrument( hpc::debug::instrument& instr )
   {
      instr_head = new instr_node( &instr, instr_head );
   }

   void
   instr_func_details( void* func_addr,
                       const char** file_name,
                       char** func_name )
   {
      hpc::debug::func_details( func_addr, file_name, func_name );
   }

   instr_enabled_node&
   instr_get_enabled_node()
   {
      instr_enabled_node* cur = &instr_enabled;
      while( cur && cur->tid != OMP_TID )
         cur = cur->next;
      if( !cur )
      {
         instr_enabled.next = new instr_enabled_node( instr_enabled.next );
         cur = instr_enabled.next;
      }
      return *cur;
   }

   bool
   instr_is_enabled()
   {
      instr_enabled_node& node = instr_get_enabled_node();
      return node.enabled;
   }

   void
   instr_set_enabled( bool enabled )
   {
      instr_enabled_node& node = instr_get_enabled_node();
      node.enabled = enabled;
   }

   void
   __cyg_profile_func_enter( void* func_addr,
                             void* call_site )
   {
      // Don't profile anything until we've hit main.
      if( !__instr_ready )
      {
         if( func_addr == &main )
            __instr_ready = true;
         return;
      }

      // Don't profile if this thread is disabled.
      if( !instr_is_enabled() )
         return;
      instr_set_enabled( false );

      const char* file_name;
      char* func_name;
      instr_func_details( func_addr, &file_name, &func_name );
      instr_node* cur = instr_head;
      while( cur && cur->instr )
      {
         cur->instr->enter( file_name, func_name );
         cur = cur->next;
      }
      if( func_name )
         free( func_name );

      instr_set_enabled( true );
   }

   void
   __cyg_profile_func_exit( void* func_addr,
                            void* call_site )
   {
      // Don't profile until we're declared as ready.
      if( !__instr_ready )
         return;

      // Don't profile if this thread is disabled.
      if( !instr_is_enabled() )
         return;
      instr_set_enabled( false );

      const char* file_name;
      char* func_name;
      instr_func_details( func_addr, &file_name, &func_name );
      instr_node* cur = instr_head;
      while( cur && cur->instr )
      {
         // If this instrument is set as ignored, it means it has
         // just been created and shouldn't be run this time.
         if( !cur->ignore )
            cur->instr->exit( file_name, func_name );
         else
            cur->ignore = false;

         cur = cur->next;
      }
      if( func_name )
         free( func_name );

      instr_set_enabled( true );
   }
}

namespace hpc {
   namespace debug {

      instrument::instrument()
      {
         add_instrument( *this );
      }
   }
}

#endif
