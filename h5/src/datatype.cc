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

      datatype datatype::native_char( H5T_NATIVE_CHAR );
      datatype datatype::native_int( H5T_NATIVE_INT );
      datatype datatype::native_ulong( H5T_NATIVE_ULONG );
      datatype datatype::native_llong( H5T_NATIVE_LLONG );
      datatype datatype::native_float( H5T_NATIVE_FLOAT );
      datatype datatype::native_double( H5T_NATIVE_DOUBLE );
      datatype datatype::std_i32be( H5T_STD_I32BE );
      datatype datatype::std_i64be( H5T_STD_I64BE );
      datatype datatype::ieee_f32be( H5T_IEEE_F32BE );
      datatype datatype::ieee_f64be( H5T_IEEE_F64BE );

      datatype::datatype( hid_t id )
      {
	 if(id < -1) {
	    switch(id) {
	       case -2: _id = H5T_NATIVE_INT; break;
	       case -3: _id = H5T_NATIVE_UINT; break;
	       case -4: _id = H5T_NATIVE_LONG; break;
	       case -5: _id = H5T_NATIVE_ULONG; break;
	       case -6: _id = H5T_NATIVE_LLONG; break;
	       case -7: _id = H5T_NATIVE_FLOAT; break;
	       case -8: _id = H5T_NATIVE_DOUBLE; break;
	       case -9: _id = H5T_NATIVE_CHAR; break;
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
         if( _id > -1 && (_id != H5T_NATIVE_CHAR &&
			  _id != H5T_NATIVE_INT &&
                          _id != H5T_NATIVE_UINT &&
                          _id != H5T_NATIVE_LONG &&
                          _id != H5T_NATIVE_ULONG &&
                          _id != H5T_NATIVE_LLONG &&
                          _id != H5T_NATIVE_FLOAT &&
                          _id != H5T_NATIVE_DOUBLE &&
                          _id != H5T_STD_I32BE &&
                          _id != H5T_STD_I64BE &&
                          _id != H5T_IEEE_F32BE &&
                          _id != H5T_IEEE_F64BE) )
         {
            INSIST( H5Tclose( _id ), >= 0 );
         }
         _id = -1;
      }

      size_t
      datatype::size() const
      {
         size_t size = H5Tget_size( _id );
         ASSERT( size > 0 );
         return size;
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
