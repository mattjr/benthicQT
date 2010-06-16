#-----------------------------------------------------------------------------
set( EXE_NAME EgBackgroundGradient )

#-----------------------------------------------------------------------------
#
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

#-----------------------------------------------------------------------------
# The sources
set (EXE_SRCS
  BackgroundGradient.cpp
)

set (EXE_INCS
)

#-----------------------------------------------------------------------------
# Add the executable.
add_executable( ${EXE_NAME} ${EXE_SRCS} ${EXE_INCS}
)

target_link_libraries ( ${EXE_NAME}
  vtkCommon
  vtkRendering
  vtkWidgets
  debug ufVTK
  optimized ufVTK
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
