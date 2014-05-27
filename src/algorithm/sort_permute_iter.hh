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

#ifndef hpc_sort_permute_iter_hh
#define hpc_sort_permute_iter_hh

#ifndef _GLIBCXX_DEBUG

#include <tuple>
#include <boost/iterator/iterator_facade.hpp>

namespace hpc {

   template< class... Args >
   struct tuple_wrapper
   {
      std::tuple<Args...> data;

      tuple_wrapper( Args... src )
         : data{ src... }
      {
      }

      tuple_wrapper( std::tuple<Args...> const& src )
         : data{ src }
      {
      }

      tuple_wrapper( tuple_wrapper<typename std::add_lvalue_reference<Args>::type...> const& src )
         : data{ src.data }
      {
      }

      template< class... SubArgs >
      tuple_wrapper&
      operator=( tuple_wrapper<SubArgs...> const& src )
      {
         data = src.data;
         return *this;
      }

      operator std::tuple<typename std::remove_reference<Args>::type...>()
      {
         return data;
      }
   };

   template< class... Args >
   void
   swap( tuple_wrapper<Args...> x,
         tuple_wrapper<Args...> y )
   {
      std::swap( x.data, y.data );
   }

   template< class SortIter,
	     class PermuteIter >
   struct sort_permute_iter_helper_type
   {
      typedef tuple_wrapper< typename std::iterator_traits<SortIter>::value_type,
                             typename std::iterator_traits<PermuteIter>::value_type > value_type;
      typedef tuple_wrapper< typename std::iterator_traits<SortIter>::value_type&,
                             typename std::iterator_traits<PermuteIter>::value_type& > reference_type;
   };

   template< class SortIter,
	     class PermuteIter >
   class sort_permute_iter
      : public boost::iterator_facade< sort_permute_iter<SortIter,PermuteIter>,
				       typename sort_permute_iter_helper_type<SortIter,PermuteIter>::value_type,
				       std::random_access_iterator_tag,
				       typename sort_permute_iter_helper_type<SortIter,PermuteIter>::reference_type,
				       typename std::iterator_traits<SortIter>::difference_type >
   {
      friend class boost::iterator_core_access;

   public:

      typedef typename std::iterator_traits<SortIter>::difference_type difference_type;

      sort_permute_iter()
      {
      }

      sort_permute_iter( SortIter si,
                         PermuteIter pi )
	 : _si( si ),
	   _pi( pi )
      {
      }

   protected:

      void
      increment()
      {
	 ++this->_si;
	 ++this->_pi;
      }

      void
      decrement()
      {
	 --this->_si;
	 --this->_pi;
      }

      bool
      equal( sort_permute_iter const& other ) const
      {
	 return this->_si == other._si;
      }

      typename sort_permute_iter_helper_type<SortIter,PermuteIter>::reference_type
      dereference() const
      {
	 return typename sort_permute_iter_helper_type<SortIter,PermuteIter>::reference_type( *this->_si, *this->_pi );
      }

      void
      advance( difference_type ii )
      {
	 this->_si += ii;
	 this->_pi += ii;
      }

      difference_type
      distance_to( sort_permute_iter const& other ) const
      {
	 return other._si - this->_si;
      }

   protected:

      SortIter _si;
      PermuteIter _pi;
   };

   template< class SortIter,
	     class PermuteIter,
	     class Comparison = std::less<typename std::iterator_traits<SortIter>::value_type> >
   struct sort_permute_iter_compare
      : public std::binary_function< typename sort_permute_iter_helper_type<SortIter,PermuteIter>::value_type,
				     typename sort_permute_iter_helper_type<SortIter,PermuteIter>::value_type,
				     bool >
   {
      typedef typename sort_permute_iter_helper_type<SortIter,PermuteIter>::value_type T;

      bool
      operator()( T const& t1,
		  T const& t2 )
      {
	 return Comparison()( std::get<0>( t1.data ), std::get<0>( t2.data ) );
      }
   };

   template< class SortIter,
	     class PermuteIter >
   sort_permute_iter<SortIter,PermuteIter>
   make_sort_permute_iter( SortIter si,
			   PermuteIter pi )
   {
      return sort_permute_iter<SortIter,PermuteIter>( si, pi );
   }

}

#endif

#endif
