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

#ifndef hpc_unit_test_fixtures_hh
#define hpc_unit_test_fixtures_hh

#define SUITE_FIXTURE( type ) hpc::test::suite_fixture<type>

namespace hpc {
   namespace test {

      template< class SuperT >
      class suite_fixture
      {
      public:

	 suite_fixture()
	    : _fix( 0 )
	 {
	 }

	 // TODO: May need to delete SuperT before destructor.
	 ~suite_fixture()
	 {
	    if( _fix )
	    {
	       delete _fix;
	       _fix = 0;
	    }
	 }

	 SuperT*
	 operator->()
	 {
	    if( !_fix )
	       _fix = new SuperT();
	    return _fix;
	 }

      protected:

	 SuperT* _fix;
      };

   }
}

#endif
