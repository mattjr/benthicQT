//
// C++ Implementation: GreatCircle
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufGreatCircle.h"

#include <cmath>

using std::sin;
using std::asin;
using std::cos;
using std::acos;
using std::abs;
using std::pow;
using std::atan2;
using std::tan;
using std::atan;
using std::sqrt;

using namespace UF::Navigation;

GreatCircle *GreatCircle::pInstance_ = 0;
bool GreatCircle::destroyed_ = false;


GreatCircle::GreatCircle() :
    pi(4.0*std::atan(1.0))
    , rtd(45.0/std::atan(1.0))
    , dtr(std::atan(1.0)/45.0)
    , earthRadius(6371009.0) // mean radius in meters
    , earthRadiusNmi(earthRadius/1852.0)
    , eps(5.0e-11)
    , lim(10.0/60.0) // 10nmi.
{
}


GreatCircle::~GreatCircle()
{
  pInstance_ = 0;
  destroyed_ = true;

 // My stuff
}

double GreatCircle::Distance(double const & lat1, double const & lon1,
                             double const & lat2, double const & lon2) const
{
  double p1 = lat1 * this->dtr;
  double e1 = lon1 * this->dtr;
  double p2 = lat2 * this->dtr;
  double e2 = lon2 * this->dtr;

  double dlon = e2 - e1;
  double clon = cos(dlon);
  double slon = sin(dlon);

  double cp1 = cos(p1);
  double sp1 = sin(p1);
  double cp2 = cos(p2);
  double sp2 = sin(p2);

  double t0 = cp2*slon;
  double t1 = cp1*sp2 - sp1*cp2*clon;
  double t2 = sqrt(t0*t0 + t1*t1);
  double t3 = sp1*sp2 + cp1*cp2*clon;

  double d = atan2(t2,t3);

  // Return in minutes == nautical miles.
  return 60.0*this->rtd*d;
}

double GreatCircle::DistanceH(double const & lat1, double const & lon1,
                             double const & lat2, double const & lon2) const
{
  double p1 = lat1 * this->dtr;
  double e1 = lon1 * this->dtr;
  double p2 = lat2 * this->dtr;
  double e2 = lon2 * this->dtr;

  double dlat = p2 - p1;
  // Calculate the haversine of the difference in latitude.
  double hvdlat = sin(dlat*0.5);
  hvdlat *= hvdlat;

  double dlon = e2 - e1;
  // Calculate the haversine of the difference in longitude.
  double hvdlon = sin(dlon*0.5);
  hvdlon *= hvdlon;

  // Calculate the square of half the chord length
  // between the two points. (The straight line distance).
  double a = hvdlat + cos(p1)*cos(p2)*hvdlon;
  // The great circle distance in radians.
  double c = 2.0 * asin(sqrt(a));

  // Return in minutes == nautical miles.
  return 60.0*this->rtd*c;
}

double GreatCircle::Course(double const & lat1, double const & lon1,
                           double const & lat2, double const & lon2) const
{
  double p1 = lat1 * this->dtr;
  double e1 = lon1 * this->dtr;
  double p2 = lat2 * this->dtr;
  double e2 = lon2 * this->dtr;

  double dlon = e2 - e1;
  double clon = cos(dlon);
  double slon = sin(dlon);

  double cp1 = cos(p1);
  double sp1 = sin(p1);
  double cp2 = cos(p2);
  double sp2 = sin(p2);

  double tc1;

  // Before we obtain the initial course we need to check if the initial point is a pole
  // (because the formula will fail in this case).
  if ( abs(dlon) < eps && abs(p1 - p2) < this->eps )
  {
    // Points are the same.
    return 0.0;
  }
  if ( abs(dlon) < eps )
  {
    // Points lie on a N->S meridian.
    // The points lie on a meridian.
    if ( p1 > 0 && p2 < 0 )
    {
      // N->S
      return 180.0;
    }
    else
    {
      if ( p1 < 0 && p2 > 0 )
      {
        // S->N
        return 0.0;
      }
      else // latr1 and latr2 lie in the same hemisphere.
      {
        if ( p1 < p2 )
        {
          // S-> N
          return 0.0;
        }
        else
        {
          // N->S
          return 180.0;
        }
      }
    }

  }
  tc1 = atan2(slon*cp2,
        cp1*sp2-sp1*cp2*clon);

  tc1 = this->mod(tc1,2.0*this->pi);


  return this->rtd*tc1;
}

double GreatCircle::LatOnGC(double const & lon,
                            double const & lat1, double const & lon1,
                            double const & lat2, double const & lon2) const
{
  double lonr = lon * this->dtr;
  double latr1 = lat1 * this->dtr;
  double lonr1 = lon1 * this->dtr;
  double latr2 = lat2 * this->dtr;
  double lonr2 = lon2 * this->dtr;

  // Will fail if on a meridian so return an impossible latitude.
  if ( abs(sin(lonr2-lonr1)) < eps )
    return -999;

  double lat = atan((sin(latr1)*cos(latr2)*sin(lonr2-lonr)
        -sin(latr2)*cos(latr1)*sin(lonr1-lonr))/
        (cos(latr1)*cos(latr2)*sin(lonr2-lonr1)));
  return lat * this->rtd;
}

