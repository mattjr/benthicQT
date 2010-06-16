set (UF_BINARY_DIR ${PROJECT_BINARY_DIR})
#-----------------------------------------------------------------------------
# Settings specific to the build tree.

# The install-only section is empty for the build tree.
set(UF_CONFIG_INSTALL_ONLY)

# The "use" file.
set(UF_USE_FILE ${UF_BINARY_DIR}/UseUF.cmake)

# The build settings file.
set(UF_BUILD_SETTINGS_FILE ${UF_BINARY_DIR}/ufBuildSettings.cmake)

# Library dependencies file.
set(VTK_LIBRARY_DEPENDS_FILE "${VTK_BINARY_DIR}/UFLibraryDepends.cmake")

# Library directory.
set(UF_LIBRARY_DIRS_CONFIG ${LIBRARY_OUTPUT_PATH})

# Runtime library directory.
set(UF_RUNTIME_LIBRARY_DIRS_CONFIG ${LIBRARY_OUTPUT_PATH})

# Binary executable directory.
set(UF_EXECUTABLE_DIRS_CONFIG ${EXECUTABLE_OUTPUT_PATH})

# Determine the include directories needed.
set(UF_INCLUDE_DIRS_CONFIG
  ${UF_INCLUDE_DIRS_BUILD_TREE}
  ${UF_INCLUDE_DIRS_SOURCE_TREE}
  ${UF_INCLUDE_DIRS_SYSTEM}
)

#-----------------------------------------------------------------------------
# Configure UFConfig.cmake for the build tree.
configure_file(${CMAKE_FILE_DIR}/ufConfig.cmake.in
               ${UF_BINARY_DIR}/ufConfig.cmake @ONLY IMMEDIATE)

#-----------------------------------------------------------------------------
# Settings specific to the install tree.

# The "use" file.
set(UF_USE_FILE "\${UF_INSTALL_PREFIX}/${UF_INSTALL_PACKAGE_DIR}/UseUF.cmake")

# The build settings file.
set(UF_BUILD_SETTINGS_FILE "\${UF_INSTALL_PREFIX}/${UF_INSTALL_PACKAGE_DIR}/ufBuildSettings.cmake")

# Include directories.
set(UF_INCLUDE_DIRS_CONFIG
  \${UF_INSTALL_PREFIX}/${UF_INSTALL_INCLUDE_DIR}
  ${UF_INCLUDE_DIRS_SYSTEM}
# For the moment hard-wire them.
  \${UF_INSTALL_PREFIX}/${UF_INSTALL_INCLUDE_DIR}/Angle
  \${UF_INSTALL_PREFIX}/${UF_INSTALL_INCLUDE_DIR}/Colour
  \${UF_INSTALL_PREFIX}/${UF_INSTALL_INCLUDE_DIR}/Conversions
  \${UF_INSTALL_PREFIX}/${UF_INSTALL_INCLUDE_DIR}/CSVParser
  \${UF_INSTALL_PREFIX}/${UF_INSTALL_INCLUDE_DIR}/DateTime
  \${UF_INSTALL_PREFIX}/${UF_INSTALL_INCLUDE_DIR}/DTM
  \${UF_INSTALL_PREFIX}/${UF_INSTALL_INCLUDE_DIR}/FileUtilities
  \${UF_INSTALL_PREFIX}/${UF_INSTALL_INCLUDE_DIR}/GeographicConversions
#  \${UF_INSTALL_PREFIX}/${UF_INSTALL_INCLUDE_DIR}/Geometry
#  \${UF_INSTALL_PREFIX}/${UF_INSTALL_INCLUDE_DIR}/Geometry2D
  \${UF_INSTALL_PREFIX}/${UF_INSTALL_INCLUDE_DIR}/GPSNavigation
  \${UF_INSTALL_PREFIX}/${UF_INSTALL_INCLUDE_DIR}/Navigation
  \${UF_INSTALL_PREFIX}/${UF_INSTALL_INCLUDE_DIR}/GPSParser
  \${UF_INSTALL_PREFIX}/${UF_INSTALL_INCLUDE_DIR}/Logger
  \${UF_INSTALL_PREFIX}/${UF_INSTALL_INCLUDE_DIR}/Macros
  \${UF_INSTALL_PREFIX}/${UF_INSTALL_INCLUDE_DIR}/Statistics
  \${UF_INSTALL_PREFIX}/${UF_INSTALL_INCLUDE_DIR}/VTK
)

