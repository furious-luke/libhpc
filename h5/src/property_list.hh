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

#ifndef hpc_h5_property_list_hh
#define hpc_h5_property_list_hh

#include "libhpc/hpcmpi/mpi.hh"
#include "dataspace.hh"

namespace hpc {
   namespace h5 {

      class property_list
      {
      public:

	 property_list( hid_t id );

	 ~property_list();

	 hid_t
	 id() const;

	 void
	 create( hid_t id );

	 void
	 close();

	 void
	 set_external( const string& name,
		       hsize_t size,
		       hsize_t offset = 0 );

      protected:

	 hid_t _id;
      };

   }
}

#endif
