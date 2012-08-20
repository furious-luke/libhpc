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

#include <stdlib.h>
#include <iostream>
#include "libhpc/sound/pcm.hh"
#include "libhpc/system/epoll.hh"

using namespace hpc;

int
main()
{
   sound::pcm dev;
   std::cout << "Rate: " << dev.sample_rate() << "\n";
   std::cout << "Period size: " << dev.period_size() << "\n";
   unix::epoll poll( dev.poll_pipes()[0] );
   // while( 1 )
   // {
      std::cout << "waiting" << "\n";
      poll.wait();
      long frames = dev.available_frames();
      std::cout << frames << "\n";

      // vector<uint16>::view buf = dev.buffer( frames );
      
   // }
   return EXIT_SUCCESS;
}
