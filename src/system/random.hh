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

#ifndef libhpc_system_random_hh
#define libhpc_system_random_hh

#include <ctime>
#include <boost/mpl/map.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/at.hpp>
#include <boost/random.hpp>

namespace mpl = boost::mpl;

namespace hpc {

   typedef boost::mt11213b engine_type;

   extern engine_type engine;

   typedef mpl::map< mpl::pair<double,   boost::random::uniform_real_distribution<double> >,
                     mpl::pair<float,    boost::random::uniform_real_distribution<float> >,
                     mpl::pair<int,      boost::random::uniform_int_distribution<int> >,
                     mpl::pair<unsigned, boost::random::uniform_int_distribution<unsigned> > > uniform_distribution_types;

   typedef mpl::map< mpl::pair<double,   boost::random::normal_distribution<double> >,
                     mpl::pair<float,    boost::random::normal_distribution<float> >,
                     mpl::pair<int,      boost::random::normal_distribution<int> >,
                     mpl::pair<unsigned, boost::random::normal_distribution<unsigned> > > normal_distribution_types;

typedef mpl::map< mpl::pair<double, boost::variate_generator<engine_type&, boost::random::uniform_real_distribution<double> > >,
		  mpl::pair<float,  boost::variate_generator<engine_type&, boost::random::uniform_real_distribution<float> > >,
		  mpl::pair<int,    boost::variate_generator<engine_type&, boost::random::uniform_int_distribution<int> > > > uniform_generator_types;

   template< class T >
   struct generator_types {
      typedef boost::variate_generator<boost::mt11213b, boost::random::uniform_real_distribution<T> > uniform_real;
   };

   template< class T >
   typename generator_types<T>::uniform_real
   uniform_real_generator( const T& low,
                           const T& high,
                           unsigned int seed = std::time( 0 ) )
   {
      boost::mt11213b eng( seed );
      boost::random::uniform_real_distribution<double> dist( low, high );
      boost::variate_generator<boost::mt11213b, boost::random::uniform_real_distribution<double> > rnd( eng, dist );
      return rnd;
   }

   template< class T >
   T
   generate_uniform( const T& low = 0,
                     const T& high = 1,
                     engine_type& engine = hpc::engine )
   {
      return typename mpl::at<uniform_distribution_types,T>::type( low, high )( engine );
   }

   template< class T >
   T
   generate_normal( const T& mean = 0,
                    const T& std_dev = 1,
                    engine_type& engine = hpc::engine )
   {
      return typename mpl::at<normal_distribution_types,T>::type( mean, std_dev )( engine );
   }

   template< class T >
   class uniform_generator
   {
   public:

      uniform_generator( const T& low = 0,
                         const T& high = 1 )
         : _dist( low, high )
      {
      }

      ~uniform_generator()
      {
      }

      void
      set_range( const T& low,
                 const T& high )
      {
         typename mpl::at<uniform_distribution_types,T>::type::param_type params( low, high );
         this->_dist.param( params );
      }

      template< class SeedType >
      void
      set_seed( SeedType seed )
      {
	 engine.seed( seed );
	 _dist.reset();
      }

      void
      reset()
      {
	 _dist.reset();
      }

      T
      operator()()
      {
         return this->_dist( engine );
      }

   protected:

      typename mpl::at<uniform_distribution_types, T>::type _dist;
   };

   template< class T >
   class normal_generator
   {
   public:

      normal_generator( const T& mean = 0,
                        const T& std_dev = 1 )
         : _dist( mean, std_dev )
      {
      }

      ~normal_generator()
      {
      }

      void
      set_range( const T& mean,
                 const T& std_dev )
      {
         typename mpl::at<normal_distribution_types,T>::type::param_type params( mean, std_dev );
         _dist.param( params );
      }

      template< class SeedType >
      void
      set_seed( SeedType seed )
      {
	 engine.seed( seed );
	 _dist.reset();
      }

      void
      reset()
      {
	 _dist.reset();
      }

      T
      operator()()
      {
         return _dist( engine );
      }

   protected:

      typename mpl::at<normal_distribution_types,T>::type _dist;
   };

}

#endif
