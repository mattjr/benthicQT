#-----------------------------------------------------------------------------
set( EXE1_NAME EgConversions )

#-----------------------------------------------------------------------------
# The sources
set (EXE1_SRCS
  Conversions.cpp
)

set (EXE1_INCS
)

#-----------------------------------------------------------------------------
# Add the executable.
add_executable( ${EXE1_NAME} ${EXE1_SRCS} ${EXE1_INCS}
)

target_link_libraries ( ${EXE1_NAME}
  ufLogger
  ufDateTime
)

#-----------------------------------------------------------------------------
set( EXE2_NAME EgStringFns )

#-----------------------------------------------------------------------------
# The sources
set (EXE2_SRCS
  StringFns.cpp
)

set (EXE2_INCS
)

if ( WIN32 AND UF_BUILD_SHARED_LIBS )
  if (UF_BINARY_DIR)
    FIND_FILE(STR_FN_SRC Conversions/ufStringFunctions.cpp ${UF_SOURCE_DIR})

    set (EXE2_SRCS ${EXE2_SRCS}
    ${STR_FN_SRC}
    )
  else (UF_BINARY_DIR)
    FIND_FILE(STR_FN_SRC /ufStringFunctions ${UF_INCLUDE_DIRS})

    set (EXE2_SRCS ${EXE2_SRCS}
    ${STR_FN_SRC}
    )
  endif (UF_BINARY_DIR)
endif ( WIN32 AND UF_BUILD_SHARED_LIBS )

#-----------------------------------------------------------------------------
# Add the executable.
add_executable( ${EXE2_NAME} ${EXE2_SRCS} ${EXE2_INCS}
)

if ( WIN32 AND UF_BUILD_SHARED_LIBS )
  target_link_libraries ( ${EXE2_NAME}
    ufLogger
    ufDateTime
  )
else ( WIN32 AND UF_BUILD_SHARED_LIBS )
  target_link_libraries ( ${EXE2_NAME}
    ufConversions
    ufLogger
    ufDateTime
  )
endif ( WIN32 AND UF_BUILD_SHARED_LIBS )

  if ( UF_USE_BOOST )
    #-----------------------------------------------------------------------------
    set( EXE3_NAME EgCoordinateTransforms )

    #-----------------------------------------------------------------------------
    # The sources
    set (EXE3_SRCS
      CoordinateTransforms.cpp
    )

    set (EXE3_INCS
    )

    #-----------------------------------------------------------------------------
    # Add the executable.
    add_executable( ${EXE3_NAME} ${EXE3_SRCS} ${EXE3_INCS}
    )

    target_link_libraries ( ${EXE3_NAME}
      ufLogger
      ufDateTime
    )
  endif ( UF_USE_BOOST )

#-----------------------------------------------------------------------------
# Installing
if(UF_BINARY_DIR)
  install(TARGETS ${EXE1_NAME} ${EXE2_NAME} ${EXE3_NAME}
    RUNTIME DESTINATION ${UF_INSTALL_BIN_DIR} COMPONENT Runtime     # .exe, .dll
    LIBRARY DESTINATION ${UF_INSTALL_LIB_DIR} COMPONENT Runtime     # .so, mod.dll
    ARCHIVE DESTINATION ${UF_INSTALL_LIB_DIR} COMPONENT Development # .a, .lib
  )
endif(UF_BINARY_DIR)
