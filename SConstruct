import sconsproject as project
from sconsproject import config

config.select(
    config.packages.boost(),
    config.packages.MPI(),
    config.packages.HDF5(),
    config.packages.rapidxml(),
    config.packages.Eigen(required=False),
    config.packages.FFTW3(use_float=True, required=False),
)

vars = project.create_variables()
vars.AddVariables(
    BoolVariable('MEMDEBUG', 'Enable memory debugging.', True),
    BoolVariable('MEMOPS', 'Log memory operations.', False),
    BoolVariable('MEMSTATS', 'Enable memory statistics.', False),
    BoolVariable('STACKTRACE', 'Enable stack trace.', False),
)

env = project.create_environment(vars)
env.AppendUnique(CCFLAGS='-std=c++11')
if not env['MEMDEBUG']:
    env.MergeFlags('-DNMEMDEBUG')
if not env['MEMOPS']:
    env.MergeFlags('-DNMEMOPS')
if not env['MEMSTATS']:
    env.MergeFlags('-DNMEMSTATS')
if not env['STACKTRACE']:
    env.MergeFlags('-DNSTACKTRACE')

layers = [
    'debug', 'memory', 'system', 'logging', 'profile',
    'containers', 'regexp', 'options', 'mpi', 'h5', 'numerics'
]

project.build(layers, proj_name='libhpc', env=env, vars=vars)
