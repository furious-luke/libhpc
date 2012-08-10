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
#include "list.hh"
#include "shared_ptr.hh"

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
         store() = 0;

         void
         set_name( const hpc::string& name );

         const hpc::string&
         name() const;

      protected:

         hpc::string _name;
      };

      ///
      ///
      ///
      class dictionary
      {
      public:

         void
         add_option( option_base* opt );

         void
         add_dictionary( dictionary* dict );

      protected:

         list<shared_ptr<option_base>> _opts;
         list<shared_ptr<dictionary>> _dicts;
      };

      ///
      /// TODO: May need to rewrite boost::any, as it uses exceptions.
      ///
      // class dictionary
      //    : public map<string,boost::spirit::hold_any>
      // {
      // public:

      //    typedef map<string,boost::spirit::hold_any> super_type;

      // public:

      //    template< class T,
      //              class Source >
      //    void
      //    option( const string& name,
      //            Source& source,
      //            optional<T> default_value = optional<T>() )
      //    {
      //       auto it = source.find( name );
      //       if( it != source.end() )
      //          insert( name, boost::lexical_cast<T>( it->second ) );
      //       else if( default_value )
      //          insert( name, *default_value );
      //    }

      //    // TODO: Fix the long iterator name.
      //    template< class T >
      //    std::pair<typename std::map<string,boost::spirit::hold_any>::iterator,bool>
      //    insert( const string& name,
      //            const T& value )
      //    {
      //       boost::spirit::hold_any any_val( value );
      //       return super_type::insert( name, any_val );
      //    }

      //    template< class T >
      //    T
      //    get( const string& name )
      //    {
      //       return boost::spirit::any_cast<T>( super_type::get( name ) );
      //    }
      // };

      ///
      ///
      ///
      template< class T >
      class option
         : public option_base
      {
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

         bool
         has_value() const
         {
            return _val;
         }

         const T&
         value() const
         {
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

         virtual
         void
         parse( const hpc::string& value );

         virtual
         hpc::string
         store();
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
         store();
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
         store();
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
         store();
      };
   }
}

#endif
