#=============================================================================
# This cmake code handles the installation process to make sure all
# application dependencies are properly included in the binary
# distribution.
#=============================================================================




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
        set(QTPLUGINS \${QTPLUGINS};${OSG_INSTALLED_PLUGINS};${QT_ADDITIONAL_PLUGINS})
        message(STATUS \"Qt plugins: \${QTPLUGINS}\")
        message(STATUS \"Grel ${OSG_INSTALLED_PLUGINS}\")
	set(FIXUP_EXTRA_LIBS \"\${OSG_INSTALLED_PLUGINS};\${QTPLUGINS}\")
        ## the qt.conf is mandatory too
        file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${EXE_TARGET_NAME}.app/Contents/Resources/qt.conf\" \"[Paths]
Plugins = plugins\")
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




