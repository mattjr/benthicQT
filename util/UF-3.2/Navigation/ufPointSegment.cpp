//
// C++ Implementation: PointSegment
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufPointSegment.h"

using namespace UF::Navigation;

void PointSegment::Init()
{
  GPXElements.clear();

  GPXElements.insert("pt");

  segment.clear();
}

std::string PointSegment::Indent(int const & indent)
{
  std::string s;
  for ( int i = 0; i < indent; ++i)
  {
    s += " ";
  }
  return s;
}

std::string PointSegment::ToXML(int indent)
{
  std::string s;
  if ( !this->segment.empty() )
  {
    std::string idnt = this->Indent(indent);
    std::ostringstream os;
    os << idnt << "<ptseg>\n";
    indent += 2;
    idnt = this->Indent(indent);
    for ( TPointSegmentCIt p = this->segment.begin(); p != this->segment.end(); ++p)
    {
      if ( p->GetLatitude() == IEEEConstants::pINFd || p->GetLongitude() == IEEEConstants::pINFd )
      {
        continue;
      }
      os << idnt << "<pt lat=\"" << std::fixed << std::setprecision(6) << p->GetLatitude() << "\" lon=\"" << p->GetLongitude() << "\">\n";
      indent += 2;
      idnt = this->Indent(indent);
      if ( p->GetElevation() != IEEEConstants::pINFd )
      {
        os << idnt << "<ele>" << std::fixed << std::setprecision(2) << p->GetElevation() << "</ele>\n";
      }
      if ( !p->GetTime().empty() )
      {
        os << idnt << "<time>" << p->GetTime() << "</time>\n";
      }
      indent -= 2;
      idnt = this->Indent(indent);
      os << idnt << "</pt>\n";
    }
    indent -= 2;
    idnt = this->Indent(indent);
    os << idnt << "</ptseg>\n";
    s = os.str();
  }
  return s;
}

void PointSegment::GetBounds(GeographicPoint & se, GeographicPoint & nw)
{
  for ( TPointSegmentIt p = this->segment.begin(); p != this->segment.end(); ++p)
  {
    if ( p->GetLatitude() == IEEEConstants::pINFd || p->GetLongitude() == IEEEConstants::pINFd )
    {
      continue;
    }

    GeographicPoint tmp = *p;
    tmp.Normalise();
    if ( p == this->segment.begin() )
    {
      se = tmp;
      nw = se;
      continue;
    }
    se.Minimum(tmp);
    nw.Maximum(tmp);
  }
}

