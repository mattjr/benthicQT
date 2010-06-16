
# NOTE:- This is an edited copy of the CMakeLists file that comes with the UF library
# This has been modifed so that the minimum cmake required version is 2.6.2, not 2.6.4 
# as previously. This needs to be copied over the UF-3.2/CMakeLists.txt file before attempting
# to build.

project(UF)

#-----------------------------------------------------------------------------
# We will enforce an out of source build.
string(COMPARE EQUAL "${UF_SOURCE_DIR}"
  "${UF_BINARY_DIR}" INSOURCE)
if(INSOURCE)
  message(FATAL_ERROR "UF requires an out of source build. Please create a separate binary directory and run CMake there.")
endif(INSOURCE)

#-----------------------------------------------------------------------------
# Don't build anything unless the version of CMake is high enough.
cmake_minimum_required(VERSION 2.6.2)

#-----------------------------------------------------------------------------
# Let's use the highest warning level.
if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake|VCExpress)")
  # Use the highest warning level for visual studio.
  set(CMAKE_CXX_WARNING_LEVEL 4)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
  
  # C++ exception handler, enable the unwind semantics.
  add_definitions(/EHsc)
  
endif(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake|VCExpress)")

#-----------------------------------------------------------------------------
# Disable deprecation warnings for standard C and STL functions in VS2005
# and later
if(CMAKE_COMPILER_2005)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE)
  add_definitions(-D_SCL_SECURE_NO_DEPRECATE)
endif(CMAKE_COMPILER_2005)

if(CMAKE_BUILD_TOOL MATCHES "make")
  if(NOT CMAKE_CXX_FLAGS MATCHES "-Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  endif(NOT CMAKE_CXX_FLAGS MATCHES "-Wall")
  if(NOT CMAKE_C_FLAGS MATCHES "-Wall")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
  endif(NOT CMAKE_C_FLAGS MATCHES "-Wall")
endif(CMAKE_BUILD_TOOL MATCHES "make")

#-----------------------------------------------------------------------------
# Load some macros.
include(${CMAKE_ROOT}/Modules/CMakeExportBuildSettings.cmake)

#-----------------------------------------------------------------------------
# Set target base name and lib name.
set( TARGET_BASE_NAME "${PROJECT_NAME}" )
set( LIB_NAME "${TARGET_BASE_NAME}Lib" )
# Set the prefix for the library.
# The library names will be prefixed with this.
set( LIB_PREFIX uf)

#-----------------------------------------------------------------------------
# Directory where the CMakeFiles are
set ( CMAKE_FILE_DIR
  ${PROJECT_SOURCE_DIR}/CMakeFiles
)

#-----------------------------------------------------------------------------
# Append the library version information to the library target
# properties.  A parent project may set its own properties and/or may
# block this.
#
# Rules for library versions (see: http://www.gnu.org/software/libtool/manual/):
#
#  Think of a library as exporting several sets of interfaces, arbitrarily represented 
#  by integers. When a program is linked against a library, it may use any subset of those interfaces.
#  Libtool's description of the interfaces that a program uses is simple: 
#  it encodes the least and the greatest interface numbers in the resulting binary (first-interface, last-interface).
#  The dynamic linker is guaranteed that if a library supports every interface number 
#  between first-interface and last-interface, then the program can be relinked against that library.
#
#  So, libtool library versions are described by three integers:
#   current
#     The most recent interface number that this library implements.
#   revision
#    The implementation number of the current interface.
#   age
#    The difference between the newest and oldest interfaces that this library implements. 
#    In other words, the library implements all the interface numbers in the range from number current - age to current. 
#
#  If two libraries have identical current and age numbers, then the dynamic linker chooses the library with the greater revision number.  
#
#   Libtool versions have the form: current[:revision[:age]]
#   If either revision or age are omitted, they default to 0. 
#   Also note that age must be less than or equal to the current interface number. 
#   Rules are:
#     1. Start with version information of ‘0:0:0’ for each libtool library.
#     2. Update the version information only immediately before a public release of your software. More frequent updates are unnecessary, and only guarantee that the current interface number gets larger faster.
#     3. If the library source code has changed at all since the last update, then increment revision (‘c:r:a’ becomes ‘c:r+1:a’).
#     4. If any interfaces have been added, removed, or changed since the last update, increment current, and set revision to 0.
#     5. If any interfaces have been added since the last public release, then increment age.
#     6. If any interfaces have been removed since the last public release, then set age to 0.
# 
# Never try to set the interface numbers so that they correspond to the release number of your package. If you do this, be warned that every release of your package will not be binary compatible with any other release. 
#
set(UF_LIB_CURRENT 3)
set(UF_LIB_REVISION 2)
set(UF_LIB_AGE 0)
if(NOT UF_NO_LIBRARY_VERSION)
  set(UF_LIBRARY_PROPERTIES ${UF_LIBRARY_PROPERTIES}
     VERSION "${UF_LIB_CURRENT}.${UF_LIB_REVISION}.${UF_LIB_AGE}"
     SOVERSION "${UF_LIB_CURRENT}"
#    VERSION "${UF_VERSION}"
#    SOVERSION "${UF_MAJOR_VERSION}.${UF_MINOR_VERSION}"
    )
endif(NOT UF_NO_LIBRARY_VERSION)

#-----------------------------------------------------------------------------
# Utility Functions version number
# From now on we will link this to the library version.
set(UF_MAJOR_VERSION ${UF_LIB_CURRENT})
set(UF_MINOR_VERSION ${UF_LIB_REVISION})
set(UF_BUILD_VERSION ${UF_LIB_AGE})
set(UF_VERSION
    "${UF_MAJOR_VERSION}.${UF_MINOR_VERSION}.${UF_BUILD_VERSION}")


#-----------------------------------------------------------------------------
# The location in which to install UF executables.
if(NOT UF_INSTALL_BIN_DIR)
  set(UF_INSTALL_BIN_DIR bin)
endif(NOT UF_INSTALL_BIN_DIR)

#-----------------------------------------------------------------------------
# Generally libraries and executables will be installed at:
# ${CMAKE_INSTALL_PREFIX}/bin or ${CMAKE_INSTALL_PREFIX}/lib
# by default.
# The location in which to install UF header files.
# Here we want a version number used.
if(NOT UF_INSTALL_INCLUDE_DIR)
  set(UF_INSTALL_INCLUDE_DIR
    include/${LIB_PREFIX}-${UF_MAJOR_VERSION}.${UF_MINOR_VERSION}
    )
endif(NOT UF_INSTALL_INCLUDE_DIR)

#-----------------------------------------------------------------------------
# The location in which to install UF libraries.
if(NOT UF_INSTALL_LIB_DIR)
  set(UF_INSTALL_LIB_DIR lib)
endif(NOT UF_INSTALL_LIB_DIR)

#-----------------------------------------------------------------------------
# The location in which to install CMake scripts for packaging UF.
if(NOT UF_INSTALL_PACKAGE_DIR)
  set(UF_INSTALL_PACKAGE_DIR
    ${UF_INSTALL_LIB_DIR}/${LIB_PREFIX}-${UF_MAJOR_VERSION}.${UF_MINOR_VERSION}
    )
endif(NOT UF_INSTALL_PACKAGE_DIR)

set(UF_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

# Any global options
include(${CMAKE_FILE_DIR}/CMakeOptions.cmake)

#-----------------------------------------------------------------------------
# Copy the CMake option to a setting with UF_ prefix for use in
# our project.  This name is used in UFConfigure.h.in.
set(UF_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

#-----------------------------------------------------------------------------
# Path to where the executable is.

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH
   "Single output directory for building all executables.")

#-----------------------------------------------------------------------------
# If we are building DLLs in windows, we need to keep the dlls with the executables.
# This allows testing to work Ok.
if (WIN32 AND UF_BUILD_SHARED_LIBS)
  set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE INTERNAL
     "Single output directory for building all libraries.")
else (WIN32 AND UF_BUILD_SHARED_LIBS)
  set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib CACHE INTERNAL
     "Single output directory for building all libraries.")
