import os
import utils

def get_build_sub_project_script():
    return os.path.join(utils.get_data_prefix(), 'helpers', 'build_sub_project.py')

def get_build_src_script():
    return os.path.join(utils.get_data_prefix(), 'helpers', 'build_src.py')

def get_build_tests_script():
    return os.path.join(utils.get_data_prefix(), 'helpers', 'build_tests.py')

def get_build_apps_script():
    return os.path.join(utils.get_data_prefix(), 'helpers', 'build_apps.py')

def get_build_exs_script():
    return os.path.join(utils.get_data_prefix(), 'helpers', 'build_apps.py')
