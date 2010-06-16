//
// C++ Interface: IEEEConstants
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NAVIGATION_IEEE_CONSTANTS_H
#define NAVIGATION_IEEE_CONSTANTS_H

#include "ufWin32Header.h"

namespace UF {
//! Classes for navigation.
namespace Navigation {

//! Provide a convenient interface for defining some constants.
struct UF_NAV_EXPORT IEEEConstants
{
  /*
    +NAN float: 7fc00000
    -NAN float: ffc00000
    +NAN double: 7ff8000000000000
    -NAN double: fff8000000000000

    +INF float: 7f800000
    -INF float: ff800000
    +INF double: 7ff0000000000000
    -INF double: fff0000000000000

    Endianness is of no concern here.
  */

  static const unsigned long pNANf_raw = 0x7fc00000;
  static const unsigned long nNANf_raw = 0xffc00000;
  static const unsigned long long pNANd_raw = 0x7ff8000000000000ULL;
  static const unsigned long long nNANd_raw = 0xfff8000000000000ULL;

  static const unsigned long pINFf_raw = 0x7f800000;
  static const unsigned long nINFf_raw = 0xff800000;
  static const unsigned long long pINFd_raw = 0x7ff0000000000000ULL;
  static const unsigned long long nINFd_raw = 0xfff0000000000000ULL;

  static const float pNANf;
  static const float nNANf;
  static const double pNANd;
  static const double nNANd;

  static const float pINFf;
  static const float nINFf;
  static const double pINFd;
  static const double nINFd;
};

} // Namespace Navigation.
} // Namespace UF.

#endif // NAVIGATION_IEEE_CONSTANTS_H

