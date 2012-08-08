#ifndef memory_state_hh
#define memory_state_hh

namespace hpc {
namespace memory {

    typedef struct {
	size_t size;
	size_t peak;
    } state_t;

}
}

#endif
