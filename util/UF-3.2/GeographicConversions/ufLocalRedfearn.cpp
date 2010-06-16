
#include <cmath>
#include <sstream>
#include <stdexcept>

#include "ufWin32Header.h"
#include "ufRedfearn.h"
#include "ufLocalRedfearn.h"

using namespace UF::GeographicConversions;

/*
  LocalRedfearn. 
  This class provides the ability to use a UTM projection and to specify the 
  central meridian of the first UTM zone. In this way mapping / navigation that
  is done close to the centre of this first zone can be done without the need to
  correct compass headings, and is such that lines of constant longitude match
  much more closely lines projected lines with a constant easting component. 

  WARNING:- EXTREME CARE must be used to ensure that all conversions using the
  ufLocalRedfearn classes and data are performed using the same central 
  meridian. Otherwise the results will be invalid...

  Thanks to Duncan Mercer for creating this class.
*/


//***********************************************************************

LocalRedfearn::LocalRedfearn(std::string const & EllipsoidName,
                             std::string const & GridName,
                            const double LocalCentralMeridian):
   Redfearn(EllipsoidName, GridName){

   // This modified local projection is only setup to work with UTM
   if(GridName != "UTM"){
       throw std::runtime_error
           ("ERR:- LocalRedfearn can only work with UTM grid");
   }

   // Normalise the central meridian ( -180 <= LocalCentralMeridian < 180 )
   double LocalMerid = (LocalCentralMeridian+180)-int((LocalCentralMeridian+180)/360)*360-180;


   // Update the parent-class and make it clear
   // that we are using a modified projection.
   std::stringstream localGridName;
   localGridName.precision(6);
   localGridName << "UTMLocalMeridian_"<< std::fixed << LocalMerid;
   this->mapGridName=localGridName.str();

   // Set the meridian we want for our projection.
   this->centralMeridianZone1 = LocalMerid;
}


//********************************************************************************

void LocalRedfearn::GetGridCoordinates (
    double const & Latitude, double const & Longitude,
    std::string & Zone, double & Easting, double & Northing,
    double & GridConvergence, double & PointScale)
{
    
    // Enforcing zone 1 only. Anything outside the zone 
    // should be using a different central meridian.
    int ZoneNumber = int((Longitude - this->LWEZone0())/this->ZoneWidth());
    if(ZoneNumber != 1){ZoneError_Throw();}

    return Redfearn::GetGridCoordinates
         (Latitude, Longitude,Zone,Easting,
               Northing,GridConvergence,PointScale);
}


//***********************************************************************************

void LocalRedfearn::GetZoneGridCoordinates (
    double const & Latitude, double const & Longitude,
    std::string const & Zone, double & Easting, double & Northing,
    double & GridConvergence, double & PointScale)
{
    // Enforcing zone 1 only. Anything outside the zone 
    // should be using a different central meridian.
    std::istringstream sstr(Zone);
    int ZoneNumber;
    char ZoneLetter;
    sstr >> ZoneNumber >> ZoneLetter;

    if(ZoneNumber != 1){ZoneError_Throw();}

    return Redfearn::GetZoneGridCoordinates (
        Latitude,Longitude,Zone,Easting,Northing,GridConvergence,PointScale);
 
}

//*******************************************************************************

void LocalRedfearn::GetGeographicCoordinates (
    std::string const &Zone, double const & Easting, double const & Northing,
    double & Latitude, double & Longitude,
    double & GridConvergence, double & PointScale)
{

    // Enforcing zone 1 only. Anything outside the zone 
    // should be using a different central meridian.
    std::istringstream sstr(Zone);
    int ZoneNumber;
    char ZoneLetter;
    sstr >> ZoneNumber >> ZoneLetter;
    if(ZoneNumber != 1){ZoneError_Throw();}

    return Redfearn::GetGeographicCoordinates (
        Zone, Easting, Northing, Latitude, Longitude,
        GridConvergence, PointScale);
}

//**********************************************************************************

void LocalRedfearn::GetGeographicCoordinatesNS (
    std::string const &ZoneHemisphere, double const & Easting, double const & Northing,
    double & Latitude, double & Longitude,
    double & GridConvergence, double & PointScale)
{
    // Enforcing zone 1 only. Anything outside the zone 
    // should be using a different central meridian.
    std::istringstream sstr(ZoneHemisphere);
    int ZoneNumber;
    char Hemisphere;
    sstr >> ZoneNumber >> Hemisphere;
    if(ZoneNumber != 1){ZoneError_Throw();}

    return Redfearn::GetGeographicCoordinatesNS (
        ZoneHemisphere, Easting, Northing, Latitude, 
           Longitude,GridConvergence,PointScale);
}


//****************************************************************************************


inline void LocalRedfearn::ZoneError_Throw(void){
    throw std::runtime_error
        ("ERR:- LocalRedfearn is only able to operate with points in local-UTM zone1\nusing the current central meridian");
}
