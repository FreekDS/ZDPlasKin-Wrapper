import os
import os.path
import platform
import subprocess
import sys

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

# Command line flags forwarded to CMake (for debug purpose)
cmake_cmd_args = []
for f in sys.argv:
    if f.startswith('-D'):
        cmake_cmd_args.append(f)


class CMakeExtension(Extension):
    def __init__(self, name, cmake_lists_dir='.', *args, **kwargs):
        print("--------> nameke ", name, type(name))
        Extension.__init__(self, name, sources=[], inplace=False, *args, **kwargs)
        self.cmake_lists_dir = os.path.abspath(cmake_lists_dir)
        self._file_name = f"{name}.pyd"



class CMakeBuild(build_ext):
    def check_cmake(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
            return out.decode().split('\n', 1)[0]
        except OSError:
            raise RuntimeError("Cmake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))

    def build_extension(self, ext):
        print(self.check_cmake())

        for ext in self.extensions:
            ext_dir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
            cfg = 'Debug' if self.debug else 'Release'
            cmake_args = [
                f"-DCMAKE_BUILD_TYPE={cfg}",
                f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{cfg.upper()}={ext_dir}",
                f"-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_{cfg.upper()}={self.build_temp}",
                f"-DPYTHON_EXECUTABLE={sys.executable}"
            ]

            if platform.system() == 'Windows':
                plat = ('x64' if platform.architecture()[0] == '64bit' else 'Win32')
                # Likely options for windows
                cmake_args += [
                    f'-DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE',
                    f'-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_{cfg.upper()}={ext_dir}'
                ]
                cmake_args += [
                    '-G', 'MinGW Makefiles'
                ]
            cmake_args += cmake_cmd_args

            if not os.path.exists(self.build_temp):
                os.makedirs(self.build_temp)

            cmake_cmd = ['cmake', ext.cmake_lists_dir] + cmake_args
            print(cmake_cmd)

            out = subprocess.call(cmake_cmd, cwd=self.build_temp)
            out = subprocess.call(['cmake', '--build', '.', '--config', cfg], cwd=self.build_temp)

            print(out)

    def get_ext_filename(self, fullname):
        return f"{fullname}.pyd"




setup(
    name='test_module',
    version='a1.0',
    ext_modules=[CMakeExtension("test_module")],
    cmdclass={'build_ext': CMakeBuild},
    zip_safe=False
)
