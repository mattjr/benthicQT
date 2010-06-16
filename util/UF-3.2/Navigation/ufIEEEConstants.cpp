//
// C++ Implementation: IEEEConstants
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufIEEEConstants.h"

using namespace UF::Navigation;

//const float IEEEConstants::pNANf = *(float*)&IEEEConstants::pNANf_raw;
const float IEEEConstants::pNANf = static_cast<float>(IEEEConstants::pNANf_raw);
const float IEEEConstants::nNANf = static_cast<float>(IEEEConstants::nNANf_raw);
const double IEEEConstants::pNANd = static_cast<double>(IEEEConstants::pNANd_raw);
const double IEEEConstants::nNANd = static_cast<double>(IEEEConstants::nNANd_raw);

const float IEEEConstants::pINFf = static_cast<float>(IEEEConstants::pINFf_raw);
const float IEEEConstants::nINFf = static_cast<float>(IEEEConstants::nINFf_raw);
const double IEEEConstants::pINFd = static_cast<double>(IEEEConstants::pINFd_raw);
const double IEEEConstants::nINFd = static_cast<double>(IEEEConstants::nINFd_raw);
