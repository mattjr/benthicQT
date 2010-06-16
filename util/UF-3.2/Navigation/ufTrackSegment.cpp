//
// C++ Implementation: TrackSegment
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufTrackSegment.h"

using namespace UF::Navigation;

void TrackSegment::Init()
{
  GPXElements.clear();

  GPXElements.insert("trkpt");
  GPXElements.insert("extensions");

  segment.clear();
}

std::string TrackSegment::Indent(int const & indent)
{
  std::string s;
  for ( int i = 0; i < indent; ++i)
  {
    s += " ";
  }
  return s;
}

std::string TrackSegment::ToXML(int indent)
{
  std::string s;
  if ( !this->segment.empty() )
  {
    std::string idnt = this->Indent(indent);
    std::ostringstream os;
    os << idnt << "<trkseg>\n";
    indent += 2;
    idnt = this->Indent(indent);
    for ( TTrackSegmentCIt p = this->segment.begin(); p != this->segment.end(); ++p)
    {
      if ( p->Waypoint::IsEmpty() )
      {
        continue;
      }
      os << idnt << "<trkpt lat=\"" << std::fixed << std::setprecision(6) << p->GetLatitude() << "\" lon=\"" << p->GetLongitude() << "\">\n";
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
      os << idnt << "</trkpt>\n";
    }
    indent -= 2;
    idnt = this->Indent(indent);
    os << idnt << "</trkseg>\n";
    s = os.str();
  }
  return s;
}

void TrackSegment::GetBounds(Waypoint & se, Waypoint & nw)
{
  for ( TTrackSegmentIt p = this->segment.begin(); p != this->segment.end(); ++p)
  {
    if ( p->GetLatitude() == IEEEConstants::pINFd || p->GetLongitude() == IEEEConstants::pINFd )
    {
      continue;
    }

    Waypoint tmp = *p;
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

