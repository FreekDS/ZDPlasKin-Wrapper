import sys

try:
    from skbuild import setup
except ImportError:
    print(
        "Please update pip, you need pip 10 or greater,\n"
        " or you need to install the PEP 518 requirements in pyproject.toml yourself",
        file=sys.stderr,
    )
    raise

from setuptools import find_packages

setup(
    name="ZDPlaskin",
    version="0.0.1",
    description="a minimal example package (with pybind11)",
    author="Freek De Sagher",
    license="MIT",
    packages=find_packages(where="./python"),
    package_dir={"": "./python"},
    # cmake_install_dir="Lib/site-packages",
    include_package_data=True,
    python_requires=">=3.6",
)
