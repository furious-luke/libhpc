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

#ifndef hpc_h5_buffer_hh
#define hpc_h5_buffer_hh

#include <vector>
#include <boost/utility/enable_if.hpp>
#include "libhpc/system/type_traits.hh"
#include "libhpc/system/reallocate.hh"
#include "dataset.hh"
#include "datatype.hh"

namespace hpc {
   namespace h5 {

      static size_t const buffer_default_size = 10000;

      template< class T >
      class buffer
      {
      public:

         typedef T element_type;

      public:

         buffer()
            : _dset( 0 ),
              _mem_type( 0 ),
              _offs( 0 )
         {
         }

	 ~buffer()
         {
            close();
         }

         void
         create( h5::dataset& dset,
                 h5::datatype const& mem_type,
                 size_t size = buffer_default_size,
                 size_t offs = 0 )
         {
            close();
            _dset = &dset;
            _mem_type = &mem_type;
            set_size( size );
            _offs = offs;
         }

         void
         close()
         {
            _write_to_file<std::vector<element_type> >( _buf );
            hpc::deallocate( _buf );
            _dset = 0;
            _mem_type = 0;
         }

         void
         set_size( size_t size )
         {
            hpc::reallocate( _buf, size );
            _buf.resize( 0 );
         }

         template< class U >
         typename boost::disable_if<random_access_trait<U> >::type
         write( U const& data )
	 {
	    if( _buf.size() == _buf.capacity() )
	    {
	       _write_to_file<std::vector<element_type> >( _buf );
	       _buf.resize( 0 );
	    }
	    _buf.push_back( data );
	 }

         template< class U >
         inline
         void
         write( std::vector<U> const& buf )
         {
            write<std::vector<U> >( buf );
         }

         template< class DataT >
         typename boost::enable_if<random_access_trait<DataT> >::type
         write( typename hpc::type_traits<DataT>::const_reference data )
         {
            size_t max_size = _buf.capacity();
            size_t size = _buf.size();
            size_t ava_size = max_size - size;

            // If we can't fit the new data in the buffer, write them
            // both out to disk immediately. This will be better in
            // scenarios where we could get large data inputs.
            if( data.size() > ava_size )
            {
               _write_to_file<std::vector<element_type> >( _buf );
               _write_to_file<DataT>( data );
               _buf.resize( 0 );
            }
            else
            {
               _buf.resize( size + data.size() );
               std::copy( data.begin(), data.end(), _buf.data() + size );
            }
         }

      protected:

         template< class DataT >
         void
         _write_to_file( typename hpc::type_traits<DataT>::const_reference data )
         {
            if( data.size() )
            {
               ASSERT( _dset );
               ASSERT( _mem_type );
               _dset->write( data.data(), *_mem_type, data.size(), _offs );
               _offs += data.size();
            }
         }

      protected:

         h5::dataset* _dset;
         h5::datatype const* _mem_type;
         std::vector<element_type> _buf;
         unsigned long long _offs;
      };

   }
}

#endif
