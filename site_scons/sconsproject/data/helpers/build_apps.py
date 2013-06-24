Import('env')

# Either create or update the current subdirectory.
if 'cur_sub_dir' not in locals():
    cur_sub_dir = ''

# Descend to sub-directories first.
for sd in sub_dirs:
    bak = cur_sub_dir
    cur_sub_dir += sd
    SConscript(sd + '/SConscript', duplicate=0, exports='cur_sub_dir')
    cur_sub_dir = bak

# Build the applications.
for srcs in sources:
    app_name = srcs[0]
    if len(srcs) > 1:
        app_srcs = srcs[1:]
    else:
        app_srcs = []
    if isinstance(app_srcs[-1], dict):
        opts = app_srcs[-1]
        app_srcs = app_srcs[:-1]
    else:
        opts = {}
    if 'LIBS' in opts:
        opts['LIBS'].extend(libs)
    else:
        opts['LIBS'] = libs
    app = env.Program(dst_dir + app_name, app_srcs, **opts)
    if env['PREFIX']:
        env.Install(env['PREFIX'] + '/bin', app)
