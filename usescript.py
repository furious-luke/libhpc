# Define some arguments.
args = (arguments()
        ('--debug', action='store_true', help='Enable debugging mode.')
        ('--prefix', default='/usr/local', help='Installation path.')
        ('--enable-instrument', dest='instrument', action='store_true', help='Enable instrumentation.')
        ('--disable-logging', dest='nlog', action='store_true', help='Disable all logging routines.'))

# Define some options.
cc_opts = (
    options(cxx11=True, pic=True, define=[platform.os_name.upper()]) + 
    options(args.debug == True, prefix='build/debug', header_dirs=['build/debug/include'], optimise=0, symbols=True) +
    options(args.debug == False, prefix='build/optimised', header_dirs=['build/optimised/include'],
            optimise=3, symbols=False, define=['NDEBUG']) +
    options(args.instrument == False, define=['NINSTRUMENT']) +
    options(args.nlog == True, define=['NLOG'])
)
bin_opts = options(cxx11=True)
cp_opts = (
    options(args.debug == True, prefix='build/debug/include/libhpc') +
    options(args.debug == False, prefix='build/optimised/include/libhpc')
)

# Define compilers/linkers.
cc  = use('cxx_compiler', cc_opts, compile=True)
sl  = use('cxx_compiler', bin_opts + cc_opts, shared_lib=True)
sl_inst = use('cxx_compiler', bin_opts + cc_opts, targets.contains('install'), shared_lib=True, prefix=args.prefix)
bin = use('cxx_compiler', bin_opts + cc_opts)
ar  = use('ar', cc_opts, add=True)

# Which packages will we be using?
boost   = use('boost')
mpi     = use('mpi')
hdf5    = use('hdf5')
pugixml = use('pugixml')
files   = use('files')
cp      = files.feature('copy', cp_opts)
hdr_inst = files.feature('copy', None, targets.contains('install'), prefix=args.prefix + '/include/libhpc')
lib_inst = files.feature('copy', None, targets.contains('install'), prefix=args.prefix)

# Setup flows.
cc  = cc  + boost + mpi + hdf5 + pugixml
sl  = sl  + boost + mpi + hdf5 + pugixml
bin = bin + boost + mpi + hdf5 + pugixml

# Copy all headers.
hdrs = rule(r'src/.+\.hh', cp & hdr_inst, target_strip_dirs=1)
tccs = rule(r'src/.+\.tcc', cp & hdr_inst, target_strip_dirs=1)

# Build all sources.
objs = rule(r'src/.+\.cc', cc)

# Link into static/shared library.
static_lib = rule(objs, ar, target=platform.make_static_library('lib/hpc'))
shared_lib = rule(objs, sl & sl_inst, target=platform.make_shared_library('lib/hpc'))
rule(static_lib, lib_inst, target_strip_dirs=2)
