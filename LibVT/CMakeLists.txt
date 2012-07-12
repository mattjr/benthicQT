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
ADD_LIBRARY(libvt ${SOURCES})
