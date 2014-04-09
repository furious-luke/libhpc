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

#include <string>
#include <boost/mpl/map.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/at.hpp>
#include "libhpc/mpi.hh"
#include <hdf5.h>

namespace hpc {
   namespace h5 {

      class datatype
      {
      public:

	 static datatype native_char;
	 static datatype native_int;
	 static datatype native_uint;
	 static datatype native_long;
	 static datatype native_ulong;
	 static datatype native_llong;
	 static datatype native_ullong;
	 static datatype native_float;
	 static datatype native_double;
	 static datatype string;
         static datatype std_i32be;
         static datatype std_i64be;
         static datatype std_u32be;
         static datatype std_u64be;
         static datatype ieee_f32be;
         static datatype ieee_f64be;

	 typedef boost::mpl::map< boost::mpl::pair<int,                boost::mpl::int_<-2> >,
                                  boost::mpl::pair<unsigned,           boost::mpl::int_<-3> >,
                                  boost::mpl::pair<long,               boost::mpl::int_<-4> >,
                                  boost::mpl::pair<unsigned long,      boost::mpl::int_<-5> >,
                                  boost::mpl::pair<long long,          boost::mpl::int_<-6> >,
                                  boost::mpl::pair<unsigned long long, boost::mpl::int_<-7> >,
                                  boost::mpl::pair<float,              boost::mpl::int_<-8> >,
                                  boost::mpl::pair<double,             boost::mpl::int_<-9> >,
                                  boost::mpl::pair<char,               boost::mpl::int_<-10> >,
                                  boost::mpl::pair<std::string,        boost::mpl::int_<-11> > > type_map;

	 datatype( hid_t id = -1 );

	 datatype( datatype const& src );

         datatype( datatype&& src );

	 ~datatype();

         void
         compound( size_t size );

         void
         close();

         size_t
         size() const;

         void
         insert( datatype const& type,
                 std::string const& desc,
                 size_t offs );

	 hid_t
	 id() const;

      protected:

	 hid_t _id;
      };
   }
}

#endif
