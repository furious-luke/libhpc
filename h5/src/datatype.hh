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

#ifndef libhpc_h5_datatype_hh
#define libhpc_h5_datatype_hh

#include <boost/mpl/map.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/at.hpp>
#include "libhpc/hpcmpi/mpi.hh"
#include <hdf5.h>

namespace mpl = boost::mpl;

namespace hpc {
   namespace h5 {

      class datatype
      {
      public:

	 static datatype native_char;
	 static datatype native_int;
	 static datatype native_ulong;
	 static datatype native_llong;
	 static datatype native_ullong;
	 static datatype native_float;
	 static datatype native_double;
	 static datatype string;
         static datatype std_i32be;
         static datatype std_i64be;
         static datatype ieee_f32be;
         static datatype ieee_f64be;

	 typedef mpl::map< mpl::pair<int, mpl::int_<-2> >,
			   mpl::pair<unsigned, mpl::int_<-3> >,
			   mpl::pair<long, mpl::int_<-4> >,
			   mpl::pair<unsigned long, mpl::int_<-5> >,
			   mpl::pair<long long, mpl::int_<-6> >,
			   mpl::pair<unsigned long long, mpl::int_<-7> >,
			   mpl::pair<float, mpl::int_<-8> >,
			   mpl::pair<double, mpl::int_<-9> >,
			   mpl::pair<char, mpl::int_<-10> >,
			   mpl::pair<std::string, mpl::int_<-11> > > type_map;

	 datatype( hid_t id=-1 );

	 ~datatype();

         void
         compound( size_t size );

         void
         close();

         size_t
         size() const;

         void
         insert( const datatype& type,
                 const hpc::string& description,
                 size_t offset );

	 hid_t
	 id() const;

      protected:

	 hid_t _id;
      };
   }
}

#endif
