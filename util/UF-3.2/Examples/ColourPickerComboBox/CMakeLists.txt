#-----------------------------------------------------------------------------
set( EXE_NAME EgColourPickerComboBox )

#-----------------------------------------------------------------------------
# Add stuff to use MFC in this executable
add_definitions( -D_AFXDLL )

# Use of MFC
# 0: Use Standard Windows Libraries
# 1: Use MFC in a Static Library
# 2: Use MFC in a Shared DLL
set(CMAKE_MFC_FLAG 2)

#-----------------------------------------------------------------------------
# Miscellaneous files.
set( MISC_FILES
  ReadMe.txt
)

#-----------------------------------------------------------------------------
# Resource files.
set( RESOURCES
  ColourPickerComboBox.rc
  res/ColourPickerComboBox.rc2
)

#-----------------------------------------------------------------------------
# The sources
set (EXE_SRCS
  stdafx.cpp
  ColourPickerComboBox.cpp
  ColourPickerComboBoxDlg.cpp
  ColourEditCtrl.cpp
  PickAColour.cpp
  ColourPickerCB.cpp
)

set (EXE_INCS
  resource.h
  stdafx.h
  ColourPickerComboBox.h
  ColourPickerComboBoxDlg.h
  ColourEditCtrl.h
  PickAColour.h
  ColourPickerCB.h
)

if (WIN32 AND UF_BUILD_SHARED_LIBS)
	if (UF_BINARY_DIR)
	  FIND_FILE(CP_COLOUR_SRC Colour/ufColour.cpp ${UF_SOURCE_DIR})
	  
	  set (EXE_SRCS ${EXE_SRCS}
		${CP_COLOUR_SRC}
	  )
	else (UF_BINARY_DIR)
	  FIND_FILE(CP_COLOUR_SRC ufColour.cpp ${UF_INCLUDE_DIRS})

	  set (EXE_SRCS ${EXE_SRCS}
		${CP_COLOUR_SRC}
	  )
	endif (UF_BINARY_DIR)
endif(WIN32 AND UF_BUILD_SHARED_LIBS)

#-----------------------------------------------------------------------------
# Source groups.
SOURCE_GROUP ( "Resource Files" FILES ${RESOURCES} )

#-----------------------------------------------------------------------------
# Add the executable.
add_executable( ${EXE_NAME} WIN32 ${EXE_SRCS} ${EXE_INCS} ${RESOURCES}
)

if (WIN32 AND UF_BUILD_SHARED_LIBS)
  target_link_libraries ( ${EXE_NAME}
	ufLogger
	ufDateTime
  )
else(WIN32 AND UF_BUILD_SHARED_LIBS)
  target_link_libraries ( ${EXE_NAME}
    ufColour
    ufLogger
    ufDateTime
  )
endif(WIN32 AND UF_BUILD_SHARED_LIBS)

#-----------------------------------------------------------------------------
# Installing
if(UF_BINARY_DIR)
  install(TARGETS ${EXE_NAME}
    RUNTIME DESTINATION ${UF_INSTALL_BIN_DIR} COMPONENT Runtime     # .exe, .dll
    LIBRARY DESTINATION ${UF_INSTALL_LIB_DIR} COMPONENT Runtime     # .so, mod.dll
    ARCHIVE DESTINATION ${UF_INSTALL_LIB_DIR} COMPONENT Development # .a, .lib
  )
endif(UF_BINARY_DIR)
