//
// C++ Implementation: Email
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Email: See COPYING file that comes with this distribution
//
//
#include "ufEmail.h"

#include <sstream>
#include <iomanip>

using namespace UF::Navigation;

void Email::Init()
{
  GPXAttributes.clear();

  GPXAttributes.insert("id");
  GPXAttributes.insert("domain");
}

std::string Email::Indent(int const & indent)
{
  std::string s;
  for ( int i = 0; i < indent; ++i)
  {
    s += " ";
  }
  return s;
}

std::string Email::ToXML(int indent, std::string const & tag)
{
  std::string s;
  if ( this->id.empty() ||
       this->domain.empty() )
  {
    return s;
  }

  std::string idnt = this->Indent(indent);
  std::ostringstream os;
  // Position information
  os << idnt << "<" << tag <<" ";
  indent += 2;
  idnt = this->Indent(indent);
  os << "id=\"" << this->id << "\" ";
  os << "domain=\"" << this->domain << "\" /> \n";
  indent -= 2;
  s = os.str();

  return s;
}
