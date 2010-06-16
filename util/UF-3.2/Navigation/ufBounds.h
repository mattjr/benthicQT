//
// C++ Interface: Bounds
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NAVIGATION_BOUNDS_H
#define NAVIGATION_BOUNDS_H

#include "ufWin32Header.h"
#include "ufIEEEConstants.h"

#include <cmath>

#include <set>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

namespace UF {
//! Classes for navigation.
namespace Navigation {

//! Bounds represents two lat/lon pairs defining the extent of an element. 
/*!
    See: http://www.topografix.com/GPX/1/1/#type_boundsType 
    There is a possibility that not all the variables in this class are valid.
    So we set doubles to pINFd and floats to pINFf.
    Since we use strings we can easily determine if these are empty.
    When outputting we should ensure only valid values are output, by comparing
    against pINFd and pINFf and testing strings.
 
 */
class UF_NAV_EXPORT Bounds
{
public:

  typedef std::set<std::string> TBoundsGPXAttributes;

  Bounds()
  {
    Init();
  }

  //! Initialise the variables.
  void Init();

  ~Bounds(){};

  //! Test for valid bounds.
  /*!
      @return true if the bounds are valid.
   */
  bool ValidBounds() const
  {
    return minlat != IEEEConstants::pINFd && minlon != IEEEConstants::pINFd
         && maxlat != IEEEConstants::pINFd && maxlon != IEEEConstants::pINFd;
  }

  //! Get the minimum latitude.
  /*!
      @return The minimum latitude.
  */
  double GetMinimumLatitude() const
  {
    return this->minlat;
  }

  //! Get the minimum longitude.
  /*!
      @return The minimum longitude.
  */
  double GetMinimumLongitude() const
  {
    return this->minlon;
  }

  //! Get the maximum latitude.
  /*!
      @return The maximum latitude.
  */
  double GetMaximumLatitude() const
  {
    return this->maxlat;
  }

  //! Get the maximum longitude.
  /*!
      @return The maximum longitude.
  */
  double GetMaximumLongitude() const
  {
    return this->maxlon;
  }

  //! Set the minimum latitude.
  /*!
      @param minlat - the minimum latitude.
  */
  void SetMinimumLatitude(double const & minlat)
  {
    this->minlat = minlat;
  }

  //! Set the minimum longitude.
  /*!
      @param minlon - the minimum longitude.
  */
  void SetMinimumLongitude(double const & minlon)
  {
    this->minlon = minlon;
  }

  //! Set the maximum latitude.
  /*!
      @param minlat - the maximum latitude.
  */
  void SetMaximumLatitude(double const & maxlat)
  {
    this->maxlat = maxlat;
  }

  //! Set the maximum longitude.
  /*!
      @param maxlon - the maximum longitude.
  */
  void SetMaximumLongitude(double const & maxlon)
  {
    this->maxlon = maxlon;
  }


  //! Set the minimum latitude.
  /*!
      @param minlat - the minimum latitude.
  */
  void SetMinimumLatitude(std::string const & minlat)
  {
    std::istringstream is;
    is.str(minlat);
    is >> this->minlat;
  }

  //! Set the minimum longitude.
  /*!
      @param minlon - the minimum longitude.
  */
  void SetMinimumLongitude(std::string const & minlon)
  {
    std::istringstream is;
    is.str(minlon);
    is >> this->minlon;
  }

  //! Set the maximum latitude.
  /*!
      @param minlat - the maximum latitude.
  */
  void SetMaximumLatitude(std::string const & maxlat)
  {
    std::istringstream is;
    is.str(maxlat);
    is >> this->maxlat;
  }

  //! Set the maximum longitude.
  /*!
      @param maxlon - the maximum longitude.
  */
  void SetMaximumLongitude(std::string const & maxlon)
  {
    std::istringstream is;
    is.str(maxlon);
    is >> this->maxlon;
  }


  //! Input the bounds.
  /*!
      @param is - the input stream.
      @param p - the object into which the data is streamed.

      The data expected is min latitude, min longitude, max latitude, max longitude.

  */
  friend std::istream & operator>>(std::istream& is, Bounds & p)
  {
    p.Init();
    std::ios::fmtflags flags = is.flags();
    is >> p.minlat >> p.minlon >> p.maxlat >> p.maxlon;
    is.flags(flags);
    is.clear();
    return is;
  }

  //! Output the bounds.
  /*!
      @param os - the output stream.
      @param p - the object from which the data is streamed.

      Output is: latitude longitude elevation time

  */  
  friend std::ostream& operator<<(std::ostream& os, Bounds & p)
  {
    std::ios::fmtflags flags = os.flags();

    if ( !p.ValidBounds() )
    {
      return os;
    }
    os << std::fixed << std::setprecision(6) << p.minlat << " ";
    os << std::fixed << std::setprecision(6) << p.minlon << " ";
    os << std::fixed << std::setprecision(6) << p.maxlat << " ";
    os << std::fixed << std::setprecision(6) << p.maxlon << " ";
    os.flags(flags);
    return os;
  }

  //! Return an XML representation of the bounds.
  /*!
      @param intent - indent.
      @param tag - the html tag for the bounds.
      @return The waypoint.
  */
  std::string ToXML(int indent = 2, std::string const & tag = "bounds");

  //! Get the GPXAttributes.
  /*!
      @return the address of GPXAttributes.
  */
  TBoundsGPXAttributes * GetGPXAttributes() { return &this->GPXAttributes; }

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

    if ( key == "minlat" )
    {
      SetMinimumLatitude(value);
      return;
    }
    if ( key == "minlon" )
    {
      SetMinimumLongitude(value);
      return;
    }
    if ( key == "maxlat" )
    {
      SetMaximumLatitude(value);
      return;
    }
    if ( key == "maxlon" )
    {
      SetMaximumLongitude(value);
      return;
    }
  }

protected:
  TBoundsGPXAttributes GPXAttributes;

  double minlat;
  double minlon;
  double maxlat;
  double maxlon;

private:
  //! Indent a string by this amount.
  /*!
      @return A series of spaces corresponding to the indent.
  */
  std::string Indent(int const & indent);


};

} // Namespace Navigation.
} // Namespace UF.

#endif // NAVIGATION_BOUNDS_H
