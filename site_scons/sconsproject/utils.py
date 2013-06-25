import os, subprocess, shlex, shutil, sys

def get_prefix():
    import sconsproject as proj
    return os.path.dirname(proj.__file__)

def get_data_prefix():
    return os.path.join(get_prefix(), 'data')

def mkdir(dir):
    path = os.path.join(*dir)
    if not os.path.exists(path):
        os.makedirs(path)

def copy_data(source, destination, force=False):
    if force or not os.path.exists(destination):
        shutil.copytree(os.path.join(get_data_prefix(), source), destination)

def subs(source, replace):
    with open(source, 'r') as src_file:
        txt = src_file.read()
    with open(source, 'w') as src_file:
        src_file.write(txt%replace)

def process_command_line(cmd):
    if not isinstance(cmd, list):
        cmd = shlex.split(cmd)
    return cmd

def check_output(cmd):
    cmd = process_command_line(cmd)
    return subprocess.check_output(cmd)

def check_call(cmd):
    cmd = process_command_line(cmd)
    return subprocess.check_call(cmd)

def message(msg):
    sys.stdout.write(msg)
    sys.stdout.flush()
