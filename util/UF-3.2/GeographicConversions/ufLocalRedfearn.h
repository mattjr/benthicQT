#if !defined(UF_LOCAL_REDFEARN_H)
#define UF_LOCAL_REDFEARN_H

#include "ufEllipsoid.h"
#include "ufMapGrid.h"
#include "ufRedfearn.h"

namespace UF {
//! Convert between various coordinate systems.
namespace GeographicConversions {


/*

  LocalRedfearn. 
  This class provides the ability to use a UTM projection and to specify the 
  central meridian of the first UTM zone. In this way mapping / navigation that
  is done close to the centre of this first zone can be done without the need to
  correct compass headings, and is such that lines of constant longitude match
  much more closely projected lines with a constant easting component.

  WARNING:- EXTREME CARE must be used to ensure that all conversions using the
  ufLocalRedfearn classes and data are performed using the same central 
  meridian. Otherwise the results will be invalid...


  \par license
  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  YOU, as the USER, take SOLE RESPONSIBILITY for, and are responsible for
  determining the FITNESS and APPLICABILITY of this software for a particular
  purpose. The author or any other person or entity are not liable in any way
  for the use or misuse of any program or software using this software.

  Thanks to Duncan Mercer for creating this class.
*/


  class UF_GEO_CONV_EXPORT LocalRedfearn: public Redfearn
  {
  public:

    //! Construct using a selected ellipsoid and map grid, 
    //! using the specified central meridian.
    LocalRedfearn (std::string const & EllipsoidName, 
                   std::string const & GridName,
                   const double LocalZone1CentralMeridian );

    virtual ~LocalRedfearn(void) {}

    //! Geographic -> Grid conversion functions and
    //! Grid -> Geographic. These all call the underlying
    //! Refearn functions after enforcing zone1 only operation.

    //! Convert geographic coordinates to grid coordinates.
    //! See matching function in Redfearn for details.
    void GetGridCoordinates (
        double const & Latitude, double const & Longitude,
        std::string & Zone, double & Easting, double & Northing,
        double & GridConvergence, double & PointScale);

    //! Convert geographic coordinates to grid coordinates for a specified zone.
    //! See matching function in Redfearn for details.
    void GetZoneGridCoordinates (
        double const & Latitude, double const & Longitude,
        std::string const & Zone, double & Easting, double & Northing,
        double & GridConvergence, double & PointScale);


    //! Convert grid coordinates to geographic coordinates.
    //! See matching function in Redfearn for details.
    void GetGeographicCoordinates (
        std::string const &Zone, double const & Easting, 
        double const & Northing, double & Latitude, double & Longitude,
        double & GridConvergence, double & PointScale);

    //! Convert grid coordinates to geographic coordinates.
    //! See matching function in Redfearn for details.    
    void GetGeographicCoordinatesNS (
        std::string const &Hemisphere, double const & Easting, 
        double const & Northing, double & Latitude, double & Longitude,
        double & GridConvergence, double & PointScale);
    
    //! LocalRedfearn only valid in local-UTM zone 1. Otherwise throw...
    inline void ZoneError_Throw(void);

  };


} // Namespace GeographicConversions.
} // Namespace UF.

#endif
