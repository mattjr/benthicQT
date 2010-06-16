#-----------------------------------------------------------------------------
set( EXE_NAME EgFileUtilities )

#-----------------------------------------------------------------------------
# The sources
set (EXE_SRCS
  FileUtilities.cpp
)

set (EXE_INCS
)

#-----------------------------------------------------------------------------
# Add some definitions.
if(WIN32)
  # Automatic linking of Boost in Windows.
  # We may as well see what is being linked.
  add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
endif(WIN32)

#-----------------------------------------------------------------------------
# Add the executable.
add_executable( ${EXE_NAME} ${EXE_SRCS} ${EXE_INCS}
)

target_link_libraries ( ${EXE_NAME}
  ufFileUtilities
  ufLogger
  ufDateTime
)

if ( ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.5 )

  # If we use automatic linking then there is no need to add in the libraries.
  if( NOT WIN32 )
    target_link_libraries ( ${EXE_NAME}
      ${Boost_LIBRARIES}
    )
  endif( NOT WIN32 )
  
else ( ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.5 )

  if(UNIX AND NOT QNXNTO)
  target_link_libraries ( ${EXE_NAME}
    boost_filesystem
  )
  endif(UNIX AND NOT QNXNTO)
  # In QNX, you may have to specify the build e.g. -gcc of the library
  # as is done here. This occurs if the library has been staged but
  # not installed.
  if(QNXNTO)
  target_link_libraries ( ${EXE_NAME}
    boost_filesystem-gcc
  )
  endif(QNXNTO)

endif( ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.5 )

#-----------------------------------------------------------------------------
# Installing
if(UF_BINARY_DIR)
  install(TARGETS ${EXE_NAME}
    RUNTIME DESTINATION ${UF_INSTALL_BIN_DIR} COMPONENT Runtime     # .exe, .dll
    LIBRARY DESTINATION ${UF_INSTALL_LIB_DIR} COMPONENT Runtime     # .so, mod.dll
    ARCHIVE DESTINATION ${UF_INSTALL_LIB_DIR} COMPONENT Development # .a, .lib
  )
endif(UF_BINARY_DIR)
