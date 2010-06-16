#-----------------------------------------------------------------------------
# The name of the library.
set(KIT VTK)
set(LIB_NAME ${LIB_PREFIX}${KIT})

#-----------------------------------------------------------------------------
# The directory in which the source files are.
set(${KIT}_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#-----------------------------------------------------------------------------
set(${KIT}_SRCS
  ufOrientationMarker.cpp
)

set(${KIT}_INCS
  ufOrientationMarker.h
)


ADD_LIBRARY( ${LIB_NAME} ${${KIT}_SRCS} ${${KIT}_INCS} )

SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES ${UF_LIBRARY_PROPERTIES})

if ( WIN32 AND UF_BUILD_SHARED_LIBS )
target_link_libraries ( ${LIB_NAME}
#  vtkCommon
#  vtkGraphics
#  vtkImaging
#  vtkIO
  vtkWidgets
)
endif ( WIN32 AND UF_BUILD_SHARED_LIBS )


#-----------------------------------------------------------------------------
# Installing
install(FILES ${${KIT}_INCS} DESTINATION ${UF_INSTALL_INCLUDE_DIR}/${KIT})
install(TARGETS ${LIB_NAME}
  RUNTIME DESTINATION bin COMPONENT Runtime     # .exe, .dll
  LIBRARY DESTINATION lib COMPONENT Runtime     # .so, mod.dll
  ARCHIVE DESTINATION lib COMPONENT Development # .a, .lib
)
