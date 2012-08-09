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

#ifndef libhpc_containers_options_hh
#define libhpc_containers_options_hh

#include <boost/lexical_cast.hpp>
#include <boost/spirit/home/support/detail/hold_any.hpp>
#include "libhpc/debug/debug.hh"
#include "string.hh"
#include "optional.hh"
#include "map.hh"

namespace hpc {
   namespace options {

      ///
      /// TODO: May need to rewrite boost::any, as it uses exceptions.
      ///
      class dictionary
         : public map<string,boost::spirit::hold_any>
      {
      public:

         typedef map<string,boost::spirit::hold_any> super_type;

      public:

         template< class T,
                   class Source >
         void
         option( const string& name,
                 Source& source,
                 optional<T> default_value = optional<T>() )
         {
            auto it = source.find( name );
            if( it != source.end() )
               insert( name, boost::lexical_cast<T>( it->second ) );
            else if( default_value )
               insert( name, *default_value );
         }

         // TODO: Fix the long iterator name.
         template< class T >
         std::pair<typename std::map<string,boost::spirit::hold_any>::iterator,bool>
         insert( const string& name,
                 const T& value )
         {
            boost::spirit::hold_any any_val( value );
            return super_type::insert( name, any_val );
         }

         template< class T >
         T
         get( const string& name )
         {
            return boost::spirit::any_cast<T>( super_type::get( name ) );
         }
      };

      // template< class T >
      // class option
      // {
      // public:

      //    explicit
      //    option( const string& name = string() )
      //       : _name( name )
      //    {
      //    }

      //    explicit
      //    option( const string& name,
      //            const T& default_value )
      //       : _name( name ),
      //         _def( default_value )
      //    {
      //    }

      //    ~option()
      //    {
      //    }

      //    void
      //    set_name( const string& name )
      //    {
      //       _name = name;
      //    }

      //    void
      //    set_default_value( const T& default_value )
      //    {
      //       _def = default_value;
      //    }

      //    const string&
      //    name() const
      //    {
      //       return _name;
      //    }

      //    const T&
      //    default_value() const
      //    {
      //       return _def;
      //    }

      //    void
      //    store( dictionary& dict,
      //           optional<const string&> value )
      //    {
      //       if( value )
      //          dict.insert( _name, boost::lexical_cast<T>( *value ) );
      //       else if( _def )
      //          dict.insert( _name, *_def );
      //    }

      // protected:

      //    string _name;
      //    optional<T> _def;
      // };

      // class group
      // {
      // public:

      //    template< class T >
      //    void
      //    add_option( const string& name )
      //    {
      //       _opts.push_back( option<T>( name ) );
      //    }

      //    template< class T >
      //    void
      //    add_option( const string& name,
      //                const T& default_value )
      //    {
      //       _opts.push_back( option<T>( name, default_value ) );
      //    }

      // protected:

      //    list<boost::spirit::hold_any> _opts;
      // };
   }
}

#endif
