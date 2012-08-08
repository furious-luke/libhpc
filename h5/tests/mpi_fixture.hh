class mpi_fixture : public CxxTest::GlobalFixture {
public:
  bool setUpWorld() {
    hpc::mpi::initialise();
    return true;
  }
  bool tearDownWorld() {
    hpc::mpi::finalise(false);
    return true;
  }
};

static mpi_fixture mpi_fix;
