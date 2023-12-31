#!/usr/bin/env python3
# Copyright (c) 2023 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os

from builder.common.logger import logger
from builder.common.utils import exec_sys_command
from builder.common.env_checker import checker
from builder.common.prebuild import get_machine_info

class Builder:
    def __init__(self, args):
        self.args = args

        self.project_dir = args.project_dir
        os_name = "linux"
        arch = "x64"
        if args.target_cpu == "auto":
            arch, os_name = get_machine_info()
        self.build_output_dir = os.path.join(args.project_dir, "out", args.build_type.title(), arch)
        self._build_tools_dir = os.path.join(args.project_dir, "prebuilts", "build-tools", os_name+"-"+arch, "bin")
        self.gn_path = os.path.join(self._build_tools_dir, "gn")
        self.ninja_path = os.path.join(self._build_tools_dir, "ninja")

        self._args_list = []

        if args.escape_build is True:
            logger.warning(f"Escaping build procedure.")
            self.should_do_build = False
            self.should_do_postbuild = False
        else:
            self.should_do_build = True
            self.should_do_postbuild = True

    def pre_build(self) -> bool:
        return checker.check_system_env(self.project_dir)

    def post_build(self) -> bool:
        if self.args.export_compile_commands is True:
            # return exec_sys_command(['ln', '-sf', os.path.join(self.build_output_dir, 'compile_commands.json'), self.project_dir])[0]
            exec_sys_command(['rm', '-f', os.path.join(self.project_dir, 'compile_commands.json')])
            return exec_sys_command(['cp', os.path.join(self.build_output_dir, 'compile_commands.json'), self.project_dir])[0]

        if self.args.install is True:
            librarys = os.listdir(os.path.join(self.build_output_dir, 'common/common/'))
            for lib in librarys:
                if '.so' in lib:
                    exec_sys_command(['cp', '-rf', os.path.join(self.build_output_dir, 'common/common/', lib), "/usr/lib64/"])[0]

        return True

    def launch_gn(self) -> bool:
        """This function executes the gn command, which generate ninja files"""
        # Generate GN build cmd
        gn_subargs = [
            f'project_root_dir="{self.project_dir}"',
        ]

        if self.args.build_type.lower() == "debug":
            gn_subargs.append(f'is_debug=true')
        else:
            gn_subargs.append(f'is_debug=false')

        if self.args.enable_musl is True:
            gn_subargs.append(f'use_musl=true')
        else:
            gn_subargs.append(f'use_musl=false')

        gn_args = [
            self.gn_path,
            'gen',
            self.build_output_dir,
            '--args={}'.format(" ".join(gn_subargs))
        ]

        if self.args.export_compile_commands is True:
            gn_args.append('--export-compile-commands')

        if self.args.verbose is True:
            gn_args.append('-v')

        # Execute GN build cmd
        logger.info(f"Launch \"gn\" with: `{' '.join(gn_args)}`")
        return exec_sys_command(gn_args)[0]

    def launch_ninja(self) -> bool:
        """This function executes the ninja command, which compiles the project"""
        ninja_args = [
            self.ninja_path,
            '-C',
            self.build_output_dir,
            f'-j {self.args.jobs}',
        ]

        if self.args.verbose is True:
            ninja_args.append('-v')

        logger.info(f"Launch \"ninja\" with: `{' '.join(ninja_args)}`")
        return exec_sys_command(ninja_args)[0]

    def get_cmds(self) -> list:
        command_chain = [ self.pre_build ]

        if self.should_do_build:
            command_chain.append(self.launch_gn)
            command_chain.append(self.launch_ninja)
        if self.should_do_postbuild:
            command_chain.append(self.post_build)

        return command_chain

    def exec_command(self) -> bool:
        logger.info(f'Exec "build" command...')

        cmds = self.get_cmds()

        for cmd in cmds:
            if cmd() is False:
                return False

        return True