void GreatCircle::Direct( double const & lat1, double const & lon1,
                          double const & crse12, double const & dist12,
                          double & lat2, double & lon2 )
{
  double latr1 = lat1 * this->dtr;
  double lonr1 = lon1 * this->dtr;
  double tcr12 = crse12 * this->dtr;
  double dr12 = (dist12/60.0) * dtr;

  double latr2 = -1;
  double lonr2 = -1;
  lat2 = lon2 = -1;

  if ( abs(cos(latr1)) < this->eps && !(abs(sin(tcr12)) < this->eps) )
  {
    // Starting at a pole. This has no meaning.
    return;
  }
  latr2 = asin(sin(latr1)*cos(dr12)+cos(latr1)*sin(dr12)*cos(tcr12));
  lat2 = latr2 * this->rtd;
  if ( abs(cos(latr2)) < this->eps )
  {
    // Endpoint is a pole.
    lonr2 = 0;
    lon2 = lonr2;
  }
  else
  {
    double dlon = atan2(sin(tcr12)*sin(dr12)*cos(latr1),cos(dr12)-sin(latr1)*sin(latr2));
//    lonr2 = mod(lonr1 - dlon + this->pi,2.0*this->pi) - this->pi;
    lonr2 = this->pi - mod(this->pi - (lonr1 + dlon),2.0*this->pi);
    lon2 = lonr2 * rtd;
  }

}

double GreatCircle::CrossTrackDistance(double const & lat, double const & lon,
                                    double const & lat1, double const & lon1,
                                    double const & lat2, double const & lon2) const
{

  // Calculate the course from P1 to P2.
  double tcr12 = this->Course(lat1,lon1,lat2,lon2) * this->dtr;
  // Calculate the course from P1 to P.
  double tcr1p = this->Course(lat1,lon1,lat,lon) * this->dtr;
  // Calculate the distance from P1 to P.
  double dr1p = this->Distance(lat1,lon1,lat,lon) * this->dtr / 60.0;

  double xtd = asin(sin(dr1p)*sin(tcr1p-tcr12));

  return xtd * this->rtd * 60.0;
}

double GreatCircle::AlongTrackDistance(double const & lat, double const & lon,
                                       double const & lat1, double const & lon1,
                                       double const & lat2, double const & lon2) const
{

  double xtd = this->CrossTrackDistance(lat,lon,lat1,lon1,lat2,lon2) * this->dtr / 60.0;
  // Calculate the distance from P1 to P.
  double dr1p = this->Distance(lat1,lon1,lat,lon) * this->dtr / 60.0;
  double atd = acos(cos(dr1p)/cos(xtd));
  if ( atd < 1.0*this->rtd/60.0 )
  {
    // Recalculate for better precision.
    double sd2 = sin(dr1p);
    sd2 *= sd2;
    double sx2 =sin(xtd);
    sx2 *= sx2;
    atd = asin(sqrt(sd2 - sx2)/cos(xtd));
  }

  return atd * this->rtd * 60.0;
}

bool GreatCircle::ATDOnGC(double const & lat, double const & lon, double const & d,
                          double const & lat1, double const & lon1,
                          double const & lat2, double const & lon2,
                          double & d1, double & d2)
{

  double xtd = this->CrossTrackDistance(lat,lon,lat1,lon1,lat2,lon2) * this->dtr / 60.0;
  double atd = this->AlongTrackDistance(lat,lon,lat1,lon1,lat2,lon2) * this->dtr / 60.0;
  double dr = abs(d) * this->dtr / 60.0;

  if ( abs(dr - xtd) < this->eps )
  {
    // One point on the great circle.
    d1 = d2 = atd * this->rtd * 60;
    return true;
  }
  if ( dr < xtd )
  {
    // No points on the great circle.
    return false;
  }

  double r = cos(xtd);

  // Two points exist.
  double t = cos(dr)/r;
  if ( t > 1 )
  {
    t = 1;
  }
  if ( t < -1 )
  {
    t = 1;
  }
  t = acos(t);
  d1 = (atd - t)* this->rtd * 60.0;
  d2 = (atd + t)* this->rtd * 60.0;

  return true;
}

bool GreatCircle::PointsOnGC(double const & lat, double const & lon, double const & d,
                             double const & lat1, double const & lon1,
                             double const & lat2, double const & lon2,
                             double & latA, double & lonA,
                             double & latB, double & lonB)
{
  double d1;
  double d2;
  // Find the along track distances.
  if (!this->ATDOnGC(lat,lon,d,lat1,lon1,lat2,lon2,d1,d2))
  {
    return false;
  }
  // Calculate the course from P1 to P2.
  double tc = this->Course(lat1,lon1,lat2,lon2);

  this->Direct(lat1,lon1,tc,d1,latA,lonA);
  this->Direct(lat1,lon1,tc,d2,latB,lonB);

  return true;
}
