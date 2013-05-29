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

#ifndef libhpc_options_cmd_line_hh
#define libhpc_options_cmd_line_hh

#include <iostream>
#include "libhpc/containers/string.hh"
#include "format.hh"
#include "dictionary.hh"

namespace hpc {
   namespace options {

      class cmd_line
         : public format
      {
      public:

	 static const char* default_equality_symbol;

      public:

	 cmd_line();

	 void
	 parse( dictionary& dict,
		int argc,
		const char* argv[] );

         virtual
         void
         start_list( const hpc::string& name );

         virtual
         void
         add_list_item( const hpc::string& value );

         virtual
         void
         end_list();

      protected:

	 const char*
	 _is_short( const char* opt );

	 const char*
	 _is_long( const char* opt );

      protected:

         hpc::string _sep;
	 hpc::string _eq_sym;
	 hpc::string _short_pre;
	 hpc::string _long_pre;
      };

   }
}

#endif
