#include "datatype.hh"

namespace hpc {
   namespace h5 {

      datatype datatype::native_int( H5T_NATIVE_INT );
      datatype datatype::native_ulong( H5T_NATIVE_ULONG );
      datatype datatype::native_float( H5T_NATIVE_FLOAT );
      datatype datatype::native_double( H5T_NATIVE_DOUBLE );

      datatype::datatype( hid_t id )
      {
	 if(id < -1) {
	    switch(id) {
	       case -2: this->_id = H5T_NATIVE_INT; break;
	       case -3: this->_id = H5T_NATIVE_UINT; break;
	       case -4: this->_id = H5T_NATIVE_LONG; break;
	       case -5: this->_id = H5T_NATIVE_ULONG; break;
	       case -6: this->_id = H5T_NATIVE_FLOAT; break;
	       case -7: this->_id = H5T_NATIVE_DOUBLE; break;
#ifndef NDEBUG
	       default: ASSERT(0);
#endif
	    }
	 }
	 else
	    this->_id = id;
      }

      datatype::~datatype()
      {
      }

      hid_t
      datatype::id() const
      {
	 return this->_id;
      }
   }
}
