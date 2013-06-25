import os

# Bring in the environment into our locals().
Import('env')

# Send it out to sub-calls.
Export('env')

# Call the 'src' SConscript. It will return a dictionary containing
# a mapping from the object path basename to the object.
if os.path.exists(os.path.join(Dir('.').srcnode().abspath, 'src')):
    obj_map = env.SConscript('src/SConscript', duplicate=0)
else:
    obj_map = {}

# Build static libraries and install them, if required.
static_libs = []
if env['BUILD_STATIC_LIBS']:
    lib = env.Library('#' + env['BUILD'] + '/lib/' + env['SUBPROJ'], obj_map.values())
    if env['PREFIX'] and env.get('INSTALL_SUB'):
        env.Install(env['PREFIX'] + '/lib', lib)
    static_libs += lib

# Build shared libraries and install them, if required.
shared_libs = []
if env['BUILD_SHARED_LIBS'] and env['PREFIX'] and env.get('INSTALL_SUB'):
    lib = env.SharedLibrary(env['PREFIX'] + '/lib/' + env['SUBPROJ'], obj_map.values())
    shared_libs += lib

# Call the 'tests' SConscript, if required.
if env['BUILD_TESTS'] and os.path.exists(os.path.join(Dir('.').srcnode().abspath, env['TESTS_DIR'])):
    env.SConscript(env['TESTS_DIR'] + '/SConscript', duplicate=0, exports=['obj_map'])

# Build any applications.
if env['BUILD_APPS'] and os.path.exists(os.path.join(Dir('.').srcnode().abspath, 'apps')):
    env.SConscript('apps/SConscript', duplicate=0, exports=['obj_map'])

# Call the 'exs' SConscript, if required. Examples will typically
# be untested demonstrations of code usage.
if env['BUILD_EXS'] and os.path.exists(os.path.join(Dir('.').srcnode().abspath, 'exs')):
    env.SConscript('exs/SConscript', duplicate=0, exports=['obj_map'])

# Build documentation.
if env['BUILD_DOC'] and os.path.exists(os.path.join(Dir('.').srcnode().abspath, 'doc')):
    env.SConscript('doc/SConscript', duplicate=0)

# Return the list of libraries.
objs = (obj_map.values(), static_libs, shared_libs)
Return('objs')
