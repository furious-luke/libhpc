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

#ifndef libhpc_options_format_hh
#define libhpc_options_format_hh

namespace hpc {
   namespace options {

      class format
      {
      public:

         virtual
         void
         start_list( const hpc::string& name ) = 0;

         virtual
         void
         add_list_item( const hpc::string& value ) = 0;

         virtual
         void
         end_list() = 0;
      };
   }
}

#endif