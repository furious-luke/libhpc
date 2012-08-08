#include <boost/algorithm/string.hpp>
#include "group.hh"

namespace hpc {
   namespace h5 {

      void
      group::split_groups( const std::string& name,
			   vector<std::string>& groups )
      {
	 groups.clear();
	 if(!name.empty()) {
	    vector<std::string> group_names;
	    boost::split(group_names, name, boost::is_any_of("/"));
	    if(group_names.size()) {
	       vector<std::string> tmp(group_names);
	       group_names.duplicate(vector<std::string>::view(tmp, tmp.size() - 1));
	    }
	 }
      }

      void
      group::split_groups( const std::string& name,
			   std::string& groups )
      {
	 vector<std::string> group_names;
	 group::split_groups(name, group_names);
	 boost::join(group_names, "/");
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
      group::open( location& loc,
		   const std::string& name )
      {
	 this->close();
	 this->_id = H5Gopen(loc.id(), name.c_str(), H5P_DEFAULT);
	 ASSERT(this->_id >= 0);
      }

      void
      group::close()
      {
	 if(this->_id >= 0)
	    INSIST(H5Gclose(this->_id), >= 0);
      }
   }
}
