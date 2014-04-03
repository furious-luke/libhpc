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
#include "libhpc/logging/logging.hh"

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
                             char* argv[],
                             std::string const& info )
      : _app_info( info )
   {
   }

   void
   application::handle_signal( int sig )
   {
      ::signal( sig, hpc_signaled );
   }

   void
   application::ignore_signal( int sig )
   {
      ::signal( sig, SIG_IGN );
   }

   void
   application::signaled( int param )
   {
      LOGILN( "Recieved signal: ", param );
      EXCEPT( 0, "Recieved signal ", param, ", terminating." );
   }

   po::options_description&
   application::options()
   {
      return _opt_desc;
   }

   po::positional_options_description&
   application::positional_options()
   {
      return _pos_opt_desc;
   }

   void
   application::parse_options( int argc,
                               char* argv[] )
   {
      _opt_desc.add_options()
	 ("help,h", "Show this help");

      po::store( po::command_line_parser( argc, argv ).options( _opt_desc ).positional( _pos_opt_desc ).run(), _vm );
      po::notify( _vm );

      if( _vm.count( "help" ) )
      {
	 if( !_app_info.empty() )
	    std::cout << "\n" << _app_info << "\n\n";
      	 std::cout << _opt_desc << "\n";
	 throw silent_terminate();
      }
   }

}
