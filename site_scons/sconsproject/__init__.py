import subprocess, os, multiprocessing
import config
import helpers


def source_environment(filename):
    stdout = subprocess.check_output(['bash', '-c', 'source ' + filename + ' && env'])
    for line in stdout.splitlines():
        key, _, value = line.partition('=')
        os.environ[key] = value


def create_variables():
    import sconsconfig as config
    from SCons.Script import Variables, EnumVariable, BoolVariable, PathVariable
    vars = Variables('config.py') # Persistent storage.
    vars.AddVariables(
        ('CC', 'Set C compiler.'),
        ('CXX', 'Set CXX compiler.'),
        EnumVariable('BUILD', 'Set the build type.', 'debug', allowed_values=('debug', 'dbgopt', 'optimised', 'testopt')),
        EnumVariable('BITS', 'Set number of bits.', 'default', allowed_values=('32', '64', 'default')),
        BoolVariable('PROF', 'Enable profiling.', False),
        BoolVariable('LOG', 'Enable logging.', True),
        # BoolVariable('LOG_TRIVIAL', 'Enable trivial logging.', True),
        # BoolVariable('LOG_DEBUG', 'Enable debug logging.', True),
        BoolVariable('WITH_OPENMP', 'Enable threading with OpenMP.', False),
        BoolVariable('WITH_TAU', 'Enable tau profiling.', False),
        BoolVariable('WITH_GCOV', 'Enable coverage testing with gcov.', False),
        PathVariable('PREFIX', 'Set install location.', '/usr/local', PathVariable.PathIsDirCreate),
        BoolVariable('BUILD_STATIC_LIBS', 'Build static libraries.', True),
        BoolVariable('BUILD_SHARED_LIBS', 'Build shared libraries.', True),
        BoolVariable('BUILD_TESTS', 'Build unit tests.', True),
        BoolVariable('BUILD_EXS', 'Build unit tests.', True),
        BoolVariable('BUILD_APPS', 'Build applications.', True),
        BoolVariable('BUILD_DOC', 'Build documentation.', False),
        BoolVariable('INSTALL_SUB', 'Install subproject libraries.', False),
        BoolVariable('DOWNLOAD_ALL', 'Download and install dependencies automatically.', False),
    )

    # Add options from any packages we want to use.
    config.add_options(vars)

    return vars

def create_environment(vars=None):
    if vars is None:
        vars = create_variables()

    import os
    from SCons.Script import Environment, Help
    tools = ['default', 'cxxtest']
    env = Environment(tools=tools, toolpath=['sconsconfig/tools'], variables=vars, ENV=os.environ, CXXTEST='cxxtest/scripts/cxxtestgen.py')

    # Check if there were any unkown variables on the command line.
    unknown = vars.UnknownVariables()
    if unknown:
        print 'Unknown variables:', unknown.keys()
        env.Exit(1)

    # Generate a help line later use.
    Help(vars.GenerateHelpText(env))

    return env

def configure_environment(env, vars=None):
    if vars is None:
        vars = create_variables()

    # Configure for build type.
    if env['BUILD'] == 'debug':
        env.MergeFlags('-g -O0')
    elif env['BUILD'] == 'testopt':
        env.MergeFlags('-g -O2 -DNDEBUG')
    elif env['BUILD'] == 'dbgopt':
        env.MergeFlags('-g -O2')
    elif env['BUILD'] == 'optimised':
        env.MergeFlags('-DNDEBUG -O2')
        env.MergeFlags('-DNLOGTRIVIAL -DNLOGDEBUG')

    # Configure the bit architecture.
    if env['BITS'] == '64':
        env.MergeFlags('-m64')
        env.AppendUnique(LINKFLAGS=['-m64'])
    elif env['BITS'] == '32':
        env.MergeFlags('-m32')
        env.AppendUnique(LINKFLAGS=['-m32'])

    if env['PROF']:
        env.MergeFlags('-g -pg')
        env.AppendUnique(LINKFLAGS=['-pg'])

    if env['WITH_GCOV']:
        env.AppendUnique(CFLAGS=['-fprofile-arcs', '-ftest-coverage'])
        env.AppendUnique(CCFLAGS=['-fprofile-arcs', '-ftest-coverage'])
        env.AppendUnique(LINKFLAGS=['-fprofile-arcs', '-ftest-coverage'])

    if env['WITH_TAU']:
        env['CC'] = 'tau_cc.sh'
        env['CXX'] = 'tau_cxx.sh'
        env.AppendUnique(CPPDEFINES=['WITH_TAU'])
        env.AppendUnique(CPPDEFINES=['NDEBUG'])

    # Configure for OpenMP.
    if env['WITH_OPENMP']:
        env.MergeFlags('-fopenmp')

    # Configure logging.
    if not env['LOG']:
        env.MergeFlags('-DNLOG')
    # if not env['LOG_TRIVIAL']:
    #     env.MergeFlags('-DNLOGTRIVIAL')
    # if not env['LOG_DEBUG']:
    #     env.MergeFlags('-DNLOGDEBUG')

    # Run the configuration and save it to file.
    config.configure(env, vars)

    # Make sure we can find CxxTest.
    env.PrependUnique(CPPPATH=['#cxxtest/include'])

    # Make sure our source code can locate installed headers and
    # libraries.
    env['BUILD'] = 'build-' + env['BUILD']
    env.PrependUnique(CPPPATH=[
        '#' + env['BUILD'] + '/include',
    ])
    env.PrependUnique(LIBPATH=['#' + env['BUILD'] + '/lib'])

