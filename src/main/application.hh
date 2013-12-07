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

#ifndef libhpc_main_application_hh
#define libhpc_main_application_hh

#include <signal.h>

namespace hpc {

   class application
   {
   public:

      application( int argc,
                   char* argv[],
                   std::string const& name = "",
                   std::string const& info = "" );

      void
      handle_signal( int sig );

      void
      ignore_signal( int sig );

      virtual
      void
      signaled( int param );

      bool
      parse_options( int argc,
                     char* argv[] );

   protected:

      options::dictionary _opts;
      std::string _app_name;
      std::string _app_info;
   };

}

#endif
