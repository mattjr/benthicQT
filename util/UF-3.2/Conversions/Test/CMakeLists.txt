set(KIT Conversions)

#-----------------------------------------------------------------------------
# The sources
set (TestSrcs
  Conversions.cpp
)
if ( UF_USE_BOOST )
  set ( TestSrcs ${TestSrcs}
    CoordinateTransforms.cpp
  )
endif ( UF_USE_BOOST )
set (TestIncs
)

if (WIN32 AND UF_BUILD_SHARED_LIBS)
  add_executable(TestConversions1 TestConversions1.cpp Conversions.cpp)
  target_link_libraries (  TestConversions1
    ufDateTime
    ufLogger
  )
  add_executable(TestConversions3 TestConversions3.cpp StringFns.cpp ${${KIT}_NOT_LIB_SRCS})
  target_link_libraries (  TestConversions3
    ufDateTime
    ufLogger
  )
  add_test(TestConversions1 ${EXECUTABLE_OUTPUT_PATH}/Conversions1Test)
  add_test(TestConversions3 ${EXECUTABLE_OUTPUT_PATH}/Conversions3Test)

  if ( UF_USE_BOOST )
    add_executable(TestConversions2 TestConversions2.cpp CoordinateTransforms.cpp)
    target_link_libraries (  TestConversions2
      ufDateTime
      ufLogger
    )

    add_test(TestConversions2 ${EXECUTABLE_OUTPUT_PATH}/Conversions2Test)

  endif ( UF_USE_BOOST )
else (WIN32 AND UF_BUILD_SHARED_LIBS)
  #-----------------------------------------------------------------------------
  create_test_sourcelist(Tests Test${KIT}.cpp ${TestSrcs} ${TestIncs})
  add_executable(Test${KIT} ${Tests})
  target_link_libraries ( Test${KIT}
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
