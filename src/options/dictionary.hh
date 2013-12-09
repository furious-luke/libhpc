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

#ifndef libhpc_options_dictionary_hh
#define libhpc_options_dictionary_hh

#include <boost/lexical_cast.hpp>
#include <boost/spirit/home/support/detail/hold_any.hpp>
#include "libhpc/debug/debug.hh"
#include "libhpc/containers/containers.hh"
#include "libhpc/regexp/multimatch.hh"
#include "option.hh"
#include "bad_option.hh"

namespace hpc {
   namespace options {

      class no_value
         : public bad_option
      {
      };

      ///
      ///
      ///
      class dictionary
      {
         friend std::ostream&
         operator<<( std::ostream& strm,
                     const dictionary& obj );

      public:

         dictionary( const hpc::string& prefix = hpc::string() );

         void
         clear();

	 void
	 set_use_config( bool state );

	 void
	 reset_positionals();

	 void
	 write_variables() const;

	 bool
	 help_requested() const;

	 void
	 print_help( const hpc::string& binary = hpc::string(),
		     const hpc::string& short_pre = "-",
		     const hpc::string& long_pre = "--" ) const;

	 bool
	 has_errors() const;

	 void
	 print_errors() const;

	 bool
	 config_requested() const;

	 template< class Format >
	 void
	 generate_config() const
	 {
	    Format fmt;
	    fmt.write( *opt<hpc::string>( "gen-config" ), *this, true );
	 }

         template< class T >
         const T
         get( const hpc::string& name ) const
         {
            typedef typename boost::mpl::at<type_map,T>::type option_type;
            const option_type& opt = (const option_type&)((*this)[name]);
            EXCEPTAS( opt.get(), no_value, "Error: No value in dictionary for option: ", name );
            return *opt.get();
         }

         template< class T >
         const T
         get( const hpc::string& name,
              const T& default_value ) const
         {
            typedef typename boost::mpl::at<type_map,T>::type option_type;
            const option_type& opt = (const option_type&)((*this)[name]);
            if( opt.get() )
               return *opt.get();
            else
               return default_value;
         }

         template< class T >
         optional<const T>
         opt( const hpc::string& name ) const
         {
            typedef typename boost::mpl::at<type_map,T>::type option_type;
            const option_type& opt = (const option_type&)((*this)[name]);
	    optional<const typename option_type::value_type&> val = opt.get();
	    if( !val )
	       return none;
	    else
	    {
	       T cast_val = *val;
	       return optional<const T>( cast_val );
	    }
         }

         template< class T >
         const hpc::list<typename boost::mpl::at<type_map,T>::type::value_type>&
         get_list( const hpc::string& name ) const
         {
            typedef typename boost::mpl::at<type_map,T>::type sub_option_type;
            typedef typename sub_option_type::value_type value_type;
            typedef options::list<sub_option_type> option_type;
            const option_type& opt = (const option_type&)((*this)[name]);
            EXCEPTAS( opt.get(), no_value, "Error: No value in dictionary for option: ", name );
            const hpc::list<value_type>& ref = *opt.get();
            return ref;
         }

         template< class T >
         const hpc::list<typename boost::mpl::at<type_map,T>::type::value_type>&
         get_list( const hpc::string& name,
                   const hpc::list<typename boost::mpl::at<type_map,T>::type::value_type>& default_value ) const
         {
            typedef typename boost::mpl::at<type_map,T>::type sub_option_type;
            typedef typename sub_option_type::value_type value_type;
            typedef options::list<sub_option_type> option_type;
            const option_type& opt = (const option_type&)((*this)[name]);
            if( opt.get() )
               return *opt.get();
            else
               return default_value;
         }

         const dictionary&
         sub( const hpc::string& prefix ) const;

         dictionary&
         sub( const hpc::string& prefix );

	 template< typename Option,
		   typename... Args >
	 void
	 add_option( Args&&... args )
	 {
	    add_option( new Option( std::forward<Args>( args )... ) );
	 }

	 void
         add_option( option_base* opt,
		     optional<const hpc::string&> prefix = optional<const hpc::string&>() );

	 void
         add_option( option_base* opt,
		     const char* prefix );

         void
         add_dictionary( dictionary* dict );

         void
         compile();

         const hpc::string&
         prefix() const;

         bool
         has_option( const hpc::string& name ) const;

	 bool
	 has_short_option( const hpc::string& name ) const;

	 const hpc::string&
	 short_to_long( const hpc::string& name ) const;

	 void
	 set_next_positional( const hpc::string& value );

         vector<shared_ptr<option_base> >::const_iterator
         options_cbegin() const;

         vector<shared_ptr<option_base> >::iterator
         options_begin();

         vector<shared_ptr<option_base> >::const_iterator
         options_cend() const;

         vector<shared_ptr<option_base> >::iterator
         options_end();

         vector<shared_ptr<dictionary> >::const_iterator
         dicts_begin() const;

         vector<shared_ptr<dictionary> >::const_iterator
         dicts_end() const;

         const option_base*
         find( const hpc::string& name ) const;

	 const dictionary*
         find_sub( const hpc::string& prefix ) const;

         dictionary*
         find_sub( const hpc::string& prefix );

         const option_base&
         operator[]( const hpc::string& name ) const;

         option_base&
         operator[]( const hpc::string& name );

      protected:

      protected:

         bool _ready;
         hpc::string _sep;
         hpc::string _pre;
         vector<shared_ptr<option_base> > _opts;
         vector<shared_ptr<dictionary> > _dicts;
	 vector<shared_ptr<option_base> > _pos_opts;
         multimatch _opts_mm;
         multimatch _dicts_mm;
	 map<hpc::string,hpc::string> _s_to_l;
	 unsigned _cur_pos;
	 bool _help_req, _help_val_req;
	 bool _use_config;
      };

   }
}

#endif
