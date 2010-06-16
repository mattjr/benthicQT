//
// C++ Interface: GeographicPoint
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NAVIGATION_GEOGRAPHIC_POINT_H
#define NAVIGATION_GEOGRAPHIC_POINT_H

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

//! GeographicPoint represents a geographic point with optional elevation and time. 
/*!
    See: http://www.topografix.com/GPX/1/1/#type_ptType 
    So we set doubles to pINFd and floats to pINFf.
    Since we use strings we can easily determine if these are empty.
    When outputting we should ensure only valid values are output, by comparing
    against pINFd and pINFf and testing strings.
 
 */
class UF_NAV_EXPORT GeographicPoint
{
public:

  typedef std::set<std::string> TGeographicPointGPXAttributes;
  typedef std::set<std::string> TGeographicPointGPXElements;

  GeographicPoint()
  {
    // Because these values may be missing we use +INF to signify this.
    Init();
  }


  ~GeographicPoint(){};

  //! Initialise the variables.
  void Init();

  //! Test to see if the instance of the class is empty.
  /*! 
      @return true if the class is empty i.e. no lat or long value present. 
   */
  bool IsEmpty() const
  {
    return this->lat == IEEEConstants::pINFd && this->lon == IEEEConstants::pINFd;
  }

  //! Get the latitude.
  /*!
      @return The latitude in degrees, North positive.
  */
  double GetLatitude() const
  {
    return this->lat;
  }

  //! Get the longitude.
  /*!
      @return The longitude in degrees, East positive.
  */
  double GetLongitude() const
  {
    return this->lon;
  }

  //! Get the latitude.
  /*!
      @return The latitude in degrees, North positive.
  */
  double GetElevation() const
  {
    return this->ele;
  }

  //! Get the time.
  /*!
      @return The UTC time of the observation, yyyy-mm-ddThh:mm:ssZ.
  */
  std::string GetTime() const
  {
    return this->time;
  }

  //! Set the latitude.
  /*!
      @param lat - Latitude in degrees, North positive.
  */
  void SetLatitude(double const & lat)
  {
    this->lat = lat;
  }

  //! Set the latitude.
  /*!
      @param lat - Latitude in degrees, North positive.
  */
  void SetLatitude(std::string const & lat)
  {
    std::istringstream is;
    is.str(lat);
    is >> this->lat;
  }

  //! Set the longitude.
  /*!
      @param lon - Longitude in degrees, East positive.
  */
  void SetLongitude(double const & lon)
  {
    this->lon = lon;
  }

  //! Set the longitude.
  /*!
      @param lon - Longitude in degrees, East positive.
  */
  void SetLongitude(std::string const & lon)
  {
    std::istringstream is;
    is.str(lon);
    is >> this->lon;
  }

  //! Set the elevation.
  /*!
      @param ele - Elevation in degrees, North positive.
  */
  void SetElevation(double const & ele)
  {
    this->ele = ele;
  }

  //! Set the elevation.
  /*!
      @param ele - Elevation in degrees, North positive.
  */
  void SetElevation(std::string const & ele)
  {
    std::istringstream is;
    is.str(ele);
    is >> this->ele;
  }

  //! Set the time.
  /*!
      @param time - UTC Time of the observation, yyyy-mm-ddThh:mm:ssZ.
  */
  void SetTime(std::string const & time)
  {
    this->time = time;
  }

  //! Input the position.
  /*!
      @param is - the input stream.
      @param p - the object into which the data is streamed.

      The data expected is latitude, longitude, elevation and time.

  */
  friend std::istream & operator>>(std::istream& is, GeographicPoint & p)
  {
    p.Init();
    std::ios::fmtflags flags = is.flags();
    is >> p.lat >> p.lon >> p.ele >> p.time;
    is.flags(flags);
    is.clear();
    return is;
  }

