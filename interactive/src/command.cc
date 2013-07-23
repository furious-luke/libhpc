#include "command.hh"

namespace tao {
   using namespace hpc;

   command::command()
   {
   }

   command::command( const string& re_str,
                     function_type action )
      : _re_str( re_str ),
        _re( re_str ),
        _act( action )
   {
   }

   const string&
   command::re_string() const
   {
      return _re_str;
   }

   const re::re&
   command::re() const
   {
      return _re;
   }

   void
   command::operator()( const re::match& match ) const
   {
      _act( match );
   }

}
