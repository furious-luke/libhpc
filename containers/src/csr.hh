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

#ifndef containers_csr_hh
#define containers_csr_hh

#include <algorithm>
#include <functional>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include "libhpc/system/stream_indent.hh"
#include "vector.hh"
#include "counts.hh"
#include "predicates.hh"
#include "functors.hh"

namespace hpc {

   template< class T >
   class csr
   {
   public:

      csr( index num_rows=0 )
	 : _num_rows(0)
      {
	 this->num_rows(num_rows);
      }

      csr( const csr& csr )
      {
	 this->duplicate(csr);
      }

      ~csr()
      {
      }

      void
      clear()
      {
         deallocate();
      }

      void
      deallocate()
      {
	 this->_displs.deallocate();
	 this->_array.deallocate();
	 this->_num_rows = 0;
      }

      bool
      empty() const
      {
	 return !this->_num_rows;
      }

      template< class Iter,
		class Map,
		class Predicate >
      void
      setup_3phase( index num_rows,
		    Iter begin,
		    Iter end,
		    Map op,
		    Predicate pred=always<typename Iter::value_type>() )
      {
	 // Clear myself out first.
	 this->deallocate();

	 // If there are no rows then finish here.
	 if(!num_rows)
	    return;
	 this->_num_rows = num_rows;

	 // Create my final displacement array.
	 this->_displs.resize(num_rows + 1);

	 // Begin the 3 phases.
	 for(int phase = 0; phase < 2; ++phase) {

	    if(phase == 0) {

	       // Set the counts to zero.
	       boost::fill(this->_displs, 0);
	    }
	    if(phase == 1) {

	       // Convert to displacements.
	       counts_to_displs(this->_displs.begin(), num_rows);

	       // Setup the array size.
	       this->_array.resize(this->_displs[num_rows]);
	    }

	    // Perform iteration.
	    Iter it(begin);
	    index ii = 0;
	    for(; it != end; ++it, ++ii) {

	       // Check the predicate.
	       if(!pred(*it))
		  continue;

	       // Execute the user defined operation.
	       typename Map::mapped_type row = op(*it);

	       // If we are in phase 1, store the result.
	       if(phase == 1)
		  this->_array[this->_displs[row]] = *it;

	       // Increment.
	       ++this->_displs[row];
	    }
	 }

	 // The third phase is transforming my displacements back to standard form.
	 std::reverse_copy(this->_displs.begin(), this->_displs.end() - 1, this->_displs.rbegin());
	 this->_displs[0] = 0;
      }

      index
      num_rows() const
      {
	 return this->_num_rows;
      }

      void
      num_rows( index num_rows )
      {
	 this->deallocate();
	 ASSERT(num_rows >= 0, "Invalid number of rows.");
	 this->_num_rows = num_rows;
	 if(num_rows) {
	    this->_displs.resize(num_rows + 1);
	    std::fill(this->_displs.begin(), this->_displs.end(), 0);
	 }
      }

      template<class Iter>
      void
      copy_counts( Iter cnts,
		   index size )
      {
	 this->num_rows(size);

	 // Need to add one to the size to ensure we have enough room for displacements. However,
	 // we can't do this if there are no counts.
	 if(size)
	    ++size;

	 copy_n( cnts, size - 1, _displs.begin() );
      }

      template<class Iter>
      void
      copy_displs( Iter displs,
		   index size )
      {
	 if(size > 1) {
	    this->num_rows(size - 1);
	    copy_n( displs, size, _displs.begin() );
	 }
	 else
	    this->deallocate();
      }

      void
      take_displs( vector<index>& displs )
      {
	 this->deallocate();
	 if(displs.size() > 1) {
	    this->_num_rows = displs.size() - 1;
	    this->_displs.take(displs);
	 }
      }

      void
      take_array( vector<T>& array )
      {
	 ASSERT((!this->_num_rows && !array.size()) ||
		array.size() == this->_displs[this->_num_rows],
		"Invalid array size.");
	 this->_array.take(array);
      }

      void
      setup_array( bool with_counts=false )
      {
	 if(with_counts)
	    counts_to_displs(this->_displs.begin(), this->_num_rows);
	 if(this->_num_rows)
	    this->_array.resize(this->_displs[this->_num_rows]);
      }

