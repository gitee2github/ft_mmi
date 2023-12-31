#!/usr/bin/env python3
# coding=utf-8
import sys
import os
import shutil
import platform
from builder.common.utils import exec_sys_command
from builder.common.logger import logger
def get_machine_info():
    arch = platform.machine()
    if arch == "x86_64":
        arch = "x64"
    os_name = platform.system().lower()
    return arch,os_name

def build_deps(output_dir):
    arch, os_name = get_machine_info()
    copy_path = "prebuilts/build-tools/" + os_name + "-" + arch + "/bin/"
    abs_copy_path = os.path.join(output_dir, "../../", copy_path)
    ninja_path = os.path.abspath(os.path.join(abs_copy_path, "ninja"))
    gn_path = os.path.abspath(os.path.join(abs_copy_path, "gn"))
    if os.path.exists(ninja_path) and os.path.exists(gn_path):
        return

    if os.path.exists(output_dir):
        shutil.rmtree(output_dir)
    if not os.path.exists(abs_copy_path):
        os.makedirs(abs_copy_path)

    usr_bin_path = '/usr/bin/'
    usr_ninja_path = os.path.join(usr_bin_path, 'ninja')
    if os.path.exists(usr_ninja_path):
        # copy ninja
        exec_sys_command(['cp', usr_ninja_path, ninja_path])
        logger.info("get ninja success")
    else:
        # build ninja
        ninja_dir = os.path.join(output_dir, "ninja-build")
        os.makedirs(ninja_dir)
        cmd = ['git', 'clone', 'https://gitee.com/src-openeuler/ninja-build.git', '-b', 'openEuler-22.03-LTS', ninja_dir]
        is_success, _output = exec_sys_command(cmd)
        if is_success:
            cur_work_dir = os.getcwd()
            exec_sys_command(['tar', 'xf', os.path.join(ninja_dir, 'ninja-1.10.2.tar.gz'), '-C', ninja_dir])
            os.chdir(os.path.join(ninja_dir, 'ninja-1.10.2'))
            exec_sys_command(['python3', 'configure.py', '--bootstrap'])
            if os.path.exists("ninja"):
                exec_sys_command(['cp', 'ninja', ninja_path])
                logger.info("build ninja success")
                exec_sys_command(['sudo', 'cp', 'ninja', usr_bin_path])
                logger.info("copy ninja success")
            os.chdir(cur_work_dir)

    usr_gn_path = os.path.join(usr_bin_path, 'gn')
    if os.path.exists(usr_gn_path):
        # copy gn
        exec_sys_command(['cp', usr_gn_path, gn_path])
        logger.info("get gn success")
    else:
        # build gn
        gn_dir = os.path.join(output_dir, "gn-build")
        os.makedirs(gn_dir)
        cmd = ['git', 'clone', 'https://gitee.com/src-openeuler/gn.git', '-b', 'openEuler-22.03-LTS-SP1', gn_dir]
        is_success, _output = exec_sys_command(cmd)
        if is_success:
            cur_work_dir = os.getcwd()
            exec_sys_command(['tar', 'xf', os.path.join(gn_dir, 'gn-e1ac69b17da0c6d4f5e34e686690ff70c6a43e6f.tar.gz'), '-C', gn_dir])
            os.chdir(gn_dir)
            #exec_sys_command(['patch', '-p1<gn-always-python3.patch'])
            os.makedirs(os.path.join(gn_dir, 'out'))
            exec_sys_command(['cp', 'last_commit_position.h', 'out/'])
            exec_sys_command(['python3', 'build/gen.py', '--no-last-commit-position', '--no-static-libstdc++'])
            exec_sys_command([os.path.join(ninja_dir, "ninja-1.10.2", "ninja"), '-C', 'out'])
            os.chdir(os.path.join(gn_dir, "out"))
            if os.path.exists("gn"):
                exec_sys_command(['cp', 'gn', gn_path])
                logger.info("build gn success")
                exec_sys_command(['sudo', 'cp', 'gn', usr_bin_path])
                logger.info("copy gn success")
            os.chdir(cur_work_dir)

if __name__ == "__main__":
    sys.exit(main())
