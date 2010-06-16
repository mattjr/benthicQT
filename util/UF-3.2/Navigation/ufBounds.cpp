//
// C++ Implementation: Bounds
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufBounds.h"

#include <sstream>
#include <iomanip>

using namespace UF::Navigation;

void Bounds::Init()
{
  minlat = IEEEConstants::pINFd;
  minlon = IEEEConstants::pINFd;
  maxlat = IEEEConstants::pINFd;
  maxlon = IEEEConstants::pINFd;

  GPXAttributes.clear();

  GPXAttributes.insert("minlat");
  GPXAttributes.insert("minlon");
  GPXAttributes.insert("maxlat");
  GPXAttributes.insert("maxlon");
  
}

std::string Bounds::Indent(int const & indent)
{
  std::string s;
  for ( int i = 0; i < indent; ++i)
  {
    s += " ";
  }
  return s;
}

std::string Bounds::ToXML(int indent, std::string const & tag)
{
  std::string s;
  if ( !ValidBounds() )
  {
    // No data.
    return s;
  }
  std::string idnt = this->Indent(indent);
  std::ostringstream os;
  // Position information
  os << idnt << "<" << tag << std::fixed << std::setprecision(6) 
    <<" minlat=\"" << this->minlat << "\" minlon=\"" << this->minlon << "\" "
    <<" maxlat=\"" << this->maxlat << "\" maxlon=\"" << this->maxlon << "\" ";
  os << " />\n";
  s = os.str();

  return s;
}

