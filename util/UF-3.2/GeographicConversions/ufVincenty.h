#if !defined(VINCENTY_H)
#define VINCENTY_H

#include "ufEllipsoid.h"

namespace UF {
//! Convert between various coordinate systems.
namespace GeographicConversions {

  //! This class calculates inverse and forward transforms using Vincnety's formulae.
  /*!
  CLASS
    Vincenty

    When using this class, you must instantiate it with an instance of the ellipsoid.

    Vincnety's formulae were published in the "Empire Survey Review", No. 176, April 1975.

    See "Geocentric Datum of Australia, Technical Manual" Version 2.2 for
    the algorithms. Avaiable from http://www.icsm.gov.au/icsm/gda/gdatm/index.html
    as a pdf file.

    "The inverse formula may give no solution over a line between two nearly antipodal points.
    This will occur when he difference in longitude is greater than pi in absolute value. (Vincenty 1975)"


  \par license
  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  YOU, as the USER, take SOLE RESPONSIBILITY for, and are responsible for
  determining the FITNESS and APPLICABILITY of this software for a particular
  purpose. The author or any other person or entity are not liable in any way
  for the use or misuse of any program or software using this software.

*/
  class UF_GEO_CONV_EXPORT Vincenty: public Ellipsoid
  {
  public:

    //! Construct using a selected ellipsoid and map grid.
    Vincenty (std::string const & EllipsoidName);

    virtual ~Vincenty(void) {}

  public:

    //! Assignment operator.
    Vincenty& operator = (Vincenty const & that)
    {
      // Handle self-assignment.
      if ( this == &that )
        return *this;

      this->ellipsoidName = that.ellipsoidName;
      this->semimajorAxis = that.semimajorAxis;
      this->inverseFlattening = that.inverseFlattening;
      //this->SetEllipsoid(that.EllipsoidName());
      return *this;

    }
    
    //! Equality.
    bool operator == (Vincenty const & that)
    {
      // Handle self-comparison.
      if ( this == &that )
        return true;

      return (
        this->ellipsoidName == that.ellipsoidName &&
        this->semimajorAxis == that.semimajorAxis &&
        this->inverseFlattening == that.inverseFlattening
        );
      //return
      //  this->EllipsoidName() == that.EllipsoidName();
    }

    //! Inequality.
    bool operator != (Vincenty const & that)
    {
      return !(*this == that);
    }


  public:

    //! Given the latitude and longitude of two points (p1 and p2), calculate the distance, forward and reverse azimuths.
    /*!
      Input:

    @param lat1 - latitude of point 1 in degrees.
    @param lon1 - longitude of point 1 in degrees.
    @param lat2 - latitude of point 2 in degrees.
    @param lon2 - longitude of point 2 in degrees.

      Output:

    @param ellipsoidalDistance - Ellipsoidal distance from p1 to p2 in meters.
    @param forwardAzimuth - Forward azimuth from p1 to p2 in degrees.
    @param reverseAzimuth - Reverse azimuth from p2 to p1 in degrees..
     */
    void inverse(double const & lat1, double const & lon1, double const & lat2, double const & lon2,
                 double & ellipsoidalDistance, double & forwardAzimuth, double & reverseAzimuth);

    //! Given the latitude and longitude of one point (p1 ), and the geodetic azimuth and distance to a second point (p2), calculate the latutude, longitude and reverse azimuth from the second point.
    /*!
      Input:

    @param lat1 - latitude of point 1 in degrees.
    @param lon1 - longitude of point 1 in degrees.
    @param ellipsoidalDistance - Ellipsoidal distance from p1 to p2 in meters.
    @param forwardAzimuth - Forward azimuth from p1 to p2 in degrees.

      Output:

    @param lat2 - latitude of point 2 in degrees.
    @param lon2 - longitude of point 2 in degrees.
    @param reverseAzimuth - Reverse azimuth from p2 to p1 in degrees..
     */
    void direct(double const & lat1, double const & lon1,  double const & ellipsoidalDistance, double const & forwardAzimuth,
                double & lat2, double & lon2, double & reverseAzimuth);

    private:

    //! Solve the geodetic inverse problem after T. Vincenty.
    /*!
      This is the solution of the geodetic inverse problem after T. Vincenty. It is a
      modified Rainsford method with Helmert ellitpical terms. It is effective in any
      azimuth and at any distance short of antipodal also the from/to stations must not be at
      the poles.


      Input:

      @param p1 - latitude of station 1 (radians).
      @param e1 - longitude of station 1 (radians).
      @param p2 - latitude of station 2 (radians).
      @param e2 - longitude of station 2 (radians).

      Output:

      @param s - Geodetic distanance between stations 1 and 2 in meters.
      @param az1 - Azimuth at station 1->2 (radians).
      @param az2 - Azimuth at station 2->1 (radians).
     */
      void vininv ( double const & p1, double const & e1, double const & p2, double const & e2,
                    double & s, double & az1, double & az2);

    //! Solve the geodetic direct problem after T. Vincenty.
    /*!
      This is the solution of the geodetic direct problem after T. Vincenty. It is a
      modified Rainsford method with Helmert ellitpical terms. It is effective in any
      azimuth and at any distance short of antipodal also the from/to stations must not be at
      the poles.


      Input:

      @param p1 - latitude of station 1 (radians).
      @param e1 - longitude of station 1 (radians).
      @param s - Geodetic distanance between stations 1 and 2 in meters.
      @param az1 - Azimuth at station 1->2 (radians).

      Output:

      @param p2 - latitude of station 2 (radians).
      @param e2 - longitude of station 2 (radians).
      @param az2 - Azimuth at station 2->1 (radians).
     */
      void vdirect ( double const & p1, double const & e1, double const & s, double const & az1,
                    double & p2, double & e2, double & az2);


    //! Calculate x mod y.
    template < typename T >
      T mod ( T const & x, T const & y ) const
    {
      double t = x-y*int(x/y);
      if ( t < 0 )
        t += y;
      return t;
    }

  private:
    //! pi.
    double const pi;
    //! 2pi
    double const twopi;
    //! pi/2
    double const halfpi;
    //! Degrees to radians.
    double const rtd;
    //! Radians to degrees.
    double const dtr;
    //! Tolerance for checking computation value.
    double const tol0;
      //! Tolerance for checking a double zero value.
    double const tol1;
      //! Tolerance for checking a close to zero value.
    double const tol2;
     //! Tolerance for checking if distance or angle is close to zero.
    double const tol3;
     //! Tolerance for checking if distance or angle is close to zero.
    double const tol4;

  };

} // Namespace GeographicConversions.
} // Namespace UF.

#endif
