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

#include "application.hh"
#include "libhpc/debug/except.hh"

namespace hpc {

   // Store a global reference to the application for
   // signal handlers.
   application* global_app = nullptr;

   ///
   /// Handle signals.
   ///
   void
   hpc_signaled( int param )
   {
      if( global_app )
         global_app->signaled( param );
   }

   application::application( int argc,
                             char* argv[] )
   {
   }

   void
   application::handle_signal( int sig )
   {
      ::signal( sig, hpc_signaled );
   }

   void
   application::signaled( int param )
   {
      EXCEPT( 0, "Recieved signal ", param, ", terminating." );
   }

}
