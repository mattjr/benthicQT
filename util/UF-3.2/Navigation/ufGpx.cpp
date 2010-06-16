//
// C++ Implementation: Gpx
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufGpx.h"

#include <sstream>
#include <iomanip>

using namespace UF::Navigation;

void Gpx::Init()
{
  GPXAttributes.clear();
  GPXElements.clear();

  GPXAttributes.insert("version");
  GPXAttributes.insert("creator");

  GPXElements.insert("metadata");
  GPXElements.insert("wpt");
  GPXElements.insert("rte");
  GPXElements.insert("trk");
  GPXElements.insert("extensions");
}

std::string Gpx::Indent(int const & indent)
{
  std::string s;
  for ( int i = 0; i < indent; ++i)
  {
    s += " ";
  }
  return s;
}

std::string Gpx::ToXML(int indent)
{
  std::string idnt = this->Indent(indent);
  std::ostringstream os;
  // Position information
  os << idnt << "version=\"" << this->version << "\"\n"
     << idnt << "creator=\"" << this->creator << "\"\n";
  indent -= 2;

  return os.str();
}
