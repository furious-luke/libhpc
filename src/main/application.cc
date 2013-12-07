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
                             std::string const& name,
                             std::string const& info )
      : _app_name( name ),
        _app_info( info )
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

   void
   application::parse_options( int argc,
                               char* argv[] )
   {
      // Be sure the options dictionary is compiled.
      _opts.compile();

      // Parse the command line options.
      options::parse<options::cmd_line>( _opts, argc, (const char**)argv );
      auto cfg = _opts.opt<string>( "config-file" );
      if( cfg )
         options::parse<options::xml>( _opts, *cfg );

      // If the user asked for help print it out.
      if( _opts.help_requested() )
      {
         if( !_app_info.empty() )
            std::cout << "\n" << _app_info << "\n";
         std::cout << "\n";
         _opts.print_help( _app_name );
         std::cout << "\n";
         throw silent_terminate();
      }

      // If not, then show any errors.
      else if( _opts.has_errors() )
      {
         _opts.print_errors();
         throw silent_terminate();
      }

      // If all else is okay, check if we wanted to dump config.
      else if( _opts.config_requested() )
      {
         _opts.generate_config<options::xml>();
         throw silent_terminate();
      }
   }

}
