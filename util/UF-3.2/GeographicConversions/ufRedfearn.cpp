
#include <cmath>
#include <sstream>

#include "ufRedfearn.h"

using namespace UF::GeographicConversions;

Redfearn::Redfearn (std::string const & EllipsoidName, std::string const & GridName) :
    Ellipsoid( EllipsoidName )
    , MapGrid ( GridName )
    , rtd(45.0/std::atan(1.0))
    , dtr(std::atan(1.0)/45.0)
{}

void Redfearn::GetGridCoordinates (
    double const & Latitude, double const & Longitude,
    std::string & Zone, double & Easting, double & Northing,
    double & GridConvergence, double & PointScale)
{
  // Normalise the longitude ( -180 <= longitude < 180 )
  double lonN = (Longitude+180)-int((Longitude+180)/360)*360-180;

  int ZoneNumber = int((lonN-this->LWEZone0())/this->ZoneWidth());

  if( Latitude >= 56.0 && Latitude < 64.0 && lonN >= 3.0 && lonN < 12.0 )
    ZoneNumber = 32;

  // Special zones for Svalbard
  if( Latitude >= 72.0 && Latitude < 84.0 )
  {
    if( lonN >= 0.0  && lonN <  9.0 ) ZoneNumber = 31;
    else if( lonN >= 9.0  && lonN < 21.0 ) ZoneNumber = 33;
    else if( lonN >= 21.0 && lonN < 33.0 ) ZoneNumber = 35;
    else if( lonN >= 33.0 && lonN < 42.0 ) ZoneNumber = 37;
  }
  char ZoneLetter = UTMLetterDesignator(Latitude);
  std::ostringstream sstr;
  sstr << ZoneNumber << ZoneLetter;
  Zone = sstr.str();

  this->GridCoordinates (
      Latitude, lonN, ZoneNumber,
      Easting, Northing,
      GridConvergence, PointScale);

}

void Redfearn::GetZoneGridCoordinates (
    double const & Latitude, double const & Longitude,
    std::string const & Zone, double & Easting, double & Northing,
    double & GridConvergence, double & PointScale)
{
  // Normalise the longitude ( -180 <= longitude < 180 )
  double lonN = (Longitude+180)-int((Longitude+180)/360)*360-180;

  std::istringstream sstr(Zone);
  int ZoneNumber;
  char ZoneLetter;
  sstr >> ZoneNumber >> ZoneLetter;

  this->GridCoordinates (
      Latitude, lonN, ZoneNumber,
      Easting, Northing,
      GridConvergence, PointScale);

}

