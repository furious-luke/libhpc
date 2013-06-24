Import('env', 'obj_map')
Export('env', 'obj_map')

for suite in suites:
    if isinstance(suite, str):
        objs = []
        ranks = []
    else:
        if len(suite) and isinstance(suite[-1], (list, tuple)):
            ranks = suite[-1]
            suite = suite[:-1]
        else:
            ranks = []
        objs = suite[1:] if (len(suite) > 1) else []
        objs = [obj_map[o] for o in objs]
        suite = suite[0]
    env.CxxTest([suite] + objs, ranks=ranks, LIBS=libs)
