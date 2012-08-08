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

#include "location.hh"
#include "group.hh"

namespace hpc {
   namespace h5 {

      location::location( hid_t id )
	 : _id(id)
      {
      }

      location::~location()
      {
      }

      hid_t
      location::id() const
      {
	 return this->_id;
      }

      bool
      location::has_link( const std::string& name ) const
      {
	 htri_t res = H5Lexists(this->_id, name.c_str(), H5P_DEFAULT);
	 ASSERT(res >= 0);
	 return res;
      }

      void
      location::open_group( const std::string& name,
			    h5::group& group ) const
      {
	 hid_t id = H5Gopen(this->_id, name.c_str(), H5P_DEFAULT);
	 ASSERT(this->_id >= 0);
	 group.set_id(id);
      }

      void
      location::create_group( const std::string& name )
      {
	 h5::group group;
	 group.create(*this, name);
      }
   }
}
