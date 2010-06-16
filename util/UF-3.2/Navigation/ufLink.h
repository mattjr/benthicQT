//
// C++ Interface: Link
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NAVIGATION_LINK_H
#define NAVIGATION_LINK_H

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

//! Information about the copyright holder and any license governing use of this file. By linking to an appropriate license, you may place your data into the public domain or grant additional usage rights. 
/*!
    See: http://www.topografix.com/GPX/1/1/#type_copyrightType 
    There is a possibility that not all the variables in this class are valid.
    So we set doubles to pINFd and floats to pINFf.
    Since we use strings we can easily determine if these are empty.
    When outputting we should ensure only valid values are output, by comparing
    against pINFd and pINFf and testing strings.
 
 */
class UF_NAV_EXPORT Link
{
public:

  typedef std::set<std::string> TLinkGPXAttributes;
  typedef std::set<std::string> TLinkGPXElements;

  Link()
  {
    Init();
  }

  ~Link(){};

  //! Initialise the variables.
  void Init();

  /*!
      @return true if the copyright is empty.
   */
  bool IsEmpty()
  {
    return this->href.empty();
  }

  //! Get the href.
  /*!
      This must be a valid URI e.g http://en.wikipedia.org/wiki/URI#Examples_of_URI_references

      @return The href.
  */
  std::string GetHref() const
  {
    return this->href;
  }

  //! Get the text.
  /*!
      @return The text.
  */
  std::string GetText() const
  {
    return this->text;
  }

  //! Get the type.
  /*!
      @return type.
  */
  std::string GetType() const
  {
    return this->type;
  }

  //! Set the href.
  /*!
      This must be a valid URI e.g http://en.wikipedia.org/wiki/URI#Examples_of_URI_references

      @param href - the author.
  */
  void SetHref(std::string const & href)
  {
    this->href = href;
  }

  //! Set the text.
  /*!
      @param text.
  */
  void SetText(std::string const & text)
  {
    this->text = text;
  }

  //! Set the type.
  /*!
      @param type.
  */
  void SetType(std::string const & type)
  {
    this->type = type;
  }

  //! Return an XML representation of the link.
  /*!
      @param intent - indent.
      @param tag - the html tag for the link.
      @return The waypoint.
  */
  std::string ToXML(int indent = 2, std::string const & tag = "link");

  //! Get the GPXAttributes.
  /*!
      @return the address of GPXAttributes.
  */
  TLinkGPXAttributes * GetGPXAttributes() { return &this->GPXAttributes; }

  //! Get the GPXElements.
  /*!
      @return the address of GPXElements.
  */
  TLinkGPXElements * GetGPXElements() { return &this->GPXElements; }

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

    if ( key == "href" )
    {
      SetHref(value);
      return;
    }
  }

  //! Set an element.
  /*!
      @param key - the attribute.
      @param value - its value.
   */
  void SetElement(std::string const & key, std::string const & value)
  {
    if ( GPXElements.find(key) == GPXElements.end() )
    {
      // Not a valid key.
      return;
    }

    if ( key == "text" )
    {
      SetText(value);
      return;
    }
    if ( key == "type" )
    {
      SetType(value);
      return;
    }
  }

protected:
  TLinkGPXAttributes GPXAttributes;
  TLinkGPXElements GPXElements;

  std::string href;
  std::string text;
  std::string type;

private:
  //! Indent a string by this amount.
  /*!
      @return A series of spaces corresponding to the indent.
  */
  std::string Indent(int const & indent);


};

} // Namespace Navigation.
} // Namespace UF.

#endif // NAVIGATION_LINK_H