endif (WIN32 AND UF_BUILD_SHARED_LIBS)

#-----------------------------------------------------------------------------
# Extra link libraries.
if(UNIX AND NOT QNXNTO)
#  LINK_LIBRARIES(rt)
endif(UNIX AND NOT QNXNTO)
if(QNXNTO)
  LINK_LIBRARIES(m)
endif(QNXNTO)

#-----------------------------------------------------------------------------
# Boost
#if ( UF_USE_BOOST )
 # # The automatic linking, uses the static version of the libraries.
 # if(WIN32)
 #   set(Boost_USE_STATIC_LIBS ON)
 # endif(WIN32)

  # Uncomment and edit if CMake cannot find boost.
  #set(Boost_ADDITIONAL_VERSIONS "1.41" "1.41.0")
 # find_package(Boost REQUIRED COMPONENTS system filesystem )
 # if ( Boost_FOUND )
 #   include_directories(${Boost_INCLUDE_DIRS})
 #   link_directories(${Boost_LIBRARY_DIRS})
 #   add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
 #   # Remember to include ${Boost_LIBRARIES} in the target_link_libraries() statement
 # endif ( Boost_FOUND )
#endif ( UF_USE_BOOST )

#-----------------------------------------------------------------------------
# At present Apple/Darwin is not linking properly as shared libraries using boost.
if (APPLE AND UF_BUILD_SHARED_LIBS AND UF_USE_BOOST)
  message(FATAL_ERROR "Apple/Darwin is not linking properly as shared libraries using Boost. Please turn OFF BUILD_SHARED_LIBS")
endif (APPLE AND UF_BUILD_SHARED_LIBS AND UF_USE_BOOST)

