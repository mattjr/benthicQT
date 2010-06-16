#if !defined(ELLIPSOID_H)
#define ELLIPSOID_H

#include <cmath>
#include <string>

#include "ufWin32Header.h"

namespace UF {

namespace GeographicConversions {

  //! Stores constants and provides functions to access the parameters of the ellipsoid that you are using.
  /*!
  CLASS
    Ellipsoid

    The class is initialised by passing to it the name of the ellipsoid you want to use.
    By using SetEllipsoid() you can change the elipsoid. However the ellipsoid you are changing
    to must exist in the static class Ellipsoids.


    \par license
    This software is distributed WITHOUT ANY WARRANTY; without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    YOU, as the USER, take SOLE RESPONSIBILITY for, and are responsible for
    determining the FITNESS and APPLICABILITY of this software for a particular
    purpose. The author or any other person or entity are not liable in any way
    for the use or misuse of any program or software using this software.
  */
  class UF_GEO_CONV_EXPORT Ellipsoid
  {
  public:
    //! Construct an ellipsoid with a given semi major axis and inverse flattening.
    /*!
      @param EllipsoidName - the name of the ellipsoid.
    */
    Ellipsoid ( std::string const & EllipsoidName );

    virtual ~Ellipsoid(void) {}

  public:

    //! Assignment operator.
    Ellipsoid& operator = (Ellipsoid const & that)
    {
      // Handle self-assignment.
      if ( this == &that )
        return *this;

      this->ellipsoidName = that.ellipsoidName;
      this->semimajorAxis = that.semimajorAxis;
      this->inverseFlattening = that.inverseFlattening;

      return *this;

    }
  
    //! Equality.
    bool operator == (Ellipsoid const & that)
    {
      // Handle self-comparison.
      if ( this == &that )
        return true;

      return (
        this->ellipsoidName == that.ellipsoidName &&
        this->semimajorAxis == that.semimajorAxis &&
        this->inverseFlattening == that.inverseFlattening
        );
    }

    //! Inequality.
    bool operator != (Ellipsoid const & that)
    {
      return !(*this == that);
    }

  public:
    //! Which ellipsoid is in use?
    std::string EllipsoidName () const { return this->ellipsoidName; }
    //! Is the ellipsoid valid?
    bool EllipsoidIsValid () const;
    //! Set the ellipsoid to use.
    void SetEllipsoid ( std::string const & EllipsoidName );
    //! Calculate the semi-major axis.
    double  SemimajorAxis ( void ) const { return this->semimajorAxis; }
    //! Calculate the semi minor axis.
    double  SemiminorAxis ( void ) const { return this->semimajorAxis * ( 1.0 - this->Flattening() ); }
    //! Calculate the flattening.
    double  Flattening ( void ) const { return 1.0 / this->inverseFlattening; }
    //! Calculate the inverse flattening.
    double  InverseFlattening ( void ) const { return this->inverseFlattening; }
    //! Calculate the eccentricity squared.
    double  EccentricitySq ( void ) const
    {
        double f = this->Flattening();
        return (2.0 - f) * f;
    }
    //! Calculate the eccentricity.
    double  Eccentricity ( void ) const
    {
        return std::sqrt(this->EccentricitySq());
    }
    //! Calculate the second eccentricity.
    double  SecondEccentricity ( void ) const
    {
        double e2 = this->EccentricitySq();
        double se = e2 / ( 1.0 - e2);
        return std::sqrt(se);
    }
    //! Calculate N = (a-b)/(a+b)
    double  N ( void ) const
    {
      double a = this->SemimajorAxis();
      double b = this->SemiminorAxis();
      return ( a - b ) / ( a + b );
    }
    //! Calculate e'^2 = (a^2-b^2)/b^2
    double  e_dash_sq ( void ) const
    {
      double a = this->SemimajorAxis();
      double b = this->SemiminorAxis();
      return ( a*a - b*b ) / ( b*b );
    }
    //! Calculate the mean length of one degree of latitude.
    double  G ( void ) const
    {
      double n = this->N();
      double n2 = n * n;
      double n3 = n * n2;
      double n4 = n * n3;
      double a = this->SemimajorAxis();
      static const double dtr = std::atan((double)1.0) / 45.0;
      return a*(1.0-n)*(1.0-n2)*(1.0+(9.0*n2)/4.0+255.0*n4/64.0)*dtr;
    }

  protected:
    //! The name of the ellipsoid.
    std::string ellipsoidName;
    //! Semimajor axis (m).
    double semimajorAxis;
    //! Inverse flattening.
    double inverseFlattening;
  };

} // Namespace GeographicConversions.
} // Namespace UF.

#endif // ELLIPSOID_H
