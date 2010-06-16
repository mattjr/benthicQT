#if !defined(ECEF_H)
#define ECEF_H

#include "ufEllipsoid.h"
#include "ufMapGrid.h"

namespace UF {

namespace GeographicConversions
{

  //! This class converts between geodetic and ECEF rectangular coordinates on the ellipsoid.
  /*!
  CLASS
    ECEF

    When using this class, you must instantiate it with an instance of the ellipsoid.

  \par license
  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  YOU, as the USER, take SOLE RESPONSIBILITY for, and are responsible for
  determining the FITNESS and APPLICABILITY of this software for a particular
  purpose. The author or any other person or entity are not liable in any way
  for the use or misuse of any program or software using this software.

  \todo

  \bug

  */
  class UF_GEO_CONV_EXPORT ECEF: public Ellipsoid
  {
  public:
    //! Construct using a selected ellipsoid
    ECEF( std::string const & EllipsoidName );

    virtual ~ECEF(void){}

  public:

    //! Assignment operator.
    ECEF& operator = (ECEF const & that)
    {
      // Handle self-assignment.
      if ( this == &that )
        return *this;

      this->SetEllipsoid(that.EllipsoidName());
      return *this;

    }
    
    //! Equality.
    bool operator == (ECEF const & that)
    {
      // Handle self-comparison.
      if ( this == &that )
        return true;

      return
        this->EllipsoidName() == that.EllipsoidName();

    }


  public:
    //! Convert geodetic coordinates to Cartesian coordinates.
    /*!

      Eastern Longitudes = Positive Y
      Northern Latitudes = Positive Z

      Input:
      Geodetic latitude, longitude and height above the ellipsoid.

      @param latitude - in degrees
      @param longitude - in degrees
      @param height - in the same units as the ellipsoid ( usually meters)

      Output:
      Earth Centered Earth Fixed Cartesian Coordinates.

      @param X
      @param Y
      @param Z
    */
    void toECEF ( double const & latitude, double const & longitude, double const & height,
      double & X, double & Y, double & Z);

    //! Convert Cartesian coordinates to geodetic coordinates.
    /*!
      This conversion is not exact but does provide centimeter accuracy for heights <= 1000km
      (Bowring B. 1976. "Transformation from spatial to geographic coordinates.", Survey Review, XXII pp323-327 )

      Eastern Longitudes = Positive Y
      Northern Latitudes = Positive Z

      Input:
      Earth Centered Earth Fixed Cartesian Coordinates.

      @param X
      @param Y
      @param Z

      Output:
      Geodetic latitude, longitude and height above the ellipsoid.

      @param latitude - in degrees
      @param longitude - in degrees
      @param height - in the same units as the ellipsoid ( usually meters)

    */
    void fromECEF_Inexact ( double const & X, double const & Y, double const & Z,
      double & latitude, double & longitude, double & height);

    //! Convert Cartesian coordinates to geodetic coordinates - exact solution.
    /*!
      This is an implementation of closed-form solution published by K. M. Borkowski.

      Eastern Longitudes = Positive Y
      Northern Latitudes = Positive Z

      Input:
      Earth Centered Earth Fixed Cartesian Coordinates.

      @param X
      @param Y
      @param Z

      Output:
      Geodetic latitude, longitude and height above the ellipsoid.

      @param latitude - in degrees
      @param longitude - in degrees
      @param height - in the same units as the ellipsoid ( usually meters)

      @return false if Z==0 or (X*X+Y*Y) == 0, true otherwise. A false return
      indicates that the calculation was not performed.

      The best closed-form solution seems  to be that of K. M. Borkowski,
      a Polish radio-astronomer. It requires  finding a root of a
      fourth-degree polynomial, and is therefore  possible using the
      classical fourth-degree formula for roots. The algorithm is given
      in detail in the (new) Explanatory Supplement for
      the Astronomical Almanac (P. Seidelmann, editor).

      There's a link to an abstract of Borkowski's paper on his
      personal Web page:

        http://www.astro.uni.torun.pl/~kb/personal.html

      The abstract is at

        http://www.astro.uni.torun.pl/~kb/abstract.html#Transf2


    */
    void fromECEF ( double const & X, double const & Y, double const & Z,
      double & latitude, double & longitude, double & height);

  private:
    //! Degrees to radians.
    double const rtd;
    //! Radians to degrees.
    double const dtr;
    //! pi/2
    double const pi2;
  };

} // Namespace GeographicConversions.
} // Namespace UF.

#endif // ECEF_H