#-----------------------------------------------------------------------------
# VTK
# FindVTK.cmake is in the modules directory of CMake.
#set( VTK_DIR VTK_DIR-NOTFOUND )
#if ( UF_USE_VTK )
#  find_package(VTK REQUIRED)
#  include(${VTK_USE_FILE})
#endif ( UF_USE_VTK )

#-----------------------------------------------------------------------------
# Any special configure files.
configure_file(
  ${CMAKE_FILE_DIR}/ufConfigure.h.in
  ${PROJECT_BINARY_DIR}/ufConfigure.h
)

configure_file(
  ${CMAKE_FILE_DIR}/UseUF.cmake.in
  ${UF_BINARY_DIR}/UseUF.cmake COPYONLY IMMEDIATE
)

#-----------------------------------------------------------------------------
# Create the list of include directories needed for Utility Function header files.
include(${CMAKE_FILE_DIR}/ufIncludeDirectories.cmake)

#-----------------------------------------------------------------------------
# You can put your include path(s) here
include_directories (
  ${UF_INCLUDE_DIRS_BUILD_TREE}
  ${UF_INCLUDE_DIRS_SOURCE_TREE}
  ${UF_INCLUDE_DIRS_SYSTEM}
  ${PROJECT_SOURCE_DIR}
  ${EXECUTABLE_OUTPUT_PATH}
)

#-----------------------------------------------------------------------------
link_directories(${LIBRARY_OUTPUT_PATH})

#-----------------------------------------------------------------------------
# Target settings for building the library.
set (UF_LIBRARY_PROPERTIES  ${UF_LIBRARY_PROPERTIES} DEBUG_POSTFIX "-d")

#-----------------------------------------------------------------------------
# Build the components of the library.
add_subdirectory(Angle)
add_subdirectory(Colour)
add_subdirectory(Conversions)
add_subdirectory(CSVParser)
add_subdirectory(DateTime)
add_subdirectory(GeographicConversions)
add_subdirectory(Navigation)
add_subdirectory(GPSParser)
add_subdirectory(Logger)
add_subdirectory(Macros)
add_subdirectory(Statistics)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(Angle/Test)
  add_subdirectory(Colour/Test)
  add_subdirectory(Conversions/Test)
  add_subdirectory(CSVParser/Test)
  add_subdirectory(DateTime/Test)
  add_subdirectory(GeographicConversions/Test)
  add_subdirectory(Navigation/Test)
  add_subdirectory(GPSParser/Test)
  add_subdirectory(Logger/Test)
  add_subdirectory(Statistics/Test)
endif(BUILD_TESTING)

#if(UF_USE_VTK)
#  add_subdirectory(DTM)
#  add_subdirectory(VTK)
 # if(BUILD_TESTING)
 #   add_subdirectory(DTM/Test)
 #   add_subdirectory(VTK/Test)
 # endif(BUILD_TESTING)
#endif(UF_USE_VTK)

#if(UF_USE_BOOST)
 # add_subdirectory(FileUtilities)
 # if(BUILD_TESTING)
 #   add_subdirectory(FileUtilities/Test)
 # endif(BUILD_TESTING)
#endif(UF_USE_BOOST)

if(BUILD_EXAMPLES)
  add_subdirectory(Examples)
endif(BUILD_EXAMPLES)

#-----------------------------------------------------------------------------
# Save library dependencies.
export_library_dependencies(${UF_BINARY_DIR}/UFLibraryDepends.cmake)

#-----------------------------------------------------------------------------
# Save the compiler settings so another project can import them.
#-----------------------------------------------------------------------------
#cmake_export_build_settings(${UF_BINARY_DIR}/ufBuildSettings.cmake)

#-----------------------------------------------------------------------------
# Install some files.
#-----------------------------------------------------------------------------
install(
  FILES
    "${PROJECT_BINARY_DIR}/ufConfigure.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/ufWin32Header.h"
  DESTINATION ${UF_INSTALL_INCLUDE_DIR}
)
# Install these also.
install(
  FILES
    "${UF_BINARY_DIR}/UseUF.cmake"
#    "${UF_BINARY_DIR}/UFLibraryDepends.cmake"
#    "${UF_BINARY_DIR}/ufBuildSettings.cmake"
  DESTINATION ${UF_INSTALL_PACKAGE_DIR}
)

#-----------------------------------------------------------------------------
# Create an uninstall target.
#-----------------------------------------------------------------------------
configure_file(
  "${CMAKE_FILE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

#-----------------------------------------------------------------------------
# Build the CPack Installer.
#-----------------------------------------------------------------------------
include(${CMAKE_FILE_DIR}/ufCPack.cmake)

#-----------------------------------------------------------------------------
include(CTest)

#-----------------------------------------------------------------------------
# The commands in this directory are intended to be executed as
# the end of the whole configuration process, as a "last step".
# This directory is typically the last SUBDIRS in the main CMakeLists.txt.
add_subdirectory(LastConfigureStep)

