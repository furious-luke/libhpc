#ifndef hpc_interactive_animation_hh
#define hpc_interactive_animation_hh

#include <stdint.h>
#include <boost/iterator/iterator_facade.hpp>
#include "libhpc/logging.hh"
#include "libhpc/system/array.hh"

namespace hpc {
   namespace inter {

      template< class T >
      class animation
         : public boost::iterator_facade< animation<T>,
                                          T,
                                          std::forward_iterator_tag,
                                          T >
      {
         friend class boost::iterator_core_access;

      public:

         typedef          boost::chrono::high_resolution_clock clock_type;
         typedef typename clock_type::time_point               time_point_type;
         typedef          boost::chrono::milliseconds          time_type;

         typedef T value_type;
         typedef value_type reference_type;

         animation()
            : _ease( 0, 0 ),
              _dur( 200 ),
              _inv_dur( 1.0/200.0 )
         {
         }

         animation( const value_type& start,
                    const value_type& finish )
            : _dur( 200 ),
#ifdef CXX_0X
              _rng{ { start, finish } },
              _ease{ { 0, 0 } },
#endif
              _inv_dur( 1.0/200.0 )
         {
#ifndef CXX_0X
            _rng[0] = start;
            _rng[1] = finish;
            _ease[0] = 0;
            _ease[1] = 0;
#endif
         }

         void
         set_range( const value_type& start,
                    const value_type& finish )
         {
            _rng[0] = start;
            _rng[1] = finish;
            reset();
         }

         void
         reset( bool forward = true )
         {
            _pos = 0;
            _fac = 0;
            _old = clock_type::now();
            _fwd = forward;
         }

         bool
         done() const
         {
            return _pos >= _dur;
         }

      protected:

         void
         increment()
         {
            time_type dur = boost::chrono::duration_cast<time_type>( clock_type::now() - _old );
            _pos = std::min<double>( (double)dur.count(), _dur );
            _fac = _pos*_inv_dur;
         }

         bool
         equal( const animation& op ) const
         {
            return _pos >= op._dur;
         }

         reference_type
         dereference() const
         {
            if( _fwd )
               return _rng[0] + (_rng[1] - _rng[0])*_fac;
            else
               return _rng[0] + (_rng[1] - _rng[0])*(1.0 - _fac);
         }

      protected:

	 hpc::array<value_type,2> _rng;
	 hpc::array<unsigned,2> _ease;
         double _pos;
         double _dur;
         double _inv_dur;
         double _fac;
         time_point_type _old;
         bool _fwd;
      };

   }
}

#endif
