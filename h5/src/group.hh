#ifndef hpc_h5_group_hh
#define hpc_h5_group_hh

#include "libhpc/containers/containers.hh"
#include "location.hh"

namespace hpc {
   namespace h5 {

      class group
	 : public location
      {
      public:

	 static void
	 split_groups( const std::string& name,
		       vector<std::string>& groups );

	 static void
	 split_groups( const std::string& name,
		       std::string& groups );

	 static void
	 create_intermediate( const std::string& name );

	 group();

	 group( location& loc,
		const std::string& name );

	 ~group();

	 void
	 set_id( hid_t id );

	 void
	 create( location& loc,
		 const std::string& name );

	 void
	 open( location& loc,
	       const std::string& name );

	 void
	 close();
      };
   }
}

#endif
