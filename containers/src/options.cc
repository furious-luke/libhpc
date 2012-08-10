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

#include "options.hh"

namespace hpc {
   namespace options {

      option_base::option_base( const hpc::string& name )
         : _name( name )
      {
      }

      option_base::~option_base()
      {
      }

      void
      option_base::set_name( const hpc::string& name )
      {
         _name = name;
      }

      const hpc::string&
      option_base::name() const
      {
         return _name;
      }

      void
      dictionary::add_option( option_base* opt )
      {
         _opts.push_back( opt );
      }

      void
      dictionary::add_dictionary( dictionary* dict )
      {
         _dicts.push_back( dict );
      }

      string::string( const hpc::string& name,
                      optional<hpc::string> default_value )
         : option<hpc::string>( name, default_value )
      {
      }

      void
      string::parse( const hpc::string& value )
      {
         _val = value;
      }

      hpc::string
      string::store()
      {
         return *_val;
      }

      boolean::boolean( const hpc::string& name,
                        optional<bool> default_value )
         : option<bool>( name, default_value )
      {
      }

      void
      boolean::parse( const hpc::string& value )
      {
         _val = lexical_cast<bool>( value );
      }

      hpc::string
      boolean::store()
      {
      }

      integer::integer( const hpc::string& name,
                        optional<unsigned long> default_value )
         : option<unsigned long>( name, default_value )
      {
      }

      void
      integer::parse( const hpc::string& value )
      {
         _val = lexical_cast<unsigned long>( value );
      }

      hpc::string
      integer::store()
      {
      }

      real::real( const hpc::string& name,
                  optional<double> default_value )
         : option<double>( name, default_value )
      {
      }

      void
      real::parse( const hpc::string& value )
      {
         _val = lexical_cast<double>( value );
      }

      hpc::string
      real::store()
      {
      }
   }
}
