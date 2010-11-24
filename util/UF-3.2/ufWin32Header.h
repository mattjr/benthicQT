// The UFWin32Header file captures some system differences between Unix and
// Windows operating systems.

#ifndef __UFWIN32Header_h
#define __UFWIN32Header_h

//#include "ufConfigure.h"

//
// Windows specific stuff------------------------------------------
#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)

#if !defined(__CYGWIN__)
#define snprintf _snprintf
#endif

#endif

#if defined(_WIN32)
  // Include the windows header here only if requested by user code.
# if defined(UF_INCLUDE_WINDOWS_H)
#  include <windows.h>
#endif
#endif

/*
 CMake provides automatic support for building sources in the library
 and not defining it elsewhere. CMake does this by defining a
 "target-name_EXPORTS" macro for sources being placed in a
 shared library "target-name".
*/

#if defined(WIN32) && defined(UF_BUILD_SHARED_LIBS)
 #define UF_EXPORT __declspec( dllexport )

 #if defined(ufAngle_EXPORTS)
  #define UF_ANGLE_EXPORT __declspec( dllexport )
 #else
  #define UF_ANGLE_EXPORT __declspec( dllimport )
 #endif

 #if defined(ufColour_EXPORTS)
  #define UF_COLOUR_EXPORT __declspec( dllexport )
 #else
  #define UF_COLOUR_EXPORT __declspec( dllimport )
 #endif

 #if defined(ufCoversions_EXPORTS)
  #define UF_CONVERSIONS_EXPORT __declspec( dllexport )
 #else
  #define UF_CONVERSIONS_EXPORT __declspec( dllimport )
 #endif

 #if defined(ufCSVParser_EXPORTS)
  #define UF_CSV_PARSER_EXPORT __declspec( dllexport )
 #else
  #define UF_CSV_PARSER_EXPORT __declspec( dllimport )
 #endif

 #if defined(ufDateTime_EXPORTS)
  #define UF_DATE_TIME_EXPORT __declspec( dllexport )
 #else
  #define UF_DATE_TIME_EXPORT __declspec( dllimport )
 #endif

 #if defined(ufDTM_EXPORTS)
  #define UF_DTM_EXPORT __declspec( dllexport )
 #else
  #define UF_DTM_EXPORT __declspec( dllimport )
 #endif

 #if defined(ufFileUtilities_EXPORTS)
  #define UF_FILE_UTILITIES_EXPORT __declspec( dllexport )
 #else
  #define UF_FILE_UTILITIES_EXPORT __declspec( dllimport )
 #endif

 #if defined(ufGeographicConversions_EXPORTS)
  #define UF_GEO_CONV_EXPORT __declspec( dllexport )
 #else
  #define UF_GEO_CONV_EXPORT __declspec( dllimport )
 #endif

 #if defined(ufGPSNavigation_EXPORTS)
  #define UF_GPS_NAV_EXPORT __declspec( dllexport )
 #else
  #define UF_GPS_NAV_EXPORT __declspec( dllimport )
 #endif

 #if defined(ufNavigation_EXPORTS)
  #define UF_NAV_EXPORT __declspec( dllexport )
 #else
  #define UF_NAV_EXPORT __declspec( dllimport )
 #endif

 #if defined(ufGPSParser_EXPORTS)
  #define UF_GPS_PARSER_EXPORT __declspec( dllexport )
 #else
  #define UF_GPS_PARSER_EXPORT __declspec( dllimport )
 #endif

 #if defined(ufLogger_EXPORTS)
  #define UF_LOGGER_EXPORT __declspec( dllexport )
 #else
  #define UF_LOGGER_EXPORT __declspec( dllimport )
 #endif

 #if defined(ufStatistics_EXPORTS)
  #define UF_STATISTICS_EXPORT __declspec( dllexport )
 #else
  #define UF_STATISTICS_EXPORT __declspec( dllimport )
 #endif

 #if defined(ufVTK_EXPORTS)
  #define UF_VTK_EXPORT __declspec( dllexport )
 #else
  #define UF_VTK_EXPORT __declspec( dllimport )
 #endif

#else
 #define UF_EXPORT
 #define UF_ANGLE_EXPORT
 #define UF_COLOUR_EXPORT
 #define UF_CONVERSIONS_EXPORT
 #define UF_CSV_PARSER_EXPORT
 #define UF_DATE_TIME_EXPORT
 #define UF_DTM_EXPORT
 #define UF_FILE_UTILITIES_EXPORT
 #define UF_GEO_CONV_EXPORT
 #define UF_GPS_NAV_EXPORT
 #define UF_NAV_EXPORT
 #define UF_GPS_PARSER_EXPORT
 #define UF_LOGGER_EXPORT
 #define UF_STATISTICS_EXPORT
 #define UF_VTK_EXPORT
#endif

#endif
