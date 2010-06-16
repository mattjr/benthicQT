#-----------------------------------------------------------------------------
set( EXE_NAME1 EgGeographicConversions )

#-----------------------------------------------------------------------------
# The sources
set (EXE_SRCS1
  TestGeographicConversions.cpp
)

set (EXE_INCS1
)

#-----------------------------------------------------------------------------
# Add the executable.
add_executable( ${EXE_NAME1} ${EXE_SRCS1} ${EXE_INCS1}
)

target_link_libraries ( ${EXE_NAME1}
  ufGeographicConversions
  ufLogger
  ufDateTime
)

#-----------------------------------------------------------------------------
# Installing
if(UF_BINARY_DIR)
  install(TARGETS ${EXE_NAME1}
    RUNTIME DESTINATION ${UF_INSTALL_BIN_DIR} COMPONENT Runtime     # .exe, .dll
    LIBRARY DESTINATION ${UF_INSTALL_LIB_DIR} COMPONENT Runtime     # .so, mod.dll
    ARCHIVE DESTINATION ${UF_INSTALL_LIB_DIR} COMPONENT Development # .a, .lib
  )
endif(UF_BINARY_DIR)

#-----------------------------------------------------------------------------
set( EXE_NAME2 EgLatLong2UTM )

#-----------------------------------------------------------------------------
# The sources
set (EXE_SRCS2
  LatLong2UTM.cpp
)

set (EXE_INCS2
)

#-----------------------------------------------------------------------------
# Add the executable.
add_executable( ${EXE_NAME2} ${EXE_SRCS2} ${EXE_INCS2}
)

target_link_libraries ( ${EXE_NAME2}
  ufGeographicConversions
)

#-----------------------------------------------------------------------------
# Installing
if(UF_BINARY_DIR)
  install(TARGETS ${EXE_NAME2}
    RUNTIME DESTINATION ${UF_INSTALL_BIN_DIR} COMPONENT Runtime     # .exe, .dll
    LIBRARY DESTINATION ${UF_INSTALL_LIB_DIR} COMPONENT Runtime     # .so, mod.dll
    ARCHIVE DESTINATION ${UF_INSTALL_LIB_DIR} COMPONENT Development # .a, .lib
  )
endif(UF_BINARY_DIR)
