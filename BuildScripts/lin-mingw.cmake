#
# This is a simple demo to test qt4 + openssl + mingw + cmake for Windows on Linux.
# License : MIT-LICENSE
#
if(WIN32)
	#We haven't built Qt with debug support, so no point setting debug flags.
	set(CMAKE_BUILD_TYPE "Release")
	ADD_DEFINITIONS(${QT_DEFINITIONS})
	SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_RELEASE QT_NO_DEBUG)

	#set the default pathes, should be changed to match your setup.
	set(WIN32_BASE "/home/mattjr/.wine/drive_c/")
	set(QT_BASE ${WIN32_BASE}/Qt/2010.05/qt)
        set(OSG_BASE ${WIN32_BASE}/OSG)
	set(APP_ICON_RC ${CMAKE_CURRENT_SOURCE_DIR}/winrez.rc)
	SET(QT_DLL_PATH ${QT_BASE}/bin/)
	SET(OSG_DLL_PATH ${OSG_BASE}/bin/)
	set(MINGW_PREFIX "i586-mingw32msvc-")
	# set "sane" default cxxflags for windows, the -mwindows so it wouldn't open a command dos window.
	set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -march=pentium4 -mtune=pentium4 -mwindows -O2")
	# we need -static-libgcc otherwise we'll link against libgcc_s_sjlj-1.dll.
	SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "-Wl,--no-undefined -static-libgcc -Wl,-O1 -Wl,--as-needed -Wl,--sort-common -s")

	#you should NOT mess with things below this line.

	#set mingw defaults
	set(CMAKE_CXX_COMPILER ${MINGW_PREFIX}g++)
	set(CMAKE_AR           ${MINGW_PREFIX}ar)
	set(CMAKE_RANLIB       ${MINGW_PREFIX}ranlib)
	set(CMAKE_LINKER       ${MINGW_PREFIX}ld)
	set(CMAKE_STRIP        ${MINGW_PREFIX}strip)

	set(CMAKE_EXECUTABLE_SUFFIX ".exe") # if we don't do this, it'll output executables without the .exe suffix

	# add an icon to the exe if we set one.
	if(APP_ICON_RC)
		set(WIN32_ICON_O ${CMAKE_CURRENT_BINARY_DIR}/_app_icon.o)
		ADD_CUSTOM_COMMAND( OUTPUT ${WIN32_ICON_O}
							COMMAND ${MINGW_PREFIX}windres
								-I${CMAKE_CURRENT_SOURCE_DIR}
								-o${WIN32_ICON_O}
								-i${APP_ICON_RC}
							)
	endif()
	set(QT_LIBRARIES "")
	foreach(module QtCore QtGui QtSvg QtNetwork QtScript QtHelp QtWebKit QtTest QtXml QtSql QtCLucene QtOpenGL)
		string(TOUPPER "${module}" umod)
#		if(QT_USE_${module})
			set(QT_${umod}_LIBRARY ${QT_BASE}/bin/${module}4.dll)
			set(QT_LIBRARIES ${QT_LIBRARIES} ${QT_${umod}_LIBRARY})
		

#		endif()
	endforeach()
	SET(OPENSCENEGRAPH_LIBRARIES  ${OSG_BASE}/bin/libOpenThreads.dll;${OSG_BASE}/bin/libosg.dll;${OSG_BASE}/bin/libosgUtil.dll;${OSG_BASE}/bin/libosgViewer.dll;${OSG_BASE}/bin/libosgGA.dll;${OSG_BASE}/bin/libosgManipulator.dll;${OSG_BASE}/bin/libosgText.dll;${OSG_BASE}/bin/libosgDB.dll)
	set(QT_INCLUDES ${QT_HEADERS_DIR})
#${QT_BASE}/include/)
	set(OPENSCENEGRAPH_INCLUDE_DIRS ${OSG_BASE}/include)
	set(OSG_INCLUDE_DIR ${OSG_BASE}/include)
	include(GetOSGVersion)
	message("Building For   : Win32")
	if(NOT IS_DIRECTORY ${QT_BASE}/lib)
		message("Error : ${QT_BASE} doesn't exist, won't be able to link against Qt4.")
	endif()
	if(NOT IS_DIRECTORY ${OSSl_BASE}/)
		message("Error : ${OSSL_BASE} doesn't exist, won't be able to link against OpenSSL.")
	endif()
else() #include Qt4 defaults for linux
	include(${QT_USE_FILE})
endif()


