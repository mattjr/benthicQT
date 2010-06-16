#-----------------------------------------------------------------------------
set( EXE_NAME EgColour )

#-----------------------------------------------------------------------------
# The sources
set (EXE_SRCS
  MyColour.cpp
)

set (EXE_INCS
)

if (WIN32 AND UF_BUILD_SHARED_LIBS)
  if (UF_BINARY_DIR)
    FIND_FILE(COLOUR_SRC Colour/ufColour.cpp ${UF_SOURCE_DIR})

    set (EXE_SRCS ${EXE_SRCS}
    ${COLOUR_SRC}
    )
  else (UF_BINARY_DIR)
    FIND_FILE(COLOUR_SRC ufColour.cpp ${UF_INCLUDE_DIRS})

    set (EXE_SRCS ${EXE_SRCS}
    ${COLOUR_SRC}
    )
  endif (UF_BINARY_DIR)
endif(WIN32 AND UF_BUILD_SHARED_LIBS)

#-----------------------------------------------------------------------------
# Add the executable.
add_executable( ${EXE_NAME} ${EXE_SRCS} ${EXE_INCS}
)

if (WIN32 AND UF_BUILD_SHARED_LIBS)
  target_link_libraries ( ${EXE_NAME}
    ufLogger
    ufDateTime
  )
else(WIN32 AND UF_BUILD_SHARED_LIBS)
  target_link_libraries ( ${EXE_NAME}
    ufColour
    ufLogger
    ufDateTime
  )
endif(WIN32 AND UF_BUILD_SHARED_LIBS)


#-----------------------------------------------------------------------------
# Installing
if(UF_BINARY_DIR)
  install(TARGETS ${EXE_NAME}
    RUNTIME DESTINATION ${UF_INSTALL_BIN_DIR} COMPONENT Runtime     # .exe, .dll
    LIBRARY DESTINATION ${UF_INSTALL_LIB_DIR} COMPONENT Runtime     # .so, mod.dll
    ARCHIVE DESTINATION ${UF_INSTALL_LIB_DIR} COMPONENT Development # .a, .lib
  )
endif(UF_BINARY_DIR)
