//
// C++ Interface: Waypoint
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NAVIGATION_WAYPOINT_H
#define NAVIGATION_WAYPOINT_H

#include "ufWin32Header.h"

#include "ufGeographicPoint.h"
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

//! Waypoint represents a waypoint, point of interest, or named feature on a map. 
/*!
    See: http://www.topografix.com/GPX/1/1/#type_wptType 
    There is a possibility that not all the variables in this class are valid.
    So we set doubles to pINFd and floats to pINFf.
    Since we use strings we can easily determine if these are empty.
    When outputting we should ensure only valid values are output, by comparing
    against pINFd and pINFf and testing strings.
 
 */
class UF_NAV_EXPORT Waypoint
  : public GeographicPoint
  , public Link
{
public:

  using GeographicPoint::IsEmpty;
  using Link::IsEmpty;

  typedef std::set<std::string> TWaypointGPXAttributes;
  typedef std::set<std::string> TWaypointGPXElements;

  Waypoint()
  {
    // Because these values may be missing we use +INF to signify this.
    Init();
    GeographicPoint::Init();
    Link::Init();
  }

  //! Initialise the variables.
  void Init();

  ~Waypoint(){};

  //! A waypoint is empty if there is no position.
  bool IsEmpty()
  {
    return this->GeographicPoint::IsEmpty();
  }

  //! Get the magnetic variation.
  /*!
      @return The magnetic variation.
  */
  double GetMagneticVariation() const
  {
    return this->magvar;
  }

  //! Get the geoid height.
  /*!
      @return The geoid height.
  */
  double GetGeoidHeight() const
  {
    return this->geoidheight;
  }

  //! Get the name.
  /*!
      @return The GPS name of the waypoint.
  */
  std::string GetName() const
  {
    return this->name;
  }

  //! Get the waypoint comment.
  /*!
      @return comment for the waypoint.
  */
  std::string GetComment() const
  {
    return this->comment;
  }

  //! Get the waypoint description.
  /*!
      @return description for the waypoint.
  */
  std::string GetDescription() const
  {
    return this->desc;
  }

  //! Get the source of the waypoint.
  /*!
      @return source of the waypoint.
  */
  std::string GetSource() const
  {
    return this->src;
  }

  //! Get the symbol name.
  /*!
      @return Text of GPS symbol name. For interchange with other programs, use the exact spelling of the symbol as displayed on the GPS. If the GPS abbreviates words, spell them out.
  */
  std::string GetSym() const
  {
    return this->sym;
  }

  //! Get the waypoint type.
  /*!
      @return waypoint type.
  */
  std::string GetType() const
  {
    return this->type;
  }

  //! Get the fix type.
  /*!
      The value should be one of: {'none'|'2d'|'3d'|'dgps'|'pps'}.
      none means GPS had no fix. To signify "the fix info is unknown", 
      leave out fixType entirely. pps = military signal used.

      @return fix, the fix type.
  */
  std::string GetFix() const
  {
    return this->fix;
  }

  //! Get the number of satellites.
  /*!
      @return sat, the number of satellites.
  */
  unsigned int GetNumberOfSatellites() const
  {
    return this->sat;
  }

  //! Get horizontal dilution of position.
  /*!
      @return hdop, the horizontal dilution of position.
  */
  double const GetHDOP() const
  {
    return this->hdop;
  }
  //! Get vertical dilution of position.
  /*!
      @return vdop, the vertical dilution of position.
  */
  double const GetVDOP() const
  {
    return this->vdop;
  }

  //! Get percent dilution of position.
  /*!
      @return pdop, the percent dilution of position.
  */
  double const GetPDOP() const
  {
    return this->pdop;
  }

  //! Get the age of the dgps data.
  /*!
      @return ageofdgpsdata, the age of the dgps data.
  */
  double const GetAgeOfDGPSData() const
  {
    return this->ageofdgpsdata;
  }

  //! Get the dgps id.
  /*!
      @return sat, the dgps id ( 0... 1023).
  */
  unsigned int const GetDGPSId() const
  {
    return this->dgpsid;
  }

  //! Set the magnetic variation.
  /*!
      @param magvar - the magnetic variation.
  */
  void SetMagneticVariation(double const & magvar)
  {
    this->magvar = magvar;
  }

  //! Set the magnetic variation.
  /*!
      @param magvar - the magnetic variation.
  */
  void SetMagneticVariation(std::string const & magvar)
  {
    std::istringstream is;
    is.str(magvar);
    is >> this->magvar;
  }

  //! Set the geoid height.
  /*!
      @param geoidheight - the geoid height.
  */
  void SetGeoidHeight(double const & geoidheight)
  {
    this->geoidheight = geoidheight;
  }

  //! Set the geoid height.
  /*!
      @param geoidheight - the geoid height.
  */
  void SetGeoidHeight(std::string const & geoidheight)
  {
    std::istringstream is;
    is.str(geoidheight);
    is >> this->geoidheight;
  }


  //! Set the name of the waypoint.
  /*!
      @param name - the GPS name of the waypoint.
  */
  void SetName(std::string const & name)
  {
    this->name = name;
  }

  //! Set the waypoint comment.
  /*!
      @param comment for the waypoint.
  */
  void SetComment(std::string const & comment)
  {
    this->comment = comment;
  }

  //! Set the waypoint description.
  /*!
      @param description for the waypoint.
  */
  void SetDescription(std::string const & desc)
  {
    this->desc = desc;
  }

  //! Set the source of the waypoint.
  /*!
      @param source of the waypoint.
  */
  void SetSource(std::string const & src)
  {
    this->src = src;
  }

  //! Set the symbol name for the waypoint.
  /*!
      @param sym - Text of GPS symbol name. For interchange with other programs, use the exact spelling of the symbol as displayed on the GPS. If the GPS abbreviates words, spell them out.
  */
  void SetSymbol(std::string const & sym)
  {
    this->sym = sym;
  }

  //! Set the waypoint type.
  /*!
      @param waypoint type.
  */
  void SetType(std::string const & type)
  {
    this->type = type;
  }

  //! Set the fix type.
  /*!
      The value should be one of: {'none'|'2d'|'3d'|'dgps'|'pps'}.
      none means GPS had no fix. To signify "the fix info is unknown", 
      leave out fixType entirely. pps = military signal used.

      @param fix, the fix type.
  */
  void SetFix(std::string const & fix)
  {
    this->fix = fix;
  }

  //! Set the number of satellites.
  /*!
      @param sat, the number of satellites.
  */
  void SetNumberOfSatellites(unsigned int const & sat)
  {
    this->sat = sat;
  }

  //! Set the number of satellites.
  /*!
      @param sat, the number of satellites.
  */
  void SetNumberOfSatellites(std::string const & sat)
  {
    std::istringstream is;
    is.str(sat);
    is >> this->sat;
  }

  //! Set horizontal dilution of position.
  /*!
      @param hdop, the horizontal dilution of position.
  */
  void SetHDOP(double const & hdop)
  {
    this->hdop = hdop;
  }

  //! Set horizontal dilution of position.
  /*!
      @param hdop, the horizontal dilution of position.
  */
  void SetHDOP(std::string const & hdop)
  {
    std::istringstream is;
    is.str(hdop);
    is >> this->hdop;
  }

  //! Set vertical dilution of position.
  /*!
      @param vdop, the vertical dilution of position.
  */
  void SetVDOP(double const & vdop)
  {
    this->vdop = vdop;
  }

  //! Set vertical dilution of position.
  /*!
      @param vdop, the vertical dilution of position.
  */
  void SetVDOP(std::string const & vdop)
  {
    std::istringstream is;
    is.str(vdop);
    is >> this->vdop;
  }

  //! Set percent dilution of position.
  /*!
      @param pdop, the percent dilution of position.
  */
  void SetPDOP(double const & pdop)
  {
    this->pdop = pdop;
  }

  //! Set percent dilution of position.
  /*!
      @param pdop, the percent dilution of position.
  */
  void SetPDOP(std::string const & pdop)
  {
    std::istringstream is;
    is.str(pdop);
    is >> this->pdop;
  }

  //! Set the age of the dgps data.
  /*!
      @param ageofdgpsdata, the age of the dgps data.
  */
  void SetAgeOfDGPSData(double const & ageofdgpsdata)
  {
    this->ageofdgpsdata = ageofdgpsdata;
  }

  //! Set the age of the dgps data.
  /*!
      @param ageofdgpsdata, the age of the dgps data.
  */
  void SetAgeOfDGPSData(std::string const & ageofdgpsdata)
  {
    std::istringstream is;
    is.str(ageofdgpsdata);
    is >> this->ageofdgpsdata;
  }

  //! Set the dgps id.
  /*!
      @param dgpsid, the dgps id ( 0... 1023).
  */
  void SetDGPSId(unsigned int const & dgpsid)
  {
    this->dgpsid = dgpsid;
  }

  //! Set the dgps id.
  /*!
      @param dgpsid, the dgps id ( 0... 1023).
  */
  void SetDGPSId(std::string const & dgpsid)
  {
    std::istringstream is;
    is.str(dgpsid);
    is >> this->dgpsid;
  }

  //! Input the position.
  /*!
      @param is - the input stream.
      @param p - the object into which the data is streamed.

      The data expected is latitude, longitude, elevation and time.

  */
  friend std::istream & operator>>(std::istream& is, Waypoint & p)
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
  friend std::ostream& operator<<(std::ostream& os, Waypoint & p)
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

  //! Return an XML representation of the waypoint.
  /*!
      @param intent - indent.
      @param tag - the html tag for the waypoint.
      @return The waypoint.
  */
  std::string ToXML(int indent = 2, std::string const & tag = "wpt");

  //! Get the GPXAttributes.
  /*!
      @return the address of GPXAttributes.
  */
  TWaypointGPXAttributes * GetGPXAttributes() { return &this->GPXAttributes; }

  // Attributes are processed in the base classes.

  //! Get the GPXElements.
  /*!
      @return the address of GPXElements.
  */
  TWaypointGPXElements * GetGPXElements() { return &this->GPXElements; }

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
      GeographicPoint::SetLatitude(value);
      return;
    }
    if ( key == "lon" )
    {
      GeographicPoint::SetLongitude(value);
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
      GeographicPoint::SetElevation(value);
      return;
    }
    if ( key == "time" )
    {
      GeographicPoint::SetTime(value);
      return;
    }

    // Can be double or string.
    if ( key == "magvar" )
    {
      SetMagneticVariation(value);
      return;
    }
    // Can be double or string.
    if ( key == "geoidheight" )
    {
      SetGeoidHeight(value);
      return;
    }

    if ( key == "name" )
    {
      SetName(value);
      return;
    }
    if ( key == "cmt" )
    {
      SetComment(value);
      return;
    }
    if ( key == "desc" )
    {
      SetDescription(value);
      return;
    }
    if ( key == "src" )
    {
      SetSource(value);
      return;
    }
    if ( key == "sym" )
    {
      SetSymbol(value);
      return;
    }
    if ( key == "type" )
    {
      SetType(value);
      return;
    }
    if ( key == "fix" )
    {
      SetFix(value);
      return;
    }
    // Can be int or string
    if ( key == "sat" )
    {
      SetNumberOfSatellites(value);
      return;
    }
    // Can be double or string
    if ( key == "hdop" )
    {
      SetHDOP(value);
      return;
    }
    // Can be double or string
    if ( key == "vdop" )
    {
      SetVDOP(value);
      return;
    }
    // Can be double or string
    if ( key == "pdop" )
    {
      SetPDOP(value);
      return;
    }
    // Can be double or string
    if ( key == "ageofgpsdata" )
    {
      SetAgeOfDGPSData(value);
      return;
    }
    // Can be double or string
    if ( key == "dgpsid" )
    {
      SetDGPSId(value);
      return;
    }
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

