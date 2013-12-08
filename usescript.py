##
## libhpc build script. The following are automatially added to
## the local scope:
##
##   files     - A package including basic file operations.
##   platform  - A package including platform specific routines.
##

##
## Define configuration/build arguments.
##

args = (arguments()
        ('--prefix', default='/usr/local', help='Installation path.')
        ('--enable-debug', dest='debug', action='boolean', default=False, help='Enable/disable debugging mode.')
        ('--enable-openmp', dest='openmp', action='boolean', default=False, help='Enable/disable OpenMP.')
        ('--enable-instrument', dest='instrument', action='boolean', default=False, help='Enable/disable instrumentation.')
        ('--enable-stacktrace', dest='stacktrace', action='boolean', default=False, help='Enable/disable debugging stacktrace.')
        ('--enable-memory-debug', dest='memory_debug', action='boolean', default=False, help='Enable/disable memory debugging.')
        ('--enable-memory-ops', dest='memory_ops', action='boolean', default=False, help='Enable/disable memory operation logging.')
        ('--enable-memory-stats', dest='memory_stats', action='boolean', default=False, help='Enable/disable memory statistics logging.')
        ('--enable-logging', dest='logging', action='boolean', default=True, help='Enable/disable all logging routines.'))

##
## Declare the packages we are interested in.
##

glut    = use('glut')
hdf5    = use('hdf5')
boost   = use('boost')
mpi     = use('mpi')
pugixml = use('pugixml')

##
## Declare option sets.
##

# C++ compiler options.
cc_opts = (

    # General options.
    options(cxx11  = True,
            pic    = True,
            define = [platform.os_name.upper()]) + 

    # Debug mode.
    options(args.debug == True,
            prefix       = 'build/debug',
            library_dirs = ['build/debug/lib'],
            rpath_dirs   = ['build/debug/lib'],
            header_dirs  = ['build/debug/include'],
            optimise     = 0,
            symbols      = True) +

    # Optimised mode.
    options(args.debug == False,
            prefix       = 'build/optimised',
            library_dirs = ['build/optimised/lib'],
            rpath_dirs   = ['build/optimised/lib'],
            header_dirs  = ['build/optimised/include'],
            optimise     = 3,
            symbols      = False,
            define       = ['NDEBUG', 'NLOGTRIVIAL', 'NLOGDEBUG']) +

    # Optional compilation/linking commands.
    options(args.instrument   == False, define=['NINSTRUMENT']) +
    options(args.logging      == False, define=['NLOG']) +
    options(args.stacktrace   == False, define=['NSTACKTRACE']) +
    options(args.memory_debug == False, define=['NMEMDEBUG']) +
    options(args.memory_ops   == False, define=['NMEMOPS']) +
    options(args.memory_stats == False, define=['NMEMSTATS']) +

    # Optional packages.
    options(args.openmp                  == True, openmp=True) + 
    options(glut.have                    == True, define=['HAVE_GLUT']) +
    options(hdf5.has_feature('parallel') == True, define=['PARALLELHDF5'])
)

# Copy/install options.
cp_opts = (
    options(args.debug == True,
            prefix='build/debug/include/libhpc') +
    options(args.debug == False,
            prefix='build/optimised/include/libhpc')
)

##
## Define compilers/linkers, or anything dependant on the options.
##

cc        =           use('cxx_compiler', cc_opts,                              compile=True)
sl        =           use('cxx_compiler', cc_opts,                              shared_lib=True)
sl_inst   =           use('cxx_compiler', cc_opts, targets.contains('install'), shared_lib=True, prefix=args.prefix)
bin       =           use('cxx_compiler', cc_opts)
ar        =           use('ar',           cc_opts,                              add=True)
cp        = files.feature('copy',         cp_opts)
hdr_inst  = files.feature('copy',         None,    targets.contains('install'), prefix=args.prefix + '/include/libhpc')
lib_inst  = files.feature('copy',         None,    targets.contains('install'), prefix=args.prefix)
run_tests = files.feature('run',          None,    targets.contains('check'))
pkgs      = boost + mpi + hdf5 + pugixml + (glut | identity)
cc        = cc  + pkgs
sl        = sl  + pkgs
bin       = bin + pkgs

##
## Declare build rules.
##

hdrs       = rule(r'src/.+\.(?:hh|hpp|tcc)$', cp & hdr_inst, target_strip_dirs=1)
objs       = rule(r'src/.+\.cc$',             cc)
static_lib = rule(objs,                       ar,            target=platform.make_static_library('lib/hpc'))
shared_lib = rule(objs,                       sl & sl_inst,  target=platform.make_shared_library('lib/hpc'))
dummy      = rule(static_lib,                 lib_inst,      target_strip_dirs=2)
tests      = rule(r'tests/.+\.cc$',           bin,           libraries=['hpc'], single=False, suffix='')
dummy      = rule(tests,                      run_tests,     target=dummies.always)

# Examples.
rule(r'exs/system/daemon/.+\.cc$', bin, target='exs/daemon', libraries=['hpc'])
