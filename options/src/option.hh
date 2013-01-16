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

#ifndef libhpc_options_option_hh
#define libhpc_options_option_hh

#include <boost/mpl/map.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/at.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/spirit/home/support/detail/hold_any.hpp>
#include "libhpc/debug/debug.hh"
#include "libhpc/containers/containers.hh"

namespace hpc {
   namespace options {

      ///
      ///
      ///
      class option_base
      {
      public:

         explicit
         option_base( const hpc::string& name = hpc::string() );

         ~option_base();

         virtual
         void
         parse( const hpc::string& value ) = 0;

         virtual
         hpc::string
         store() const = 0;

         void
         set_name( const hpc::string& name );

         const hpc::string&
         name() const;

         bool
         has_value() const;

         option_base&
         operator=( const hpc::string& value );

      protected:

         hpc::string _name;
         bool _has_val;
      };

      ///
      ///
      ///
      template< class T >
      class option
         : public option_base
      {
      public:

         typedef T value_type;

      public:

         option( const hpc::string& name,
                 optional<T> default_value = optional<T>() )
            : option_base( name ),
              _def( default_value )
         {
         }

         ~option()
         {
         }

         void
         set_default_value( const T& default_value )
         {
            _def = default_value;
         }

         bool
         has_default_value() const
         {
            return _def;
         }

         const T&
         default_value() const
         {
            return *_def;
         }

         const T&
         value() const
         {
            ASSERT( _val, "Value has not been set." );
            return *_val;
         }

         optional<const T&>
         get() const
         {
            if( _val )
               return *_val;
            else if( _def )
               return *_def;
            else
               return none;
         }

         friend std::ostream&
         operator<<( std::ostream& strm,
                     const option<T>& obj )
         {
            return strm;
         }

      protected:

         optional<T> _val;
         optional<T> _def;
      };

      ///
      ///
      ///
      class string
         : public option<hpc::string>
      {
      public:

         string( const hpc::string& name,
                 optional<hpc::string> default_value = optional<hpc::string>() );

         string( const hpc::string& name,
                 optional<const char*> default_value );

         virtual
         void
         parse( const hpc::string& value );

         virtual
         hpc::string
         store() const;
      };

      ///
      ///
      ///
      class boolean
         : public option<bool>
      {
      public:

         boolean( const hpc::string& name,
                  optional<bool> default_value = optional<bool>() );

         virtual
         void
         parse( const hpc::string& value );

         virtual
         hpc::string
         store() const;
      };

      ///
      ///
      ///
      class integer
         : public option<unsigned long>
      {
      public:

         integer( const hpc::string& name,
                  optional<unsigned long> default_value = optional<unsigned long>() );

         virtual
         void
         parse( const hpc::string& value );

         virtual
         hpc::string
         store() const;
      };

      ///
      ///
      ///
      class real
         : public option<double>
      {
      public:

         real( const hpc::string& name,
               optional<double> default_value = optional<double>() );

         virtual
         void
         parse( const hpc::string& value );

         virtual
         hpc::string
         store() const;
      };

      // ///
      // ///
      // ///
      // template< class T >
      // class list
      //    : public option_base
      // {
      // public:

      //    typedef typename Option::value_type sub_value_type;
      //    typedef list<sub_value_type> value_type;

      // public:

      //    virtual
      //    void
      //    parse( const hpc::string& value );

      // protected:

      //    value_type _val;
      // };

      typedef boost::mpl::map< boost::mpl::pair<hpc::string, string>,
                               boost::mpl::pair<bool, boolean>,
                               boost::mpl::pair<int, integer>,
                               boost::mpl::pair<unsigned int, integer>,
                               boost::mpl::pair<long, integer>,
                               boost::mpl::pair<unsigned long, integer>,
                               boost::mpl::pair<float, real>,
                               boost::mpl::pair<double, real> > type_map;
   }
}

#endif
