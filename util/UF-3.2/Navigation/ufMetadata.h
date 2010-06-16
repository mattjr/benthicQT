//
// C++ Interface: Metadata
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NAVIGATION_METADATA_H
#define NAVIGATION_METADATA_H

#include "ufWin32Header.h"
#include "ufBounds.h"
#include "ufPerson.h"
#include "ufCopyright.h"

#include <cmath>

#include <set>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

namespace UF {
//! Classes for navigation.
namespace Navigation {

//! Metadata represents the metadata information found in the gpx preamble. 
/*!
    See: http://www.topografix.com/GPX/1/1/#type_metadataType 
    There is a possibility that not all the variables in this class are valid.
    So we set doubles to pINFd and floats to pINFf.
    Since we use strings we can easily determine if these are empty.
    When outputting we should ensure only valid values are output, by comparing
    against pINFd and pINFf and testing strings.
 
 */
class UF_NAV_EXPORT Metadata 
  : public Person 
  , virtual public Link
  , virtual public Copyright
  , public Bounds
{
public:
  
  typedef std::set<std::string> TMetadataGPXElements;

  Metadata()
  {
    Init();
    Person::Init();
    Link::Init();
    Copyright::Init();
  }

  ~Metadata(){};

  //! Initialise the variables.
  void Init();

  //! Get the name.
  /*!
      @return name.
  */
  std::string GetName() const
  {
    return this->name;
  }

  //! Get the description.
  /*!
      @return desc.
  */
  std::string GetDescription() const
  {
    return this->desc;
  }

  //! Get the time.
  /*!
      @return time.
  */
  std::string GetTime() const
  {
    return this->time;
  }

  //! Get the keywords.
  /*!
      @return keywords.
  */
  std::string GetKeywords() const
  {
    return this->keywords;
  }

  //! Set the name.
  /*!
      @param name - the name.
  */
  void SetName(std::string const & name)
  {
    this->name = name;
  }

  //! Set the description.
  /*!
      @param description.
  */
  void SetDescription(std::string const & desc)
  {
    this->desc = desc;
  }

  //! Set the time.
  /*!
      @param time.
  */
  void SetTime(std::string const & time)
  {
    this->time = time;
  }

  //! Set the keywords.
  /*!
      @param keywords.
  */
  void SetKeywords(std::string const & keywords)
  {
    this->keywords = keywords;
  }

  //! Return an XML representation of the metadata.
  /*!
      @param intent - indent.
      @param tag - the html tag for the metadata.
      @return The waypoint.
  */
  std::string ToXML(int indent = 2, std::string const & tag = "metadata");

  //! Get the GPXElements.
  /*!
      @return the address of GPXElements.
  */
  TMetadataGPXElements * GetGPXElements() { return &this->GPXElements; }

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
    if ( key == "time" )
    {
      SetTime(value);
      return;
    }
    if ( key == "keywords" )
    {
      SetKeywords(value);
      return;
    }
    if ( key == "desc" )
    {
      SetDescription(value);
      return;
    }
  }

  //! Get the author.
  /*!
      @return the author
   */
  Person GetAuthor() const
  {
    Person a;
    a.SetName(this->Person::GetName());
    a.SetEmail(this->Person::GetEmail());
    a.SetLink(this->Person::GetLink());
    return a;
  }

  //! Set the author.
  /*!
      @param the author
   */
  void SetAuthor(Person const & a)
  {
    this->Person::SetName(a.GetName());
    this->Person::SetEmail(a.GetEmail());
    this->Person::SetLink(a.GetLink());
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

  //! Get the copyright.
  /*!
      @return the copyright
   */
  Copyright GetCopyright() const
  {
    Copyright c;
    c.SetAuthor(this->Copyright::GetAuthor());
    c.SetYear(this->Copyright::GetYear());
    c.SetLicense(this->Copyright::GetLicense());
    return c;
  }

  //! Set the copyright.
  /*!
      @param the copyright
   */
  void SetCopyright(Copyright const & c)
  {
    this->Copyright::SetAuthor(c.GetAuthor());
    this->Copyright::SetYear(c.GetYear());
    this->Copyright::SetLicense(c.GetLicense());
  }

  //! Get the bounds.
  /*!
      @return the bounds
   */
  Bounds GetBounds() const
  {
    Bounds b;
    b.SetMinimumLatitude(this->Bounds::GetMinimumLatitude());
    b.SetMinimumLongitude(this->Bounds::GetMinimumLongitude());
    b.SetMaximumLatitude(this->Bounds::GetMaximumLatitude());
    b.SetMaximumLongitude(this->Bounds::GetMaximumLongitude());
    return b;
  }

  //! Set the bounds.
  /*!
      @param the bounds
   */
  void SetBounds(Bounds const & b)
  {
    this->Bounds::SetMinimumLatitude(b.GetMinimumLatitude());
    this->Bounds::SetMinimumLongitude(b.GetMinimumLongitude());
    this->Bounds::SetMaximumLatitude(b.GetMaximumLatitude());
    this->Bounds::SetMaximumLongitude(b.GetMaximumLongitude());
  }

protected:
  TMetadataGPXElements GPXElements;

  //! Name.
  std::string name;
  //! A text description, holds additional information intended for the user.
  std::string desc;
  //std::string link;
  //! Creation/modification timestamp for element. Date and time in are in Univeral Coordinated Time (UTC), not local time! Conforms to ISO 8601 specification for date/time representation. Fractional seconds are allowed for millisecond timing.
  std::string time;
  std::string keywords;

private:
  //! Indent a string by this amount.
  /*!
      @return A series of spaces corresponding to the indent.
  */
  std::string Indent(int const & indent);

};

} // Namespace Navigation.
} // Namespace UF.

#endif // NAVIGATION_METADATA_H
