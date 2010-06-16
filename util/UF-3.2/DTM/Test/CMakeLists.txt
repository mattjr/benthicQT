set(KIT DTM)

#-----------------------------------------------------------------------------
#
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

#-----------------------------------------------------------------------------
# The sources
set (TestSrcs
  DTMMain.cpp
)

set (TestIncs
)

#-----------------------------------------------------------------------------
create_test_sourcelist(Tests Test${KIT}.cpp ${TestSrcs} ${TestIncs}
#   ${${KIT}_NOT_LIB_SRCS} ${${KIT}_NOT_LIB_INCS} 
)
add_executable(Test${KIT} ${Tests})
target_link_libraries (  Test${KIT}
  ufDTM
  ufDateTime
  ufLogger

  vtkCommon
  vtkRendering
  vtkVolumeRendering
  vtkWidgets
)
set(TestsToRun ${Tests})
remove(TestsToRun  Test${KIT}.cpp)
#
# Add in all the executables
foreach(test ${TestsToRun})
  get_filename_component(TName ${test} NAME_WE)
  add_test(${TName} ${EXECUTABLE_OUTPUT_PATH}/Test${KIT} ${TName})
endforeach(test)

