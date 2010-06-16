
#include "ufECEF.h"
#include <cmath>

using namespace UF::GeographicConversions;

ECEF::ECEF( std::string const & EllipsoidName ) :
  Ellipsoid( EllipsoidName )
  , rtd(45.0/std::atan(1.0))
  , dtr(std::atan(1.0)/45.0)
  , pi2(2.0*std::atan(1.0))
{}

void ECEF::toECEF ( double const & latitude, double const & longitude, double const & height,
  double & X, double & Y, double & Z)
{

  double lat = dtr * latitude;
  double slat = std::sin(lat);
  double clat = std::cos(lat);
  double lng = dtr * longitude;
  double clng = std::cos(lng);
  double slng = std::sin(lng);

  // Radius of curvature in the prime vertical.
  double N = SemimajorAxis()/std::sqrt(1.0-this->EccentricitySq()*slat*slat);

  X = (N+height)*clat*clng;
  Y = (N+height)*clat*slng;
  Z = (N*(1-EccentricitySq())+height)*slat;
}

void ECEF::fromECEF_Inexact ( double const & X, double const & Y, double const & Z,
  double & latitude, double & longitude, double & height)
{
  double a = SemimajorAxis();
  double b = SemiminorAxis();
  double p = std::sqrt(X*X+Y*Y);

  // Here we are somewhere on a line netween the North or South pole.
  if ( p == 0 )
  {
    if ( Z < 0 )
    {
      latitude = -90;
      height = -Z - b;
    }
    else
    {
      latitude = 90;
      height = Z - b;
    }
    longitude = 0;
    return ;
  }

  // Here we are somewhere on the equator.
  if ( Z == 0 )
  {
    latitude = 0;
    longitude = std::atan2(Y,X)*rtd;
    height = p - a;
    return ;
  }


  double t = std::atan(Z*a/(p*b));
  double st = std::sin(t);
  double ct = std::cos(t);

  latitude = std::atan2((p-this->EccentricitySq()*a*ct*ct*ct),(Z + this->e_dash_sq()*b*st*st*st));
  latitude = -(latitude - pi2);

  double slat = std::sin(latitude);
  double clat = std::cos(latitude);
  // Radius of curvature in the prime vertical.
  double Rn = a/std::sqrt(1.0-this->EccentricitySq()*slat*slat);

  height = p/clat - Rn;
  latitude = latitude * rtd;
  longitude = std::atan2(Y,X)*rtd;

}


void ECEF::fromECEF ( double const & X, double const & Y, double const & Z,
  double & latitude, double & longitude, double & height)
{
  double a = SemimajorAxis();
  double b = SemiminorAxis();
  double r = std::sqrt(X*X+Y*Y);

  // Here we are somewhere on a line netween the North or South pole.
  if ( r == 0 )
  {
    if ( Z < 0 )
    {
      latitude = -90;
      height = -Z - b;
    }
    else
    {
      latitude = 90;
      height = Z - b;
    }
    longitude = 0;
    return ;
  }

  // Here we are somewhere on the equator.
  if ( Z == 0 )
  {
    latitude = 0;
    longitude = std::atan2(Y,X)*rtd;
    height = r - a;
    return ;
  }

  // Adjust for the southern hemisphere.
  if ( Z < 0 ) b = -b;

  double E = (b*Z-(a*a-b*b))/(a*r);
  double F = (b*Z+(a*a-b*b))/(a*r);
  double P = (E*F+1.0)*4.0/3.0;
  double Q = (E*E-F*F)*2.0;
  double D = P*P*P+Q*Q;
  double v;
  if ( D >= 0 )
  {
    double s1 = std::sqrt(D)-Q;
    if ( s1 > 0 )
    {
      s1 = std::pow(std::abs(s1),1.0/3.0);
    }
    else
    {
      s1 = -std::pow(std::abs(s1),1.0/3.0);
    }
    double s2 = std::sqrt(D)+Q;
    if ( s2 > 0 )
    {
      s2 = std::pow(s2,1.0/3.0);
    }
    else
    {
      s2 = -std::pow(std::abs(s2),1.0/3.0);
    }
    v = s1-s2;
    // This improves the accuracy of the solution.
    // The following equation is more accurate than v (above)
    // if v^2 < P, which is the case everywhere outside the radial
    // distance from the Earth's centre of about 70km.
    // if ( v * v < abs(P) )
      v = -(v*v*v+2.0*Q)/(3.0*P);
  }
  else
  {
    v = 2.0 * std::sqrt(-P)*std::cos(std::acos(Q/P/std::sqrt(-(P)))/3.0);
  }
  double G = 0.5*(E+std::sqrt(E*E+v));
  double t = std::sqrt(G*G+(F-v*G)/(G+G-E))-G;
  double fi = std::atan((1-t*t)*a/(2.0*b*t));
  height = (r-a*t)*std::cos(fi)+(Z-b)*std::sin(fi);
  latitude = fi * rtd;
  longitude = std::atan2(Y,X)*rtd;

}

