# Try to ascertain the version...
if(OSG_INCLUDE_DIR)
    if(OpenSceneGraph_DEBUG)
        message("[ FindOpenSceneGraph.cmake:${CMAKE_CURRENT_LIST_LINE} ] "
            "Detected OSG_INCLUDE_DIR = ${OSG_INCLUDE_DIR}")
    endif()

    file(READ "${OSG_INCLUDE_DIR}/osg/Version" _osg_Version_contents)

    string(REGEX MATCH ".*#define OSG_VERSION_MAJOR[ \t]+[0-9]+.*"
        _osg_old_defines ${_osg_Version_contents})
    string(REGEX MATCH ".*#define OPENSCENEGRAPH_MAJOR_VERSION[ \t]+[0-9]+.*"
        _osg_new_defines ${_osg_Version_contents})
    if(_osg_old_defines)
        string(REGEX REPLACE ".*#define OSG_VERSION_MAJOR[ \t]+([0-9]+).*"
            "\\1" _osg_VERSION_MAJOR ${_osg_Version_contents})
        string(REGEX REPLACE ".*#define OSG_VERSION_MINOR[ \t]+([0-9]+).*"
            "\\1" _osg_VERSION_MINOR ${_osg_Version_contents})
        string(REGEX REPLACE ".*#define OSG_VERSION_PATCH[ \t]+([0-9]+).*"
            "\\1" _osg_VERSION_PATCH ${_osg_Version_contents})
    elseif(_osg_new_defines)
        string(REGEX REPLACE ".*#define OPENSCENEGRAPH_MAJOR_VERSION[ \t]+([0-9]+).*"
            "\\1" _osg_VERSION_MAJOR ${_osg_Version_contents})
        string(REGEX REPLACE ".*#define OPENSCENEGRAPH_MINOR_VERSION[ \t]+([0-9]+).*"
            "\\1" _osg_VERSION_MINOR ${_osg_Version_contents})
        string(REGEX REPLACE ".*#define OPENSCENEGRAPH_PATCH_VERSION[ \t]+([0-9]+).*"
            "\\1" _osg_VERSION_PATCH ${_osg_Version_contents})
    else()
        message("[ FindOpenSceneGraph.cmake:${CMAKE_CURRENT_LIST_LINE} ] "
            "Failed to parse version number, please report this as a bug")
    endif()

    set(OPENSCENEGRAPH_VERSION "${_osg_VERSION_MAJOR}.${_osg_VERSION_MINOR}.${_osg_VERSION_PATCH}"
                                CACHE INTERNAL "The version of OSG which was detected")
    if(OpenSceneGraph_DEBUG)
        message("[ FindOpenSceneGraph.cmake:${CMAKE_CURRENT_LIST_LINE} ] "
            "Detected version ${OPENSCENEGRAPH_VERSION}")
    endif()
endif()
