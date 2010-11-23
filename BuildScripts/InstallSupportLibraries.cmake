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


#-----------------------------------------------------------------------------
# MacOS X specific tasks
#-----------------------------------------------------------------------------

if(APPLE)
    #--------------------------------------------------------------------------------
    # Use BundleUtilities to get all other dependencies for the application to work.
    # It takes a bundle or executable along with possible plugins and inspects it
    # for dependencies.

    # directories to look for dependencies
    get_filename_component(OSG_LIBRARY_DIR ${OSG_LIBRARY} PATH)
    set(FIXUP_LIBRARY_SEARCH_PATH ${QT_LIBRARY_DIR} ${OSG_LIBRARY_DIR} ${QT_PLUGINS_DIR}/imageformats)

    # Now the work of copying dependencies into the bundle/package
    # The quotes are escaped and variables to use at install time have their $ escaped
    # An alternative is the do a configure_file() on a script and use install(SCRIPT  ...).
    # over.
#	set(FIXUP_EXTRA_LIBS ${OSG_INSTALLED_PLUGINS};${OPENSCENEGRAPH_LIBRARIES};\${QTPLUGINS})
    install(CODE "
        ##include(InstallRequiredSystemLibraries)
        file(GLOB_RECURSE QTPLUGINS
            \"\${CMAKE_INSTALL_PREFIX}/${QT_PLUGIN_DEST}/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
        set(QTPLUGINS \${QTPLUGINS};${OSG_INSTALLED_PLUGINS})
        message(STATUS \"Qt plugins: \${QTPLUGINS}\")
        message(STATUS \"Grel ${OSG_INSTALLED_PLUGINS}\")
	set(FIXUP_EXTRA_LIBS \"\${OSG_INSTALLED_PLUGINS};\${QTPLUGINS}\")
	 include(BundleUtilities)
        message(STATUS \"About to apply bundle fixup to: \${CMAKE_INSTALL_PREFIX}/${EXE_TARGET_NAME}.app\")
        fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/${EXE_TARGET_NAME}.app\" \"\${FIXUP_EXTRA_LIBS}\" \"${FIXUP_LIBRARY_SEARCH_PATH}\")
        " COMPONENT Runtime)
        INSTALL(CODE "EXECUTE_PROCESS( COMMAND ../BuildScripts/gendmg.sh)")

#        INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/${EXE_TARGET_NAME}.app/Contents/MacOS/osgdb_serializers_osg.so"
 #           DESTINATION
  #          "${CMAKE_CURRENT_BINARY_DIR}/${EXE_TARGET_NAME}.app/Contents/PlugIns/osgPlugins-${OPENSCENEGRAPH_VERSION}/")

# INSTALL(CODE "
#    file(GLOB_RECURSE QTPLUGINS
#      \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
#    include(BundleUtilities)
#    fixup_bundle(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")
#    " COMPONENT Runtime)

endif()




