
if(UF_BINARY_DIR)
  # Then we are part of the source build of the library.
  # Note that, in this case, these files can then be installed in the
  # binary directory of the library.
  #
  # If you install them and if you have used relative paths, then these
  # files will be relinked if necessary so that the libraries used will
  # be those in the lib directory.
  #
  
  add_subdirectory(Colour)
  add_subdirectory(Conversions)
  add_subdirectory(CSVParser)
  add_subdirectory(DateTime)
  add_subdirectory(GeographicConversions)
  add_subdirectory(GPSParser)
  add_subdirectory(Logger)
  add_subdirectory(Statistics)

  if ( UF_USE_BOOST )
   # The automatic linking, uses the static version of the libraries.
   if(WIN32)
     set(Boost_USE_STATIC_LIBS ON)
   endif(WIN32)

   # Uncomment and edit if CMake cannot find boost.
   #set(Boost_ADDITIONAL_VERSIONS "1.41" "1.41.0")
   find_package(Boost REQUIRED COMPONENTS system filesystem )
   if ( Boost_FOUND )
     include_directories(${Boost_INCLUDE_DIRS})
     link_directories(${Boost_LIBRARY_DIRS})
     add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
     # Remember to include ${Boost_LIBRARIES} in the target_link_libraries() statement
   endif ( Boost_FOUND )
  
 if(UF_USE_BOOST)
   add_subdirectory(FileUtilities)
 endif(UF_USE_BOOST)

endif ( UF_USE_BOOST )

  if (UF_USE_VTK)
    add_subdirectory(VTK/BackgroundGradient)
    add_subdirectory(VTK/CuboidDecomposition)
    add_subdirectory(VTK/DTM)
    add_subdirectory(VTK/ReadXYZRGB)
    add_subdirectory(VTK/ReadXYZ)
  endif(UF_USE_VTK)

  if(WIN32)
    add_subdirectory(ColourPickerComboBox)
  endif(WIN32)

else(UF_BINARY_DIR)

project("UF_Examples")

  #-----------------------------------------------------------------------------
  # We will enforce an out of source build.
  string(COMPARE EQUAL "${UF_Examples_SOURCE_DIR}"
    "${UF_Examples_BINARY_DIR}" INSOURCE)
  if(INSOURCE)
    message(FATAL_ERROR "UF_Examples requires an out of source build. Please create a separate binary directory and run CMake there.")
  endif(INSOURCE)


  #-----------------------------------------------------------------------------
  # Don't build anything unless the version of CMake is high enough.
  cmake_minimum_required(VERSION 2.6.4)

  #-----------------------------------------------------------------------------
  # Path to where the executable is.

  SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH
     "Single output directory for building all executables.")

  #-----------------------------------------------------------------------------
  # UF
  include(FindUF.cmake REQUIRED)

  #-----------------------------------------------------------------------------
  # Boost
  if ( UF_USE_BOOST )
    # The automatic linking, uses the static version of the libraries.
    if(WIN32)
      set(Boost_USE_STATIC_LIBS ON)
    endif(WIN32)

    # Uncomment and edit if CMake cannot find boost.
    #set(Boost_ADDITIONAL_VERSIONS "1.41" "1.41.0")
    find_package(Boost REQUIRED COMPONENTS system filesystem )
    if ( Boost_FOUND )
      include_directories(${Boost_INCLUDE_DIRS})
      link_directories(${Boost_LIBRARY_DIRS})
      add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
      # Remember to include ${Boost_LIBRARIES} in the target_link_libraries() statement
    endif ( Boost_FOUND )
  endif ( UF_USE_BOOST )


  #-----------------------------------------------------------------------------
  # VTK
  # FindVTK.cmake is in the modules directory of CMake.
  #SET( VTK_DIR VTK_DIR-NOTFOUND )
  if ( UF_USE_VTK )
    find_package(VTK REQUIRED)
    include(${VTK_USE_FILE})
  endif ( UF_USE_VTK )

  add_subdirectory(Colour)
  add_subdirectory(Conversions)
  add_subdirectory(CSVParser)
  add_subdirectory(DateTime)
  add_subdirectory(GeographicConversions)
  add_subdirectory(GPSParser)
  add_subdirectory(Logger)
  add_subdirectory(Statistics)

  if(UF_USE_BOOST)
    add_subdirectory(FileUtilities)
  endif(UF_USE_BOOST)

  if (UF_USE_VTK)
    add_subdirectory(VTK/BackgroundGradient)
    add_subdirectory(VTK/CuboidDecomposition)
    add_subdirectory(VTK/DTM)
    add_subdirectory(VTK/ReadXYZRGB)
    add_subdirectory(VTK/ReadXYZ)
  endif(UF_USE_VTK)

  if(UF_USE_MFC)
    add_subdirectory(ColourPickerComboBox)
  endif(UF_USE_MFC)
  
endif(UF_BINARY_DIR)