      template< class MultiMap,
	        class RowMap >
      void
      set_from_multimap( const MultiMap& entry_map,
			 const RowMap& row_map )
      {
	 typedef typename MultiMap::const_iterator multimap_citer;

	 // Set the number of rows to the same size as the number of entries in the row map.
	 this->num_rows(row_map.size());

	 // Count the number of entries per row.
	 for(multimap_citer it = entry_map.begin(); it != entry_map.end(); ++it)
	    ++this->_displs[row_map.get(it->first)];

	 // Prepare the array using counts.
	 this->setup_array(true);

	 // Insert each element. Note that there is no need to increment the iterator as the
	 // upper_bound operation will do this.
	 for(multimap_citer it = entry_map.begin(); it != entry_map.end();) {
	    multimap_citer up = entry_map.upper_bound(it->first);
	    boost::copy(boost::make_iterator_range(it, up) | boost::adaptors::map_values,
			(*this)[row_map.get(it->first)].begin());
	    it = up;
	 }
      }

      void duplicate( const csr<T>& csr )
      {
	 this->_num_rows = csr._num_rows;
	 this->_displs.duplicate(csr._displs);
	 this->_array.duplicate(csr._array);
      }

      void take( csr<T>& csr )
      {
	 this->_num_rows = csr._num_rows;
	 this->_displs.take(csr._displs);
	 this->_array.take(csr._array);
	 csr._num_rows = 0;
      }

      void take( vector<index>& displs,
		 vector<T>& array )
      {
	 if(displs.size()) {
	    ASSERT(displs[displs.size() - 1] == array.size(),
		   "Cannot setup a CSR from mismatching array sizes.");
	    this->_num_rows = displs.size() - 1;
	    this->_displs.take(displs);
	    this->_array.take(array);
	 }
	 else
	    this->deallocate();
      }

      typename vector<index>::view
      counts()
      {
	 return vector<index>::view(this->_displs, this->_num_rows);
      }

      void
      counts( vector<index>& cnts ) const
      {
	 cnts.resize(this->_num_rows);
	 displs_to_counts(this->_displs.begin(), this->_displs.end(), cnts.begin());
      }

      const vector<index>&
      displs() const
      {
	 return this->_displs;
      }

      vector<index>&
      mod_displs()
      {
	 return this->_displs;
      }

      const vector<T>&
      array() const
      {
	 return this->_array;
      }

      vector<T>&
      mod_array()
      {
	 return this->_array;
      }

      index
      row_size( index row ) const
      {
	 return this->_displs[row + 1] - this->_displs[row];
      }

      void
      row_sizes( vector<index>& sizes ) const
      {
	 sizes.resize(this->_num_rows);
	 for(index ii = 0; ii < this->_num_rows; ++ii)
	    sizes[ii] = this->_displs[ii + 1] - this->_displs[ii];
      }

      const typename vector<T>::view
      operator[]( index row ) const
      {
	 return typename vector<T>::view(this->_array, this->row_size(row), this->_displs[row]);
      }

      typename vector<T>::view
      operator[]( index row )
      {
	 return typename vector<T>::view(this->_array, this->row_size(row), this->_displs[row]);
      }

      const T&
      operator()( index row,
		  index col ) const
      {
	 ASSERT(row >= 0 && row < this->_num_rows, "Invalid row index.");
	 ASSERT(col >= 0 && col < this->row_size(row), "Invalid column index.");
	 return this->_array[this->_displs[row] + col];
      }

      T&
      operator()( index row,
		  index col )
      {
	 ASSERT(row >= 0 && row < this->_num_rows, "Invalid row index.");
	 ASSERT(col >= 0 && col < this->row_size(row), "Invalid column index.");
	 return this->_array[this->_displs[row] + col];
      }

      bool
      operator==( const csr& op ) const
      {
	 if(this->_num_rows != op._num_rows)
	    return false;
	 if(this->_displs != op._displs)
	    return false;
	 return this->_array == op._array;
      }

      bool
      operator!=( const csr& op ) const
      {
	 return !(*this == op);
      }

