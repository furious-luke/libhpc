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
#include "data_type.hh"
#include "insist.hh"

namespace hpc {
   namespace mpi {

      mpi::data_type data_type::null( MPI_DATATYPE_NULL );
      mpi::data_type data_type::boolean;
      mpi::data_type data_type::byte;
      mpi::data_type data_type::character;
      mpi::data_type data_type::integer;
      mpi::data_type data_type::unsigned_integer;
      mpi::data_type data_type::long_integer;
      mpi::data_type data_type::unsigned_long;
      mpi::data_type data_type::floating;
      mpi::data_type data_type::double_floating;

#if !( defined( MPICH ) || defined( MPICH2 ) )
      MPI_Datatype data_type::_type_map[9];
#endif

      data_type::data_type( MPI_Datatype type )
	 : _type(type)
      {
	 this->calc_size();
      }

      data_type::~data_type()
      {
	 this->clear();
      }

      void
      data_type::clear()
      {
	 if(this->_type != MPI_DATATYPE_NULL &&
#if defined( MPICH ) || defined( MPICH2 )
	    this->_type != MPIR_CXX_BOOL &&
#else
            this->_type != MPI_C_BOOL &&
#endif
	    this->_type != MPI_INT &&
	    this->_type != MPI_UNSIGNED &&
	    this->_type != MPI_LONG &&
	    this->_type != MPI_UNSIGNED_LONG &&
	    this->_type != MPI_BYTE &&
	    this->_type != MPI_CHAR &&
	    this->_type != MPI_FLOAT &&
	    this->_type != MPI_DOUBLE)
	 {
	    MPI_Type_free(&this->_type);
	 }
	 this->_type = MPI_DATATYPE_NULL;
	 this->_size = 0;
      }

      void
      data_type::mpi_data_type( MPI_Datatype type )
      {
	 this->clear();
	 this->_type = type;
	 this->calc_size();
      }

      const MPI_Datatype&
      data_type::mpi_data_type() const
      {
	 return this->_type;
      }

      void
      data_type::contiguous(mpi::lindex size,
			    const data_type& base,
			    mpi::lindex block_size,
			    mpi::lindex offs)
      {
	 ASSERT(size >= 0);
	 ASSERT(block_size >= 0);
	 this->clear();
	 if(offs)
	    MPI_Type_create_indexed_block(1, size*block_size, (int*)&offs, base._type, &this->_type);
	 else
	    MPI_Type_contiguous(size*block_size, base._type, &this->_type);
	 MPI_Type_commit(&this->_type);
	 this->calc_size();
      }

      void
      data_type::indexed( const vector<mpi::lindex>::view& idxs,
			  const data_type& base,
			  mpi::lindex block_size )
      {
	 this->clear();

	 // If we have block_size > 1, we need to multiply the indices.
	 if(block_size > 1) {
	    boost::transform(idxs, ((vector<mpi::lindex>::view&)idxs).begin(), std::bind1st(std::multiplies<mpi::lindex>(), block_size));
	    MPI_Type_create_indexed_block(idxs.size(), block_size, (int*)idxs.data(), base._type, &this->_type);
	    boost::transform(idxs, ((vector<mpi::lindex>::view&)idxs).begin(), std::bind2nd(std::divides<mpi::lindex>(), block_size));
	 }
	 else
	    MPI_Type_create_indexed_block(idxs.size(), block_size, (int*)idxs.data(), base._type, &this->_type);
	 MPI_Type_commit(&this->_type);
	 this->calc_size();
      }

      void
      data_type::indexed_csr(const vector<index>::view& displs,
			     const vector<mpi::lindex>::view& idxs,
			     const data_type& base,
			     mpi::lindex block_size)
      {
	 ASSERT(block_size >= 0);
	 this->clear();

	 vector<mpi::lindex> block_displs(idxs.size());
	 vector<mpi::lindex> block_cnts(idxs.size());

	 for(index ii = 0; ii < idxs.size(); ++ii) {
	    mpi::lindex idx = idxs[ii];
	    block_displs[ii] = block_size*displs[idx];
	    block_cnts[ii] = block_size*displs[idx + 1] - block_displs[ii];
	 }

	 MPI_Type_indexed(idxs.size(), (int*)block_cnts.data(), (int*)block_displs.data(), base._type, &this->_type);
	 MPI_Type_commit(&this->_type);
	 this->calc_size();
      }

      mpi::lindex
      data_type::size() const
      {
	 return this->_size;
      }

      bool
      data_type::operator==(const data_type& op) const
      {
	 return this->_type == op._type;
      }

      bool
      data_type::operator==(const MPI_Datatype& op) const
      {
	 return this->_type == op;
      }

      bool
      data_type::operator!=(const data_type& op) const
      {
	 return this->_type != op._type;
      }

      bool
      data_type::operator!=(const MPI_Datatype& op) const
      {
	 return this->_type != op;
      }

      std::ostream&
      operator<<( std::ostream& strm,
                  const data_type& obj )
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
	 else if(obj._type == MPI_FLOAT)
	    strm << "FLOAT";
	 else if(obj._type == MPI_DOUBLE)
	    strm << "DOUBLE";
	 else
	    strm << "CUSTOM(" << obj._size << ")";
	 return strm;
      }

      void data_type::calc_size() {
	 if(this->_type != MPI_DATATYPE_NULL)
	    MPI_INSIST(MPI_Type_size(this->_type, (int*)&this->_size));
	 else
	    this->_size = 0;
      }
   }
}
