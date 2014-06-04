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

#ifndef hpc_system_stream_hh
#define hpc_system_stream_hh

#include "cc_version.hh"
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <set>
#include <map>
#ifdef CXX_0X
#include <array>
#endif
#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#ifdef HAVE_THRUST
#include <thrust/host_vector.h>
#endif

namespace hpc {

   struct setindent_t {
      int indent;
   };

   setindent_t
   setindent( int indent );

   std::ostream&
   operator<<( std::ostream& strm,
               setindent_t si );

   std::ostream&
   indent( std::ostream& strm );

}

namespace std {

   template< class T,
             class U >
   std::ostream&
   operator<<( std::ostream& strm,
               std::pair<T,U> const& obj )
   {
      strm << "(" << obj.first << "," << obj.second << ")";
      return strm;
   }

   template< class T >
   std::ostream&
   operator<<( std::ostream& strm,
               std::vector<T> const& obj )
   {
      strm << "[";
      if( !obj.empty() )
      {
         typename std::vector<T>::const_iterator it = obj.begin();
         strm << *it++;
         while( it != obj.end() )
         {
            strm << ", " << *it++;
         }
      }
      strm << "]";
      return strm;
   }

   template< class T >
   std::ostream&
   operator<<( std::ostream& strm,
               std::set<T> const& obj )
   {
      strm << "{";
      if( !obj.empty() )
      {
         typename std::set<T>::const_iterator it = obj.begin();
         strm << *it++;
         while( it != obj.end() )
         {
            strm << ", " << *it++;
         }
      }
      strm << "}";
      return strm;
   }

   template< class Tk,
             class Tv >
   std::ostream&
   operator<<( std::ostream& strm,
               std::map<Tk,Tv> const& obj )
   {
      strm << "{";
      if( !obj.empty() )
      {
         typename std::map<Tk,Tv>::const_iterator it = obj.begin();
         strm << it->first << ": " << it->second;
         ++it;
         while( it != obj.end() )
         {
            strm << ", " << it->first << ": " << it->second;
            ++it;
         }
      }
      strm << "}";
      return strm;
   }

   template< class T >
   std::ostream&
   operator<<( std::ostream& strm,
               std::list<T> const& obj )
   {
      strm << "[";
      if( !obj.empty() )
      {
         typename std::list<T>::const_iterator it = obj.begin();
         strm << *it++;
         while( it != obj.end() )
         {
            strm << ", " << *it++;
         }
      }
      strm << "]";
      return strm;
   }

#ifdef CXX_0X

   template< class T,
             size_t N >
   std::ostream&
   operator<<( std::ostream& strm,
               std::array<T,N> const& obj )
   {
      strm << "(";
      if( N )
      {
         typename std::array<T,N>::const_iterator it = obj.begin();
         strm << *it++;
         while( it != obj.end() )
            strm << ", " << *it++;
      }
      strm << ")";
      return strm;
   }

#endif

}

namespace boost {

   template< class T,
             size_t N >
   std::ostream&
   operator<<( std::ostream& strm,
               boost::array<T,N> const& obj )
   {
      strm << "(";
      if( N )
      {
         typename boost::array<T,N>::const_iterator it = obj.begin();
         strm << *it++;
         while( it != obj.end() )
            strm << ", " << *it++;
      }
      strm << ")";
      return strm;
   }

}

// template< class Type,
//           int N,
//           int Last >
// struct tuple_output
// {
//    static
//    void
//    write( std::ostream& strm,
//           Type const& obj )
//    {
//       strm << std::get<N>( obj ) << ",";
//       tuple_output<Type,N + 1,Last>::write( strm, obj );
//    }
// };

// template< class Type,
//           int N >
// struct tuple_output<Type,N,N>
// {
//    static
//    void
//    write( std::ostream& strm,
//           Type const& obj )
//    {
//       strm << std::get<N>( obj );
//    }
// };

// template< class... Args >
// std::ostream&
// operator<<( std::ostream& strm,
//             boost::tuple<Args...> const& obj )
// {
//    strm << "(";
//    tuple_output<boost::tuple<Args...>,0,sizeof...(Args) - 1>::write( strm, obj );
//    strm << ")";
//    return strm;
// }

#ifdef HAVE_THRUST

namespace thrust {

   template< class T >
   std::ostream&
   operator<<( std::ostream& strm,
               thrust::host_vector<T> const& obj )
   {
      strm << "[";
      if( !obj.empty() )
      {
         typename thrust::host_vector<T>::const_iterator it = obj.begin();
         strm << *it++;
         while( it != obj.end() )
         {
            strm << ", " << *it++;
         }
      }
      strm << "]";
      return strm;
   }

}

#endif

#ifdef __CUDACC__

inline
std::ostream&
operator<<( std::ostream& strm,
            uint3 const& obj )
{
   return strm << "(" << obj.x << ", " << obj.y << ", " << obj.z << ")";
}

inline
std::ostream&
operator<<( std::ostream& strm,
            double3 const& obj )
{
   return strm << "(" << obj.x << ", " << obj.y << ", " << obj.z << ")";
}

inline
std::ostream&
operator<<( std::ostream& strm,
            double4 const& obj )
{
   return strm << "(" << obj.x << ", " << obj.y << ", " << obj.z << ", " << obj.w << ")";
}

#endif

#endif
