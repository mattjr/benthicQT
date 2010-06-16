#-----------------------------------------------------------------------------
# The name of the library.
set(KIT FileUtilities)
set(LIB_NAME ${LIB_PREFIX}${KIT})

#-----------------------------------------------------------------------------
# The directory in which the source files are.
set(${KIT}_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#-----------------------------------------------------------------------------
set(${KIT}_SRCS
  ufFileUtilities.cpp
)

set(${KIT}_INCS
  ufFileUtilities.h
)

ADD_LIBRARY( ${LIB_NAME} ${${KIT}_SRCS} ${${KIT}_INCS} )

SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES ${UF_LIBRARY_PROPERTIES})

#-----------------------------------------------------------------------------
# Installing
install(FILES ${${KIT}_INCS} DESTINATION ${UF_INSTALL_INCLUDE_DIR}/${KIT})
install(TARGETS ${LIB_NAME}
  RUNTIME DESTINATION bin COMPONENT Runtime     # .exe, .dll
  LIBRARY DESTINATION lib COMPONENT Runtime     # .so, mod.dll
  ARCHIVE DESTINATION lib COMPONENT Development # .a, .lib
)
