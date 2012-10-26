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

#include <stdlib.h>
#include <stdio.h>
#include "libhpc/containers/containers.hh"
#include "omp_file.hh"

#if !defined(NLOG) && defined(_OPENMP)

namespace hpc {
   namespace logging {
      namespace omp {

	 file::file( const std::string& filename,
		     unsigned min_level )
	    : logging::file( filename, min_level ),
	      _base( filename )
	 {
	 }

	 file::~file()
	 {
	 }

	 void
	 file::open()
	 {
	    _tids.clear();
	    _new_line = true;
	 }

	 void
	 file::prefix()
	 {
	    if( _new_line ) {
	       int tid = omp_get_thread_num();
	       _buf << tid << " : " << indent;
	       _new_line = false;
	    }
	 }

	 void
	 file::new_line()
	 {
	    if( visible() )
	    {
	       // Open the file right now.
	       int tid = omp_get_thread_num();
	       std::stringstream ss;
	       ss << _base << std::setfill( '0' ) << std::setw( 5 ) << tid;
	       std::string filename = ss.str();
	       // if( _tids.insert( tid ).second )
	       // 	  remove( _filename.c_str() );
	       std::ofstream file( filename, std::fstream::out | std::fstream::app );

	       // Output.
	       file << _buf.str() << std::endl;

	       // Close the file.
	       _close_file();

	       // Clear buffer.
	       _buf.str( std::string() );
	       _new_line = true;
	    }
	 }

	 void
	 file::_open_file()
	 {

	 }

	 void
	 file::_close_file()
	 {
	    _file.close();
	 }
      }
   }
}

#endif
