//
// C++ Interface: Email
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Email: See COPYING file that comes with this distribution
//
//
#ifndef NAVIGATION_EMAIL_H
#define NAVIGATION_EMAIL_H

#include "ufWin32Header.h"

#include <cmath>

#include <set>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

namespace UF {
//! Classes for navigation.
namespace Navigation {

//! Email address, this is split into id and domain. 
/*!
    See: http://www.topografix.com/GPX/1/1/#type_copyrightType 
    There is a possibility that not all the variables in this class are valid.
    So we set doubles to pINFd and floats to pINFf.
    Since we use strings we can easily determine if these are empty.
    When outputting we should ensure only valid values are output, by comparing
    against pINFd and pINFf and testing strings.
 
 */
class UF_NAV_EXPORT Email
{
public:

  typedef std::set<std::string> TEmailGPXAttributes;
  typedef std::set<std::string> TEmailGPXElements;

  Email()
  {
    Init();
  }

  ~Email(){};

  //! Initialise the variables.
  void Init();

  /*!
      @return true if the email address is empty.
   */
  bool IsEmpty()
  {
    return this->id.empty() || domain.empty();
  }

  //! Get the email address.
  /*!
      @return The email address
  */
  std::string GetEmail() const
  {
    return this->id + "@" + this->domain;
  }

  //! Set the email address.
  /*!
      @param email - the email address.
  */
  void SetEmail(std::string const & email)
  {
    this->id.clear();
    this->domain.clear();
    if ( email.find('@') != std::string::npos )
    {
      this->id = email.substr(0,email.find('@'));
      this->domain = email.substr(email.find('@')+1,email.size()-1);
    }
  }

  //! Return an XML representation of the email address.
  /*!
      @param intent - indent.
      @param tag - the html tag for the email address.
      @return The waypoint.
  */
  std::string ToXML(int indent = 2, std::string const & tag = "email");

  //! Get the GPXAttributes.
  /*!
      @return the address of GPXAttributes.
  */
  TEmailGPXAttributes * GetGPXAttributes() { return &this->GPXAttributes; }

  //! Set an attribute.
  /*!
      @param key - the attribute.
      @param value - its value.
   */
  void SetAttribute(std::string const & key, std::string const & value)
  {
    if ( GPXAttributes.find(key) == GPXAttributes.end() )
    {
      // Not a valid key.
      return;
    }

    if ( key == "id" )
    {
      id = value;
      return;
    }
    if ( key == "domain" )
    {
      domain = value;
      return;
    }
  }

protected:
  TEmailGPXAttributes GPXAttributes;

  std::string id;
  std::string domain;

private:
  //! Indent a string by this amount.
  /*!
      @return A series of spaces corresponding to the indent.
  */
  std::string Indent(int const & indent);


};

} // Namespace Navigation.
} // Namespace UF.

#endif // NAVIGATION_EMAIL_H
