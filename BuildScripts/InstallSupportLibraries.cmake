#=============================================================================
# This cmake code handles the installation process to make sure all
# application dependencies are properly included in the binary
# distribution.
#=============================================================================


#-----------------------------------------------------------------------------
# Windows specific tasks
#-----------------------------------------------------------------------------

if(WIN32)

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_INSTALL_DEBUG_LIBRARIES TRUE)
    endif()
    
    # This is critical for windows.
    include(InstallRequiredSystemLibraries)
    

	# ---------- Find/Install the needed Qt4 libraries. 
	#get_filename_component(QT_DLL_PATH ${QT_QMAKE_EXECUTABLE} PATH)
	if(NOT QT_COMPONENTS OR NOT QT_DLL_PATH) 
		message(FATAL_ERROR "QT_COMPONENTS or QT_DLL_PATH needs to be defined.")
	endif()	
	message(STATUS "Qt library path: ${QT_DLL_PATH}, components ${QT_COMPONENTS}")
	if( QT_COMPONENTS AND QT_DLL_PATH)
		foreach( component ${QT_COMPONENTS} )
			install(FILES ${QT_DLL_PATH}/${component}d4.dll 
				DESTINATION bin 
				CONFIGURATIONS Debug RelWithDebInfo
				COMPONENT Runtime
			)
			install(FILES ${QT_DLL_PATH}/${component}4.dll 
				DESTINATION bin 
				CONFIGURATIONS Release RelWithDebInfo
				COMPONENT Runtime
			)
		endforeach()
	endif()

	# ---------- Find/Install the needed OpenSceneGraph libraries. 
    #get_filename_component(OSG_DLL_PATH ${OSG_LIBRARY} PATH)
	set(OSG_DLL_PATH ${OSG_DLL_PATH}/../bin)
	if(NOT OSG_COMPONENTS OR NOT OSG_DLL_PATH)
		message(FATAL_ERROR "OSG_COMPONENTS needs to be defined.")
	endif()
	list(APPEND OSG_COMPONENTS osg osgFX osgSim osgTerrain osgVolume)
	message(STATUS "OSG library path: ${OSG_DLL_PATH}, components ${OSG_COMPONENTS}")	
	if (OSG_COMPONENTS AND OSG_DLL_PATH)
	  	foreach( component ${OSG_COMPONENTS} )
			install(FILES ${OSG_DLL_PATH}/lib${component}d.dll 
				DESTINATION bin 
				CONFIGURATIONS Debug RelWithDebInfo
				COMPONENT Runtime
			)
			install(FILES ${OSG_DLL_PATH}/lib${component}.dll 
				DESTINATION bin 
				CONFIGURATIONS Release RelWithDebInfo
				COMPONENT Runtime
			)
		endforeach()
		# ---------- Find/Install the needed OpenThreads libraries. 
		install(FILES ${OSG_DLL_PATH}/libOpenThreads.dll 
			DESTINATION bin 
			CONFIGURATIONS Release RelWithDebInfo
			COMPONENT Runtime
		)
		install(FILES ${OSG_DLL_PATH}/libOpenThreadsd.dll 
			DESTINATION bin 
			CONFIGURATIONS Debug RelWithDebInfo
			COMPONENT Runtime
		)

	      install(FILES ${QT_DLL_PATH}/libgcc_s_dw2-1.dll
			DESTINATION bin 
			COMPONENT Runtime
		)
	      install(FILES ${QT_DLL_PATH}/mingwm10.dll
			DESTINATION bin 
			COMPONENT Runtime
		)
	      install(FILES ${OSG_DLL_PATH}/zlib1.dll
			DESTINATION bin 
			COMPONENT Runtime
		)
	      foreach(ITEM ${OSGPLUGINS})
		install(FILES ${OSG_DLL_PATH}/osgPlugins-${OPENSCENEGRAPH_VERSION}/mingw_${ITEM}.dll
		  DESTINATION bin/osgPlugins-${OPENSCENEGRAPH_VERSION}/
		  COMPONENT Runtime
		  )
	      endforeach()
#        get_filename_component(Qwt5_DLL_DIR ${Qwt5_Qt4_LIBRARY} PATH)
#        install(FILES ${Qwt5_DLL_DIR}/qwt5.dll
#            DESTINATION bin
#            CONFIGURATIONS Release RelWithDebInfo
#            COMPONENT Runtime
#        )
		
#        install(FILES ${Qwt5_DLL_DIR}/qwt5d.dll
#            DESTINATION bin
#            RENAME qwt5.dll
#            CONFIGURATIONS Debug RelWithDebInfo
#            COMPONENT Runtime
#        )
	endif()
	
endif()


