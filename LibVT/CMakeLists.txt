SET(SOURCES 
LibVT.cpp
LibVT_Cache.cpp
LibVT_ImageDecompression.cpp
LibVT_OpenCL.cpp
LibVT_PageLoadingThread.cpp
LibVT_PageTable.cpp
LibVT_Readback.cpp
LibVT_Utilities.cpp
)

SET(SHADERS readback.frag
readback.vert
renderVT.frag
renderVT.vert )

find_program (SED_TOOL NAMES sed)

FIND_PROGRAM(XXD_EXECUTABLE xxd)
                # To avoid the dependency, simply compile it if not provided
                if (NOT XXD_EXECUTABLE)
                   SET_SOURCE_FILES_PROPERTIES(helper/xxd.c PROPERTIES COMPILE_DEFINITIONS UNIX)
                   ADD_EXECUTABLE(xxd helper/xxd.c)
                   SET(XXD_EXECUTABLE xxd)
                endif (NOT XXD_EXECUTABLE)

foreach(FILENAME ${SHADERS})
  set(separation_inline_src "${LIBRARY_OUTPUT_PATH}/${FILENAME}.c")

  add_custom_command(
    OUTPUT "${separation_inline_src}"
    COMMAND ${XXD_EXECUTABLE} -i "${FILENAME}" "${separation_inline_src}"
    COMMAND ${SED_TOOL} -i 's/}\\;/,0x00}\\;/' '${separation_inline_src}'
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/LibVT/"
    DEPENDS "${PROJECT_SOURCE_DIR}/LibVT/${FILENAME}"
    MAIN_DEPENDENCY "${PROJECT_SOURCE_DIR}/LibVT/${FILENAME}"
    COMMENT "Inlining separation kernel source")


     list(APPEND kernel_srcs ${separation_inline_src} )
endforeach(FILENAME)

ADD_LIBRARY(libvt ${SOURCES}  ${kernel_srcs})


