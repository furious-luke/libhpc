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

#include "file.hh"

namespace hpc {
   namespace logging {

#ifndef NLOG

      file::file( const std::string& filename,
                  unsigned min_level )
         : logger( min_level ),
           _filename( filename )
      {
      }

      file::~file()
      {
         close();
      }

      void
      file::open()
      {
         close();
         remove( _filename.c_str() );
         _file.open( _filename, std::fstream::out | std::fstream::app );
         _new_line = true;
      }

      void
      file::close()
      {
         if( _file.is_open() )
         {
            _file << _buf.str();
            _file.close();
         }
      }

      void
      file::new_line()
      {
         if( visible() )
         {
            _file << _buf.str() << std::endl;
            _buf.str( std::string() );
            _new_line = true;
         }
      }

#endif
   }
}
