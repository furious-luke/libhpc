# Define some arguments.
args = (arguments()
        ('--debug', action='store_true', help='Enable debugging mode.')
        ('--prefix', help='Installation path.')
        ('--enable-instrument', dest='instrument', action='store_true', help='Enable instrumentation.')
        ('--disable-logging', dest='nlog', action='store_true', help='Disable all logging routines.'))

# Define some options.
cc_opts = (
    options(cxx11=True, pic=True, define=[platform.os_name.upper()]) + 
    options(args.debug == True, prefix='build/debug', header_dirs=['build/debug/include'], optimise=0, symbols=True) +
    options(args.debug == False, prefix='build/optimised', header_dirs=['build/optimised/include'], optimise=3, symbols=False) +
    options(args.instrument == False, define=['NINSTRUMENT']) +
    options(args.nlog == True, define=['NLOG'])
)
bin_opts = options(cxx11=True)
cp_opts = (
    options(args.debug == True, prefix='build/debug/include/libhpc') +
    options(args.debug == False, prefix='build/optimised/include/libhpc')
)

# Define compilers/linkers.
cc  = use('clangxx', cc_opts, compile=True)
sl  = use('clangxx', bin_opts + cc_opts, shared_lib=True)
bin = use('clangxx', bin_opts + cc_opts)
ar  = use('ar', cc_opts, add=True)

# Which packages will we be using?
boost   = use('boost')
mpi     = use('mpich2')
hdf5    = use('hdf5')
pugixml = use('pugixml')
files   = use('files')
cp      = files.feature('copy', cp_opts)

# Setup flows.
cc  = cc  + boost + mpi + hdf5 + pugixml
sl  = sl  + boost + mpi + hdf5 + pugixml
bin = bin + boost + mpi + hdf5 + pugixml

# Copy all headers.
hdrs = rule(r'src/.+\.hh', cp, target_strip_dirs=1)
tccs = rule(r'src/.+\.tcc', cp, target_strip_dirs=1)

# Build all sources.
objs = rule(r'src/.+\.cc', cc)

# Link into static/shared library.
static_lib = rule(objs, ar, target=platform.make_static_library('lib/hpc'))
shared_lib = rule(objs, sl, target=platform.make_shared_library('lib/hpc'))
