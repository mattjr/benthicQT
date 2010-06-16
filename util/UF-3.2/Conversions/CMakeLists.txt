#-----------------------------------------------------------------------------
# The name of the library.
set(KIT Conversions)
set(LIB_NAME ${LIB_PREFIX}${KIT})

#-----------------------------------------------------------------------------
# The directory in which the source files are.
set(${KIT}_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#-----------------------------------------------------------------------------
set(${KIT}_SRCS
  ufStringFunctions.cpp
)

set(${KIT}_INCS
  ufAngleDateTimeConversions.h
  ufNumberConversions.h
  ufNumberStringConversions.h
  ufPackUnpack.h
  ufStringFormat.h
  ufRotateVector.h
  ufStringFunctions.h
)

if ( UF_USE_BOOST )
  set ( ${KIT}_INCS ${${KIT}_INCS}
    ufCoordinateTransforms.h
  )
endif ( UF_USE_BOOST )

if ( WIN32 AND UF_BUILD_SHARED_LIBS )
  set_SOURCE_FILES_PROPERTIES (
    ufRotateVector.h
    WRAP_EXCLUDE
  )
  remove(${KIT}_SRCS ufStringFunctions.cpp)
  remove(${KIT}_INCS ufStringFunctions.h)
  if ( UF_USE_BOOST )
    set_source_files_properties (
      ufCoordinateTransforms.h
      WRAP_EXCLUDE
   )
  endif ( UF_USE_BOOST )

  # These variables are global.
  set ( ${KIT}_NOT_LIB_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/ufStringFunctions.cpp
    CACHE INTERNAL ""
  )
  set ( ${KIT}_NOT_LIB_INCS
     ${CMAKE_CURRENT_SOURCE_DIR}/ufRotateVector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ufStringFunctions.h
    CACHE INTERNAL ""
  )
  
  if ( UF_USE_BOOST )
    set ( ${KIT}_NOT_LIB_INCS ${${KIT}_NOT_LIB_INCS}
      ${CMAKE_CURRENT_SOURCE_DIR}/ufCoordinateTransforms.h
    )
  endif ( UF_USE_BOOST )
else ( WIN32 AND UF_BUILD_SHARED_LIBS )
  add_library( ${LIB_NAME}  ${${KIT}_SRCS} ${${KIT}_INCS} )
  set_target_properties(${LIB_NAME} PROPERTIES ${UF_LIBRARY_PROPERTIES})
#  set_target_properties(${LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)
endif ( WIN32 AND UF_BUILD_SHARED_LIBS )

#-----------------------------------------------------------------------------
# Installing
install(FILES ${${KIT}_INCS} ${${KIT}_NOT_LIB_INCS} DESTINATION ${UF_INSTALL_INCLUDE_DIR}/${KIT})
if ( WIN32 AND UF_BUILD_SHARED_LIBS )
  install(FILES ${${KIT}_NOT_LIB_SRCS} DESTINATION ${UF_INSTALL_INCLUDE_DIR}/${KIT})
else ( WIN32 AND UF_BUILD_SHARED_LIBS )
install(TARGETS ${LIB_NAME}
  RUNTIME DESTINATION bin COMPONENT Runtime     # .exe, .dll
  LIBRARY DESTINATION lib COMPONENT Runtime     # .so, mod.dll
  ARCHIVE DESTINATION lib COMPONENT Development # .a, .lib
)
endif ( WIN32 AND UF_BUILD_SHARED_LIBS )

