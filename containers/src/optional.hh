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
