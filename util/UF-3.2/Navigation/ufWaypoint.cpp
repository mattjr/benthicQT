//
// C++ Implementation: Waypoint
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufWaypoint.h"

#include <sstream>
#include <iomanip>

using namespace UF::Navigation;

void Waypoint::Init()
{
  this->magvar = IEEEConstants::pINFd;
  this->geoidheight = IEEEConstants::pINFd;
  sat = (unsigned int)(-1);
  hdop = IEEEConstants::pINFd;
  vdop = IEEEConstants::pINFd;
  pdop = IEEEConstants::pINFd;
  ageofdgpsdata = IEEEConstants::pINFd;
  dgpsid = (unsigned int)(-1);

  GPXAttributes.clear();
  GPXElements.clear();

  GPXAttributes.insert("lat");
  GPXAttributes.insert("lon");

  GPXElements.insert("ele");
  GPXElements.insert("time");
  GPXElements.insert("magvar");
  GPXElements.insert("geoidheight");
  GPXElements.insert("name");
  GPXElements.insert("cmt");
  GPXElements.insert("desc");
  GPXElements.insert("src");
  GPXElements.insert("link");
  GPXElements.insert("sym");
  GPXElements.insert("type");
  GPXElements.insert("fix");
  GPXElements.insert("sat");
  GPXElements.insert("hdop");
  GPXElements.insert("vdop");
  GPXElements.insert("pdop");
  GPXElements.insert("ageofgpsdata");
  GPXElements.insert("dgpsid");
  GPXElements.insert("extensions");
}

std::string Waypoint::Indent(int const & indent)
{
  std::string s;
  for ( int i = 0; i < indent; ++i)
  {
    s += " ";
  }
  return s;
}

std::string Waypoint::ToXML(int indent, std::string const & tag)
{
  std::string s;
  if ( this->GeographicPoint::IsEmpty() )
  {
    // No lat/lon.
    return s;
  }
  std::string idnt = this->Indent(indent);
  std::ostringstream os;
  // Position information
  os << idnt << "<" << tag <<" lat=\"" << std::fixed << std::setprecision(6) << this->lat << "\" lon=\"" << this->lon << "\">\n";
  indent += 2;
  idnt = this->Indent(indent);
  if ( this->ele != IEEEConstants::pINFd )
  {
    os << idnt << "<ele>" << std::fixed << std::setprecision(2) << this->ele << "</ele>\n";
  }
  if ( !this->time.empty() )
  {
    os << idnt << "<time>" << this->time << "</time>\n";
  }
  if ( this->magvar != IEEEConstants::pINFd )
  {
    os << idnt << "<magvar>" << std::fixed << std::setprecision(1) << this->magvar << "</magvar>\n";
  }
  if ( this->geoidheight != IEEEConstants::pINFd )
  {
    os << idnt << "<geoidheight>" << std::fixed << std::setprecision(2) << this->geoidheight << "</geoidheight>\n";
  }
  // Description information
  if ( !this->name.empty() )
  {
    os << idnt << "<name>" << this->name << "</name>\n";
  }
  if ( !this->comment.empty() )
  {
    os << idnt << "<cmt>" << this->comment << "</cmt>\n";
  }
  if ( !this->desc.empty() )
  {
    os << idnt << "<desc>" << this->desc << "</desc>\n";
  }
  if ( !this->src.empty() )
  {
    os << idnt << "<src>" << this->src << "</src>\n";
  }
  if ( !this->Link::IsEmpty() )
  {
    os << this->Link::ToXML(indent);
  }
  if ( !this->sym.empty() )
  {
    os << idnt << "<sym>" << this->sym << "</sym>\n";
  }
  if ( !this->type.empty() )
  {
    os << idnt << "<type>" << this->type << "</type>\n";
  }
  // Accuracy information
  if ( !this->fix.empty() )
  {
    os << idnt << "<fix>" << this->fix << "</fix>\n";
  }
  if ( this->sat != (unsigned int)(-1) )
  {
    os << idnt << "<sat>" << this->sat << "</sat>\n";
  }
  if ( this->hdop != IEEEConstants::pINFd )
  {
    os << idnt << "<hdop>" << std::fixed << std::setprecision(2) << this->hdop << "</hdop>\n";
  }
  if ( this->vdop != IEEEConstants::pINFd )
  {
    os << idnt << "<vdop>" << std::fixed << std::setprecision(2) << this->vdop << "</vdop>\n";
  }
  if ( this->pdop != IEEEConstants::pINFd )
  {
    os << idnt << "<pdop>" << std::fixed << std::setprecision(2) << this->pdop << "</pdop>\n";
  }
  if ( this->ageofdgpsdata != IEEEConstants::pINFd )
  {
    os << idnt << "<ageofdgpsdata>" << std::fixed << std::setprecision(2) << this->ageofdgpsdata << "</ageofdgpsdata>\n";
  }
  if ( this->dgpsid != (unsigned int)(-1) )
  {
    os << idnt << "<dgpsid>" << this->dgpsid << "</dgpsid>\n";
  }
  indent -= 2;
  idnt = this->Indent(indent);
  os << idnt << "</" << tag << ">";
  s = os.str();

  return s;
}
