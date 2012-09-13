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

#include "datatype.hh"

namespace hpc {
   namespace h5 {

      datatype datatype::native_int( H5T_NATIVE_INT );
      datatype datatype::native_ulong( H5T_NATIVE_ULONG );
      datatype datatype::native_float( H5T_NATIVE_FLOAT );
      datatype datatype::native_double( H5T_NATIVE_DOUBLE );
      datatype datatype::ieee_f64be( H5T_IEEE_F64BE );

      datatype::datatype( hid_t id )
      {
	 if(id < -1) {
	    switch(id) {
	       case -2: _id = H5T_NATIVE_INT; break;
	       case -3: _id = H5T_NATIVE_UINT; break;
	       case -4: _id = H5T_NATIVE_LONG; break;
	       case -5: _id = H5T_NATIVE_ULONG; break;
	       case -6: _id = H5T_NATIVE_FLOAT; break;
	       case -7: _id = H5T_NATIVE_DOUBLE; break;
#ifndef NDEBUG
	       default: ASSERT(0);
#endif
	    }
	 }
	 else
	    _id = id;
      }

      datatype::~datatype()
      {
         close();
      }

      void
      datatype::compound( size_t size )
      {
         close();
         _id = H5Tcreate( H5T_COMPOUND, size );
         ASSERT( _id > 0 );
      }

      void
      datatype::close()
      {
         if( _id > 0 )
            INSIST( H5Tclose( _id ), >= 0 );
         _id = 0;
      }

      void
      datatype::insert( const datatype& type,
                        const string& description,
                        size_t offset )
      {
         INSIST( H5Tinsert( _id, description.c_str(), offset, type.id() ), >= 0 );
      }

      hid_t
      datatype::id() const
      {
	 return _id;
      }
   }
}
