//!
//! \file auv_map_projection.cpp
//!
//! Conversion between geographic coordinates 2D map projection coordinates
//!
//! \author Ian Mahon
//! 

#include "auv_map_projection.hpp"

#include <GeographicConversions/ufLocalRedfearn.h>

using namespace UF::GeographicConversions;
using namespace std;

#define LOCAL_REDFEARN_ELLIPSOID_NAME "WGS84"
#define LOCAL_REDFEARN_GRID_NAME      "UTM"


//
// FIXME: What are grid_convergence and point_scale, and what should I do 
//        with them?
//

//! Create a Local_WGS84_TM_Projection object
Local_WGS84_TM_Projection::Local_WGS84_TM_Projection( double origin_latitude, 
                                                      double origin_longitude )
{
   double grid_convergence, point_scale;

   // Create projection object
   local_redfearn = new LocalRedfearn( LOCAL_REDFEARN_ELLIPSOID_NAME,
                                       LOCAL_REDFEARN_GRID_NAME,
                                       origin_longitude );

   // Calculate the map coords produced by LocalRedfearn for the origin.
   // These offsets will are stored in the instance variables origin_easting and
   // origin_northing, are are used to transform the map coords such that the
   // origin is (0,0).
   local_redfearn->GetGridCoordinates( origin_latitude, origin_longitude, 
                                       origin_zone, 
                                       origin_easting, origin_northing,
                                       grid_convergence, point_scale );
}


//! Local_WGS84_TM_Projection destructor
Local_WGS84_TM_Projection::~Local_WGS84_TM_Projection( void )
{
   delete local_redfearn;
}


//! Calculate map coordinates for the given geographic coordinates (latitude
//! and longitude)
void Local_WGS84_TM_Projection::calc_map_coords( double latitude, 
                                                 double longitude,
                                                 double &map_northing, 
                                                 double &map_easting ) const
{
   double grid_convergence, point_scale;
   double northing, easting;
   local_redfearn->GetZoneGridCoordinates( latitude, longitude, 
                                           origin_zone, easting, northing,
                                           grid_convergence, point_scale );

   // Remove origin offset
   map_northing = northing - origin_northing;
   map_easting = easting - origin_easting;
}


//! Calculate geographic coordinates (latitude and longitude) for the given
//! map coordinates
void Local_WGS84_TM_Projection::calc_geo_coords( double map_northing, 
                                                 double map_easting,
                                                 double &latitude, 
                                                 double &longitude ) const
{
   // Add origin offset
   double northing = map_northing + origin_northing;
   double easting = map_easting + origin_easting;

   double grid_convergence, point_scale;
   local_redfearn->GetGeographicCoordinates( origin_zone, easting, northing,
                                             latitude, longitude,
                                             grid_convergence, point_scale );
}
