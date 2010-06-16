//
// C++ Implementation: Link
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufLink.h"

#include <sstream>
#include <iomanip>

using namespace UF::Navigation;

void Link::Init()
{
  GPXAttributes.clear();
  GPXElements.clear();

  GPXAttributes.insert("href");

  GPXElements.insert("text");
  GPXElements.insert("type"); 
}

std::string Link::Indent(int const & indent)
{
  std::string s;
  for ( int i = 0; i < indent; ++i)
  {
    s += " ";
  }
  return s;
}

std::string Link::ToXML(int indent, std::string const & tag)
{
  std::string s;
  if ( this->href.empty() &&
       this->text.empty() &&
       this->type.empty() )
  {
    return s;
  }

  std::string idnt = this->Indent(indent);
  std::ostringstream os;

  os << idnt << "<" << tag <<" ";
  indent += 2;
  idnt = this->Indent(indent);
  if ( !this->href.empty() )
  {
    os << "href=\"" << this->href << "\" > ";
  }
  if ( !this->text.empty() )
  {
    os << "<text> " << this->text << " </text> ";
  }
  if ( !this->type.empty() )
  {
    os << "<type> " << this->type << " </type>";
  }
  os << "</" << tag << ">\n";
  indent -= 2;
  s = os.str();

  return s;
}