# Library dependencies file.
set(VTK_LIBRARY_DEPENDS_FILE "\${UF_INSTALL_PREFIX}/${UF_INSTALL_PACKAGE_DIR}/UFLibraryDepends.cmake")

# Link directories.
if(CYGWIN AND BUILD_SHARED_LIBS)
  # In Cygwin programs directly link to the .dll files.
  set(UF_LIBRARY_DIRS_CONFIG "\${UF_INSTALL_PREFIX}/${UF_INSTALL_BIN_DIR}")
else(CYGWIN AND BUILD_SHARED_LIBS)
  set(UF_LIBRARY_DIRS_CONFIG "\${UF_INSTALL_PREFIX}/${UF_INSTALL_LIB_DIR}")
endif(CYGWIN AND BUILD_SHARED_LIBS)

# Executable and runtime library directories.
if(WIN32)
  set(UF_EXECUTABLE_DIRS_CONFIG "\${UF_INSTALL_PREFIX}/${UF_INSTALL_BIN_DIR}")
  set(UF_RUNTIME_LIBRARY_DIRS_CONFIG "\${UF_INSTALL_PREFIX}/${UF_INSTALL_BIN_DIR}")
else(WIN32)
  set(UF_EXECUTABLE_DIRS_CONFIG "\${UF_INSTALL_PREFIX}/${UF_INSTALL_BIN_DIR}")
  set(UF_RUNTIME_LIBRARY_DIRS_CONFIG "\${UF_INSTALL_PREFIX}/${UF_INSTALL_LIB_DIR}")
endif(WIN32)

if(WIN32)
  set(UF_EXE_EXT ".exe")
endif(WIN32)

#-----------------------------------------------------------------------------
# Configure UFConfig.cmake for the install tree.

# Construct the proper number of GET_FILENAME_COMPONENT(... PATH)
# calls to compute the installation prefix from UF_DIR.
string(REGEX REPLACE "/" ";" UF_INSTALL_PACKAGE_DIR_COUNT
  "${UF_INSTALL_PACKAGE_DIR}")
set(UF_CONFIG_INSTALL_ONLY "
# Compute the installation prefix from UF_DIR.
set(UF_INSTALL_PREFIX \"\${UF_DIR}\")
")
foreach(p ${UF_INSTALL_PACKAGE_DIR_COUNT})
  set(UF_CONFIG_INSTALL_ONLY
    "${UF_CONFIG_INSTALL_ONLY}GET_FILENAME_COMPONENT(UF_INSTALL_PREFIX \"\${UF_INSTALL_PREFIX}\" PATH)\n"
    )
endforeach(p)

# The install tree only has one configuration.
set(UF_CONFIGURATION_TYPES_CONFIG)

if(CMAKE_CONFIGURATION_TYPES)
  # There are multiple build configurations.  Configure one
  # UFConfig.cmake for each configuration.
  foreach(config ${CMAKE_CONFIGURATION_TYPES})
    set(UF_BUILD_TYPE_CONFIG ${config})
    configure_file(${CMAKE_FILE_DIR}/ufConfig.cmake.in
                   ${UF_BINARY_DIR}/Utilities/${config}/ufConfig.cmake
                   @ONLY IMMEDIATE)
  endforeach(config)

  # Install the config file corresponding to the build configuration
  # specified when building the install target.  The BUILD_TYPE variable
  # will be set while CMake is processing the install files.
  install(
    FILES
      "${UF_BINARY_DIR}/Utilities/\${BUILD_TYPE}/ufConfig.cmake"
    DESTINATION ${UF_INSTALL_PACKAGE_DIR}
  )
else(CMAKE_CONFIGURATION_TYPES)
  # There is only one build configuration.  Configure one UFConfig.cmake.
  set(UF_BUILD_TYPE_CONFIG ${CMAKE_BUILD_TYPE})
  configure_file(${CMAKE_FILE_DIR}/ufConfig.cmake.in
                 ${UF_BINARY_DIR}/Utilities/ufConfig.cmake @ONLY IMMEDIATE)

  # Setup an install rule for the config file.
  install(
    FILES
      "${UF_BINARY_DIR}/Utilities/ufConfig.cmake"
    DESTINATION ${UF_INSTALL_PACKAGE_DIR}
  )
endif(CMAKE_CONFIGURATION_TYPES)
