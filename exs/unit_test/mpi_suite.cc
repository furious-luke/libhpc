#include <libhpc/unit_test/mpi_main.hh>

TEST_CASE( "name" )
{
   if( hpc::comm::world.rank() == 0 )
      TEST_TRUE( true );
   else
      TEST_TRUE( false );
}