void Redfearn::GridCoordinates (
      double const & Latitude, double const & Longitude,
      const int & ZoneNumber, double & Easting, double & Northing,
      double & GridConvergence, double & PointScale)
{
  double lat = Latitude * this->dtr; // In radians.
  // The ellipsoid
  double a = this->SemimajorAxis();
  double e = this->Eccentricity();

  // Calculate the meridian distance.
  // Use a series expansion.
  double e2 = e*e;
  double e4 = e2*e2;
  double e6 = e4*e2;
  double A0 = 1-(e2/4.0)-(3.0*e4/64.0)-(5.0*e6/256.0);
  double A2 = (3.0/8.0)*(e2+e4/4.0+15.0*e6/128.0);
  double A4 = (15.0/256.0)*(e4+3.0*e6/4.0);
  double A6 = 35.0*e6/3072.0;
  double s = std::sin(lat);
  double s2 = std::sin(2.0*lat);
  double s4 = std::sin(4.0*lat);
  double s6 = std::sin(6.0*lat);
  double m = a*(A0*lat-A2*s2+A4*s4-A6*s6);

  // Radii of curvature.
  double rho = a*(1-e2)/std::pow(1.0-e2*s*s,3.0/2.0);
  double nu = a/std::sqrt(1-e2*s*s);
  double psi = nu / rho;
  double psi2 = psi*psi;
  double psi3 = psi*psi2;
  double psi4 = psi*psi3;

  // Geographical to Grid

  double LongCMZone = ZoneNumber * this->ZoneWidth() + this->CMZone0();
  double w = (Longitude - LongCMZone)*this->dtr;
  double w2 = w*w;
  double w3 = w*w2;
  double w4 = w*w3;
  double w5 = w*w4;
  double w6 = w*w5;
  double w7 = w*w6;
  double w8 = w*w7;

  double c = std::cos(lat);
  double c2 = c*c;
  double c3 = c*c2;
  double c4 = c*c3;
  double c5 = c*c4;
  double c6 = c*c5;
  double c7 = c*c6;

  double t = std::tan(lat);
  double t2 = t*t;
  double t3 = t*t2;
  double t4 = t*t3;
  double t5 = t*t4;
  double t6 = t*t5;

  // Northing
  double term1 = w2*c/2.0;
  double term2 = w4*c3*(4.0*psi2+psi-t2)/24.0;
  double term3 = w6*c5*(8.0*psi4*(11.0-24.0*t2)-28*psi3*(1-6.0*t2)+psi2*(1-32*t2)-psi*(2.0*t2)+t4)/720.0;
  double term4 = w8*c7*(1385.0-3111.0*t2+543.0*t4-t6)/40320.0;
  if ( Latitude < 0 )
    Northing = this->CentralScaleFactor()*(m+nu*s*(term1+term2+term3+term4))+this->FalseNorthing();
  else
    Northing = this->CentralScaleFactor()*(m+nu*s*(term1+term2+term3+term4));

  // Easting
  term1 = w*c;
  term2 = w3*c3*(psi-t2)/6.0;
  term3 = w5*c5*(4.0*psi3*(1.0-6.0*t2)+psi2*(1.0+8.0*t2)-psi*(2.0*t2)+t4)/120.0;
  term4 = w7*c7*(61.0-479.0*t2+179.0*t4-t6)/5040.0;
  Easting = nu*this->CentralScaleFactor()*(term1+term2+term3+term4)+this->FalseEasting();

  // Grid Convergence
  term1 = -w;
  term2 = -w3*c2*(2.0*psi2-psi)/3.0;
  term3 = -w5*c4*(psi4*(11.0-24.0*t2)-psi3*(11.0-36.0*t2)+2.0*psi2*(1.0-7.0*t2)+psi*t2)/15.0;
  term4 = w7*c6*(17.0-26.0*t2+2.0*t4)/315.0;
  GridConvergence = s*(term1+term2+term3+term4)*this->rtd;

  // Point Scale
  term1 = 1.0+(w2*c2*psi)/2.0;
  term2 = w4*c4*(4.0*psi3*(1.0-6.0*t2)+psi2*(1.0+24.0*t2)-4.0*psi*t2)/24.0;
  term3 = w6*c6*(61.0-148.0*t2+16.0*t4)/720.0;
  PointScale = this->CentralScaleFactor()*(term1+term2+term3);
}


void Redfearn::GetGeographicCoordinates (
    std::string const &Zone, double const & Easting, double const & Northing,
    double & Latitude, double & Longitude,
    double & GridConvergence, double & PointScale)
{
  std::istringstream sstr(Zone);
  int ZoneNumber;
  char ZoneLetter;
  bool NorthernHemisphere = false;
  sstr >> ZoneNumber >> ZoneLetter;
  if ( ZoneLetter - 'N' >= 0 )
    NorthernHemisphere = true;
  else
    NorthernHemisphere = false;
  this->GeographicCoordinates (
      ZoneNumber,NorthernHemisphere,
      Easting, Northing,
      Latitude, Longitude,
      GridConvergence, PointScale);
}

void Redfearn::GetGeographicCoordinatesNS (
    std::string const &ZoneHemisphere, double const & Easting, double const & Northing,
    double & Latitude, double & Longitude,
    double & GridConvergence, double & PointScale)
{
  std::istringstream sstr(ZoneHemisphere);
  int ZoneNumber;
  char Hemisphere;
  bool NorthernHemisphere = false;
  sstr >> ZoneNumber >> Hemisphere;
  if ( Hemisphere == 'N' )
    NorthernHemisphere = true;
  else
    NorthernHemisphere = false;
  this->GeographicCoordinates (
      ZoneNumber,NorthernHemisphere,
      Easting, Northing,
      Latitude, Longitude,
      GridConvergence, PointScale);
}

