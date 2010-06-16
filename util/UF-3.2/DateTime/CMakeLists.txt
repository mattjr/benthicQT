#-----------------------------------------------------------------------------
# The name of the library.
set(KIT DateTime)
set(LIB_NAME ${LIB_PREFIX}${KIT})

#-----------------------------------------------------------------------------
# The directory in which the source files are.
set(${KIT}_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#-----------------------------------------------------------------------------
set(${KIT}_SRCS
  ufDateTime.cpp
  ufSystemTime.cpp
  ufISODateTime.cpp
  ufGPSDateTime.cpp
  ufTimeConversions.cpp
)

set(${KIT}_INCS
  ufDateTime.h
  ufSystemTime.h
  ufISODateTime.h
  ufGPSDateTime.h
  ufTimeConversions.h
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

