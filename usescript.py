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
        ('--prefix', default='/usr/local',
         help='Installation path.')
        ('--enable-debug', dest='debug', action='boolean', default=False,
         help='Enable/disable debugging mode.')
        ('--enable-openmp', dest='openmp', action='boolean', default=False,
         help='Enable/disable OpenMP.')
        ('--enable-instrument', dest='instrument', action='boolean', default=False,
         help='Enable/disable instrumentation.')
        ('--enable-stacktrace', dest='stacktrace', action='boolean', default=False,
         help='Enable/disable debugging stacktrace.')
        ('--enable-memory-debug', dest='memory_debug', action='boolean', default=False,
         help='Enable/disable memory debugging.')
        ('--enable-memory-ops', dest='memory_ops', action='boolean', default=False,
         help='Enable/disable memory operation logging.')
        ('--enable-memory-stats', dest='memory_stats', action='boolean', default=False,
         help='Enable/disable memory statistics logging.')
        ('--enable-logging', dest='logging', action='boolean', default=True,
         help='Enable/disable all logging routines.'))

##
## Declare the packages we are interested in.
##

glut    = use('glut')
hdf5    = use('hdf5')
boost   = use('boost')
mpi     = use('mpi')
pugixml = use('pugixml')
eigen   = use('eigen')

##
## Declare option sets.
##

opts = (

    # General options.
    options(cxx11        = True,
            pic          = True,
            library_dirs = ['{prefix}/lib'],
            rpath_dirs   = ['{prefix}/lib'],
            header_dirs  = ['{prefix}/include'],
            define       = [platform.os_name.upper()]) + 

    # Debug mode.
    options(args.debug == True,
            prefix   = 'build/debug',
            optimise = 0,
            symbols  = True) +

    # Optimised mode.
    options(args.debug == False,
            prefix   = 'build/optimised',
            optimise = 3,
            symbols  = False,
            define   = ['NDEBUG', 'NLOGTRIVIAL', 'NLOGDEBUG']) +

    # Will override previous prefixes if enabled.
    options(targets.contains('install'), prefix = args.prefix) + 

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
    options(hdf5.has_feature('parallel') == True, define=['PARALLELHDF5']) + 
    options(eigen.have                   == True, define=['HAVE_EIGEN'])
)

##
## Define compilers/linkers, or anything dependant on the options.
##

cc        =           use('cxx_compiler', opts, compile=True)
sl        =           use('cxx_compiler', opts, shared_lib=True)
bin       =           use('cxx_compiler', opts)
ar        =           use('ar',           opts, add=True)
cp        = files.feature('copy',         opts, join_path='include/libhpc')
run_tests = files.feature('run',          None, targets.contains('check'))
pkgs      = boost + mpi + hdf5 + pugixml + (glut | identity) + (eigen | identity)
cc        = cc  + pkgs
sl        = sl  + pkgs
bin       = bin + pkgs

##
## Declare build rules.
##

hdrs = rule(('src', r'.+\.(?:hh|hpp|tcc)$)', cp, target_strip_dirs=1)
objs = rule(('src', r'.+\.cc$'),             cc)

static_lib = rule(objs, ar, target=platform.make_static_library('lib/hpc'))
shared_lib = rule(objs, sl, target=platform.make_shared_library('lib/hpc'))

tests = rule(('tests', r'.+\.cc$'), bin,       single=False, suffix='')
dummy = rule(tests,                 run_tests, target=dummies.always)

# Examples.
dummy = rule(('exs/system/daemon', r'.+\.cc$'), bin, target='exs/daemon')
dummy = rule(('exs/mpi/host',      r'.+\.cc$'), bin, target='exs/host')
