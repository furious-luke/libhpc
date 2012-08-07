======
libhpc
======

Description
===========

libhpc is a collection of C++ data structures and algorithms to support
development of high-performance computing applications. In particular, careful
attention has been given to memory efficiency and algorithm complexity. MPI
(Message Passing Interface) is used to support distributed parallel execution.

The current feature set includes:
  * Basic data structures, like vectors, binary tree sets and maps, and hash
    table sets and maps.
  * Convenience wrappers for MPI and HDF5 structures and functions.
  * Efficient memory profiling, logging and timing.

License
=======

libhpc is released under the GPLv3, a copy of the license should have been 
included with the project in the root folder in a file named COPYING. If you 
require a more commercial friendly license, please contact me at 
<furious.luke@gmail.com>.

Dependencies
============

  MPI
    Message Passing Interface implementation. Tested with MPICH2 and OpenMPI.

  HDF5
    Hierarchical Data Format libraries. Currently tested with version 1.8.7.
