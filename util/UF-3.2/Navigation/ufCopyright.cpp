//
// C++ Implementation: Copyright
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufCopyright.h"

#include <sstream>
#include <iomanip>

using namespace UF::Navigation;

void Copyright::Init()
{
  GPXAttributes.clear();
  GPXElements.clear();

  GPXAttributes.insert("author");

  GPXElements.insert("year");
  GPXElements.insert("license"); 
}

std::string Copyright::Indent(int const & indent)
{
  std::string s;
  for ( int i = 0; i < indent; ++i)
  {
    s += " ";
  }
  return s;
}

std::string Copyright::ToXML(int indent, std::string const & tag)
{
  std::string s;
  if ( this->author.empty() &&
       this->year.empty() &&
       this->license.empty() )
  {
    return s;
  }

  std::string idnt = this->Indent(indent);
  std::ostringstream os;
  // Position information
  os << idnt << "<" << tag <<" ";
  indent += 2;
  idnt = this->Indent(indent);
  if ( !this->author.empty() )
  {
    os << "author=\"" << this->author << "\" >\n";
  }
  if ( !this->year.empty() )
  {
    os << idnt << "<year> " << this->year << " </year>\n";
  }
  if ( !this->license.empty() )
  {
    os << idnt << "<license> " << this->license << " </license>\n";
  }
  indent -= 2;
  idnt = this->Indent(indent);
  os << idnt << "</" << tag << ">\n";
  s = os.str();

  return s;
}

