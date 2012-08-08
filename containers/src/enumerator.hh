#ifndef hpc_containers_enumerate_hh
#define hpc_containers_enumerate_hh

#include <utility>
#include <boost/iterator/iterator_adaptor.hpp>

// TODO: These classes are pretty rough. It works, but needs plenty of cleaning up.

namespace hpc {

   template< class Iter, class Num >
   class enumerator
      : public boost::iterator_adaptor< enumerator< Iter, Num >,
					Iter,
					const std::pair< Num, typename Iter::value_type >,
					boost::forward_traversal_tag >
   {
   protected:
      typedef typename boost::iterator_adaptor< enumerator< Iter, Num >,
						Iter,
						const std::pair< Num, typename Iter::value_type >,
						boost::forward_traversal_tag > super_t;

   public:

      enumerator()
	 : _cnt(0)
      {
      }

      // explicit enumerator(
      // 	 const enumerator& src
      // 	 )
      // 	 : _cnt(src._cnt)
      // {
      // }

      enumerator(
	 const Iter& iter
	 )
	 : super_t(iter),
	   _cnt(0)
      {
      }

      ~enumerator()
      {
      }

      bool
      operator==(const Iter& op) const
      {
	 return this->base_reference() == op;
      }

      bool
      operator!=(const Iter& op) const
      {
	 return this->base_reference() != op;
      }

      const std::pair<Num, typename Iter::value_type>
      operator*() const
      {
	 return std::make_pair(this->_cnt, *this->base_reference());
      }

   private:
      friend class boost::iterator_core_access;

      const std::pair<Num, typename Iter::value_type>
      dereference() const
      {
	 return std::make_pair(this->_cnt, *this->base_reference());
      }

      void
      increment()
      {
	 ++this->base_reference();
	 ++this->_cnt;
      }

      Num _cnt;
   };

   template< class Iter, class Num >
   class renumerator
      : public boost::iterator_adaptor< renumerator< Iter, Num >,
					Iter,
					const std::pair< typename Iter::value_type, Num >,
					boost::forward_traversal_tag >
   {
   protected:
      typedef typename boost::iterator_adaptor< renumerator< Iter, Num >,
						Iter,
						const std::pair< typename Iter::value_type, Num >,
						boost::forward_traversal_tag > super_t;

   public:

      renumerator()
	 : _cnt(0)
      {
      }

      // explicit renumerator(
      // 	 const renumerator& src
      // 	 )
      // 	 : _cnt(src._cnt)
      // {
      // }

      renumerator(
	 const Iter& iter
	 )
	 : super_t(iter),
	   _cnt(0)
      {
      }

      ~renumerator()
      {
      }

      bool
      operator==(const Iter& op) const
      {
	 return this->base_reference() == op;
      }

      bool
      operator!=(const Iter& op) const
      {
	 return this->base_reference() != op;
      }

      const std::pair<typename Iter::value_type, Num>
      operator*() const
      {
	 return std::make_pair(*this->base_reference(), this->_cnt);
      }

   private:
      friend class boost::iterator_core_access;

      const std::pair<typename Iter::value_type, Num>
      dereference() const
      {
	 return std::make_pair(*this->base_reference(), this->_cnt);
      }

      void
      increment()
      {
	 ++this->base_reference();
	 ++this->_cnt;
      }

      Num _cnt;
   };
}

#endif
