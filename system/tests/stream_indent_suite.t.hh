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

#include <string.h>
#include <map>
#include <ostream>
#include <fstream>
#include <cxxtest/TestSuite.h>
#include "libhpc/system/stream_indent.hh"

using namespace hpc;

namespace hpc {
   namespace impl {
      extern std::map<std::ostream*,int> curindent;
   }
}

class stream_indent_suite : public CxxTest::TestSuite {
public:

   void test_set_indent()
   {
      std::string filename = tmpnam( NULL );
      std::ofstream file( filename, std::fstream::out | std::fstream::app );

      TS_ASSERT( impl::curindent.find( (std::ostream*)&file ) == impl::curindent.end() );
      file << setindent( 2 );
      TS_ASSERT( impl::curindent.find( (std::ostream*)&file ) != impl::curindent.end() );
      TS_ASSERT_EQUALS( impl::curindent[(std::ostream*)&file], 2 );
      file << setindent( -1 );
      TS_ASSERT( impl::curindent.find( (std::ostream*)&file ) != impl::curindent.end() );
      TS_ASSERT_EQUALS( impl::curindent[(std::ostream*)&file], 1 );
      file << setindent( -1 );
      TS_ASSERT( impl::curindent.find( (std::ostream*)&file ) == impl::curindent.end() );

      file.close();
      remove( filename.c_str() );
   }

   void test_indent()
   {
      std::string filename = tmpnam( NULL );
      std::fstream file( filename, std::fstream::out | std::fstream::app );

      file << indent << "0" << std::endl;
      file << setindent( 2 ) << indent << "2" << std::endl;
      file << setindent( -2 ) << indent << "0" << std::endl;

      file.close();
      file.open( filename, std::fstream::in );
      char buf[100];
      file.getline( buf, 100 );
      TS_ASSERT( !strcmp( buf, "0" ) );
      file.getline( buf, 100 );
      TS_ASSERT( !strcmp( buf, "  2" ) );
      file.getline( buf, 100 );
      TS_ASSERT( !strcmp( buf, "0" ) );

      file.close();
      remove( filename.c_str() );
   }
};
