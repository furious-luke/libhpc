#ifndef hpc_containers_optional_hh
#define hpc_containers_optional_hh

#include <boost/optional.hpp>
#include <boost/none.hpp>

using boost::none;
using boost::optional;

// namespace hpc {

//    template< class T >
//    class optional
//       : public boost::optional<T>
//    {
//    public:

//       optional()
//       {
//       }

//       optional( none_t none_ )
//          : optional<T>( none_ )
//       {
//       }

//       optional( argument_type val )
//          : optional<T>( val )
//       {
//       }

//       optional ( bool cond, argument_type val ) : base(cond,val) {}
//    };
// }

#endif
