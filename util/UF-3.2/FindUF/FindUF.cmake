# Find the UF installation or build tree.
# The following variables are set if UF is found.  If UF is not
# found, UF_FOUND is set to false.
#  UF_FOUND         - Set to true when UF is found.
#  UF_USE_FILE      - CMake file to use UF.
#  UF_MAJOR_VERSION - The UF major version number.
#  UF_MINOR_VERSION - The UF minor version number
#                       (odd non-release).
#  UF_BUILD_VERSION - The UF patch level
#                       (meaningless for odd minor).
#  UF_INCLUDE_DIRS  - Include directories for UF
#  UF_LIBRARY_DIRS  - Link directories for UF libraries
# The following cache entries must be set by the user to locate UF:
#  UF_DIR  - The directory containing ufConfig.cmake.
#             This is either the root of the build tree,
#             or the lib/uf directory.  This is the
#             only cache entry.

# Construct consitent error messages for use below.
set(UF_DIR_DESCRIPTION "directory containing ufConfig.cmake.  This is either the root of the build tree, or PREFIX/lib/uf for an installation.")
set(UF_DIR_MESSAGE "UF not found.  Set the UF_DIR cmake cache entry to the ${UF_DIR_DESCRIPTION}")

#If the location is not already known, then search.
if( NOT UF_DIR )
  # Get the system search path as a list.
  if(UNIX)
    STRING(REGEX MATCHALL "[^:]+" UF_DIR_SEARCH1 "$ENV{PATH}")
  else(UNIX)
    STRING(REGEX REPLACE "\\\\" "/" UF_DIR_SEARCH1 "$ENV{PATH}")
  endif(UNIX)
  if ( WIN32 )
    # Assume this path exists.
    set ( UF_DIR_SEARCH1
      ${UF_DIR_SEARCH1}
      "C:/Program Files/UF"
      "C:/Program Files/UF 4.0"
      "C:/Program Files/UF 3.9"
      "C:/Program Files/UF 3.8"
      "C:/Program Files/UF 3.7"
      "C:/Program Files/UF 3.6"
      "C:/Program Files/UF 3.5"
      "C:/Program Files/UF 3.4"
      "C:/Program Files/UF 3.3"
      "C:/Program Files/UF 3.2"
      "C:/Program Files/UF 3.1"
      "C:/Program Files/UF 3.0"
    )
  endif ( WIN32 )
  STRING(REGEX REPLACE "/;" ";" UF_DIR_SEARCH2 "${UF_DIR_SEARCH1}")

  set ( UF_DIR_SEARCH "${UF_DIR_SEARCH2}" } )

  # Add in some path suffixes. These will have to be updated whenever a new UF version comes out.
  set ( SUFFIX_FOR_PATH
    lib/uf-4.0
    lib/uf-3.9
    lib/uf-3.8
    lib/uf-3.7
    lib/uf-3.6
    lib/uf-3.5
    lib/uf-3.4
    lib/uf-3.3
    lib/uf-3.2
    lib/uf-3.1
    lib/uf-3.0
    lib/uf
  )

  #
  # Look for an installation or build tree.
  #
  FIND_PATH(UF_DIR NAMES ufConfig.cmake PATH_SUFFIXES ${SUFFIX_FOR_PATH} PATHS

     # Look for an environment variable UF_DIR.
      $ENV{UF_DIR}

      # Look in places relative to the system executable search path.
      ${UF_DIR_SEARCH}

      # Look in standard UNIX install locations.
      /usr/local/lib/uf
      /usr/lib/uf

      # Help the user find it if we cannot.
      DOC "The ${UF_DIR_DESCRIPTION}"
  )
endif ( NOT UF_DIR )

# If UF was found, load the configuration file to get the rest of the
# settings.
if(UF_DIR)
  # We found UF.  Load the settings.
  set(UF_FOUND 1)
  INCLUDE(${UF_DIR}/ufConfig.cmake)
else(UF_DIR)
  # We did not find UF.
  set(UF_FOUND 0)
  message(FATAL_ERROR ${UF_DIR_MESSAGE})
endif(UF_DIR)

if ( UF_FOUND )
  if ( EXISTS ${UF_USE_FILE} )
    INCLUDE(${UF_USE_FILE})
  else ( EXISTS ${UF_USE_FILE} )
    message(FATAL_ERROR "UseUF.cmake not found in ${UF_DIR}")
    set(UF_FOUND 0)
  endif ( EXISTS ${UF_USE_FILE} )
endif ( UF_FOUND )
