#-----------------------------------------------------------------------------
# The name of the library.
set(KIT Macros)

#-----------------------------------------------------------------------------
# The directory in which the source files are.
set(${KIT}_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#-----------------------------------------------------------------------------
set(${KIT}_SRCS
)

set(${KIT}_INCS
  ufSetGetMacros.h
)

#-----------------------------------------------------------------------------
# Installing
install(FILES ${${KIT}_INCS} DESTINATION ${UF_INSTALL_INCLUDE_DIR}/${KIT})
