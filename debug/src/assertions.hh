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

#ifndef debug_assertions_hh
#define debug_assertions_hh

#ifndef NDEBUG

#include <iostream>
#include <string>
#include <exception>

namespace hpc {
   namespace debug {

      ///
      ///
      ///
      class assertion
         : public std::exception
      {
      public:

         assertion( const char* msg=NULL ) throw();

         assertion( const assertion& ass );

         virtual
         ~assertion() throw();

         void
         details( const char* file,
                  int line,
                  const char* expr ) throw();

         virtual const char*
         what() const throw();

      protected:

         void
         _write_buffer() throw();

      private:

         const char* _file;
         int _line;
         const char* _expr;
         char _msg[1024];
         char _buf[1024];
      };

      ///
      ///
      ///
      class out_of_memory
         : public assertion
      {
      public:

         out_of_memory();
      };

      ///
      ///
      ///
      class invalid_index
         : public assertion
      {
      public:

         invalid_index();
      };
   }
}

#endif

#endif