void Redfearn::GeographicCoordinates (
    int const & ZoneNumber, bool const & NorthernHemisphere,
    double const & Easting, double const & Northing,
    double & Latitude, double & Longitude,
    double & GridConvergence, double & PointScale)
{
  double m;
  if ( NorthernHemisphere )
    m = Northing/this->CentralScaleFactor();
  else
    m = (Northing-this->FalseNorthing())/this->CentralScaleFactor();

  // The ellipsoid
  double a = this->SemimajorAxis();
  double e = this->Eccentricity();

  // Calculate the meridian distance.
  // Use a series expansion.
  double e2 = e*e;
  //double e4 = e2*e2;
  //double e6 = e4*e2;
  //double m = (Northing-this->FalseNorthing())/this->CentralScaleFactor();

  // Foot-point latitude
  // This is the latitude for which the meridian distance equals
  // the true northing divided by the central scale factor.
  double n = this->N();
  double n2 = n*n;
  double n3 = n*n2;
  double n4 = n*n3;
  double sig = m / this->G() * this->dtr;
  double sig2 = std::sin(2.0*sig);
  double sig4 = std::sin(4.0*sig);
  double sig6 = std::sin(6.0*sig);
  double sig8 = std::sin(8.0*sig);
  double fpl = sig+((3.0*n/2.0)-(27.0*n3/32.0))*sig2+
    ((21.0*n2/16.0)-(55.0*n4/32.0))*sig4+(151.0*n3/96.0)*sig6+
    (1097.0*n4/512.0)*sig8;

  double s = std::sin(fpl);

  // Radii of curvature.
  double rho = a*(1-e2)/std::pow(1.0-e2*s*s,3.0/2.0);
  double nu = a/std::sqrt(1-e2*s*s);
  double psi = nu / rho;
  double psi2 = psi*psi;
  double psi3 = psi*psi2;
  double psi4 = psi*psi3;

  double ss = 1.0/std::cos(fpl); // secant

  double t = std::tan(fpl);
  double t2 = t*t;
  double t3 = t*t2;
  double t4 = t*t3;
  double t5 = t*t4;
  double t6 = t*t5;

  //double tknu = t / (this->CentralScaleFactor() * nu );

  double K0 = this->CentralScaleFactor();
  double E = Easting-this->FalseEasting();
  double Ek = E/K0;

  // Ok generate some more powers.
  double x = Ek/nu;
  double x2 = x*x;
  double x3 = x*x2;
  double x4 = x*x3;
  double x5 = x*x4;
  double x6 = x*x5;
  double x7 = x*x6;

  double y = Ek*Ek/(nu*rho);
  double y2 = y*y;
  double y3 = y*y2;

  // Latitude
  double term1 = (x*E/2.0);
  double term2 = (x3*E/24.0)*(-4.0*psi2+9.0*psi*(1.0-t2)+12.0*t2);
  double term3 = (x5*E/720.0)*(8.0*psi4*(11.0-24.0*t2)-12.0*psi3*(21.0-71.0*t2)+15.0*psi2*(15.0-98.0*t2+15*t4)+180.0*psi*(5.0*t2-3.0*t4)+360*t4);
  double term4 = (x7*E/40320.0)*(1385.0+3633.0*t2+4095.0*t4+1575.0*t6);
  Latitude = (fpl+(t/(K0*rho))*(-term1+term2-term3+term4))*this->rtd;

  // Longitude
  double CentralMeridian = ZoneNumber*this->ZoneWidth()+this->CentralMeridianZone1()-this->ZoneWidth();
  term1 = x;
  term2 = (x3/6.0)*(psi+2.0*t2);
  term3 = (x5/120.0)*(-4.0*psi3*(1.0-6.0*t2)+psi2*(9.0-68.0*t2)+72.0*psi*t2+24.0*t4);
  term4 = (x7/5040.0)*(61.0+662.0*t2+1320.0*t4+720.0*t6);
  Longitude = CentralMeridian+ss*(term1-term2+term3-term4)*this->rtd;

  // Grid convergence
  term1 = -t*x;
  term2 = (t*x3/3.0)*(-2.0*psi2+3.0*psi+t2);
  term3 = -(t*x5/15.0)*(psi4*(11.0-24.0*t2)-3.0*psi3*(8.0-23.0*t2)+5.0*psi2*(3.0-14.0*t2)+30.0*psi*t2+3.0*t4);
  term4 = (t*x7/315.0)*(17.0+77.0*t2+105.0*t4+45.0*t6);
  GridConvergence = (term1+term2+term3+term4)*this->rtd;

  // Point scale
  term1 = 1.0+y/2.0;
  term2 = (y2/24.0)*(4.0*psi*(1-6*t2)-3*(1-16*t2)-24*t2/psi);
  term3 = y3/720.0;
  PointScale = this->CentralScaleFactor()*(term1+term2+term3);
}

