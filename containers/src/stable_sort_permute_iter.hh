#ifndef hpc_stable_sort_permute_iter_hh
#define hpc_stable_sort_permute_iter_hh

#include <boost/tuple/tuple.hpp>
#include <boost/iterator/iterator_facade.hpp>

namespace hpc {

   template< class SortIter,
	     class PermuteIter >
   struct stable_sort_permute_iter_helper_type
   {
      typedef boost::tuple< typename std::iterator_traits<SortIter>::value_type,
			    typename std::iterator_traits<PermuteIter>::value_type > value_type;
      typedef boost::tuple< typename std::iterator_traits<SortIter>::value_type&,
			    typename std::iterator_traits<PermuteIter>::value_type& >& reference_type;
   };

   template< class SortIter,
	     class PermuteIter >
   class stable_sort_permute_iter
      : public boost::iterator_facade< stable_sort_permute_iter<SortIter, PermuteIter>,
				       typename stable_sort_permute_iter_helper_type<SortIter, PermuteIter>::value_type,
				       std::random_access_iterator_tag,
				       typename stable_sort_permute_iter_helper_type<SortIter, PermuteIter>::reference_type,
				       typename std::iterator_traits<SortIter>::difference_type >
   {
   public:

      typedef typename std::iterator_traits<SortIter>::difference_type difference_type;

      stable_sort_permute_iter()
      {
      }

      stable_sort_permute_iter( SortIter si, PermuteIter pi )
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
      equal( stable_sort_permute_iter const& other ) const
      {
	 return this->_si == other._si;
      }

      typename stable_sort_permute_iter_helper_type<SortIter, PermuteIter>::reference_type
      dereference() const
      {
	 this->_tmp = boost::tuple<typename std::iterator_traits<SortIter>::value_type&,
				   typename std::iterator_traits<PermuteIter>::value_type&>(*this->_si, *this->_pi);
	 return this->_tmp;
      }

      void
      advance( difference_type ii )
      {
	 this->_si += ii;
	 this->_pi += ii;
      }

      difference_type
      distance_to( stable_sort_permute_iter const& other ) const
      {
	 return other._si - this->_si;
      }

      boost::tuple<typename std::iterator_traits<SortIter>::value_type&,
		   typename std::iterator_traits<PermuteIter>::value_type&> _tmp;
   };

   template< class SortIter,
	     class PermuteIter >
   struct stable_sort_permute_iter_compare
      : public std::binary_function< typename stable_sort_permute_iter_helper_type<SortIter, PermuteIter>::value_type,
				     typename stable_sort_permute_iter_helper_type<SortIter, PermuteIter>::value_type,
				     bool >
   {
      typedef typename stable_sort_permute_iter_helper_type<SortIter, PermuteIter>::value_type T;

      bool
      operator()( const T& t1,
		  const T& t2)
      {
	 return boost::tuples::get<0>(t1) < boost::tuples::get<0>(t2);
      }
   };

   template< class SortIter,
	     class PermuteIter >
   stable_sort_permute_iter<SortIter, PermuteIter>
   make_stable_sort_permute_iter( SortIter si,
			   PermuteIter pi )
   {
      return stable_sort_permute_iter<SortIter, PermuteIter>(si, pi);
   };
};

#endif
