
#include <iostream>
#include <iomanip>
#include <cmath>
#include "ufVincenty.h"

using std::sin;
using std::asin;
using std::cos;
using std::acos;
using std::abs;
using std::pow;
using std::tan;
using std::atan;
using std::atan2;
using std::sqrt;

using namespace UF::GeographicConversions;

Vincenty::Vincenty (std::string const & EllipsoidName) :
    Ellipsoid( EllipsoidName )
    , pi(4.0*std::atan(1.0))
    , twopi(8.0*std::atan(1.0))
    , halfpi(2.0*std::atan(1.0))
    , rtd(45.0/std::atan(1.0))
    , dtr(std::atan(1.0)/45.0)
    , tol0(5.0e-15)
    , tol1(5.0e-14)
    , tol2(5.0e-3)
    , tol3(5.0e-5)
    , tol4(5.0e-13)
{}


void Vincenty::inverse(double const & lat1, double const & lon1, double const & lat2, double const & lon2,
                       double & ellipsoidalDistance, double & forwardAzimuth, double & reverseAzimuth)
{

  double latr1 = lat1 * this->dtr;
  double lonr1 = lon1 * this->dtr;
  double latr2 = lat2 * this->dtr;
  double lonr2 = lon2 * this->dtr;

  double az1;
  double az2;

  if ( lonr1 < 0 )
  {
    lonr1 += this->twopi;
  }

  if ( lonr2 < 0 )
  {
    lonr2 += this->twopi;
  }

  if ( abs(lonr1 - lonr2) < this->tol1 && abs(latr1 - latr2) < this->tol1 )
  {
    // Same point
    ellipsoidalDistance = forwardAzimuth = reverseAzimuth = 0.0;
    return;
  }

  this->vininv(latr1,lonr1,latr2,lonr2,ellipsoidalDistance,az1,az2);

  if ( ellipsoidalDistance < this->tol3 )
  {
    az1 = 0;
    az2 = 0;
  }

  // Check to see if the points lie on a meridian.
  if ( abs(lonr1 - lonr2) < this->tol1 )
  {
    // The points lie on a meridian.
    if ( latr1 > 0 && latr2 < 0 )
    {
      // N->S
      az1 = this->pi;
      az2 = 0;
    }
    else
    {
      if ( latr1 < 0 && latr2 > 0 )
      {
        // S->N
        az1 = 0;
        az2 = this->pi;;
      }
      else // latr1 and latr2 lie in the same hemisphere.
      {
        if ( latr1 < latr2 )
        {
          // S-> N
          az1 = 0;
          az2 = this->pi;
        }
        else
        {
          // N->S
          az1 = this->pi;
          az2 = 0;
        }
      }
    }
  }

  forwardAzimuth = this->mod(az1 * this->rtd,360.0);
  reverseAzimuth = this->mod(az2 * this->rtd,360.0);
}

void Vincenty::vininv ( double const & p1, double const & e1, double const & p2, double const & e2,
                        double & s, double & az1, double & az2)
{
  s = az1 = az2 = 0;

  double const f = this->Flattening();

  // Warning: p1, p2 !=. 90;
  double u1 = atan((1.0 - f)*tan(p1));
  double u2 = atan((1.0 - f)*tan(p2));

  double su1 = sin(u1);
  double cu1 = cos(u1);

  double su2 = sin(u2);
  double cu2 = cos(u2);


  double dlon = e2 - e1;
  double w = dlon;
  double clon;
  double slon;
  double diff;
  double salf;
  double c2alf;
  double csig;
  double ssig;
  double c2sigm;
  double sig;
  double iter = 0;

  // This converges quite quickly (around 5 iterations).
  do
  {
    iter++;

    double olddlon = dlon;

    clon = cos(dlon);
    slon = sin(dlon);

    double t1 = cu2*slon;
    double t2 = cu1*su2 - su1*cu2*clon;
    ssig = sqrt(t1*t1 + t2*t2);
    csig = su1*su2 + cu1*cu2*clon;

    sig = atan2(ssig,csig);
    salf = cu1*cu2*slon/ssig;
    c2alf = 1.0 - salf*salf;

    c2sigm = csig-2.0*su1*su2/c2alf;

    double C = (f/16.0)*c2alf*(4.0 + f*(4.0 - 3.0*c2alf));

    double del = (1 - C)*f*salf*(sig + C*ssig*(c2sigm + C*csig*(-1.0 + 2.0*c2sigm*c2sigm)));

    dlon = w + del;

    diff = dlon - olddlon;

  }
  while ( iter < 15 && abs(diff) > this->tol0 );

  double const a = this->SemimajorAxis();
  double const b = this->SemiminorAxis();

  double calf = cos(asin(salf));

  double u = calf*calf*((a*a - b*b)/(b*b)); // u

  double A = 1.0 + (u/16384.0)*(4096.0 + u*(-768.0 + u*(320.0 - 175.0*u)));

  double B = (u/1024.0)*(256.0 + u*(-128.0 + u*(74.0 - 47.0*u)));

  double dsig = B*ssig*(c2sigm + (B/4.0)*(csig*(-1.0 + 2.0*c2sigm*c2sigm)-(B/6.0)*c2sigm*(-3.0 + 4.0*ssig*ssig*(-3.0 + 4.0*c2sigm*c2sigm))));

   s = b*A*(sig - dsig);

   clon = cos(dlon);
   slon = sin(dlon);
   az1 = atan2(cu2*slon,cu1*su2 - su1*cu2*clon);
   az2 = atan2(cu1*slon,-su1*cu2 + cu1*su2*clon) + this->pi;
}

