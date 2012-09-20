import sconsproject as project
from sconsproject import config

config.select(
    config.packages.boost(),
    config.packages.MPI(),
    config.packages.HDF5(),
    config.packages.rapidxml(),
    config.packages.Eigen(required=False),
)

vars = project.create_variables()
vars.AddVariables(
    BoolVariable('MEMOPS', 'Log memory operations.', False),
    BoolVariable('MEMSTATS', 'Enable memory statistics.', False),
    BoolVariable('LOG', 'Enable logging.', False),
)

env = project.create_environment(vars)
env.MergeFlags('-std=c++0x')
if not env['MEMOPS']:
    env.MergeFlags('-DNMEMOPS')
if not env['MEMSTATS']:
    env.MergeFlags('-DNMEMSTATS')
if not env['LOG']:
    env.MergeFlags('-DNLOG')

layers = ['debug', 'memory', 'system', 'logging', 'containers', 'regexp',
          'options', 'mpi', 'h5', 'numerics']

project.build(layers, proj_name='libhpc', env=env, vars=vars)
