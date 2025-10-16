# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/i.dymov/Desktop/WORK/wch_demo_vscode-master/wch_demo_vscode-master/_deps/ch32-libs-src")
  file(MAKE_DIRECTORY "C:/Users/i.dymov/Desktop/WORK/wch_demo_vscode-master/wch_demo_vscode-master/_deps/ch32-libs-src")
endif()
file(MAKE_DIRECTORY
  "C:/Users/i.dymov/Desktop/WORK/wch_demo_vscode-master/wch_demo_vscode-master/_deps/ch32-libs-build"
  "C:/Users/i.dymov/Desktop/WORK/wch_demo_vscode-master/wch_demo_vscode-master/_deps/ch32-libs-subbuild/ch32-libs-populate-prefix"
  "C:/Users/i.dymov/Desktop/WORK/wch_demo_vscode-master/wch_demo_vscode-master/_deps/ch32-libs-subbuild/ch32-libs-populate-prefix/tmp"
  "C:/Users/i.dymov/Desktop/WORK/wch_demo_vscode-master/wch_demo_vscode-master/_deps/ch32-libs-subbuild/ch32-libs-populate-prefix/src/ch32-libs-populate-stamp"
  "C:/Users/i.dymov/Desktop/WORK/wch_demo_vscode-master/wch_demo_vscode-master/_deps/ch32-libs-subbuild/ch32-libs-populate-prefix/src"
  "C:/Users/i.dymov/Desktop/WORK/wch_demo_vscode-master/wch_demo_vscode-master/_deps/ch32-libs-subbuild/ch32-libs-populate-prefix/src/ch32-libs-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/i.dymov/Desktop/WORK/wch_demo_vscode-master/wch_demo_vscode-master/_deps/ch32-libs-subbuild/ch32-libs-populate-prefix/src/ch32-libs-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/i.dymov/Desktop/WORK/wch_demo_vscode-master/wch_demo_vscode-master/_deps/ch32-libs-subbuild/ch32-libs-populate-prefix/src/ch32-libs-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
