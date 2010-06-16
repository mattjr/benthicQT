//
// C++ Interface: Copyright
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NAVIGATION_COPYRIGHT_H
#define NAVIGATION_COPYRIGHT_H

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
class UF_NAV_EXPORT Copyright
{
public:

  typedef std::set<std::string> TCopyrightGPXAttributes;
  typedef std::set<std::string> TCopyrightGPXElements;

  Copyright()
  {
    Init();
  }

  ~Copyright(){};

  //! Initialise the variables.
  void Init();

  /*!
      @return true if the copyright is empty.
   */
  bool IsEmpty()
  {
    return this->author.empty();
  }

  //! Get the author.
  /*!
      @return The author.
  */
  std::string GetAuthor() const
  {
    return this->author;
  }

  //! Get the year.
  /*!
      @return The year.
  */
  std::string GetYear() const
  {
    return this->year;
  }

  //! Get the link for the license.
  /*!
      A link to an external resource (Web page, digital photo, video clip, etc) with additional information.
      
      @return link for the license.
  */
  std::string GetLicense() const
  {
    return this->license;
  }

  //! Set the author.
  /*!
      @param author - the author.
  */
  void SetAuthor(std::string const & author)
  {
    this->author = author;
  }

  //! Set the year.
  /*!
      @param year.
  */
  void SetYear(int const & year)
  {
    std::ostringstream os;
    os << year;
    this->year = os.str();
  }

  //! Set the year.
  /*!
      @param year.
  */
  void SetYear(std::string const & year)
  {
    this->year = year;
  }

  //! Set the license.
  /*!
      A link to an external resource (Web page, digital photo, video clip, etc) with additional information.

      This must be a valid URI e.g http://en.wikipedia.org/wiki/URI#Examples_of_URI_references

      @param license.
  */
  void SetLicense(std::string const & license)
  {
    this->license = license;
  }

  //! Return an XML representation of the copyright.
  /*!
      @param intent - indent.
      @param tag - the html tag for the copyright.
      @return The waypoint.
  */
  std::string ToXML(int indent = 2, std::string const & tag = "copyright");

  //! Get the GPXAttributes.
  /*!
      @return the address of GPXAttributes.
  */
  TCopyrightGPXAttributes * GetGPXAttributes() { return &this->GPXAttributes; }

  //! Get the GPXElements.
  /*!
      @return the address of GPXElements.
  */
  TCopyrightGPXElements * GetGPXElements() { return &this->GPXElements; }

  //! Set an attribute.
  /*!
      @param key - the attribute.
      @param value - its value.
   */
  template <typename T>
  void SetAttribute(std::string const & key, T const & value)
  {
    if ( GPXAttributes.find(key) == GPXAttributes.end() )
    {
      // Not a valid key.
      return;
    }

    if ( key == "author" )
    {
      SetAuthor(value);
      return;
    }
  }

  //! Set an element.
  /*!
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

    // This can be int or string.
    if ( key == "year" )
    {
      SetYear(value);
      return;
    }
    if ( key == "license" )
    {
      SetLicense(value);
      return;
    }
  }


protected:
  TCopyrightGPXAttributes GPXAttributes;
  TCopyrightGPXElements GPXElements;

  std::string author;
  std::string year;
  std::string license;

private:
  //! Indent a string by this amount.
  /*!
      @return A series of spaces corresponding to the indent.
  */
  std::string Indent(int const & indent);


};

} // Namespace Navigation.
} // Namespace UF.

#endif // NAVIGATION_COPYRIGHT_H
