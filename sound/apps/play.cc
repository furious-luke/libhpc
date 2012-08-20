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
