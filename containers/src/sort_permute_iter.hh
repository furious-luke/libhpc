#ifndef hpc_sort_permute_iter_hh
#define hpc_sort_permute_iter_hh

#include <boost/tuple/tuple.hpp>
#include <boost/iterator/iterator_facade.hpp>

namespace hpc {

   template< class SortIter,
	     class PermuteIter >
   struct sort_permute_iter_helper_type
   {
      typedef boost::tuple< typename std::iterator_traits<SortIter>::value_type,
			    typename std::iterator_traits<PermuteIter>::value_type > value_type;
      typedef boost::tuple< typename std::iterator_traits<SortIter>::value_type&,
			    typename std::iterator_traits<PermuteIter>::value_type& > reference_type;
   };

   template< class SortIter,
	     class PermuteIter >
   class sort_permute_iter
      : public boost::iterator_facade< sort_permute_iter<SortIter, PermuteIter>,
				       typename sort_permute_iter_helper_type<SortIter, PermuteIter>::value_type,
				       std::random_access_iterator_tag,
				       typename sort_permute_iter_helper_type<SortIter, PermuteIter>::reference_type,
				       typename std::iterator_traits<SortIter>::difference_type >
   {
   public:

      typedef typename std::iterator_traits<SortIter>::difference_type difference_type;

      sort_permute_iter()
      {
      }

      sort_permute_iter( SortIter si, PermuteIter pi )
	 : _si(si),
	   _pi(pi)
      {
      }

      SortIter _si;
      PermuteIter _pi;

   private:
      friend class boost::iterator_core_access;

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

      typename sort_permute_iter_helper_type<SortIter, PermuteIter>::reference_type
      dereference() const
      {
	 return typename sort_permute_iter_helper_type<SortIter, PermuteIter>::reference_type(*this->_si, *this->_pi);
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
   };

   template< class SortIter,
	     class PermuteIter,
	     class Comparison=std::less<typename std::iterator_traits<SortIter>::value_type> >
   struct sort_permute_iter_compare
      : public std::binary_function< typename sort_permute_iter_helper_type<SortIter, PermuteIter>::value_type,
				     typename sort_permute_iter_helper_type<SortIter, PermuteIter>::value_type,
				     bool >
   {
      typedef typename sort_permute_iter_helper_type<SortIter, PermuteIter>::value_type T;

      bool
      operator()( const T& t1,
		  const T& t2 )
      {
	 return Comparison()(boost::tuples::get<0>(t1), boost::tuples::get<0>(t2));
      }
   };

   template< class SortIter,
	     class PermuteIter >
   sort_permute_iter<SortIter, PermuteIter>
   make_sort_permute_iter( SortIter si,
			   PermuteIter pi )
   {
      return sort_permute_iter<SortIter, PermuteIter>(si, pi);
   };
};

#endif