      friend std::ostream&
      operator<<( std::ostream& strm,
		  const csr<T>& obj )
      {
	 strm << "num_rows: " << obj._num_rows << "\n";
	 if(obj._num_rows) {
	    strm << setindent(2);
	    if(obj.row_size(0))
	       strm << obj[0];
	    else
	       strm << "<empty>";
	    strm << "\n";
	    for(index ii = 1; ii < obj._num_rows; ++ii) {
	       if(obj.row_size(ii))
		  strm << obj[ii];
	       else
		  strm << "<empty>";
	       strm << "\n";
	    }
	    strm << setindent(-2);
	 }
	 return strm;
      }

   private:
      index _num_rows;
      vector<hpc::index> _displs;
      vector<T> _array;
   };

   template< class Displ,
	     class T,
	     class Iter,
	     class Map,
	     class Predicate >
   void
   csr_3phase( index num_rows,
	       vector<Displ>& displs,
	       vector<T>& array,
	       Iter begin,
	       Iter end,
	       Map mapping,
	       Predicate pred=always<typename Iter::value_type>() )
   {
      // Begin from scratch.
      displs.deallocate();
      array.deallocate();

      // If there are no rows then finish here.
      if(!num_rows)
	 return;

      // Create my final displacement array.
      displs.resize(num_rows + 1);

      // Begin the 3 phases.
      for(int phase = 0; phase < 2; ++phase) {

	 if(phase == 0) {

	    // Set the counts to zero.
	    boost::fill(displs, 0);
	 }
	 if(phase == 1) {

	    // Convert to displacements.
	    counts_to_displs(displs.begin(), num_rows);

	    // Setup the array size.
	    array.resize(displs[num_rows]);
	 }

	 // Perform iteration.
	 Iter it(begin);
	 for(; it != end; ++it) {

	    // Check the predicate.
	    if(!pred(*it))
	       continue;

	    // Execute the user defined mapping operation.
	    typename Map::mapped_type row = mapping(*it);

	    // If we are in phase 1, store the result.
	    if(phase == 1)
	       array[displs[row]] = *it;

	    // Increment.
	    ++displs[row];
	 }
      }

      // The third phase is transforming my displacements back to standard form.
      std::reverse_copy(displs.begin(), displs.end() - 1, displs.rbegin());
      displs[0] = 0;
   }

   template< class Displ,
	     class T,
	     class Index,
	     class Iter,
	     class Map,
	     class Predicate >
   void
   csr_3phase( index num_rows,
	       vector<Displ>& displs,
	       vector<T>& array,
	       vector<Index>& indices,
	       Iter begin,
	       Iter end,
	       Map mapping,
	       Predicate pred=always<typename Iter::value_type>() )
   {
      // Begin from scratch.
      displs.deallocate();
      array.deallocate();
      indices.deallocate();

      // If there are no rows then finish here.
      if(!num_rows)
	 return;

      // Create my final displacement array.
      displs.resize(num_rows + 1);

      // Begin the 3 phases.
      for(int phase = 0; phase < 2; ++phase) {

	 if(phase == 0) {

	    // Set the counts to zero.
	    boost::fill(displs, 0);
	 }
	 if(phase == 1) {

	    // Convert to displacements.
	    counts_to_displs(displs.begin(), num_rows);

	    // Setup the array size and indices.
	    array.resize(displs[num_rows]);
	    indices.resize(displs[num_rows]);
	 }

	 // Perform iteration.
	 Iter it(begin);
	 Index ii = 0;
	 for(; it != end; ++it, ++ii) {

	    // Check the predicate.
	    if(!pred(*it))
	       continue;

	    // Execute the user defined mapping operation.
	    typename Map::mapped_type row = mapping(*it);

	    // If we are in phase 1, store the result.
	    if(phase == 1) {
	       array[displs[row]] = *it;
	       indices[displs[row]] = ii;
	    }

	    // Increment.
	    ++displs[row];
	 }
      }

      // The third phase is transforming my displacements back to standard form.
      std::reverse_copy(displs.begin(), displs.end() - 1, displs.rbegin());
      displs[0] = 0;
   }
}

#endif
