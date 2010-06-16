set(KIT Colour)

#-----------------------------------------------------------------------------
# The sources
set (TestSrcs
  ColourMain.cpp
  MakeColourTable.cpp
)

set (TestIncs
)

if (WIN32 AND UF_BUILD_SHARED_LIBS)
  add_executable(TestColour1 TestColour1.cpp
    ${TestSrcs} ${TestIncs}
    ${${KIT}_NOT_LIB_SRCS} ${${KIT}_NOT_LIB_INCS}
  )
  add_executable(TestColour2 TestColour2.cpp
    ${TestSrcs} ${TestIncs}
    ${${KIT}_NOT_LIB_SRCS} ${${KIT}_NOT_LIB_INCS}
  )
  target_link_libraries (  TestColour1
    ufDateTime
    ufLogger
  )
  target_link_libraries (  TestColour2
    ufDateTime
    ufLogger
  )

  add_test(TestColour1 ${EXECUTABLE_OUTPUT_PATH}/Colour1Test)
  add_test(TestColour2 ${EXECUTABLE_OUTPUT_PATH}/Colour2test)

else (WIN32 AND UF_BUILD_SHARED_LIBS)
  #-----------------------------------------------------------------------------
  create_test_sourcelist(Tests Test${KIT}.cpp ${TestSrcs} ${TestIncs})
  add_executable(Test${KIT} ${Tests})
  target_link_libraries (  Test${KIT} ufColour
    ufDateTime
    ufLogger
  )
  set(TestsToRun ${Tests})
  remove(TestsToRun  Test${KIT}.cpp)
  #
  # Add in all the executables
  foreach(test ${TestsToRun})
    get_filename_component(TName ${test} NAME_WE)
    add_test(${TName} ${EXECUTABLE_OUTPUT_PATH}/Test${KIT} ${TName})
  endforeach(test)
endif (WIN32 AND UF_BUILD_SHARED_LIBS)

