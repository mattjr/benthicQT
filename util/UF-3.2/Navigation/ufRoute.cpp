//
// C++ Implementation: Route
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufRoute.h"

using namespace UF::Navigation;

void Route::Init()
{
  GPXElements.clear();

  GPXElements.insert("name");
  GPXElements.insert("cmt");
  GPXElements.insert("desc");
  GPXElements.insert("src");
  GPXElements.insert("link");
  GPXElements.insert("number");
  GPXElements.insert("type");
  GPXElements.insert("extensions");
  GPXElements.insert("rtept");

  route.clear();
}

std::string Route::Indent(int const & indent)
{
  std::string s;
  for ( int i = 0; i < indent; ++i)
  {
    s += " ";
  }
  return s;
}

std::string Route::ToXML(int indent)
{
  std::string s;
  if ( !this->route.empty() )
  {
    std::string idnt = this->Indent(indent);
    std::ostringstream os;
    os << idnt << "<rte>\n";
    indent += 2;
    idnt = this->Indent(indent);
    if ( !this->name.empty() )
    {
      os << idnt << "<name>" << this->name << "</name>\n";
    }
    if ( !this->comment.empty() )
    {
      os << idnt << "<cmt>" << this->comment << "</cmt>\n";
    }
    if ( !this->description.empty() )
    {
      os << idnt << "<desc>" << this->description << "</desc>\n";
    }
    if ( !this->source.empty() )
    {
      os << idnt << "<src>" << this->source << "</src>\n";
    }
    if ( !this->Link::IsEmpty() )
    {
      os << this->Link::ToXML(indent);
    }
    if ( this->number != (unsigned int) (-1) )
    {
      os << idnt << "<number>" << this->number << "</number>\n";
    }
    if ( !this->type.empty() )
    {
      os << idnt << "<type>" << this->type << "</type>\n";
    }
    for ( TRouteIt p = this->route.begin(); p != this->route.end(); ++p)
    {
      if ( p->Waypoint::IsEmpty() )
      {
        continue;
      }
      os << p->ToXML(indent+2,"rtept") << std::endl;
    }
    indent -= 2;
    os << idnt << "</rte>\n";
    s = os.str();
  }
  return s;
}

void Route::GetBounds(Waypoint & se, Waypoint & nw)
{
  for ( TRouteIt p = this->route.begin(); p != this->route.end(); ++p)
  {
    if ( p->GetLatitude() == IEEEConstants::pINFd || p->GetLongitude() == IEEEConstants::pINFd )
    {
      continue;
    }

    Waypoint tmp = *p;
    tmp.Normalise();
    if ( p == this->route.begin() )
    {
      se = tmp;
      nw = se;
      continue;
    }
    se.Minimum(tmp);
    se.Maximum(tmp);
  }
}

