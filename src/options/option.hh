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

#include <list>
#include <boost/mpl/map.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/at.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/spirit/home/support/detail/hold_any.hpp>
#include "libhpc/debug/debug.hh"
#include "libhpc/containers/containers.hh"
#include "format.hh"

namespace hpc {
   namespace options {

      ///
      ///
      ///
      class option_base
      {
      public:

	 enum kind_type
	 {
	    LIST,
	    BOOLEAN,
	    NORMAL
	 };

      public:

         explicit
         option_base( const hpc::string& name = hpc::string(),
		      const hpc::string& short_name = hpc::string(),
		      const hpc::string& desc = hpc::string(),
		      kind_type kind = NORMAL );

         ~option_base();

         virtual
         void
         parse( const hpc::string& value ) = 0;

         virtual
         hpc::string
         store() const = 0;

         virtual
         void
         store_visit( format& fmt );

	 virtual
	 void
	 write_variable() const;

         void
         set_name( const hpc::string& name );

         const hpc::string&
         name() const;

	 const hpc::string&
	 short_name() const;

	 const hpc::string&
	 description() const;

         bool
         has_value() const;

         bool
         has_value_or_default() const;

         bool
         is_list() const;

         bool
         is_boolean() const;

         option_base&
         operator=( const hpc::string& value );

      protected:

         hpc::string _name;
	 hpc::string _short;
	 hpc::string _desc;
         bool _has_val;
	 bool _has_def;
	 kind_type _kind;
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
		 const hpc::string& short_name = hpc::string(),
                 optional<T> def = optional<T>(),
		 optional<T&> variable = optional<T&>(),
		 const hpc::string& desc = hpc::string(),
		 kind_type kind = NORMAL )
            : option_base( name, short_name, desc, kind ),
              _def( def ),
	      _var( variable )
         {
	    if( def )
	       _has_def = true;
         }

         ~option()
         {
         }

         virtual
         void
         parse( const hpc::string& value )
	 {
	    _val = boost::lexical_cast<value_type>( value );
	    _has_val = true;
	 }

         virtual
         hpc::string
         store() const
	 {
	    if( _val )
	       return to_string( *_val );
	    else
	       return to_string( *_def );
	 }

	 virtual
	 void
	 write_variable() const
	 {
	    if( _var )
	    {
	       auto val = get();
	       if( val )
		  *_var = *val;
	    }
	 }

         void
         set_value( const T& value )
         {
            _val = value;
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
	 optional<T&> _var;
      };

      ///
      ///
      ///
      class string
         : public option<hpc::string>
      {
      public:

         string( const hpc::string& name,
		 const hpc::string& short_name,
                 optional<hpc::string> default_value,
		 optional<hpc::string&> variable = optional<hpc::string&>(),
		 const hpc::string& desc = hpc::string(),
		 bool strip = true );

         string( const hpc::string& name,
		 const hpc::string& short_name = hpc::string(),
                 const char* default_value = NULL,
		 optional<hpc::string&> variable = optional<hpc::string&>(),
		 const hpc::string& desc = hpc::string(),
		 bool strip = true );

         virtual
         void
         parse( const hpc::string& value );

         virtual
         hpc::string
         store() const;

      protected:

	 bool _strip;
      };

      class boolean
         : public option<bool>
      {
      public:

         boolean( const hpc::string& name,
		  const hpc::string& short_name,
		  optional<bool> default_value,
		  optional<bool&> variable = optional<bool&>(),
		  const hpc::string& desc = hpc::string() );

         virtual
         void
         parse( const hpc::string& value );
      };

      typedef option<int>                integer;
      typedef option<unsigned>           integer_u;
      typedef option<long long>          integer_ll;
      typedef option<unsigned long long> integer_ull;
      typedef option<float>              real;
      typedef option<double>             real_d;

      ///
      ///
      ///
      template< class Option >
      class list
         : public option_base
      {
      public:

         typedef Option sub_option_type;
         typedef typename sub_option_type::value_type sub_value_type;
         typedef std::list<sub_value_type> value_type;

      public:

         list( const hpc::string& name,
	       const hpc::string& short_name = hpc::string(),
	       const hpc::string& desc = hpc::string() )
            : option_base( name, short_name, desc, LIST ),
              _sub_opt( "" )
         {
         }

         virtual
         void
         parse( const hpc::string& value )
         {
            _sub_opt.parse( value );
            if( !_val )
               _val = value_type();
            _val->push_back( _sub_opt.value() );
            _has_val = true;
         }

         virtual
         hpc::string
         store() const
         {
            hpc::string txt;
            if( _val )
            {
               sub_option_type opt( "" );
               txt = "[";
               typename value_type::const_iterator it = _val->cbegin();
               opt.set_value( *it++ );
               txt += opt.store();
               while( it != _val->cend() )
               {
                  opt.set_value( *it++ );
                  txt += ", ";
                  txt += opt.store();
               }
               txt += "]";
            }
            return txt;
         }

         virtual
         void
         store_visit( format& fmt )
         {
            if( _val )
            {
               fmt.start_list( _name );
               for( typename value_type::iterator it = _val->begin();
                    it != _val->end();
                    ++it )
               {
                  _sub_opt.set_value( *it );
                  fmt.add_list_item( _sub_opt.store() );
               }
               fmt.end_list();
            }
         }

         optional<const value_type&>
         get() const
         {
            if( _val )
               return *_val;
            else
            {
               // TODO: Prepare defaults.
               return none;
            }
         }

      protected:

         optional<value_type> _val;
         sub_option_type _sub_opt;
      };

      typedef boost::mpl::map< boost::mpl::pair<hpc::string,        string>,
                               boost::mpl::pair<bool,               boolean>,
                               boost::mpl::pair<int,                integer>,
                               boost::mpl::pair<unsigned int,       integer_u>,
                               boost::mpl::pair<long,               integer>,
                               boost::mpl::pair<unsigned long,      integer_u>,
                               boost::mpl::pair<long long,          integer_ll>,
                               boost::mpl::pair<unsigned long long, integer_ull>,
                               boost::mpl::pair<float,              real>,
                               boost::mpl::pair<double,             real_d> > type_map;
   }
}

#endif
