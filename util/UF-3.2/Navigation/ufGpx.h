//
// C++ Interface: Gpx
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NAVIGATION_GPX_H
#define NAVIGATION_GPX_H

#include "ufWin32Header.h"

#include <set>
#include <string>

namespace UF {
//! Classes for navigation.
namespace Navigation {

//! Gpx is a store for some of the variables used in the gpx preamble. 
/*!
    See: http://www.topografix.com/GPX/1/1/#type_gpxType 
    There is a possibility that not all the variables in this class are valid.
    So we set doubles to pINFd and floats to pINFf.
    Since we use strings we can easily determine if these are empty.
    When outputting we should ensure only valid values are output, by comparing
    against pINFd and pINFf and testing strings.
 
 */
class UF_NAV_EXPORT Gpx
{
public:

  typedef std::set<std::string> TGpxGPXAttributes;
  typedef std::set<std::string> TGpxGPXElements;

  Gpx()
  {
    Init();
  }

  ~Gpx(){};

  //! Initialise the variables.
  void Init();

  //! Get the version.
  /*!
      @return The version.
  */
  std::string GetVersion() const
  {
    return this->version;
  }

  //! Get the creator.
  /*!
      @return The creator.
  */
  std::string GetCreator() const
  {
    return this->creator;
  }

  //! Set the version.
  /*!
      @param version - the version.
  */
  void SetVersion(std::string const & version)
  {
    this->version = version;
  }

  //! Set creator.
  /*!
      @param creator - the creator.
  */
  void SetCreator(std::string const & creator)
  {
    this->creator = creator;
  }

  //! Return an XML representation of the these parts of the gpx type.
  /*!
      Note: This is not bracketed by <gpx> ... </gpx>
      @param intent - indent.
      @return The waypoint.
  */
  std::string ToXML(int indent = 2);


  //! Get the GPXAttributes.
  /*!
      @return the address of GPXAttributes.
  */
  TGpxGPXAttributes * GetGPXAttributes() { return &this->GPXAttributes; }

  //! Get the GPXElements.
  /*!
      @return the address of GPXElements.
  */
  TGpxGPXElements * GetGPXElements() { return &this->GPXElements; }

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

    if ( key == "version" )
    {
      SetVersion(value);
      return;
    }
    if ( key == "creator" )
    {
      SetCreator(value);
      return;
    }
  }

  // Elements are processed in the base classes.


protected:
  TGpxGPXAttributes GPXAttributes;
  TGpxGPXElements GPXElements;

  //! The version of the GPX used.
  std::string version;
  //! Creator.
  std::string creator;

private:
  //! Indent a string by this amount.
  /*!
      @return A series of spaces corresponding to the indent.
  */
  std::string Indent(int const & indent);


};

} // Namespace Navigation.
} // Namespace UF.

#endif // NAVIGATION_GPX_H
