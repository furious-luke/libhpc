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

#ifndef hpc_sort_permute2_iter_hh
#define hpc_sort_permute2_iter_hh

#ifndef _GLIBCXX_DEBUG

namespace hpc {

   template< class SortIter,
	     class PermuteIter1,
	     class PermuteIter2 >
   struct sort_permute2_iter_helper_type
   {
      typedef boost::tuple< typename std::iterator_traits<SortIter>::value_type,
			    typename std::iterator_traits<PermuteIter1>::value_type,
			    typename std::iterator_traits<PermuteIter2>::value_type > value_type;
      typedef boost::tuple< typename std::iterator_traits<SortIter>::value_type&,
			    typename std::iterator_traits<PermuteIter1>::value_type&,
			    typename std::iterator_traits<PermuteIter2>::value_type& > reference_type;
   };

   template< class SortIter,
	     class PermuteIter1,
	     class PermuteIter2 >
   class sort_permute2_iter
      : public boost::iterator_facade< sort_permute2_iter<SortIter,PermuteIter1,PermuteIter2>,
				       typename sort_permute2_iter_helper_type<SortIter,PermuteIter1,PermuteIter2>::value_type,
				       std::random_access_iterator_tag,
				       typename sort_permute2_iter_helper_type<SortIter,PermuteIter1,PermuteIter2>::reference_type,
				       typename std::iterator_traits<SortIter>::difference_type >
   {
      friend class boost::iterator_core_access;

   public:

      typedef typename std::iterator_traits<SortIter>::difference_type difference_type;

      sort_permute2_iter()
      {
      }

      sort_permute2_iter( SortIter si,
                          PermuteIter1 pi1,
                          PermuteIter2 pi2 )
	 : _si( si ),
	   _pi1( pi1 ),
	   _pi2( pi2 )
      {
      }

      SortIter _si;
      PermuteIter1 _pi1;
      PermuteIter2 _pi2;

   private:

      void
      increment()
      {
	 ++this->_si;
	 ++this->_pi1;
	 ++this->_pi2;
      }

      void
      decrement()
      {
	 --this->_si;
	 --this->_pi1;
	 --this->_pi2;
      }

      bool
      equal( sort_permute2_iter const& other ) const
      {
	 return this->_si == other._si;
      }

      typename sort_permute2_iter_helper_type<SortIter,PermuteIter1,PermuteIter2>::reference_type
      dereference() const
      {
	 return typename sort_permute2_iter_helper_type<SortIter,PermuteIter1,PermuteIter2>::reference_type( *this->_si, *this->_pi1, *this->_pi2 );
      }

      void
      advance( difference_type ii )
      {
	 this->_si += ii;
	 this->_pi1 += ii;
	 this->_pi2 += ii;
      }

      difference_type
      distance_to( sort_permute2_iter const& other ) const
      {
	 return other._si - this->_si;
      }
   };

   template< class SortIter,
	     class PermuteIter1,
	     class PermuteIter2 >
   struct sort_permute2_iter_compare
      : public std::binary_function< typename sort_permute2_iter_helper_type<SortIter,PermuteIter1,PermuteIter2>::value_type,
				     typename sort_permute2_iter_helper_type<SortIter,PermuteIter1,PermuteIter2>::value_type,
				     bool >
   {
      typedef typename sort_permute2_iter_helper_type<SortIter, PermuteIter1, PermuteIter2>::value_type T;

      bool
      operator()( T const& t1,
		  T const& t2 )
      {
	 return boost::tuples::get<0>( t1 ) < boost::tuples::get<0>( t2 );
      }
   };

   template< class SortIter,
	     class PermuteIter1,
	     class PermuteIter2 >
   sort_permute2_iter<SortIter,PermuteIter1,PermuteIter2>
   make_sort_permute2_iter( SortIter si,
                            PermuteIter1 pi1,
                            PermuteIter2 pi2 )
   {
      return sort_permute2_iter<SortIter,PermuteIter1,PermuteIter2>( si, pi1, pi2 );
   }

}

#endif

#endif
