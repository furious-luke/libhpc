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

#include "function.hh"

#if !defined( NINSTRUMENT ) || !defined( NSTACKTRACE )

#include <stdlib.h>
#include <string.h>
#include <memory>
#include <list>
#include <dlfcn.h>
#include <cxxabi.h>
#include "func.hh"

namespace hpc {
   namespace debug {

      void
      func_details( void* func_addr,
                    const char** file_name,
                    char** func_name )
      {
         Dl_info info;
         if( dladdr( func_addr, &info ) )
         {
            *file_name = info.dli_fname;
            if( info.dli_sname )
            {
               int status;
               char* demangled = abi::__cxa_demangle( info.dli_sname, NULL, 0, &status );
               if( status == 0 && demangled )
                  *func_name = demangled;
               else
                  *func_name = strdup( info.dli_sname );
            }
            else
               *func_name = NULL;
         }
         else
         {
            *file_name = NULL;
            *func_name = NULL;
         }
      }
   }
}

#endif
