//
// C++ Implementation: Person
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufPerson.h"

#include <sstream>
#include <iomanip>

using namespace UF::Navigation;

void Person::Init()
{
  GPXElements.clear();

  GPXElements.insert("name");
  GPXElements.insert("email");
  GPXElements.insert("link"); 
}

std::string Person::Indent(int const & indent)
{
  std::string s;
  for ( int i = 0; i < indent; ++i)
  {
    s += " ";
  }
  return s;
}

std::string Person::ToXML(int indent, std::string const & tag)
{
  std::string s;
  if ( this->name.empty() &&
       this->Email::IsEmpty() &&
       this->Link::IsEmpty() )
  {
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
  if ( !this->Email::IsEmpty() )
  {
    os << this->Email::ToXML(indent);
  }
  if ( !this->Link::IsEmpty() )
  {
    os << this->Link::ToXML(indent);
  }
  indent -= 2;
  idnt = this->Indent(indent);
  os << idnt << "</" << tag << ">\n";
  s = os.str();

  return s;
}
