//
// C++ Implementation: Metadata
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufMetadata.h"

#include <sstream>
#include <iomanip>

using namespace UF::Navigation;

void Metadata::Init()
{
  GPXElements.clear();

  GPXElements.insert("name");
  GPXElements.insert("desc");
  GPXElements.insert("author");
  GPXElements.insert("copyright");
  GPXElements.insert("link");
  GPXElements.insert("time");
  GPXElements.insert("keywords");
  GPXElements.insert("bounds");
  GPXElements.insert("extensions");
}

std::string Metadata::Indent(int const & indent)
{
  std::string s;
  for ( int i = 0; i < indent; ++i)
  {
    s += " ";
  }
  return s;
}

std::string Metadata::ToXML(int indent, std::string const & tag)
{
  std::string s;
  if ( this->name.empty() &&
       this->desc.empty() &&
       this->Link::IsEmpty() &&
       this->time.empty() &&
       this->keywords.empty() &&
       !this->Bounds::ValidBounds() &&
       this->Person::IsEmpty() &&
       this->Copyright::IsEmpty()
    )
  {
    // No metadata.
    return s;
  }
  std::string idnt = this->Indent(indent);
  std::ostringstream os;
  // Position information
  os << idnt << "<" << tag <<">\n";
  indent += 2;
  idnt = this->Indent(indent);
  if ( !this->name.empty() )
  {
    os << idnt << "<name> " << this->name << " </name>\n";
  }
  if ( !this->desc.empty() )
  {
    os << idnt << "<desc> " << this->desc << idnt << "</desc>\n";
  }
  if ( !this->Person::IsEmpty() )
  {
    os << this->Person::ToXML(indent,"author");
  }
  if ( !this->Copyright::IsEmpty() )
  {
    os << this->Copyright::ToXML(indent);
  }
  if ( !this->Link::IsEmpty() )
  {
    os << this->Link::ToXML(indent);
  }
  if ( !this->time.empty() )
  {
    os << idnt << "<time>" << this->time << "</time>\n";
  }
  if ( !this->keywords.empty() )
  {
    os << idnt << "<keywords> " << this->keywords << " </keywords>\n";
  }
  if ( this->Bounds::ValidBounds() )
  {
    os << this->Bounds::ToXML(indent);
  }
  indent -= 2;
  idnt = this->Indent(indent);
  os << idnt << "</" << tag << ">\n";
  s = os.str();

  return s;
}
