#-----------------------------------------------------------------------------
# The name of the library.
set(KIT Colour)
set(LIB_NAME ${LIB_PREFIX}${KIT})

#-----------------------------------------------------------------------------
# The directory in which the source files are.
set(${KIT}_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#-----------------------------------------------------------------------------
set(${KIT}_SRCS
  ufColour.cpp
)

set(${KIT}_INCS
  ufColour.h
)

if ( WIN32 AND UF_BUILD_SHARED_LIBS )
  set_source_files_properties (
    ufColour.h
    ufColour.cpp
    WRAP_EXCLUDE
  )
endif ( WIN32 AND UF_BUILD_SHARED_LIBS )

#-----------------------------------------------------------------------------
# Classes that cannot be compiled into a dll.

if ( WIN32 AND UF_BUILD_SHARED_LIBS )
  # These variables are global.
  set ( ${KIT}_NOT_LIB_SRCS
     ${CMAKE_CURRENT_SOURCE_DIR}/ufColour.cpp
    CACHE INTERNAL ""
  )
  set ( ${KIT}_NOT_LIB_INCS
     ${CMAKE_CURRENT_SOURCE_DIR}/ufColour.h
    CACHE INTERNAL ""
  )
else ( WIN32 AND UF_BUILD_SHARED_LIBS )
  add_library( ${LIB_NAME}  ${${KIT}_SRCS} ${${KIT}_INCS} )
  set_target_properties(${LIB_NAME} PROPERTIES ${UF_LIBRARY_PROPERTIES})
endif ( WIN32 AND UF_BUILD_SHARED_LIBS )


#-----------------------------------------------------------------------------
# Installing
install(FILES ${${KIT}_INCS} DESTINATION ${UF_INSTALL_INCLUDE_DIR}/${KIT})
if ( WIN32 AND UF_BUILD_SHARED_LIBS )
  install(FILES ${${KIT}_SRCS} DESTINATION ${UF_INSTALL_INCLUDE_DIR}/${KIT})
else ( WIN32 AND UF_BUILD_SHARED_LIBS )
install(TARGETS ${LIB_NAME}
  RUNTIME DESTINATION bin COMPONENT Runtime     # .exe, .dll
  LIBRARY DESTINATION lib COMPONENT Runtime     # .so, mod.dll
  ARCHIVE DESTINATION lib COMPONENT Development # .a, .lib
)
endif ( WIN32 AND UF_BUILD_SHARED_LIBS )

