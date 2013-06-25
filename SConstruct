# Modify Python path to locate local scons-config and
# scons-project.
import sys, os
sys.path.append(os.path.join(os.getcwd(), 'site_scons'))

import sconsproject as project
from sconsproject import config

config.select(
    config.packages.clock(),
    config.packages.boost(),
    config.packages.MPI(),
    config.packages.HDF5(),
    config.packages.pugixml(),
#    config.packages.Eigen(required=False),
#    config.packages.FFTW3(use_float=True, required=False),
    config.packages.CUDA(required=False),
)

vars = project.create_variables()
vars.AddVariables(
    BoolVariable('MEMDEBUG', 'Enable memory debugging.', True),
    BoolVariable('MEMOPS', 'Log memory operations.', False),
    BoolVariable('MEMSTATS', 'Enable memory statistics.', False),
    BoolVariable('STACKTRACE', 'Enable stack trace.', False),
    BoolVariable('INSTRUMENT', 'Enable instrumentation.', False),
    BoolVariable('WITH_CUDA', 'Enable CUDA.', False),
)

env = project.create_environment(vars)
if env['WITH_CUDA']:
    env.Tool('nvcc')
else:
    env.AppendUnique(CCFLAGS='-std=c++11')
if not env['MEMDEBUG']:
    env.MergeFlags('-DNMEMDEBUG')
if not env['MEMOPS']:
    env.MergeFlags('-DNMEMOPS')
if not env['MEMSTATS']:
    env.MergeFlags('-DNMEMSTATS')
if env['STACKTRACE']:
    env.MergeFlags('-rdynamic')
    env.AppendUnique(LINKFLAGS='-rdynamic')
    env.AppendUnique(LIBS='dl')
else:
    env.MergeFlags('-DNSTACKTRACE')
if env['INSTRUMENT']:
    flags = '-finstrument-functions -finstrument-functions-exclude-function-list=__cyg_profile_func_enter,__cyg_profile_func_exit,add_instrument,instr_func_details,instr_get_enabled_node,instr_is_enabled,instr_set_enabled -rdynamic'
    env.MergeFlags(flags)
    env.AppendUnique(LINKFLAGS=flags.split(' '))
    env.AppendUnique(LIBS='dl')
else:
    env.MergeFlags('-DNINSTRUMENT')

layers = [
    'debug', 'memory', 'system', 'logging', 'profile',
    'containers', 'algorithm', 'regexp', 'options',
    'mpi', 'h5', 'numerics',
]

project.build(layers, proj_name='libhpc', env=env, vars=vars)
