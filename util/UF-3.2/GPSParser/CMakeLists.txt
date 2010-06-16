#-----------------------------------------------------------------------------
# The name of the library.
set(KIT GPSParser)
set(LIB_NAME ${LIB_PREFIX}${KIT})

#-----------------------------------------------------------------------------
# The directory in which the source files are.
set(${KIT}_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#-----------------------------------------------------------------------------
set(${KIT}_SRCS
  ufBOD.cpp
  ufBWC.cpp
  ufCASDTC.cpp
  ufCASRG.cpp
  ufGGA.cpp
  ufGLL.cpp
  ufGRME.cpp
  ufGRMM.cpp
  ufGRMSTO.cpp
  ufGRMZ.cpp
  ufGSA.cpp
  ufGSV.cpp
  ufHCHDG.cpp
  ufHDT.cpp
  ufNMEASentence.cpp
  ufPASHS.cpp
  ufPTPSRATT.cpp
  ufRMB.cpp
  ufRMC.cpp
  ufRTE.cpp
  ufSiRF103.cpp
  ufVTG.cpp
  ufWPL.cpp
  ufXTE.cpp
)

set(${KIT}_INCS
  ufBOD.h
  ufBWC.h
  ufCASDTC.h
  ufCASRG.h
  ufGGA.h
  ufGLL.h
  ufGRME.h
  ufGRMM.h
  ufGRMSTO.h
  ufGRMZ.h
  ufGSA.h
  ufGSV.h
  ufHCHDG.h
  ufHDT.h
  ufNMEASentence.h
  ufPASHS.h
  ufPTPSRATT.h
  ufRMB.h
  ufRMC.h
  ufRTE.h
  ufSiRF103.h
  ufVTG.h
  ufWPL.h
  ufXTE.h
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

