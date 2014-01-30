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

#ifndef libhpc_system_stream_output_hh
#define libhpc_system_stream_output_hh

#include <iostream>
#include <array>
#include <vector>
#include <list>
#include <set>

template< class T >
std::ostream&
operator<<( std::ostream& strm,
            const std::vector<T>& obj )
{
   strm << "[";
   if( !obj.empty() )
   {
      auto it = obj.cbegin();
      strm << *it++;
      while( it != obj.cend() )
      {
         strm << ", " << *it++;
      }
   }
   strm << "]";
   return strm;
}

namespace hpc {

   template< class T,
	     size_t N >
   std::ostream&
   operator<<( std::ostream& strm,
	       const std::array<T,N>& obj )
   {
      strm << "(";
      if( N )
      {
	 auto it = obj.cbegin();
	 strm << *it++;
	 while( it != obj.cend() )
	 {
	    strm << ", " << *it++;
	 }
      }
      strm << ")";
      return strm;
   }

   template< class T >
   std::ostream&
   operator<<( std::ostream& strm,
	       const std::vector<T>& obj )
   {
      strm << "[";
      if( !obj.empty() )
      {
	 auto it = obj.cbegin();
	 strm << *it++;
	 while( it != obj.cend() )
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
	       const std::set<T>& obj )
   {
      strm << "{";
      if( !obj.empty() )
      {
	 auto it = obj.cbegin();
	 strm << *it++;
	 while( it != obj.cend() )
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
	       const std::map<Tk,Tv>& obj )
   {
      strm << "{";
      if( !obj.empty() )
      {
	 auto it = obj.cbegin();
	 strm << it->first << ": " << it->second;
	 ++it;
	 while( it != obj.cend() )
	 {
	    strm << it->first << ": " << it->second;
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
	 auto it = obj.cbegin();
	 strm << *it++;
	 while( it != obj.cend() )
	 {
	    strm << ", " << *it++;
	 }
      }
      strm << "]";
      return strm;
   }

}

#endif
