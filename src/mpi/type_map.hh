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

#ifndef hpc_mpi_type_map_hh
#define hpc_mpi_type_map_hh

#include <boost/mpl/map.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/at.hpp>
#include "libhpc/system/varray.hh"
#include "init.hh"

namespace hpc {
   namespace mpi {

#define MPI_MAP_TYPE_SIZE( T ) mapped_type<T>::size

#if defined( MPICH ) || defined( MPICH2 )

#define MPI_MAP_TYPE( T )                       \
      mapped_type<T>::type_id

      typedef boost::mpl::map< boost::mpl::pair<unsigned char,      boost::mpl::int_<MPI_BYTE> >,
                               boost::mpl::pair<bool,               boost::mpl::int_<MPIR_CXX_BOOL> >,
                               boost::mpl::pair<char,               boost::mpl::int_<MPI_CHAR> >,
                               boost::mpl::pair<int,                boost::mpl::int_<MPI_INT> >,
                               boost::mpl::pair<unsigned int,       boost::mpl::int_<MPI_UNSIGNED> >,
                               boost::mpl::pair<long,               boost::mpl::int_<MPI_LONG> >,
                               boost::mpl::pair<unsigned long,      boost::mpl::int_<MPI_UNSIGNED_LONG> >,
                               boost::mpl::pair<long long,          boost::mpl::int_<MPI_LONG_LONG> >,
                               boost::mpl::pair<unsigned long long, boost::mpl::int_<MPI_UNSIGNED_LONG_LONG> >,
                               boost::mpl::pair<float,              boost::mpl::int_<MPI_FLOAT> >,
                               boost::mpl::pair<double,             boost::mpl::int_<MPI_DOUBLE> > > type_map;

#else

#define MPI_MAP_TYPE( T )                                               \
      hpc::mpi::_type_map[mapped_type<T>::type_id]

      typedef boost::mpl::map< boost::mpl::pair<unsigned char,      boost::mpl::int_<0> >,
                               boost::mpl::pair<bool,               boost::mpl::int_<1> >,
                               boost::mpl::pair<char,               boost::mpl::int_<2> >,
                               boost::mpl::pair<int,                boost::mpl::int_<3> >,
                               boost::mpl::pair<unsigned int,       boost::mpl::int_<4> >,
                               boost::mpl::pair<long,               boost::mpl::int_<5> >,
                               boost::mpl::pair<unsigned long,      boost::mpl::int_<6> >,
                               boost::mpl::pair<long long,          boost::mpl::int_<7> >,
                               boost::mpl::pair<unsigned long long, boost::mpl::int_<8> >,
                               boost::mpl::pair<float,              boost::mpl::int_<9> >,
                               boost::mpl::pair<double,             boost::mpl::int_<10> > > type_map;
      extern MPI_Datatype _type_map[11];

#endif

      template< class T >
      struct mapped_type
      {
         static int const    type_id = boost::mpl::at<hpc::mpi::type_map,T>::type::value;
         static size_t const size = 1;
      };

      template< class T,
                size_t N >
      struct mapped_type< hpc::varray<T,N> >
      {
         static int const    type_id = boost::mpl::at<hpc::mpi::type_map,T>::type::value;
         static size_t const size = N;
      };

   }
}

#endif
