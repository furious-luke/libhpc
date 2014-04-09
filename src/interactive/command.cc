#include "command.hh"

namespace hpc {
   namespace command {

      command::command()
      {
      }

      command::command( std::string const& expr,
                        function_type action )
         : _re_str( expr ),
           _re( expr ),
           _act( action )
      {
      }

      std::string const&
      command::expression() const
      {
         return _re_str;
      }

      boost::regex const&
      command::re() const
      {
         return _re;
      }

      void
      command::operator()( boost::smatch const& match ) const
      {
         _act( match );
      }

   }
}
