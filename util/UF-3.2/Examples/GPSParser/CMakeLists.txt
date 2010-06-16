#-----------------------------------------------------------------------------
set( EXE_NAME EgGPSParser )

#-----------------------------------------------------------------------------
# The sources
set (EXE_SRCS
  TestGPSParser.cpp
)

set (EXE_INCS
)

#-----------------------------------------------------------------------------
# Add the executable.
add_executable( ${EXE_NAME} ${EXE_SRCS} ${EXE_INCS}
)

target_link_libraries ( ${EXE_NAME}
  ufGPSParser
  ufLogger
  ufDateTime
)

#-----------------------------------------------------------------------------
# Installing
if(UF_BINARY_DIR)
  install(TARGETS ${EXE_NAME}
    RUNTIME DESTINATION ${UF_INSTALL_BIN_DIR} COMPONENT Runtime     # .exe, .dll
    LIBRARY DESTINATION ${UF_INSTALL_LIB_DIR} COMPONENT Runtime     # .so, mod.dll
    ARCHIVE DESTINATION ${UF_INSTALL_LIB_DIR} COMPONENT Development # .a, .lib
  )
endif(UF_BINARY_DIR)
