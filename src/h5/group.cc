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

#include <boost/algorithm/string.hpp>
#include "group.hh"

namespace hpc {
   namespace h5 {

      void
      group::split_groups( const std::string& name,
			   std::vector<std::string>& groups )
      {
	 groups.clear();
	 if( !name.empty() )
         {
            std::vector<std::string> group_names;
	    boost::split( group_names, name, boost::is_any_of( "/" ) );
	    if( group_names.size() )
            {
               std::vector<std::string> tmp( group_names );
	       assign( group_names, view<std::vector<std::string> >( tmp, tmp.size() - 1 ) );
	    }
	 }
      }

      void
      group::split_groups( std::string const& name,
			   std::string& groups )
      {
         std::vector<std::string> group_names;
	 group::split_groups( name, group_names );
	 boost::join( group_names, "/" );
      }

      group::group()
	 : location()
      {
      }

      group::group( location& loc,
		    const std::string& name )
	 : location()
      {
	 this->create(loc, name);
      }

      group::group( group&& src )
	 : location( src._id )
      {
	 src._id = -1;
      }

      group::~group()
      {
	 this->close();
      }

      void
      group::set_id( hid_t id )
      {
	 this->close();
	 this->_id = id;
      }

      void
      group::create( location& loc,
		     const std::string& name )
      {
	 this->close();
	 this->_id = H5Gcreate2(loc.id(), name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	 ASSERT(this->_id >= 0);
      }

      void
      group::open( location const& loc,
		   std::string const& name )
      {
	 close();
	 _id = H5Gopen2( loc.id(), name.c_str(), H5P_DEFAULT );
	 ASSERT( _id >= 0 );
      }

      void
      group::close()
      {
	 if(this->_id >= 0)
	 {
	    INSIST(H5Gclose(this->_id), >= 0);
	    _id = -1;
	 }
      }

      hsize_t
      group::size() const
      {
	 hsize_t size;
	 INSIST( H5Gget_num_objs( _id, &size ), >= 0 );
	 return size;
      }

   }
}
