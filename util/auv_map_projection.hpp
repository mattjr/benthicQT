//!
//! \file auv_map_projection.hpp
//!
//! Conversion between geographic coordinates 2D map projection coordinates
//!
//! \author Ian Mahon
//! 
#ifndef LIBPLANKTON_MAP_PROJECTION_HPP
#define LIBPLANKTON_MAP_PROJECTION_HPP

#include <string>

// Forward declaration
namespace UF{ namespace GeographicConversions{ class LocalRedfearn; } }


//!
//! A class implementing a transverse Mercator projection using the WGS84
//! ellipsoid.
//!
//! This class can be used to convert between geographic coordinate (latitude
//! longitude) and cartesian map coordinates.
//!
//! The origin of the map coords is specifed with a latitude and longitude. The 
//! central meridian of the transverse Mercator projection is the origin
//! longitude.
//!
//! The projection will be most accurate near the origin.
//!
//! This class used to create the Cartesian coordinate system used to the 
//! Seabed navigation frame. It is also used in several GUIs.
//!
//! When creating a map for external use (which you will give to someone else),
//! consider using a Universal Transverse Mercator (UTM) projection instead of
//! using this class.
//!
//! This class is a thin wrapper around the LocalRedfearn class in the UF 
//! library.
//!
class Local_WGS84_TM_Projection
{
public:
   //! Create a Local_WGS84_TM_Projection object
   Local_WGS84_TM_Projection( double origin_latitude, double origin_longitude );


   //! Local_WGS84_TM_Projection destructor
   ~Local_WGS84_TM_Projection( void );


   //! Calculate map coordinates for the given geographic coordinates (latitude
   //! and longitude)
   void calc_map_coords( double latitude, double longitude,
                         double &map_northing, double &map_easting ) const;
     
   
   //! Calculate geographic coordinates (latitude and longitude) for the given
   //! map coordinates
   void calc_geo_coords( double map_northing, double map_easting,
                         double &latitude, double &longitude ) const;

private:
   UF::GeographicConversions::LocalRedfearn *local_redfearn;
   std::string origin_zone;
   double origin_northing;
   double origin_easting;
};






#endif //!LIBPLANKTON_MAP_PROJECTION_HPP
