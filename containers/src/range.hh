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

#ifndef hpc_containers_range_hh
#define hpc_containers_range_hh

#include "libhpc/debug/debug.hh"
#include "vector.hh"

namespace hpc {

   template< class T >
   class range
   {
   public:

      range()
         : _start( 0 ),
           _finish( 0 )
      {
      }

      range( const T& start,
             const T& finish )
      {
         this->set( start, finish );
      }

      range( const std::pair<T, T>& value )
      {
         this->set( value.first, value.second );
      }

      range( const range& src )
         : _start( src._start ),
           _finish( src._finish )
      {
      }

      void
      set( const T& start,
           const T& finish )
      {
         ASSERT( start <= finish );
         this->_start = start;
         this->_finish = finish;
      }

      void
      split( const range& op,
             vector<range>& ranges ) const
      {
         ranges.resize( 1 );
         unsigned pos = 0;
         if( this->_start < op._start )
         {
            ranges.resize( ranges.size() + 1 );
            ranges[pos]._start = this->_start;
            ranges[pos++]._finish = op._start;
            ranges[pos]._start = op._start;
         }
         else if( this->_start > op._start )
         {
            ranges.resize( ranges.size() + 1 );
            ranges[pos]._start = op._start;
            ranges[pos++]._finish = this->_start;
            ranges[pos]._start = this->_start;
         }
         else
            ranges[pos]._start = this->_start;
         if( this->_finish < op._finish )
         {
            ranges.resize( ranges.size() + 1 );
            ranges[pos++]._finish = this->_finish;
            ranges[pos]._start = this->_finish;
            ranges[pos]._finish = op._finish;
         }
         else if( this->_finish > op._finish )
         {
            ranges.resize( ranges.size() + 1 );
            ranges[pos++]._finish = op._finish;
            ranges[pos]._start = op._finish;
            ranges[pos]._finish = this->_finish;
         }
         else
            ranges[pos]._finish = this->_finish;
      }

      const T&
      start() const
      {
         return this->_start;
      }

      const T&
      finish() const
      {
         return this->_finish;
      }

      T
      length() const
      {
         return this->_finish - this->_start;
      }

      bool
      has( const T& value ) const
      {
         return value >= this->_start && value < this->_finish;
      }

      range&
      operator=( const std::pair<T, T>& value )
      {
         this->set( value.first, value.second );
         return *this;
      }

      bool
      operator==( const range& op ) const
      {
         return this->_start == op._start && this->_finish == op._finish;
      }

      bool
      operator!=( const range& op ) const
      {
         return !(*this == op);
      }

      bool
      operator<( const range& op ) const
      {
         return this->_finish <= op._start;
      }

      bool
      operator>( const range& op ) const
      {
         return this->_start >= op._finish;
      }

      friend std::ostream&
      operator<<( std::ostream& strm,
		  const range& obj )
      {
	 strm << "(" << obj._start << ", " << obj._finish << ")";
	 return strm;
      }

   protected:

      T _start;
      T _finish;
   };
}

#endif
