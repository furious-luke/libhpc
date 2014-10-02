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

#ifndef libhpc_h5_group_hh
#define libhpc_h5_group_hh

#include "location.hh"

namespace hpc {
   namespace h5 {

      class group
	 : public location
      {
      public:

	 static void
	 split_groups( const std::string& name,
		       std::vector<std::string>& groups );

	 static void
	 split_groups( const std::string& name,
		       std::string& groups );

	 static void
	 create_intermediate( const std::string& name );

	 group();

	 group( location& loc,
		const std::string& name );

	 group( group&& src );

	 ~group();

	 void
	 set_id( hid_t id );

	 void
	 create( location& loc,
		 const std::string& name );

	 void
	 open( location const& loc,
	       std::string const& name );

	 void
	 close();

	 hsize_t
	 size() const;
      };

   }
}

#endif
