#include <sys/types.h>
#include <unistd.h>
#include "id.hh"

namespace hpc {
   namespace os {

      long
      get_process_id()
      {
	 return getpid();
      }
   }
}
