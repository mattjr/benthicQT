option(BUILDING_MOMENTICS_PROJECTS
  "Build a QNX momentics IDE Make project."
  OFF)

#-----------------------------------------------------------------------------
if(UNIX AND NOT QNXNTO AND NOT APPLE)
  option(BUILD_SHARED_LIBS
           "Build with shared libraries."
           ON)
endif(UNIX AND NOT QNXNTO AND NOT APPLE)
if(QNXNTO)
  option(BUILD_SHARED_LIBS
           "Build with shared libraries."
           OFF)
endif(QNXNTO)
if(WIN32)
  option(BUILD_SHARED_LIBS
           "Build with shared libraries."
           OFF)
endif(WIN32)
if(APPLE)
  option(BUILD_SHARED_LIBS
           "Build with shared libraries."
           OFF)
endif(APPLE)

#-----------------------------------------------------------------------------
option(BUILD_EXAMPLES
  "Build the examples."
  OFF)

#-----------------------------------------------------------------------------
# Copy the CMake option to a setting with UF_ prefix for use in
# our project.  This name is used in ufConfigure.h.in.
set(UF_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

#-----------------------------------------------------------------------------
option(USE_BOOST
         "Use the Boost library."
         ON)
set(UF_USE_BOOST ${USE_BOOST})

#-----------------------------------------------------------------------------
option(USE_VTK
         "Use the VTK library."
         ON)
set(UF_USE_VTK ${USE_VTK})

#-----------------------------------------------------------------------------
if(WIN32)
  option(USE_MFC
           "Use the MFC Classes."
           OFF)
  set(UF_USE_MFC ${USE_MFC})
else(WIN32)
  set(UF_USE_MFC OFF)
endif(WIN32)
