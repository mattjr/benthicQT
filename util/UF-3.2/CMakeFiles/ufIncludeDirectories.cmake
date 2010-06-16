#-----------------------------------------------------------------------------
# Include directories for other projects installed on the system.
set(UF_INCLUDE_DIRS_SYSTEM "")

set(UF_INCLUDE_DIRS_SYSTEM ${UF_INCLUDE_DIRS_SYSTEM})

#-----------------------------------------------------------------------------
# Include directories from the build tree.
set(UF_INCLUDE_DIRS_BUILD_TREE
  ${UF_BINARY_DIR}
  )


#-----------------------------------------------------------------------------
# Include directories from the source tree.
set(UF_INCLUDE_DIRS_SOURCE_TREE "")

# These directories are always needed.
set(UF_INCLUDE_DIRS_SOURCE_TREE ${UF_INCLUDE_DIRS_SOURCE_TREE}
  ${UF_SOURCE_DIR}
  ${UF_SOURCE_DIR}/Angle
  ${UF_SOURCE_DIR}/Colour
  ${UF_SOURCE_DIR}/Conversions
  ${UF_SOURCE_DIR}/CSVParser
  ${UF_SOURCE_DIR}/DateTime
  ${UF_SOURCE_DIR}/DTM
  ${UF_SOURCE_DIR}/FileUtilities
  ${UF_SOURCE_DIR}/GeographicConversions
  #  ${UF_SOURCE_DIR}/GPSNavigation
  ${UF_SOURCE_DIR}/Navigation
  ${UF_SOURCE_DIR}/GPSParser
  ${UF_SOURCE_DIR}/Logger
  ${UF_SOURCE_DIR}/Macros
  ${UF_SOURCE_DIR}/Statistics
  ${UF_SOURCE_DIR}/VTK
)


#-----------------------------------------------------------------------------
# Include directories for 3rd-party utilities provided by UF.

# Include GUI support
