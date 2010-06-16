#-----------------------------------------------------------------------------
set( EXE1_NAME EgDateTime )

#-----------------------------------------------------------------------------
# The sources
set (EXE1_SRCS
  DateTime.cpp
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
set( EXE2_NAME EgGPSTime )

#-----------------------------------------------------------------------------
# The sources
set (EXE2_SRCS
  GPSTime.cpp
)

set (EXE2_INCS
)

#-----------------------------------------------------------------------------
# Add the executable.
add_executable( ${EXE2_NAME} ${EXE2_SRCS} ${EXE2_INCS}
)

target_link_libraries ( ${EXE2_NAME}
  ufLogger
  ufDateTime
)

#-----------------------------------------------------------------------------
# Installing
if(UF_BINARY_DIR)
  install(TARGETS ${EXE1_NAME} ${EXE2_NAME}
    RUNTIME DESTINATION ${UF_INSTALL_BIN_DIR} COMPONENT Runtime     # .exe, .dll
    LIBRARY DESTINATION ${UF_INSTALL_LIB_DIR} COMPONENT Runtime     # .so, mod.dll
    ARCHIVE DESTINATION ${UF_INSTALL_LIB_DIR} COMPONENT Development # .a, .lib
  )
endif(UF_BINARY_DIR)
