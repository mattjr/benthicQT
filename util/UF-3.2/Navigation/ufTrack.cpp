//
// C++ Implementation: Track
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufTrack.h"

using namespace UF::Navigation;

void Track::Init()
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
  GPXElements.insert("trkseg");

  track.clear();
}

std::string Track::Indent(int const & indent)
{
  std::string s;
  for ( int i = 0; i < indent; ++i)
  {
    s += " ";
  }
  return s;
}

std::string Track::ToXML(int indent)
{
  std::string s;
  if ( !this->track.empty() )
  {
    std::string idnt = this->Indent(indent);
    std::ostringstream os;
    os << idnt << "<trk>\n";
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
    for ( TTrackIt p = this->track.begin(); p != this->track.end(); ++p)
    {
      os << p->ToXML(indent);
    }
    indent -= 2;
    idnt = this->Indent(indent);
    os << idnt << "</trk>\n";
    s = os.str();
  }
  return s;
}

void Track::GetBounds(Waypoint & se, Waypoint & nw)
{
  for ( TTrackIt p = this->track.begin(); p != this->track.end(); ++p)
  {
    // Bounds of each segment.
    Waypoint ses;
    Waypoint nws;
    p->GetBounds(ses,nws);
    ses.Normalise();
    nws.Normalise();

    if ( p == this->track.begin() )
    {
      se = ses;
      nw = nws;
      continue;
    }
    se.Minimum(ses);
    nw.Maximum(nws);
  }
}

