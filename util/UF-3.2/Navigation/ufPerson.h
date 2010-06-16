//
// C++ Interface: Person
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NAVIGATION_PERSON_H
#define NAVIGATION_PERSON_H

#include "ufWin32Header.h"
#include "ufEmail.h"
#include "ufLink.h"

#include <cmath>
#include <set>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

namespace UF {
//! Classes for navigation.
namespace Navigation {

//! Person represents a person or organisation 
/*!
    See: http://www.topografix.com/GPX/1/1/#type_personType 
    There is a possibility that not all the variables in this class are valid.
    So we set doubles to pINFd and floats to pINFf.
    Since we use strings we can easily determine if these are empty.
    When outputting we should ensure only valid values are output, by comparing
    against pINFd and pINFf and testing strings.
 
 */
class UF_NAV_EXPORT Person 
  : public Email 
  , virtual public Link
{
public:
  typedef std::set<std::string> TPersonGPXElements;

  Person()
  {
    Init();
    Email::Init();
    Link::Init();
  }

  ~Person(){};

  //! Initialise the variables.
  void Init();

  /*!
      @return true if there is no data.
   */
  bool IsEmpty()
  {
    return this->name.empty() 
      && this->Email::IsEmpty()
      && this->Link::IsEmpty();
  }

  //! Get the name.
  /*!
      @return The name.
  */
  std::string GetName() const
  {
    return this->name;
  }

  //! Set the name.
  /*!
      @param name - the name.
  */
  void SetName(std::string const & name)
  {
    this->name = name;
  }

  //! Get the link.
  /*!
      @return the link
   */
  Link GetLink() const
  {
    Link l;
    l.SetHref(this->Link::GetHref());
    l.SetText(this->Link::GetText());
    l.SetType(this->Link::GetType());
    return l;
  }

  //! Set the link.
  /*!
      @param the link
   */
  void SetLink(Link const & l)
  {
    this->Link::SetHref(l.GetHref());
    this->Link::SetText(l.GetText());
    this->Link::SetType(l.GetType());
  }

  //! Return an XML representation of the person.
  /*!
      @param intent - indent.
      @param tag - the html tag for the person.
      @return The waypoint.
  */
  std::string ToXML(int indent = 2, std::string const & tag = "person");

  //! Get the GPXElements.
  /*!
      @return the address of GPXElements.
  */
  TPersonGPXElements * GetGPXElements() { return &this->GPXElements; }

  //! Set an element.
  /*!
      Elements not set here are processed in the base classes.

      @param key - the attribute.
      @param value - its value.
   */
  template <typename T>
  void SetElement(std::string const & key, T const & value)
  {
    if ( GPXElements.find(key) == GPXElements.end() )
    {
      // Not a valid key.
      return;
    }

    if ( key == "name" )
    {
      SetName(value);
      return;
    }
  }

protected:
  TPersonGPXElements GPXElements;

  std::string name;

private:
  //! Indent a string by this amount.
  /*!
      @return A series of spaces corresponding to the indent.
  */
  std::string Indent(int const & indent);


};

} // Namespace Navigation.
} // Namespace UF.

#endif // NAVIGATION_PERSON_H
