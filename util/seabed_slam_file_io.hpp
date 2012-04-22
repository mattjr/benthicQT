//!
//! \file seabed_slam_file_io.hpp
//!
//! Functions to read and write the output files of seabed_slam.
//!
#ifndef SEABED_SLAM_FILE_IO_HPP
#define SEABED_SLAM_FILE_IO_HPP

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

#include <iostream>

//----------------------------------------------------------------------------//
//   Constants                                                                //
//----------------------------------------------------------------------------//

//! This value is reported for the altitude in vehicle and stereo pose
//! estimates if no altitude measurement is available.
#define AUV_NO_ALTITUDE 0


//----------------------------------------------------------------------------//
//   Output File Names                                                        //
//----------------------------------------------------------------------------//

#define VEH_POSE_EST_FILE_NAME    "vehicle_pose_est.data"
#define VEH_POSE_COV_FILE_NAME    "vehicle_pose_cov.data"
#define STEREO_POSE_EST_FILE_NAME "stereo_pose_est.data"
#define SOUND_SPEED_FILE_NAME     "sound_speed.data"
#define STEREO_OBS_FILE_NAME      "stereo_obs_info.data"
#define IMAGE_FEATURE_FILE_NAME	 "image_features.data"
#define IMAGE_LABEL_FILE_NAME     "image_label.data"
#define GPS_OBS_FILE_NAME         "gps_obs.data"


//----------------------------------------------------------------------------//
//   IO Exceptions                                                            //
//----------------------------------------------------------------------------//
// FIXME: All the file reading stuff should be put in a namespace (perhaps
//        in auv_data_tools) with a namespace. Use a common exception
//        hierarchy without the huge Seabed_SLAM_IO prefix

//! Base class for exceptions thrown while reading or writing seabed slam 
//! output files 
class Seabed_SLAM_IO_Exception : public std::runtime_error 
{
protected:
   Seabed_SLAM_IO_Exception( const std::string &error );
};

//! Exception thrown when an input file is missing, or an output file cannot
//! be created
class Seabed_SLAM_IO_File_Exception : public Seabed_SLAM_IO_Exception
{
public:
   Seabed_SLAM_IO_File_Exception( const std::string &error );
};

//! Exception thrown when a file cannot be parsed as expected
class Seabed_SLAM_IO_Parse_Exception : public Seabed_SLAM_IO_Exception
{
public:
   Seabed_SLAM_IO_Parse_Exception( const std::string &error );
};

//! Exception thrown when an obsolete file format is encountered
class Seabed_SLAM_IO_Obsolete_File_Exception : public Seabed_SLAM_IO_Exception
{
public:
   Seabed_SLAM_IO_Obsolete_File_Exception( const std::string &error );
};



//----------------------------------------------------------------------------//
//   Vehicle Pose Estimate File                                               //
//----------------------------------------------------------------------------//

//! Information about a vehicle pose augmented to the SLAM state vector.
class Vehicle_Pose
{
public:
   Vehicle_Pose( void );

   unsigned int pose_id;
   double pose_time;
   double latitude;
   double longitude;
   std::vector<double> pose_est; // size AUV_NUM_POSE_STATES
   double altitude;              // Altitude above the seafloor (not altitude 
                                 // above the ellipsoid/geoid from GPS)
};


//! Data stored in a vehicle pose file
class Vehicle_Pose_File
{
public:
   double origin_latitude;  //!< Latitude defining the origin of the X/Y coords
   double origin_longitude; //!< Longitude defining the origin of the X/Y coords

   std::vector<Vehicle_Pose> poses;
};


//! Read a vehicle pose estimate file
Vehicle_Pose_File read_vehicle_pose_est_file( const std::string &file_name );


//! Write a vehicle pose estimate file
void write_vehicle_pose_est_file( const std::string &file_name,
                                  const std::string &custom_header,
                                  const Vehicle_Pose_File &data );


//----------------------------------------------------------------------------//
//   Stereo Pose Estimate File                                                //
//----------------------------------------------------------------------------//

//! Information about a stereo-rig pose augmented to the SLAM state vector.
class Stereo_Pose
{
public:
   Stereo_Pose( void );
 
   unsigned int pose_id;
   double pose_time;
   double latitude;
   double longitude;
   std::vector<double> pose_est; // size AUV_NUM_POSE_STATES
   
   std::string left_image_name;
   std::string right_image_name;
   double altitude; // Altitude above the seafloor (not altitude above the 
                    // ellipsoid/geoid from GPS)
   double image_footprint_radius;
   bool likely_overlap;
   std::string dir_name;
};


//! Data stored in a stereo pose file
class Stereo_Pose_File
{
public:
   double origin_latitude;  //!< Latitude defining the origin of the X/Y coords
   double origin_longitude; //!< Longitude defining the origin of the X/Y coords

