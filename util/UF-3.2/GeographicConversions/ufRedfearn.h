#if !defined(REDFEARN_H)
#define REDFEARN_H

#include "ufEllipsoid.h"
#include "ufMapGrid.h"

namespace UF {
//! Convert between various coordinate systems.
namespace GeographicConversions {

  //! This class converts between geographic and grid coordinates using Redfearn's formulae.
  /*!
  CLASS
    Redfearn

    When using this class, you must instantiate it with an instance of the ellipsoid
    and map grid.

    Redfearn's formulae were published in the "Empire Survey Review", No. 69, 1948.
    They may be used to  convert between latitude & longitude and easting,
    northing & zone for a Transverse Mercator projection, such as the Map
    Grid of Australia (MGA). These formulae are accurate to better than
    1 mm in any zone of the Map Grid of Australia and for the purposes
    of definition may be regarded as exact.
    See "Geocentric Datum of Australia, Technical Manual" Version 2.2 for
    the algorithms. Avaiable from http://www.icsm.gov.au/icsm/gda/gdatm/index.html
    as a pdf file.


  \par license
  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  YOU, as the USER, take SOLE RESPONSIBILITY for, and are responsible for
  determining the FITNESS and APPLICABILITY of this software for a particular
  purpose. The author or any other person or entity are not liable in any way
  for the use or misuse of any program or software using this software.

*/
  class UF_GEO_CONV_EXPORT Redfearn: public Ellipsoid, public MapGrid
  {
  public:

    //! Construct using a selected ellipsoid and map grid.
    Redfearn (std::string const & EllipsoidName, std::string const & GridName);

    virtual ~Redfearn(void) {}

  public:

    //! Assignment operator.
    Redfearn& operator = (Redfearn const & that)
    {
      // Handle self-assignment.
      if ( this == &that )
        return *this;

      this->ellipsoidName = that.ellipsoidName;
      this->semimajorAxis = that.semimajorAxis;
      this->inverseFlattening = that.inverseFlattening;
      this->mapGridName = that.mapGridName;
      this->falseEasting = that.falseEasting;
      this->falseNorthing = that.falseNorthing;
      this->centralScaleFactor = that.centralScaleFactor;
      this->zoneWidth = that.zoneWidth;
      this->centralMeridianZone1 = that.centralMeridianZone1;

      return *this;

    }
    
    //! Equality.
    bool operator == (Redfearn const & that)
    {
      // Handle self-comparison.
      if ( this == &that )
        return true;

      return (
        this->ellipsoidName == that.ellipsoidName &&
        this->semimajorAxis == that.semimajorAxis &&
        this->inverseFlattening == that.inverseFlattening &&
        this->mapGridName == that.mapGridName &&
        this->falseEasting == that.falseEasting &&
        this->falseNorthing == that.falseNorthing &&
        this->centralScaleFactor == that.centralScaleFactor &&
        this->zoneWidth == that.zoneWidth &&
        this->centralMeridianZone1 == that.centralMeridianZone1
        );
    }

    //! Inequality.
    bool operator != (Redfearn const & that)
    {
      return !(*this == that);
    }

  public:
    //! Convert geographic coordinates to grid coordinates.
    /*!
      Input:

        @param Latitude - in degrees.
        @param Longitude - in degrees.

      Output:

        @param Zone - the zone longitude number and zone latitude letter.
        @param Easting - in meters.
        @param Northing - in meters.
        @param GridConvergence
        @param PointScale
    */
    void GetGridCoordinates (
        double const & Latitude, double const & Longitude,
        std::string & Zone, double & Easting, double & Northing,
        double & GridConvergence, double & PointScale);

    //! Convert geographic coordinates to grid coordinates for a specified zone.
    /*!

      Useful when it is necessary to compute grid coordinates in a particular
      zone for a point whose grid coordinates are known in the adjacent zone.

      Input:

        @param Latitude - in degrees.
        @param Longitude - in degrees.
        @param Zone

      Output:

        @param Easting - in meters.
        @param Northing - in meters.
        @param GridConvergence
        @param PointScale
    */
    void GetZoneGridCoordinates (
        double const & Latitude, double const & Longitude,
        std::string const & Zone, double & Easting, double & Northing,
        double & GridConvergence, double & PointScale);

    //! Convert grid coordinates to geographic coordinates.
    /*!
      Input:

        @param Zone - the zone longitude number and zone latitude letter.
        @param Easting - in meters.
        @param Northing - in meters.

      Output:

        @param Latitude - in degrees.
        @param Longitude - in degrees.
        @param GridConvergence
        @param PointScale
     */
    void GetGeographicCoordinates (
        std::string const &Zone, double const & Easting, double const & Northing,
        double & Latitude, double & Longitude,
        double & GridConvergence, double & PointScale);

    //! Convert grid coordinates to geographic coordinates.
    /*!
      Input:

        @param Hemisphere - Zone number with either "N" or "S" appended for the hemisphere.
                                No checking is done for upper or lower case or other letters
                                so make sure that only "N"or "S" is passed to this function.
        @param Easting - in meters.
        @param Northing - in meters.

      Output:

        @param Latitude - in degrees.
        @param Longitude - in degrees.
        @param GridConvergence
        @param PointScale
     */
    void GetGeographicCoordinatesNS (
        std::string const &Hemisphere, double const & Easting, double const & Northing,
        double & Latitude, double & Longitude,
        double & GridConvergence, double & PointScale);

    private:
    //! Determine the correct UTM letter designator for the given latitude
    /*! Return 'Z' if latitude is outside the UTM limits of 84N to 80S
        Written by Chuck Gantz- chuck.gantz@globalstar.com
    */
    char UTMLetterDesignator(double const & Latitude);
    //! Return an integer correspondinv to the UTM letter.
    int UTMLetterNumber(char const & Letter);
    //! Return the bound in desrees of the latitude band.
    int UTMBound(char const & Letter);

     //! Convert geographic coordinates to grid coordinates.
    /*!
      Input:

        @param Latitude - in degrees.
        @param Longitude - in degrees.

      Output:

        @param Zone - the zone longitude number and zone latitude letter.
        @param Easting - in meters.
        @param Northing - in meters.
        @param GridConvergence
        @param PointScale
     */
    void GridCoordinates (
        double const & Latitude, double const & Longitude,
        int const & ZoneNumber, double & Easting, double & Northing,
        double & GridConvergence, double & PointScale);

   //! Performs the conversion from grid coordinates to geographic coordinates.
    /*!
      Input:

        @param Zone - the zone longitude number.
        @param NorthernHemisphere - true if the zone is in the northern hemisphere.
        @param Easting - in meters.
        @param Northing - in meters.

      Output:

        @param Latitude - in degrees.
        @param Longitude - in degrees.
        @param GridConvergence
        @param PointScale
     */
    void GeographicCoordinates (
        int const & ZoneNumber, bool const & NorthernHemisphere,
        double const & Easting, double const & Northing,
        double & Latitude, double & Longitude,
        double & GridConvergence, double & PointScale);

  private:
    //! Degrees to radians.
    double const rtd;
    //! Radians to degrees.
    double const dtr;
  };

} // Namespace GeographicConversions.
} // Namespace UF.

#endif
