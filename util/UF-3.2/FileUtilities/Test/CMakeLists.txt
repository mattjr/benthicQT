#-----------------------------------------------------------------------------
# Include files from the library.
set(KIT FileUtilities)

#-----------------------------------------------------------------------------
# The sources
set (TestSrcs
  FileUtilitiesMain.cpp
)

set (TestIncs
)

#-----------------------------------------------------------------------------
create_test_sourcelist(Tests Test${KIT}.cpp ${TestSrcs} ${TestIncs})

add_executable(Test${KIT} ${Tests})
target_link_libraries ( Test${KIT}
  ufFileUtilities
  ufLogger
  ufDateTime
)

if ( ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.5 )

  # If we use automatic linking then there is no need to add in the libraries.
  if( NOT WIN32 )
    target_link_libraries (  Test${KIT}
      ${Boost_LIBRARIES}
    )
  endif( NOT WIN32 )
  
else ( ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.5 )

  if(UNIX AND NOT QNXNTO)
  target_link_libraries (  Test${KIT}
    boost_filesystem
  )
  endif(UNIX AND NOT QNXNTO)
  # In QNX, you may have to specify the build e.g. -gcc of the library
  # as is done here. This occurs if the library has been staged but
  # not installed.
  if(QNXNTO)
  target_link_libraries (  Test${KIT}
    boost_filesystem-gcc
  )
  endif(QNXNTO)

endif( ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.5 )

set(TestsToRun ${Tests})
remove(TestsToRun  Test${KIT}.cpp)
#
# Add in all the executables
foreach(test ${TestsToRun})
  get_filename_component(TName ${test} NAME_WE)
  add_test(${TName} ${EXECUTABLE_OUTPUT_PATH}/Test${KIT} ${TName})
endforeach(test)