   std::vector<Stereo_Pose> poses;
};


//! Read a stereo pose estimate file
Stereo_Pose_File read_stereo_pose_est_file( const std::string &file_name );


//! Write a stereo pose estimate file
void write_stereo_pose_est_file( const std::string &file_name,
                                 const std::string &custom_header,
                                 const Stereo_Pose_File &data );



//----------------------------------------------------------------------------//
//   Sound Speed File                                                         //
//----------------------------------------------------------------------------//

//! Information about sound speed at pose estimates.
class Sound_Speed
{
public:
   unsigned int pose_id;
   double pose_time;
   double ss_head;
   double ss_mean;
};

//! Read a sound speed estimate file
std::vector<Sound_Speed> read_sound_speed_file( const std::string &file_name );

//! Write a sound speed estimate file
void write_sound_speed_file( const std::string &file_name,
                             const std::string &custom_header,
                             const std::vector<Sound_Speed> &data );


//----------------------------------------------------------------------------//
//   Tide Correction File                                                     //
//----------------------------------------------------------------------------//

//! Information about tide correction 
class Tide_Correction
{
public:
   std::string year;
   std::string time;
   double timestamp;
   double dz;
};

//! Read a sound speed estimate file
std::vector<Tide_Correction> 
read_tide_correction_file( const std::string &file_name );


//----------------------------------------------------------------------------//
//   Vehicle Pose Cov File                                                    //
//----------------------------------------------------------------------------//

// FIXME: 
// * Replace ublas vectors and matrix with std::vector to remove this dependency
#if 0
#include <libplankton/auv_matrix.hpp>


//! Information about a vehicle pose and associated covariance augmented to the
//! SLAM state vector.
class Vehicle_Pose_Cov
{
public:
   Vehicle_Pose_Cov( void );

   unsigned int pose_id;
   double pose_time;
   libplankton::Vector pose_est;  // AUV_NUM_POSE_STATES
   libplankton::Matrix pose_cov;  // AUV_NUM_POSE_STATES x AUV_NUM_POSE_STATES
};


//! Read a vehicle pose covariance file
std::vector<Vehicle_Pose_Cov> 
read_vehicle_pose_cov_file( const std::string &file_name );


//! Write a vehicle pose covariance file
void write_vehicle_pose_cov_file( const std::string &file_name,
                                  const std::string &custom_header,
                                  const std::vector<Vehicle_Pose_Cov> &data );


//----------------------------------------------------------------------------//
//   Vehicle Estimate File                                                    //
//----------------------------------------------------------------------------//

class Vehicle_Est
{
public:
   Vehicle_Est( void );
   
   double time;                   // Timestamp
   libplankton::Vector est;       // Vehicle state estimate 
   libplankton::Vector cov_diag;  // Diagonal of the covariance matrix
};  

//! Read a vehicle pose covariance file
std::vector<Vehicle_Est> read_vehicle_est_file( const std::string &file_name );


//! Write a vehicle pose covariance file
void write_vehicle_est_file( const std::string &file_name,
                             const std::string &custom_header,
                             const std::vector<Vehicle_Est> &data );


//----------------------------------------------------------------------------//
//   Relative Pose Observation Info File                                      //
//----------------------------------------------------------------------------//

//! Source of a relative pose observation applied to the SLAM filter
typedef enum
{
   REL_POSE_SOURCE_AUTO_ODOM=0, // Found automatically during incremental 
                                // processing for odometry observations
   REL_POSE_SOURCE_AUTO_LC=1,   // Found automatically during incremental 
                                // processing for loop-closure observations
   REL_POSE_SOURCE_AUTO_OPT=2,  // Found automatically during map optimisation
   REL_POSE_SOURCE_MANUAL=3     // Found during a manual search
} Rel_Pose_Source;


//! Information about a relative pose observation
class Rel_Pose_Obs_Info
{
public:
   Rel_Pose_Obs_Info( void );
   Rel_Pose_Obs_Info( unsigned int pose1_id, unsigned int pose2_id,
                      Rel_Pose_Source source,
                      const std::string &cache_key );

   unsigned int pose1_id;
   unsigned int pose2_id;
   Rel_Pose_Source source;
   std::string cache_key;
};


//! Read a relative pose observation info file
std::vector<Rel_Pose_Obs_Info> 
read_rel_pose_obs_file( const std::string &file_name );


//! Write a relative pose observation info file
void write_rel_pose_obs_file( const std::string &file_name,
                              const std::string &custom_header,
                              const std::vector<Rel_Pose_Obs_Info> &data );

//----------------------------------------------------------------------------//
//   Image Feature File                                                       //
//----------------------------------------------------------------------------//
#endif
// Information about the image features
class Image_Feats
{
public:

   // Some properties of the features
   static const unsigned int LAB_LENGTH = 3;
   static const unsigned int LBP_LENGTH = 10;

   // Image features
   unsigned int pose_id;
   double pose_time;
   std::string left_image_name;
   std::string right_image_name;
   double sp_rgsty;
   double sp_slope;
   double sp_aspct;
   double m5_rgsty;
   double m5_slope;
   double m5_aspct;
   double m10_rgsty;
   double m10_slope;
   double m10_aspct; 
   double m20_rgsty;
   double m20_slope;
   double m20_aspct;
   double meanmod[LAB_LENGTH];
   double stdmod[LAB_LENGTH];
   double segsize;
   double lbp[LBP_LENGTH];
   double stdgray;
   double meangray; 
};

// Read an image feature file
std::vector<Image_Feats> read_image_feature_file( const std::string &file_name );

// Write an image feature file TODO
//void write_image_feature_file( const std::string &file_name,
//                             const std::string &custom_header,
//                             const std::vector<Image_Label> &data );



//----------------------------------------------------------------------------//
//   Image Label File                                                         //
//----------------------------------------------------------------------------//

// Information about sound speed at pose estimates.
class Image_Label
{
public:
   unsigned int pose_id;
   double pose_time;
   std::string left_image_name;
   std::string right_image_name;
   unsigned int label;
};

// Read an image label file
std::vector<Image_Label> read_image_label_file( const std::string &file_name );

// Write an image label file
void write_image_label_file( const std::string &file_name,
                             const std::string &custom_header,
                             const std::vector<Image_Label> &data );



//----------------------------------------------------------------------------//
//   GPS Observation File                                                     //
//----------------------------------------------------------------------------//

// GPS Observation information
class GPS_Obs
{
public:
   GPS_Obs( );
   GPS_Obs( double time, double latitude, double longitude,
            double northins, double eastings );

   double time;
   double latitude;
   double longitude;
   double northings;
   double eastings;
};

// Read a GPS observation file
std::vector<GPS_Obs> read_gps_obs_file( const std::string &file_name );

// Write an image label file
void write_gps_obs_file( const std::string &file_name,
                         const std::string &custom_header,
                         const std::vector<GPS_Obs> &data );


//----------------------------------------------------------------------------//
//   Backward Compatibility (Don't Use)                                       //
//----------------------------------------------------------------------------//

// Only to be used by the utilities update_vehicle_pose_file and 
// update_stereo_pose_file
namespace seabed_slam_io_private 
{

#define STEREO_ASSOC_FILE_NAME "stereo_assoc.data"

//! Information about features associated between two pairs of stereo images.
class Stereo_Assoc_Data
{
public:
   std::string pose1_left_image_name;
   std::string pose1_right_image_name;
   std::string pose2_left_image_name;
   std::string pose2_right_image_name;

   unsigned int num_features;
   std::vector<double> pose1_left_coords;
   std::vector<double> pose1_right_coords;
   std::vector<double> pose2_left_coords;
   std::vector<double> pose2_right_coords;
};


//! Read a stereo feature association file
std::vector<Stereo_Assoc_Data> 
read_stereo_assoc_file( const std::string &file_name );


//
// Read an obsolete vehicle pose estimate file
//
// This reads the obsolete versions 1 and 2 of the vehicle pose file format,
// and fills in the misssing latitude and longitude of each pose by reverting 
// the UTM map projection previously used by seabed_slam to obtain the 
// X/Y coordinates. (Note: seabed_slam now uses a local transverse Mercator
// projection).
//
// The errors due to the inappropriate map projection used to produce the 
// results in these files will still remain, and it is recommended you redo
// the SLAM processing to produce better results.
//
// This function should only be used by the utility update_vehicle_pose_file
//
Vehicle_Pose_File read_obsolete_vehicle_pose_est_file( 
                                                   const std::string &file_name,
                                                   double origin_latitude,
                                                   double origin_longitude );

//
// Read an obsolete stereo pose estimate file
//
// This reads the obsolete version 1 of the stereo pose file format,
// and fills in the misssing latitude and longitude of each pose by reverting 
// the UTM map projection previously used by seabed_slam to obtain the 
// X/Y coordinates. (Note: seabed_slam now uses a local transverse Mercator
// projection).
//
// The errors due to the inappropriate map projection used to produce the 
// results in these files will still remain, and it is recommended you redo
// the SLAM processing to produce better results.
//
// This function should only be used by the utility update_stereo_pose_file
//
Stereo_Pose_File read_obsolete_stereo_pose_est_file( 
                                                   const std::string &file_name,
                                                   double origin_latitude,
                                                   double origin_longitude );

} // namespace seabed_slam_io_private

#endif // !SEABED_SLAM_FILE_IO_HPP

