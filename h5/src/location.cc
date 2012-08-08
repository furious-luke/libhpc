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
