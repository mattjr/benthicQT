#-----------------------------------------------------------------------------
# The name of the library.
SET(KIT Statistics)
SET(LIB_NAME ${LIB_PREFIX}${KIT})

#-----------------------------------------------------------------------------
# The directory in which the source files are.
SET(${KIT}_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#-----------------------------------------------------------------------------
SET(${KIT}_SRCS
)

SET(${KIT}_INCS
  ufMeanSD.h
)

#-----------------------------------------------------------------------------
# Classes that cannot be compiled into a lib.

SET ( ${KIT}_NOT_LIB_INCS
   ${CMAKE_CURRENT_SOURCE_DIR}/ufMeanSD.h
)
SET ( ${KIT}_NOT_LIB_SRCS
)

#-----------------------------------------------------------------------------
# Installing
INSTALL(FILES ${${KIT}_INCS} DESTINATION ${UF_INSTALL_INCLUDE_DIR}/${KIT})