void Vincenty::direct(double const & lat1, double const & lon1,  double const & ellipsoidalDistance, double const & forwardAzimuth,
            double & lat2, double & lon2, double & reverseAzimuth)
{
  double latr1 = lat1 * this->dtr;
  double lonr1 = lon1 * this->dtr;
  double az1 = forwardAzimuth * this->dtr;

  double latr2;
  double lonr2;
  double az2;

  this->vdirect (latr1,lonr1,ellipsoidalDistance,az1,latr2,lonr2,az2);

  lat2 = latr2 * this->rtd;
  lon2 = lonr2 * this->rtd;
  reverseAzimuth = this->mod(az2 * this->rtd,360.0);
}

void Vincenty::vdirect ( double const & p1, double const & e1, double const & s, double const & az1,
               double & p2, double & e2, double & az2)
{
  p2 = e2 = az2 = 0.0;

  double const f = this->Flattening();

  // Warning: p1 !=. 90;
  double tu1 = (1.0 - f)*tan(p1);
  double u1 = atan(tu1);

  double su1 = sin(u1);
  double cu1 = cos(u1);

  double saz1 = sin(az1);
  double caz1 = cos(az1);

  double salf = cu1*saz1;
  double alf = asin(cu1*saz1);
  double calf = cos(alf);

  double const a = this->SemimajorAxis();
  double const b = this->SemiminorAxis();

  double u = calf*calf*((a*a - b*b)/(b*b)); // u

  double A = 1.0 + (u/16384.0)*(4096.0 + u*(-768.0 + u*(320.0 - 175.0*u)));

  double B = (u/1024.0)*(256.0 + u*(-128.0 + u*(74.0 - 47.0*u)));

  double sig1 = atan2(tu1,caz1);

  double sig = s/(b*A);

  double ssig;
  double csig;
  double sigold;
  double c2sigm;
  double twosigm;

  double diff;
  double iter = 0;
  do
  {
    iter++;

    ssig = sin(sig);
    csig = cos(sig);

    twosigm = 2.0*sig1 + sig;

    c2sigm = cos(twosigm);

    double dsig = B*ssig*(c2sigm + (B/4.0)*(csig*(-1.0 + 2.0*c2sigm*c2sigm)-(B/6.0)*c2sigm*(-3.0 + 4.0*ssig*ssig*(-3.0 + 4.0*c2sigm*c2sigm))));

    sigold = sig;
    sig = s/(b*A) + dsig;
    diff = sig - sigold;
  }
  while ( iter < 15 && abs(diff) > this->tol0 );

  ssig = sin(sig);
  csig = cos(sig);

  double t0 = su1*csig + cu1*ssig*caz1;
  double t1 = su1*ssig - cu1*csig*caz1;
  t1 = (1.0 - f)*sqrt(salf*salf + t1*t1);

  p2 = atan2(t0,t1);

  t0 = ssig*saz1;
  t1 = cu1*csig - su1*ssig*caz1;

  double l = atan2(t0,t1);

  double c2alf = calf * calf;
  double C = (f/16.0)*c2alf*(4.0 + f*(4.0 - 3.0*c2alf));

  twosigm = 2.0*sig1 + sig;
  c2sigm = cos(twosigm);
  double w = l - (1 - C)*f*salf*(sig + C*ssig*(c2sigm + C*csig*(-1.0 + 2.0*c2sigm*c2sigm)));

  e2 = e1 + w;

  if ( e2 < -this->pi )
  {
    e2 += this->twopi;
  }
  if ( e2 > this->pi )
  {
    e2 -= this->twopi;
  }

  az2 = atan2(salf,(-su1*ssig + cu1*csig*caz1)) + this->pi;
}