  //! Output the position.
  /*!
      @param os - the output stream.
      @param p - the object from which the data is streamed.

      Output is: latitude longitude elevation time

  */  
  friend std::ostream& operator<<(std::ostream& os, GeographicPoint & p)
  {
    std::ios::fmtflags flags = os.flags();

    if ( p.lat == IEEEConstants::pINFd || p.lon == IEEEConstants::pINFd)
    {
      // We don't have a valid point.
      return os;
    }
    os << std::fixed << std::setprecision(6) << p.lat << " ";
    os << std::fixed << std::setprecision(6) << p.lon << " ";
    if ( p.ele != IEEEConstants::pINFd )
    {
      os << std::fixed << std::setprecision(2) << p.ele << " ";
    }
    if ( p.time.empty() )
    {
      os << p.time;
    }
    os.flags(flags);
    return os;
  }

  //! Return an XML representation of the point.
  /*!
      @param intent - indent.
      @param tag - the html tag for the point.
      @return The point.
  */
  std::string ToXML(int indent = 2, std::string const & tag = "pt");


  //! Set normalise the point.
  /*!
      -90 <= latitude <= 90, -180 < longitude <= 180

      @param pt the point to compare with.
  */
  void Normalise ();

  //! Set this point to be the minimum of this point and pt.
  /*!
      If any of latitude, longitude or elevation are undefined in this point
      they will be replaced by those in pt if they are defined.

      To be precise:
      The southernmost latitude will be kept.
      The westernmost longitude will be kept.
      The minimum elevation will be kept.
      So this is the southwestern bound.

      The difference between the westernmost longitude calculated here and the easternmost logitude
      calculated using the Maximum function will always be less than 180 degrees if the same track is used.
      It is possible that the track defined by a set of points will exceed 180 degrees. This function should
      really only be used for a single track or route. 

      @param pt the point to compare with.
  */
  void Minimum (GeographicPoint const & pt);

  //! Set this point to be the maximum of this point and pt.
  /*!
      If any of latitude, longitude or elevation are undefined in this point
      they will be replaced by those in pt if they are defined.

      To be precise:
      The northernmost latitude will be kept.
      The easternmost longitude will be kept.
      The maximum elevation will be kept.
      So this is the northeastern bound.

      The difference between the easternmost longitude calculated here and the westernmost logitude
      calculated using the Minumum function will always be less than 180 degrees if the same track is used.
      It is possible that the track defined by a set of points will exceed 180 degrees. This function should
      really only be used for a single track or route. 

      @param pt the point to compare with.
  */
  void Maximum (GeographicPoint const & pt);

  //! Get the GPXAttributes.
  /*!
      @return the address of the GPXAttributes.
  */
  TGeographicPointGPXAttributes * GetGPXAttributes() { return &this->GPXAttributes; }

  //! Get the GPXElements.
  /*!
      @return the address of GPXElements.
  */
  TGeographicPointGPXElements * GetGPXElements() { return &this->GPXElements; }

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

    if ( key == "lat" )
    {
      SetLatitude(value);
      return;
    }
    if ( key == "lon" )
    {
      SetLongitude(value);
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

    if ( key == "ele" )
    {
      SetElevation(value);
      return;
    }
    if ( key == "time" )
    {
      SetTime(value);
      return;
    }
  }

protected:
  TGeographicPointGPXAttributes GPXAttributes;
  TGeographicPointGPXElements GPXElements;

  //! The latitude of the point. Decimal degrees, WGS84 datum. North positive.
  double lat;
  //! The longitude of the point. Decimal degrees, WGS84 datum. East positive.
  double lon;
  //! Elevation (in meters) of the point.
  double ele;
  //! Creation/modification timestamp for element. Date and time in are in Univeral Coordinated Time (UTC), not local time! Conforms to ISO 8601 specification for date/time representation. Fractional seconds are allowed for millisecond timing in tracklogs.
  std::string time;

private:
  //! Indent a string by this amount.
  /*!
      @return A series of spaces corresponding to the indent.
  */
  std::string Indent(int const & indent);


};

} // Namespace Navigation.
} // Namespace UF.

#endif // NAVIGATION_GEOGRAPHIC_POINT_H
