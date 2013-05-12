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

#ifndef hpc_containers_scoped_ptr_hh
#define hpc_containers_scoped_ptr_hh

#if CXX_0X
#include <memory>
#endif
#include "libhpc/debug/assert.hh"

namespace hpc {

#if CXX_0X

   template< typename T >
   class scoped_ptr final
      : public std::unique_ptr<T>
   {
   public:

      scoped_ptr( T* ptr = 0 )
         : std::unique_ptr<T>( ptr )
      {
      }

      scoped_ptr( scoped_ptr&& src )
         : std::unique_ptr<T>( std::move( src ) )
      {
      }

      bool
      empty()
      {
         return this->get() != 0;
      }

      scoped_ptr&
      operator=( T* ptr )
      {
         this->reset( ptr );
         return *this;
      }
   };

#else

   ///
   /// TODO: Extend to arrays (or wait for nvcc to support C++11).
   ///
   template< class T >
   class scoped_ptr
   {
   public:

      scoped_ptr( T* ptr = 0 )
         : _ptr( ptr )
      {
      }

      scoped_ptr( const scoped_ptr<T>& src )
         : _ptr( 0 )
      {
      }

      scoped_ptr( scoped_ptr<T>& src )
         : _ptr( src._ptr )
      {
         src._ptr = 0;
      }

      ~scoped_ptr()
      {
         if( _ptr )
            delete _ptr;
      }

      T*
      get()
      {
         return _ptr;
      }

      const T*
      get() const
      {
         return _ptr;
      }

      T*
      release()
      {
         T* tmp = _ptr;
         _ptr = NULL;
         return tmp;
      }

      bool
      empty() const
      {
         return _ptr == 0;
      }

      scoped_ptr<T>&
      operator=( T* ptr )
      {
         if( _ptr )
            delete _ptr;
         _ptr = ptr;
         return *this;
      }

      const T&
      operator*() const
      {
         ASSERT( _ptr != 0 );
         return *_ptr;
      }

      T&
      operator*()
      {
         ASSERT( _ptr != 0 );
         return *_ptr;
      }

      const T*
      operator->() const
      {
         ASSERT( _ptr != 0 );
         return _ptr;
      }

      T*
      operator->()
      {
         ASSERT( _ptr != 0 );
         return _ptr;
      }

      operator bool() const
      {
         return _ptr != 0;
      }

   protected:

      T* _ptr;
   };

#endif

}

#endif
