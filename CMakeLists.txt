#--------------------------------------------------------------------------------
# This is the cmake project file for EWS, derived from the code from
# http://www.itk.org/Wiki/BundleUtilitiesExample
#
# To have CMake pick a Qt installation of your choice that won't be found
# automatically, set the CMAKE_PREFIX_PATH environment variable.
# For example: "export CMAKE_PREFIX_PATH=/usr/local/trolltech/qt4.3.5"
#  

project(benthicQT)
cmake_minimum_required(VERSION 2.8)

set(EXE_TARGET_NAME ${CMAKE_PROJECT_NAME})
set(BQT_APP_NAME "benthicQT")
set(BQT_VERSION_MAJOR "0")
set(BQT_VERSION_MINOR "1")
set(BQT_VERSION ${BQT_VERSION_MAJOR}.${BQT_VERSION_MINOR})
set(BQT_COPYRIGHT "2010 NVIDIA Foundation")
set(BQT_WEBSITE "http://www.acfr.usyd.edu.au/")
set(BQT_EMAIL   "m.roberson@cas.edu.au")
set(BQT_ICON_MAC "bqt-icon.icns")
set(BQT_ICON_ICO "bqt-icon.ico")
set(BQT_ICON_BMP "bqt-icon.bmp")
set(BQT_BUNDLE_ID "com.acfr.bqt")
set(BQT_ORGANIZATION_NAME "ACFR")
set(APP_ICON_RC "${CMAKE_CURRENT_SOURCE_DIR}/images/benthicQT.ico")
# Add to search path our custom scripts
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/BuildScripts;${CMAKE_MODULE_PATH}")
subdirs(  util/UF-3.2 LibVT)
add_definitions("-g")
#----------------------------------------------------------------------
# Macro to get the current date, from:
# http://www.cmake.org/pipermail/cmake/2009-February/027014.html
macro (curr_date RESULT)
    if(UNIX)
        execute_process(COMMAND "date" "+%d/%m/%Y" OUTPUT_VARIABLE CMD_OUT)
        string(REGEX REPLACE "(..)/(..)/(....).*" "\\3\\2\\1" ${RESULT} ${CMD_OUT})
elseif(CMAKE_CROSSCOMPILING)
        execute_process(COMMAND "date" "+%d/%m/%Y" OUTPUT_VARIABLE CMD_OUT)
        string(REGEX REPLACE "(..)/(..)/(....).*" "\\3\\2\\1" ${RESULT} ${CMD_OUT})
  # elseif (WIN32)
#        execute_process(COMMAND "cmd" "/C" "date" "/T" OUTPUT_VARIABLE CMD_OUT)
#        string(REGEX REPLACE ".*(..)/(..)/(....).*" "\\3\\2\\1" ${RESULT} ${CMD_OUT})    
    else()
        message(SEND_ERROR "date not available")
        set(${RESULT} 000000)
    endif()
endmacro()
curr_date(BQT_BUILD_DATE)

option(TESTING "build unit tests and include 'test' target" ON)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin CACHE PATH
  "Single output directory for building all executables.")
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})


set(BQT_MODULES app/widget app/drawable app/model physics geometry util test)

