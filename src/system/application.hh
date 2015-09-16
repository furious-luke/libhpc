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

#ifndef libhpc_system_application_hh
#define libhpc_system_application_hh

#include <signal.h>
#include <string>
#include <boost/program_options.hpp>
#include "libhpc/debug.hh"

namespace hpc {
   namespace po = boost::program_options;

   class silent_terminate
      : hpc::exception
   {
   };

   class application
   {
   public:

      application( int argc,
                   char* argv[],
                   std::string const& info = std::string() );

      void
      handle_signal( int sig );

      void
      ignore_signal( int sig );

      virtual
      void
      signaled( int param );

      po::options_description&
      options();

      po::positional_options_description&
      positional_options();

      void
      parse_options( int argc,
                     char* argv[] );

      bool
      has_option( std::string const& opt ) const;

   protected:

      po::options_description _opt_desc;
      po::positional_options_description _pos_opt_desc;
      po::variables_map _vm;
      std::string _app_info;
   };

}

#endif