protected:
  TWaypointGPXAttributes GPXAttributes;
  TWaypointGPXElements GPXElements;

  // Position information
  double magvar;
  double geoidheight;

  // Description information.
  //! The GPS name of the waypoint. This field will be transferred to and from the GPS. GPX does not place restrictions on the length of this field or the characters contained in it. It is up to the receiving application to validate the field before sending it to the GPS.
  std::string name;
  //! Comment about the waypoint.
  std::string comment;
  //! A text description of the element. Holds additional information about the element intended for the user, not the GPS.
  std::string desc;
  //! Source of the waypoint.
  std::string src;
  //! Text of GPS symbol name. For interchange with other programs, use the exact spelling of the symbol as displayed on the GPS. If the GPS abbreviates words, spell them out.
  std::string sym;
  //! Type of waypoint.
  std::string type;

  // Accuracy information
  //! Type of GPS fix. none means GPS had no fix. To signify "the fix info is unknown", leave out fixType entirely. pps = military signal used. 
  std::string fix;
  //! Number of satellites.
  unsigned int sat;
  //! hdop
  double hdop;
  //! vdop
  double vdop;
  //! pdop
  double pdop;
  //! Age of dgps data
  double ageofdgpsdata;
  //! Represents a differential GPS station. 0..1023
  unsigned int dgpsid;


private:
  //! Indent a string by this amount.
  /*!
      @return A series of spaces corresponding to the indent.
  */
  std::string Indent(int const & indent);


};

} // Namespace Navigation.
} // Namespace UF.

#endif // NAVIGATION_WAYPOINT_H
