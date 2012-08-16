import sconsproject as project
from sconsproject import config

config.select(
    config.packages.boost(required=True),
    config.packages.MPI(required=True),
    config.packages.HDF5(required=True),
    config.packages.rapidxml(required=True),
    config.packages.libusb(required=True),
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

project.build(
    ['debug', 'memory', 'system', 'logging', 'containers', 'options', 'mpi', 'h5'],
    proj_name='libhpc',
    env=env,
    vars=vars
)
