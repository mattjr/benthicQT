#-----------------------------------------------------------------------------
set( EXE_NAME1 EgStatistics )

#-----------------------------------------------------------------------------
# The sources
set (EXE_SRCS1
  TestStatistics.cpp
)

set (EXE_INCS1
)

#-----------------------------------------------------------------------------
# Add the executable.
add_executable( ${EXE_NAME1} ${EXE_SRCS1} ${EXE_INCS1}
)

target_link_libraries ( ${EXE_NAME1}
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

if (UF_USE_BOOST)
  #-----------------------------------------------------------------------------
  set( EXE_NAME2 EgRNG )

  #-----------------------------------------------------------------------------
  # The sources
  set (EXE_SRCS2
    TestRNG.cpp
  )

  set (EXE_INCS2
  )

  #-----------------------------------------------------------------------------
  # Add the executable.
  add_executable( ${EXE_NAME2} ${EXE_SRCS2} ${EXE_INCS2}
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

endif (UF_USE_BOOST)
