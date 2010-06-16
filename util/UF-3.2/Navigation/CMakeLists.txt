#-----------------------------------------------------------------------------
# The name of the library.
set(KIT Navigation)
set(LIB_NAME ${LIB_PREFIX}${KIT})

#-----------------------------------------------------------------------------
# The directory in which the source files are.
set(${KIT}_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#-----------------------------------------------------------------------------
set(${KIT}_SRCS
  ufIEEEConstants.cpp
  ufGreatCircle.cpp
  ufPerson.cpp
  ufCopyright.cpp
  ufBounds.cpp
  ufEmail.cpp
  ufLink.cpp
  ufMetadata.cpp
  ufGpx.cpp
  ufGeographicPoint.cpp
  ufPointSegment.cpp
  ufRoute.cpp
  ufTrack.cpp
  ufTrackSegment.cpp
  ufWaypoint.cpp
  ufNavigationContainer.cpp
)

set(${KIT}_INCS
  ufIEEEConstants.h
  ufGreatCircle.h
  ufPerson.h
  ufCopyright.h
  ufBounds.h
  ufEmail.h
  ufLink.h
  ufMetadata.h
  ufGpx.h
  ufGeographicPoint.h
  ufPointSegment.h
  ufRoute.h
  ufTrack.h
  ufTrackSegment.h
  ufWaypoint.h
  ufNavigationContainer.h
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