#--------------------------------------------------------------------------------
# This is the list of source files that make up the physics engine
#--------------------------------------------------------------------------------
file(GLOB BQT_PHYS_SRC physics/*.cpp)
find_package(JPEG REQUIRED)
#--------------------------------------------------------------------------------
# This is the list of source files that need to be compiled
#--------------------------------------------------------------------------------
file(GLOB BQT_SRCS main.cpp app/widget/*.cpp app/model/*.cpp app/drawable/*.cpp physics/*.cpp util/*.cpp)
#--------------------------------------------------------------------------------
#FFMPEG
#--------------------------------------------------------------------------------
SET(FFMPEG_FOUND "NO")
IF(NOT WIN32)

include(FindFFmpeg)
FIND_PACKAGE(FFmpeg)
ENDIF(NOT WIN32)


IF(FFMPEG_FOUND MATCHES "YES")
INCLUDE_DIRECTORIES( ${FFMPEG_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/LibVT)
LINK_DIRECTORIES(${FFMPEG_LIBRARY_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/LibVT)
SET(TARGET_EXTERNAL_LIBRARIES ${FFMPEG_LIBRARIES} x264)
#message("Using FFMPEG libs ${TARGET_EXTERNAL_LIBRARIES}")
add_definitions("-DUSE_FFMPEG")
IF(FFMPEG_LIBSWSCALE_FOUND)

    INCLUDE_DIRECTORIES( ${FFMPEG_LIBSWSCALE_INCLUDE_DIRS} ${FFMPEG_LIBSWSCALE_INCLUDE_DIRS}/libswscale )

    ADD_DEFINITIONS(-DUSE_SWSCALE)

    SET(TARGET_EXTERNAL_LIBRARIES ${FFMPEG_LIBRARIES} ${FFMPEG_LIBSWSCALE_LIBRARIES})

ENDIF()


ELSE()
file(GLOB_RECURSE FFMPEG_DEP_SRC app/widget/VideoStreamer.cpp)
list(REMOVE_ITEM BQT_SRCS ${FFMPEG_DEP_SRC})
ENDIF(FFMPEG_FOUND MATCHES "YES")
#--------------------------------------------------------------------------------
# Regular (non-qt) header files
#--------------------------------------------------------------------------------
file(GLOB BQT_HDRS physics/*.h util/*.h geometry/*.h BQTDefine.h)


#--------------------------------------------------------------------------------
# another list, this time it includes all header files that should be treated with moc
#--------------------------------------------------------------------------------
file(GLOB BQT_MOC_HDRS app/widget/*.h app/model/*.h app/drawable/*.h)
file(GLOB BQT_TEST_MOC_HDRS test/physics/*.h)
# The following is suppposed to add header files to the IDE without them having
# be be added to the target executable.


# Move the given file from the BQT_MOC_HDRS list to the BQT_HDRS list
macro(unmoc FILENAME)
    file(GLOB_RECURSE MOCKERY ${FILENAME})
    if(NOT MOCKERY)
        message(FATAL_ERROR "Couldn't find ${FILENAME} to unmoc.")    
    else()    
        list(REMOVE_ITEM BQT_MOC_HDRS ${MOCKERY})
        list(APPEND BQT_HDRS ${MOCKERY})
    endif()
endmacro()    

unmoc(IVideoStreamer.h)
unmoc(DrawableFactory.h)
unmoc(CameraController.h)
unmoc(WWManipulator.hpp)
unmoc(VertexHeightUpdater.h)
unmoc(PickHandler.h)
unmoc(Knob.h)
unmoc(PotentialUpdater.h)
unmoc(PositionHandler.h)
unmoc(WaterBoundaryDragConstraint.h)
unmoc(RStarBoundingBox.h)
unmoc(RStarTree.h)
unmoc(RStarVisitor.h)
unmoc(ProgressBar.h)
unmoc(GLPreCompile.h)
unmoc(Bboxes.hpp)
unmoc(VideoStreamer.h)
unmoc(OSGMovieCallback.h)
unmoc(ScreenTools.h)
unmoc(FindNode.h)
unmoc(MyShaderGen.h)

#message("moc files: ${BQT_MOC_HDRS}")

set_source_files_properties(${BQT_HDRS} PROPERTIES HEADER_FILE_ONLY TRUE) 


#--------------------------------------------------------------------------------
# .ui files that are used in the project
#--------------------------------------------------------------------------------
file(GLOB BQT_UIS app/widget/*.ui)

#--------------------------------------------------------------------------------
# and finally any resource files. 
#--------------------------------------------------------------------------------
file(GLOB_RECURSE BQT_RCS *.qrc)

#--------------------------------------------------------------------------------
# Vertex and fragment sharders
#--------------------------------------------------------------------------------
file(GLOB BQT_SHADERS app/drawable/waves.vs app/drawable/waves.fs)

#--------------------------------------------------------------------------------
#--------------------------------------------------------------------------------
# This sets the windows build that will need the special winmain@16 call. Qt provides
#  this for us in the qtmain.lib file. Using this cmake code will ensure we have it
#  linked into our build. Not needed on Unix/OS X/Linux which is why we have the 
#  if(WIN32) conditional.
if (WIN32)
    set (QT_USE_QTMAIN TRUE)
endif (WIN32)

#--------------------------------------------------------------------------------
# this command finds Qt4 libraries and sets all required variables
# note that it's Qt4, not QT4 or qt4
set(QT_COMPONENTS QtCore QtGui QtOpenGL QtSvg QtXml)
set(OSG_COMPONENTS osg osgUtil osgViewer osgGA osgManipulator osgText osgDB osgSim)
if(CMAKE_CROSSCOMPILING)
	set(QT_HEADERS_DIR /home/mattjr/.wine/drive_c/Qt/2010.05/qt/include)
	foreach(module QtCore QtGui QtSvg QtNetwork QtScript QtHelp QtWebKit QtTest QtXml QtSql QtCLucene QtOpenGL)
	  set(QT_HEADERS_DIR "/home/mattjr/.wine/drive_c/Qt/2010.05/qt/include/${module};${QT_HEADERS_DIR}")
	endforeach()
find_package(Qt4)
message("Cross compiling")
else()
# Add OpenSceneGraph suppport
find_package(OpenSceneGraph REQUIRED COMPONENTS ${OSG_COMPONENTS})
find_package(Qt4 4.5 REQUIRED COMPONENTS ${QT_COMPONENTS})
endif()
#-------------------------------------------------------------------------------
# QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to
# a CMake script that sets up this directory for building with Qt4
include( ${QT_USE_FILE} )
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------

if(WIN32)
  include(lin-mingw)
  include_directories(${QT_INCLUDES})
  add_definitions("-Wno-attributes")
endif(WIN32)

#-------------------------------------------------------------------------------

# Use qt menu nib stuff
 IF (APPLE)
 #   IF (BPC_BUNDLE_ICON)
 #     GET_FILENAME_COMPONENT(bundle_icon_file "${BQT_ICON_MAC}" NAME)
  #    SET(apple_bundle_sources ${bundle_icon_file})
  #    SET_SOURCE_FILES_PROPERTIES(
  #      ${BQT_ICON_MAC}
  #      PROPERTIES
  #      MACOSX_PACKAGE_LOCATION Resources
  #    )
      IF(QT_MAC_USE_COCOA)
        GET_FILENAME_COMPONENT(qt_menu_nib
          "@QT_QTGUI_LIBRARY_RELEASE@/Resources/qt_menu.nib"
          REALPATH)

        set(qt_menu_nib_sources
          "${qt_menu_nib}/classes.nib"
          "${qt_menu_nib}/info.nib"
          "${qt_menu_nib}/keyedobjects.nib"
          )
        SET_SOURCE_FILES_PROPERTIES(
          ${qt_menu_nib_sources}
          PROPERTIES
          MACOSX_PACKAGE_LOCATION Resources/qt_menu.nib
        )
      ELSE(QT_MAC_USE_COCOA)
        set(qt_menu_nib_sources)
      ENDIF(QT_MAC_USE_COCOA)
   # ENDIF (BPC_BUNDLE_ICON)
    SET(MAKE_BUNDLE MACOSX_BUNDLE)
  ENDIF (APPLE)



#--------------------------------------------------------------------------------
# this command will generate rules that will run rcc on all files from BQT_RCS
# in result BQT_GEN_RCS variable will contain paths to files produced by rcc
qt4_add_resources( BQT_GEN_RCS ${BQT_RCS} )
set_source_files_properties(${BQT_GEN_RCS} PROPERTIES GENERATED 1)
foreach(SHADE_SRC ${SHADERS})
    set_source_files_properties(${BQT_GEN_RCS} PROPERTIES OBJECT_DEPENDS ${SHADE_SRC})
endforeach()

macro(do_configure OUTFILE)
    set(INFILE ${OUTFILE}.in)
    configure_file(
        ${INFILE}
        ${OUTFILE}
    )    
    set_source_files_properties(${BQT_GEN_RCS} PROPERTIES OBJECT_DEPENDS ${INFILE})
endmacro()

do_configure(${CMAKE_CURRENT_SOURCE_DIR}/app/widget/about.html)
do_configure(${CMAKE_CURRENT_SOURCE_DIR}/util/BQTVersion.h)

#--------------------------------------------------------------------------------
# this will run uic on .ui files:
qt4_wrap_ui( BQT_GEN_UIS ${BQT_UIS} )

#--------------------------------------------------------------------------------
# and finally this will run moc on the headers
qt4_wrap_cpp( BQT_GEN_MOC_HDRS ${BQT_MOC_HDRS} )
qt4_wrap_cpp( BQT_TEST_GEN_MOC_HDRS ${BQT_TEST_MOC_HDRS} )



include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS} LibVT)

find_package(OpenGL REQUIRED)

#--------------------------------------------------------------------------------
# Apply platform specific compiler options
if(APPLE)
# Make application compatible with MacOS X Leopard.
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -isysroot /Developer/SDKs/MacOSX10.5.sdk")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isysroot /Developer/SDKs/MacOSX10.5.sdk")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmacosx-version-min=10.5")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmacosx-version-min=10.5")

#    set(CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)

   # if(${CMAKE_GENERATOR} STREQUAL "Xcode")
   #     set(CMAKE_OSX_ARCHITECTURES "i386;")
   # else()
        # Build for single architecture. Using this flag requires more than one
        # architecture in the list, so this hack of only providing one forces
        # a single architecture compile.
    #    set(CMAKE_OSX_ARCHITECTURES "i386;i386")
    #endif()
elseif(WIN32)
	# Stuff borrowed from OpenSceneGraph...
    if(MSVC)
        # This option is to enable the /MP switch for Visual Studio 2005 and above compilers
        OPTION(WIN32_USE_MP "Set to ON to build OpenSceneGraph with the /MP option (Visual Studio 2005 and above)." ON)
        MARK_AS_ADVANCED(WIN32_USE_MP)
        IF(WIN32_USE_MP)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        ENDIF(WIN32_USE_MP)

        # turn off various warnings
        # foreach(warning 4244 4251 4267 4275 4290 4786 4305 4996)
        #     SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd${warning}")
        # endforeach(warning)

        # More MSVC specific compilation flags
        ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
        ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
                ADD_DEFINITIONS(-DQWT_DLL)
    endif(MSVC)
endif()

#--------------------------------------------------------------------------------
# we need this to be able to include headers produced by uic in our code
# CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES()
#  works just like INCLUDEPATH from qmake

set(CMAKE_INCLUDE_CURRENT_DIR ON) # Adds binary dir.
include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${BQT_MODULES}
  util/UF-3.2
  ${UF_BINARY_DIR}
)

#--------------------------------------------------------------------------------
# For Apple set the icns file containing icons
if(APPLE)
  set(ICON_NAME bqt-icon.icns)
  # Set the executable wrapper type
  set(EXE_TYPE MACOSX_BUNDLE)
  # set how it shows up in the Info.plist file
  set(MACOSX_BUNDLE_ICON_FILE ${ICON_NAME}) 
  # set where in the bundle to put the icns file
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/images/${ICON_NAME} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  # include the icns file in the target
  set(BQT_SRCS ${BQT_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/images/${ICON_NAME})
  # Set additional application metadata.
  set(MACOSX_BUNDLE_BUNDLE_NAME ${BQT_APP_NAME})
  set(MACOSX_BUNDLE_IDENTIFIER ${BQT_BUNDLE_ID}) # Doesn't seem to work in cmake 2.6
  set(MACOSX_BUNDLE_INFO_STRING "${BQT_APP_NAME} - Version ${BQT_VERSION}")
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${BQT_VERSION})
  set(MACOSX_BUNDLE_BUNDLE_VERSION ${BQT_VERSION})
  set(MACOSX_BUNDLE_COPYRIGHT ${BQT_COPYRIGHT})  
elseif(WIN32)
  # Set the executable wrapper type
  set(EXE_TYPE WIN32)
  set(WIN_RC ${CMAKE_CURRENT_SOURCE_DIR}/winrez.rc)
endif()

#--------------------------------------------------------------------------------
# IDE conveniences
source_group("Headers" FILES ${BQT_HDRS})
source_group("Headers (Qt MOC)"  FILES ${BQT_MOC_HDRS})
source_group("Source (Qt Designer)" REGULAR_EXPRESSION ".*\\.ui$")
source_group("Source (Generated)" FILES ${BQT_GEN_MOC_HDRS} ${BQT_GEN_UIS} ${BQT_GEN_RCS})
source_group("Shaders" FILES ${BQT_SHADERS})

#--------------------------------------------------------------------------------
#  CMake's way of creating an executable
add_executable( ${EXE_TARGET_NAME} ${EXE_TYPE} 
  ${BQT_SRCS}
  ${BQT_GEN_MOC_HDRS} 
  ${BQT_GEN_UIS}
  ${BQT_GEN_RCS}
  ${WIN_RC}
  ${WIN32_ICON_O}
  ${BQT_MOC_HDRS}
  ${BQT_HDRS}
  ${apple_bundle_sources}
  ${qt_menu_nib_sources}
)
#--------------------------------------------------------------------------------
# Tell CMake which libraries we need to link our executable against.
set(ALL_QT_OSG_LIBS     
    ${QT_LIBRARIES}
    ${OPENSCENEGRAPH_LIBRARIES} 
    ${TARGET_EXTERNAL_LIBRARIES}
)

##message("libs:  ${ALL_QT_OSG_LIBS}")
add_executable(bosgviewer bosgviewer.cpp app/drawable/MyShaderGen.cpp)
target_link_libraries(${EXE_TARGET_NAME} ${OPENGL_LIBRARIES} ${ALL_QT_OSG_LIBS} ufGeographicConversions libvt ${JPEG_LIBRARY} )
target_link_libraries(bosgviewer ${OPENGL_LIBRARIES} ${ALL_QT_OSG_LIBS} ufGeographicConversions libvt ${JPEG_LIBRARY} )

#--------------------------------------------------------------------------------
#--------------------------------------------------------------------------------
# Installation setup stuff below
#--------------------------------------------------------------------------------
# On CMake 2.6.x use the newly minted CMAKE_LIBRARY_OUTPUT_DIRECTORY,
# CMAKE_ARCHIVE_OUTPUT_DIRECTORY & CMAKE_RUNTIME_OUTPUT_DIRECTORY

#--------------------------------------------------------------------------------
# Setup general and platform specific items for the target executable 

set(QT_CONF_DEST "bin")
set(QT_PLUGIN_DEST "bin/plugins")
SET(OSGPLUGINS osgdb_ive osgdb_serializers_osg osgdb_osg osgdb_freetype)

if(APPLE)
    set(QT_PLUGIN_DEST "${EXE_TARGET_NAME}.app/Contents/MacOS/plugins")  
    set(QT_CONF_DEST "${EXE_TARGET_NAME}.app/Contents/Resources")  
    get_filename_component(OSG_LIBRARY_DIR ${OSG_LIBRARY} PATH)
    set(OSG_PLUGINS_DIR "${OSG_LIBRARY_DIR}/osgPlugins-${OPENSCENEGRAPH_VERSION}")
    foreach(ITEM ${OSGPLUGINS})
      set(OSG_INSTALLED_PLUGINS "${CMAKE_BINARY_DIR}/${EXE_TARGET_NAME}.app/Contents/PlugIns/osgPlugins-${OPENSCENEGRAPH_VERSION}/${ITEM}.so;${OSG_INSTALLED_PLUGINS}")
    INSTALL(FILES "${OSG_PLUGINS_DIR}/${ITEM}.so"
    DESTINATION
    "${EXE_TARGET_NAME}.app/Contents/PlugIns/osgPlugins-${OPENSCENEGRAPH_VERSION}/")
#    "${EXE_TARGET_NAME}.app/Contents/MacOS")
    endforeach()

endif()

install(TARGETS ${EXE_TARGET_NAME} 
	BUNDLE DESTINATION . 
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION bin
	COMPONENT Runtime
)

#--------------------------------------------------------------------------------
# Install needed Qt plugins by copying directories from the qt installation
# One can cull what gets copied by using 'REGEX "..." EXCLUDE' 
# NB: this is important because icon loading, etc. need image plugins.

message(STATUS "Qt plugins dir = ${QT_PLUGINS_DIR}, extension = ${CMAKE_SHARED_LIBRARY_SUFFIX}")

install(DIRECTORY "${QT_PLUGINS_DIR}/imageformats" 
	DESTINATION ${QT_PLUGIN_DEST} 
	COMPONENT Runtime
	FILES_MATCHING PATTERN "*${CMAKE_SHARED_LIBRARY_SUFFIX}"
)


#--------------------------------------------------------------------------------
# install a qt.conf file
# this inserts some cmake code into the install script to write the file
# This is needed so Qt can find search path for plugins.
INSTALL(CODE "
    message(STATUS \"Creating: \${CMAKE_INSTALL_PREFIX}/${QT_CONF_DEST}/qt.conf\")
    file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${QT_CONF_DEST}/qt.conf\" \"\") "
    COMPONENT Runtime)
    
# Platform specific tweaks to get the library dependencies installed.
if(APPLE)
    include(InstallSupportLibraries)
elseif(WIN32)
	include(InstallSupportLibraries)
	file(GLOB WIN_MANIFEST ${CMAKE_CURRENT_BINARY_DIR}/*.manifest)
	install(FILES ${WIN_MANIFEST} DESTINATION bin COMPONENT Runtime)
endif()


#--------------------------------------------------------------------------------
#--------------------------------------------------------------------------------
# CPack configuration
#--------------------------------------------------------------------------------
set(CPACK_PACKAGE_NAME ${EXE_TARGET_NAME})
set(CPACK_PACKAGE_VENDOR "${BQT_ORGANIZATION_NAME}")
set(CPACK_GENERATOR ZIP)
set(CPACK_SOURCE_GENERATOR ZIP)
set(CPACK_MONOLITHIC_INSTALL TRUE)
set(CPACK_PACKAGE_FILE_NAME "Install-${CMAKE_PROJECT_NAME}-${BQT_VERSION}")

#set(CPACK_RESOURCE_FILE_LICENSE  "${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${BQT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${BQT_VERSION_MINOR}")

#  List components for CPACK_INSTALL_CMAKE_PROJECTS are (path;name;component;subdirectory).
if(APPLE)
    set(CPACK_GENERATOR DragNDrop)
elseif(WIN32)
    set(CPACK_GENERATOR NSIS)
	set(CPACK_NSIS_PACKAGE_NAME "${BQT_APP_NAME}")
	set(CPACK_NSIS_DISPLAY_NAME "${BQT_APP_NAME}")
	set(CPACK_NSIS_CONTACT "${BQT_EMAIL}")
	set(CPACK_NSIS_URL_INFO_ABOUT "${BQT_WEBSITE}")
    set(CPACK_NSIS_HELP_LINK "${BQT_WEBSITE}")
	set(CPACK_PACKAGE_EXECUTABLES "${EXE_TARGET_NAME}" "${BQT_APP_NAME}")
	set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CMAKE_PROJECT_NAME}-${BQT_VERSION}")

    # tell cpack to create a desktop link to CMakeSetup
    set(CPACK_CREATE_DESKTOP_LINKS "${EXE_TARGET_NAME}")
    set(CPACK_NSIS_MENU_LINKS "${BQT_WEBSITE}" "Homepage for ${BQT_APP_NAME}")

    # set the install icon used for the installer itself (.ico format) TODO: doesn't work
#    set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/images/${BQT_ICON_ICO}")
    # set the package header icon (.bmp format) TODO: doesn't work
 #   set(CPACK_PACKAGE_ICON  "${CMAKE_CURRENT_SOURCE_DIR}/images/${BQT_ICON_BMP}")

	# Icon in the add/remove control panel. Must be an .exe file on destination
	set(CPACK_NSIS_INSTALLED_ICON_NAME bin\\\\${EXE_TARGET_NAME}.exe)


    set(CPACK_NSIS_PAGE_COMPONENTS "
        !define MUI_FINISHPAGE_RUN \\\"$INSTDIR\\\\bin\\\\${EXE_TARGET_NAME}.exe\\\"
        !define MUI_STARTMENUPAGE_NODISABLE
    ")

    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
        CreateShortCut \\\"$DESKTOP\\\\${BQT_APP_NAME}.lnk\\\" \\\"$INSTDIR\\\\bin\\\\${EXE_TARGET_NAME}.exe\\\"
    ")

    set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
        Delete \\\"$DESKTOP\\\\${BQT_APP_NAME}.lnk\\\"
    ")
endif()


include(CPack)

# Get all defined components
get_cmake_property(CPACK_COMPONENTS_ALL COMPONENTS)

message(STATUS "CPack components: ${CPACK_COMPONENTS_ALL}")
