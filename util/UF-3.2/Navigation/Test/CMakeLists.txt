set(KIT Navigation)

#-----------------------------------------------------------------------------
# The sources
set (TestSrcs
  NavigationMain.cpp
  GPXMain.cpp
)

set (TestIncs
)

#-----------------------------------------------------------------------------
create_test_sourcelist(Tests Test${KIT}.cpp ${TestSrcs} ${TestIncs})
add_executable(Test${KIT} ${Tests})
target_link_libraries (  Test${KIT}
  ufNavigation
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