def build(subdirs, proj_name='', env=None, vars=None, **kwargs):
    if vars is None:
        vars = create_variables()
    if env is None:
        env = create_environment(vars)

    # Check for different directories.
    env['TESTS_DIR'] = kwargs.get('tests_dir', 'tests')

    # If the user requested help don't bother continuing with the build.
    from SCons.Script import GetOption
    if not GetOption('help'):
        configure_environment(env, vars)

        # Add the specific project name to the CPPPATH.
        if proj_name:
            env['PROJECT_NAME'] = proj_name
            env.PrependUnique(CPPPATH=['#' + env['BUILD'] + '/include/' + proj_name])

        # These will be returned from subscripts.
        sources = []
        static_libs = []
        shared_libs = []

        # Call sub scripts.
        env.Export('env')
        for sd in subdirs:
            objs = env.SConscript(sd + '/SConscript', variant_dir=env['BUILD'] + '/' + sd, duplicate=0)
            sources.extend(objs[0])
            static_libs.extend(objs[1])
            shared_libs.extend(objs[2])

        # Process the project level directory.
        env['SUBPROJ'] = ''
        if os.path.exists(os.path.join(env.Dir('.').srcnode().abspath, 'src')):
            obj_map = env.SConscript('src/SConscript', variant_dir=env['BUILD'] + '/src', duplicate=0)
        else:
            obj_map = {}
        if env['BUILD_STATIC_LIBS'] and proj_name:
            lib = env.Library('#' + env['BUILD'] + '/lib/' + env['PROJECT_NAME'], obj_map.values() + sources)
            if env['PREFIX']:
                env.Install(env['PREFIX'] + '/lib', lib)
        if env['BUILD_SHARED_LIBS'] and env['PREFIX'] and proj_name:
            env.SharedLibrary(env['PREFIX'] + '/lib/' + env['PROJECT_NAME'], obj_map.values() + sources)
        if env['BUILD_TESTS'] and os.path.exists(os.path.join(env.Dir('.').srcnode().abspath, env['TESTS_DIR'])):
            env.SConscript('tests/SConscript', variant_dir=env['BUILD'] + '/' + env['TESTS_DIR'], duplicate=0, exports=['obj_map'])
        if env['BUILD_APPS'] and os.path.exists(os.path.join(env.Dir('.').srcnode().abspath, 'apps')):
            env.SConscript('apps/SConscript', variant_dir=env['BUILD'] + '/apps', duplicate=0, exports=['obj_map'])
        if env['BUILD_EXS'] and os.path.exists(os.path.join(env.Dir('.').srcnode().abspath, 'exs')):
            env.SConscript('exs/SConscript', variant_dir=env['BUILD'] + '/exs', duplicate=0, exports=['obj_map'])
        if env['BUILD_DOC'] and os.path.exists(os.path.join(env.Dir('.').srcnode().abspath, 'doc')):
            env.SConscript('doc/SConscript', variant_dir=env['BUILD'] + '/doc', duplicate=0)

        # Alias any special targets.
        env.Alias('install', env['PREFIX'])
