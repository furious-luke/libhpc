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

#include <boost/algorithm/string/trim.hpp>
#include "option.hh"

namespace hpc {
   namespace options {

      option_base::option_base( const hpc::string& name,
				const hpc::string& short_name,
				const hpc::string& desc,
				kind_type kind )
         : _name( name ),
	   _short( short_name ),
	   _desc( desc ),
           _has_val( false ),
	   _has_def( false ),
	   _kind( kind )
      {
	 std::transform( _name.begin(), _name.end(), _name.begin(), ::tolower );
      }

      option_base::~option_base()
      {
      }

      void
      option_base::store_visit( format& fmt )
      {
         ASSERT( 0, "Not implemented." );
      }

      void
      option_base::write_variable() const
      {
      }

      void
      option_base::set_name( const hpc::string& name )
      {
         _name = name;
	 std::transform( _name.begin(), _name.end(), _name.begin(), ::tolower );
      }

      const hpc::string&
      option_base::name() const
      {
         return _name;
      }

      const hpc::string&
      option_base::short_name() const
      {
         return _short;
      }

      const hpc::string&
      option_base::description() const
      {
	 return _desc;
      }

      bool
      option_base::has_value() const
      {
         return _has_val;
      }

      bool
      option_base::has_value_or_default() const
      {
	 return _has_val | _has_def;
      }

      bool
      option_base::is_list() const
      {
         return _kind == LIST;
      }

      bool
      option_base::is_boolean() const
      {
         return _kind == BOOLEAN;
      }

      option_base&
      option_base::operator=( const hpc::string& value )
      {
         parse( value );
         return *this;
      }

      string::string( const hpc::string& name,
		      const hpc::string& short_name,
                      optional<hpc::string> default_value,
		      optional<hpc::string&> variable,
		      const hpc::string& desc,
		      bool strip )
         : option<hpc::string>( name, short_name, default_value, variable, desc ),
	   _strip( strip )
      {
      }

      string::string( const hpc::string& name,
		      const hpc::string& short_name,
		      const char* default_value,
		      optional<hpc::string&> variable,
		      const hpc::string& desc,
		      bool strip )
	 : option<hpc::string>( name, short_name, none, variable, desc ),
	   _strip( strip )
      {
	 if( default_value != NULL )
	    _def = hpc::string( default_value );
      }

      void
      string::parse( const hpc::string& value )
      {
         _val = boost::algorithm::trim_copy( value );
         _has_val = true;
      }

      hpc::string
      string::store() const
      {
	 if( _val )
	    return *_val;
	 else
	    return *_def;
      }

      boolean::boolean( const hpc::string& name,
			const hpc::string& short_name,
			optional<bool> default_value,
			optional<bool&> variable,
			const hpc::string& desc )
	 : option<bool>( name, short_name, default_value, variable, desc, BOOLEAN )
      {
      }

      void
      boolean::parse( const hpc::string& value )
      {
	 // TODO: Optimise this with RE.
	 hpc::string low;
	 low.resize( value.size() );
	 std::transform( value.begin(), value.end(), low.begin(), ::tolower );
         _val = (value == "true" ||
		 value == "t" ||
		 value == "1" ||
		 value == "yes" ||
		 value == "y");
         _has_val = true;
      }

   }
}