char Redfearn::UTMLetterDesignator(double const & Latitude)
{
  char LetterDesignator;

  if((84 >= Latitude) && (Latitude >= 72)) LetterDesignator = 'X';
  else if((72 > Latitude) && (Latitude >= 64)) LetterDesignator = 'W';
  else if((64 > Latitude) && (Latitude >= 56)) LetterDesignator = 'V';
  else if((56 > Latitude) && (Latitude >= 48)) LetterDesignator = 'U';
  else if((48 > Latitude) && (Latitude >= 40)) LetterDesignator = 'T';
  else if((40 > Latitude) && (Latitude >= 32)) LetterDesignator = 'S';
  else if((32 > Latitude) && (Latitude >= 24)) LetterDesignator = 'R';
  else if((24 > Latitude) && (Latitude >= 16)) LetterDesignator = 'Q';
  else if((16 > Latitude) && (Latitude >= 8)) LetterDesignator = 'P';
  else if(( 8 > Latitude) && (Latitude >= 0)) LetterDesignator = 'N';
  else if(( 0 > Latitude) && (Latitude >= -8)) LetterDesignator = 'M';
  else if((-8> Latitude) && (Latitude >= -16)) LetterDesignator = 'L';
  else if((-16 > Latitude) && (Latitude >= -24)) LetterDesignator = 'K';
  else if((-24 > Latitude) && (Latitude >= -32)) LetterDesignator = 'J';
  else if((-32 > Latitude) && (Latitude >= -40)) LetterDesignator = 'H';
  else if((-40 > Latitude) && (Latitude >= -48)) LetterDesignator = 'G';
  else if((-48 > Latitude) && (Latitude >= -56)) LetterDesignator = 'F';
  else if((-56 > Latitude) && (Latitude >= -64)) LetterDesignator = 'E';
  else if((-64 > Latitude) && (Latitude >= -72)) LetterDesignator = 'D';
  else if((-72 > Latitude) && (Latitude >= -80)) LetterDesignator = 'C';
  else LetterDesignator = 'Z'; //This is here as an error flag to show that the Latitude is outside the UTM limits

  return LetterDesignator;
}

int Redfearn::UTMLetterNumber(char const & Letter)
{
  int LetterNumber;

  switch ( Letter )
  {
  case 'X':
    LetterNumber = 0;
    break;
  case 'W':
    LetterNumber = 1;
    break;
  case 'V':
    LetterNumber = 2;
    break;
  case 'U':
    LetterNumber = 3;
    break;
  case 'T':
    LetterNumber = 4;
    break;
  case 'S':
    LetterNumber = 5;
    break;
  case 'R':
    LetterNumber = 6;
    break;
  case 'Q':
    LetterNumber = 7;
    break;
  case 'P':
    LetterNumber = 8;
    break;
  case 'N':
    LetterNumber = 9;
    break;
  case 'M':
    LetterNumber = 10;
    break;
  case 'L':
    LetterNumber = 11;
    break;
  case 'K':
    LetterNumber = 12;
    break;
  case 'J':
    LetterNumber = 13;
    break;
  case 'H':
    LetterNumber = 14;
    break;
  case 'G':
    LetterNumber = 15;
    break;
  case 'F':
    LetterNumber = 16;
    break;
  case 'E':
    LetterNumber = 17;
    break;
  case 'D':
    LetterNumber = 18;
    break;
  case 'C':
    LetterNumber = 19;
  default:
    LetterNumber = -1; // Invalid letter.
  }
  return LetterNumber;
}

int Redfearn::UTMBound(char const & Letter)
{
  int UTMBound;

  switch ( Letter )
  {
  case 'X':
    UTMBound = 72;
    break;
  case 'W':
    UTMBound = 64;
    break;
  case 'V':
    UTMBound = 56;
    break;
  case 'U':
    UTMBound = 48;
    break;
  case 'T':
    UTMBound = 40;
    break;
  case 'S':
    UTMBound = 32;
    break;
  case 'R':
    UTMBound = 24;
    break;
  case 'Q':
    UTMBound = 16;
    break;
  case 'P':
    UTMBound = 8;
    break;
  case 'N':
    UTMBound = 0;
    break;
  case 'M':
    UTMBound = -8;
    break;
  case 'L':
    UTMBound = -16;
    break;
  case 'K':
    UTMBound = -24;
    break;
  case 'J':
    UTMBound = -32;
    break;
  case 'H':
    UTMBound = -40;
    break;
  case 'G':
    UTMBound = -48;
    break;
  case 'F':
    UTMBound = -56;
    break;
  case 'E':
    UTMBound = -64;
    break;
  case 'D':
    UTMBound = -72;
    break;
  case 'C':
    UTMBound = -80;
  default:
    UTMBound = -1; // Invalid letter.
  }
  return UTMBound;
}
