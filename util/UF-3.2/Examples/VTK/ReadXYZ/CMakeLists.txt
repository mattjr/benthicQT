#-----------------------------------------------------------------------------
set( EXE_NAME EgReadXYZ )

#-----------------------------------------------------------------------------
# The sources
set (EXE_SRCS
  reader.cxx
)

set (EXE_INCS
)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

#-----------------------------------------------------------------------------
# Add the executable.
add_executable( ${EXE_NAME} ${EXE_SRCS} ${EXE_INCS}
)

target_link_libraries ( ${EXE_NAME}
  vtkCommon
  vtkIO
  vtkRendering
)

#-----------------------------------------------------------------------------
# Installing
if(UF_BINARY_DIR)
  install(TARGETS ${EXE_NAME}
    RUNTIME DESTINATION ${UF_INSTALL_BIN_DIR} COMPONENT Runtime     # .exe, .dll
    LIBRARY DESTINATION ${UF_INSTALL_LIB_DIR} COMPONENT Runtime     # .so, mod.dll
    ARCHIVE DESTINATION ${UF_INSTALL_LIB_DIR} COMPONENT Development # .a, .lib
  )
endif(UF_BINARY_DIR)

#-----------------------------------------------------------------------------
# Copying Files.
set ( TEST_DATA
  ${CMAKE_CURRENT_SOURCE_DIR}/Seamount.txt
)

ADD_CUSTOM_TARGET(CopyParticleDataXYZ ALL echo "Copying ReadXYZ data files ...")

foreach(file ${TEST_DATA})
  get_filename_component(fn ${file} NAME)
  set(tgt ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/${fn})
  set(src ${file})
  add_custom_command(
    TARGET CopyParticleDataXYZ
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy_if_different ${src} ${tgt}
    COMMENT "Source copy of data files."
  )
endforeach(file)
