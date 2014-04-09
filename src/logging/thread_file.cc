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

#if !defined(NLOG) && !defined(NTHREAD)

#include <stdlib.h>
#include <stdio.h>
#include "thread_file.hh"

namespace hpc {
   namespace log {
      namespace thread {

	 file::file( std::string const& filename,
		     unsigned min_level )
	    : log::file( filename, min_level ),
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
	    _get_new_line() = true;
	 }

	 void
	 file::write()
	 {
            // Open the file right now.
            std::thread::id tid = std::this_thread::get_id();
            std::stringstream ss;
            ss << _base << tid;
            std::string filename = ss.str();
            _write.lock();
            if( _tids.insert( tid ).second )
               remove( filename.c_str() );
            _write.unlock();
            std::ofstream file( filename, std::fstream::out | std::fstream::app );

            // Output.
            file << buffer().str();
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
