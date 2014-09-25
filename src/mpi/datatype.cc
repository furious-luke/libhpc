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

#include <boost/range/algorithm.hpp>
#include "datatype.hh"
#include "insist.hh"

namespace hpc {
   namespace mpi {

      mpi::datatype datatype::null( MPI_DATATYPE_NULL );
      mpi::datatype datatype::boolean;
      mpi::datatype datatype::byte;
      mpi::datatype datatype::character;
      mpi::datatype datatype::integer;
      mpi::datatype datatype::unsigned_integer;
      mpi::datatype datatype::long_integer;
      mpi::datatype datatype::unsigned_long;
      mpi::datatype datatype::long_long;
      mpi::datatype datatype::unsigned_long_long;
      mpi::datatype datatype::floating;
      mpi::datatype datatype::double_floating;

      datatype::datatype( MPI_Datatype type )
	 : _type( type )
      {
      }

      datatype::~datatype()
      {
	 this->clear();
      }

      datatype::datatype( datatype&& src )
	 : _type( src._type )
      {
	 if( !src.is_primitive() )
	    src._type = MPI_DATATYPE_NULL;
      }

      datatype&
      datatype::operator=( datatype&& other )
      {
	 clear();
	 _type = other._type;
	 other._type = MPI_DATATYPE_NULL;
	 return *this;
      }

      void
      datatype::clear()
      {
	 if( !is_primitive() )
	 {
	    MPI_Type_free( &_type );
	    _type = MPI_DATATYPE_NULL;
	 }
      }

      bool
      datatype::is_primitive() const
      {
	 return _type == MPI_DATATYPE_NULL ||
#if defined( MPICH ) || defined( MPICH2 )
	    _type == MPIR_CXX_BOOL ||
#elif OMPI_MAJOR_VERSION <= 1 || (OMPI_MAJOR_VERSION == 1 && OMPI_MINOR_VERSION <= 4)
            _type == MPI_CHAR ||
#else
            _type == MPI_C_BOOL ||
#endif
	    _type == MPI_INT ||
	    _type == MPI_UNSIGNED ||
	    _type == MPI_LONG ||
	    _type == MPI_UNSIGNED_LONG ||
	    _type == MPI_LONG_LONG ||
	    _type == MPI_UNSIGNED_LONG_LONG ||
	    _type == MPI_BYTE ||
	    _type == MPI_CHAR ||
	    _type == MPI_FLOAT ||
	    _type == MPI_DOUBLE;
      }

      void
      datatype::mpi_datatype( MPI_Datatype type )
      {
	 this->clear();
	 this->_type = type;
      }

      const MPI_Datatype&
      datatype::mpi_datatype() const
      {
	 return this->_type;
      }

      void
      datatype::contiguous(size_t size,
			    const datatype& base,
			    size_t block_size,
			    size_t offs)
      {
	 ASSERT(size >= 0);
	 ASSERT(block_size >= 0);
	 this->clear();
	 if(offs)
	    MPI_Type_create_indexed_block(1, size*block_size, (int*)&offs, base._type, &this->_type);
	 else
	    MPI_Type_contiguous(size*block_size, base._type, &this->_type);
	 MPI_Type_commit(&this->_type);
      }

      // void
      // datatype::indexed( const vector<mpi::lindex>::view& idxs,
      //   		  const datatype& base,
      //   		  mpi::lindex block_size )
      // {
      //    this->clear();

      //    // If we have block_size > 1, we need to multiply the indices.
      //    if(block_size > 1) {
      //       boost::transform(idxs, ((vector<mpi::lindex>::view&)idxs).begin(), std::bind1st(std::multiplies<mpi::lindex>(), block_size));
      //       MPI_Type_create_indexed_block(idxs.size(), block_size, (int*)idxs.data(), base._type, &this->_type);
      //       boost::transform(idxs, ((vector<mpi::lindex>::view&)idxs).begin(), std::bind2nd(std::divides<mpi::lindex>(), block_size));
      //    }
      //    else
      //       MPI_Type_create_indexed_block(idxs.size(), block_size, (int*)idxs.data(), base._type, &this->_type);
      //    MPI_Type_commit(&this->_type);
      // }

      size_t
      datatype::size() const
      {
	 int sz;
	 if( _type != MPI_DATATYPE_NULL )
	    MPI_INSIST( MPI_Type_size( _type, (int*)&sz ) );
	 else
	    sz = 0;
	 return sz;
      }

      bool
      datatype::operator==(const datatype& op) const
      {
	 return this->_type == op._type;
      }

      bool
      datatype::operator==(const MPI_Datatype& op) const
      {
	 return this->_type == op;
      }

      bool
      datatype::operator!=(const datatype& op) const
      {
	 return this->_type != op._type;
      }

      bool
      datatype::operator!=(const MPI_Datatype& op) const
      {
	 return this->_type != op;
      }

      std::ostream&
      operator<<( std::ostream& strm,
                  const datatype& obj )
      {
	 if(obj._type == MPI_DATATYPE_NULL)
	    strm << "NULL";
	 else if(obj._type == MPI_BYTE)
	    strm << "BYTE";
	 else if(obj._type == MPI_CHAR)
	    strm << "CHAR";
	 else if(obj._type == MPI_INT)
	    strm << "INT";
	 else if(obj._type == MPI_LONG)
	    strm << "LONG";
	 else if(obj._type == MPI_UNSIGNED_LONG)
	    strm << "UNSIGNED LONG";
	 else if(obj._type == MPI_LONG_LONG)
	    strm << "LONG LONG";
	 else if(obj._type == MPI_UNSIGNED_LONG_LONG)
	    strm << "UNSIGNED LONG LONG";
	 else if(obj._type == MPI_FLOAT)
	    strm << "FLOAT";
	 else if(obj._type == MPI_DOUBLE)
	    strm << "DOUBLE";
	 else
	    strm << "CUSTOM";
	 return strm;
      }

   }
}
