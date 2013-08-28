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

#if !defined( NDEBUG ) || !defined( NEXCEPT )

#include <sstream>
#include "assertions.hh"
#include "omp_help.hh"

namespace hpc {
   namespace debug {

      assertion::assertion( const char* expr,
                            const char* file,
                            int line,
#ifndef NSTACKTRACE
                            const stacktrace& st,
#endif
                            const std::string msg ) throw()
         : exception(),
           _expr( expr ),
           _file( file ),
           _line( line ),
#ifndef NSTACKTRACE
           _st( st ),
#endif
           _msg( msg )
      {
         _write_buffer( _buf );
      }

      assertion::assertion( const assertion& asrt )
         : exception( asrt ),
           _file( asrt._file ),
           _line( asrt._line ),
           _expr( asrt._expr ),
           _msg( asrt._msg ),
           _buf( asrt._buf )
      {
      }

      assertion::~assertion() throw()
      {
      }

      void
      assertion::details( const char* file,
                          int line,
                          const char* expr
#ifndef NSTACKTRACE
                          , const stacktrace& st
#endif
         ) throw()
      {
         _file = file;
         _line = line;
         _expr = expr;
#ifndef NSTACKTRACE
         _st = st;
#endif

         // Need to do this here because the "what" method
         // is const.
         _write_buffer( _buf );
      }

      const char*
      assertion::what() const throw()
      {
         return _buf.c_str();
      }

      const std::string&
      assertion::message() const
      {
         return _msg;
      }

      void
      assertion::_write_buffer( std::string& buf ) throw()
      {
         std::stringstream ss;
         ss << "\n\nFile:  " << _file << "\n";
         ss << "Line:  " << _line << "\n";
         ss << "Expr:  " << _expr << "\n";
#ifdef _OPENMP
         ss << "Thread ID: " << OMP_TID << "\n";
#endif
#ifndef NSTACKTRACE
         ss << "Stack trace:\n";
         for( const auto& level : _st )
            ss << "  " << level.func_name << "\n";
#endif
         ss << "\n" << _msg << "\n";
         buf = ss.str();
      }

   }

   exception::exception( const char* expr,
                         const char* file,
                         int line,
#ifndef NSTACKTRACE
                         const stacktrace& st,
#endif
                         const std::string msg ) throw()
      : debug::assertion( expr, file, line,
#ifndef NSTACKTRACE
                          st,
#endif
                          msg )
   {
   }

}

#endif
