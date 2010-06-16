#-----------------------------------------------------------------------------
set( EXE1_NAME EgDTMConverter )

#-----------------------------------------------------------------------------
#
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable( ${EXE1_NAME} DTMConverter.cpp
)
target_link_libraries ( ${EXE1_NAME}
  ufDTM
  
  vtkCommon
  vtkGraphics
  vtkImaging
  vtkIO
  vtkWidgets
  vtkVolumeRendering
)

#-----------------------------------------------------------------------------
set( EXE2_NAME EgDTMVolume )

add_executable( ${EXE2_NAME} DTMVolume.cpp
)
target_link_libraries ( ${EXE2_NAME}
  ufDTM
  
  vtkCommon
  vtkRendering
  vtkVolumeRendering
  vtkWidgets
)

#-----------------------------------------------------------------------------
set( EXE3_NAME EgReadDTM )

add_executable( ${EXE3_NAME} ReadDTM.cpp
)

#-----------------------------------------------------------------------------
# Installing
if(UF_BINARY_DIR)
  install(TARGETS ${EXE1_NAME} ${EXE2_NAME} ${EXE3_NAME}
    RUNTIME DESTINATION ${UF_INSTALL_BIN_DIR} COMPONENT Runtime     # .exe, .dll
    LIBRARY DESTINATION ${UF_INSTALL_LIB_DIR} COMPONENT Runtime     # .so, mod.dll
    ARCHIVE DESTINATION ${UF_INSTALL_LIB_DIR} COMPONENT Development # .a, .lib
  )
endif(UF_BINARY_DIR)

#-----------------------------------------------------------------------------
# Copying Files.
set ( TEST_DATA
  ${CMAKE_CURRENT_SOURCE_DIR}/Data/Plane.txt
  ${CMAKE_CURRENT_SOURCE_DIR}/Data/SplitSphere.txt
)

ADD_CUSTOM_TARGET(CopyDTMData ALL echo "Copying DTM data files ...")

foreach(file ${TEST_DATA})
  get_filename_component(fn ${file} NAME)
  set(tgt ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/DTMData/${fn})
  set(src ${file})
  add_custom_command(
    TARGET CopyDTMData
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy_if_different ${src} ${tgt}
    COMMENT "Source copy of data files."
  )
endforeach(file)
