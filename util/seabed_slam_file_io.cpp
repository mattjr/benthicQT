//!
//! \file seabed_slam_file_io.cpp
//!
#include "seabed_slam_file_io.hpp"

#include <sstream>
#include <iomanip>


using namespace std;


// Lines in the data files beginning with this character are ignored
#define SEABED_SLAM_FILE_COMMENT_CHAR '%'

// A string used in the first line of a file specifying file type and version
#define VERSION_STRING "VERSION"

// Precision for output data
#define PRECISION 16

//----------------------------------------------------------------------------//
//   Exceptions                                                               //
//----------------------------------------------------------------------------//

Seabed_SLAM_IO_Exception::Seabed_SLAM_IO_Exception( const string &error )
   : runtime_error( error ) { }

Seabed_SLAM_IO_File_Exception::Seabed_SLAM_IO_File_Exception( const string &error )
   : Seabed_SLAM_IO_Exception( error ) { }

Seabed_SLAM_IO_Parse_Exception::Seabed_SLAM_IO_Parse_Exception( const string &error )
   : Seabed_SLAM_IO_Exception( error ) { }

Seabed_SLAM_IO_Obsolete_File_Exception::Seabed_SLAM_IO_Obsolete_File_Exception( const string &error )
   : Seabed_SLAM_IO_Exception( error ) { }




//----------------------------------------------------------------------------//
//   File Types and Versions (Private)                                        //
//----------------------------------------------------------------------------//
//
// When adding a new file type:
//  * Add a value to the 'File_Type' enumeration
//  * Update the 'file_type_strings' and 'file_type_current_versions' arrays
//
// Whenever a change is made to one of the file formats:
//  * Increment the corresponding version number in file_type_current_versions
//  * Set default values for new variables in the data class constructors, so 
//    they are initialised when reading an older file format.
//  * Modify the input and output operators. Backward compatibility should be
//    maintained by checking the version number before trying to read a new
//    variable. See the altitude variable in the vehicle pose input operator 
//    for an example.
//  * Update the comment at the top of the code for that file type which lists
//    the changes made in each version.
//
typedef enum { FILE_TYPE_VEHICLE_POSE,
               FILE_TYPE_STEREO_POSE,
               FILE_TYPE_STEREO_ASSOC,
               FILE_TYPE_SOUND_SPEED,
               FILE_TYPE_TIDE_CORRECTION,
               FILE_TYPE_VEHICLE_POSE_COV,
               FILE_TYPE_REL_POSE_OBS,
               FILE_TYPE_IMAGE_FEATURE,   
               FILE_TYPE_IMAGE_LABEL,
               FILE_TYPE_GPS_OBS,
               FILE_TYPE_VEHICLE_EST,
               NUM_FILE_TYPES } File_Type;

static const char *file_type_strings[NUM_FILE_TYPES] = 
   { "VEHICLE_POSE_FILE",      // FILE_TYPE_VEHICLE_POSE
     "STEREO_POSE_FILE",       // FILE_TYPE_STEREO_POSE
     "STEREO_ASSOC_FILE",      // FILE_TYPE_STEREO_ASSOC
     "SOUND_SPEED_FILE",       // FILE_TYPE_SOUND_SPEED
     "TIDE_CORRECTION",        // FILE_TYPE_TIDE_CORRECTION
     "VEHICLE_POSE_COV_FILE",  // FILE_TYPE_VEHICLE_POSE_COV
     "REL_POSE_OBS_INFO_FILE", // FILE_TYPE_REL_POSE_OBS
     "IMAGE_FEATURE_FILE",     // FILE_TYPE_IMAGE_FEATURE
     "IMAGE_LABEL_FILE",       // FILE_TYPE_IMAGE_LABEL
     "GPS_OBS_FILE",           // FILE_TYPE_GPS_OBS
     "VEHICLE_EST_FILE"        // FILE_TYPE_VEHICLE_EST
   };

static unsigned int file_type_current_versions[NUM_FILE_TYPES] = 
  { 3, // FILE_TYPE_VEHICLE_POSE
    2, // FILE_TYPE_STEREO_POSE
    1, // FILE_TYPE_STEREO_ASSOC
    1, // FILE_TYPE_SOUND_SPEED
    1, // FILE_TYPE_TIDE_CORRECTION
    1, // FILE_TYPE_VEHICLE_POSE_COV
    1, // FILE_TYPE_REL_POSE_OBS
    1, // FILE_TYPE_IMAGE_FEATURE
    1, // FILE_TYPE_IMAGE_LABEL
    1, // FILE_TYPE_GPS_OBS
    1  // FILE_TYPE_VEHICLE_EST
   };


// Is the given string present in the array 'file_type_strings'
static bool is_valid_file_type_string( const string &s )
{
   for( unsigned int i=0; i<NUM_FILE_TYPES; i++ )
   {
      if( s.compare(file_type_strings[i]) == 0 )
         return true;
   }
   return false;
}

//----------------------------------------------------------------------------//
//   Generic File Reading (Private)                                           //
//----------------------------------------------------------------------------//

static void open_input_stream( const string &file_name, ifstream &in_file )
{
   in_file.open( file_name.c_str() );
   if( !in_file )
   {
      stringstream ss;
      ss << "Unable to open input data file '" << file_name << "'";
      throw Seabed_SLAM_IO_File_Exception( ss.str() );
   }
}

//
// The format for a named value is a name, followed by a space,
// followed by the value.
//
template<class T>
void parse_named_value( istream &in,
                        const string &name,
                        T &data,
                        const string &comment )
{
   string current;
   if( !(in>>current) || current.compare(name) != 0  ||
       !(in>>data) )
   {
      stringstream message;
      message << "Unable to parse " << comment;
      throw Seabed_SLAM_IO_Parse_Exception( message.str() );
   }
}



// Skip over lines starting with the comment character
static void skip_comments( istream &in_file )
{
   bool done = false;
   while( !done )
   {
      int pos = in_file.tellg();

      char next_char;
      if( !(in_file >> next_char) )
         return;

      if( next_char == SEABED_SLAM_FILE_COMMENT_CHAR )
      {
         string comment_line;
         getline( in_file, comment_line );
      }
      else
      {
         in_file.seekg( pos );
         done = true;
      } 
   }
}


// Try to read the file version at the beginning of the file.
// If it doesn't exist, we assume it is version 1
static int read_file_version( File_Type file_type, istream &in_file )
{
   // Try to read the file type and version
   char c;
   string type_str;
   string ver_str;
   int version;
   if( !(in_file>>c)        || c!=SEABED_SLAM_FILE_COMMENT_CHAR || 
       !(in_file>>type_str) || is_valid_file_type_string(type_str)==false ||
       !(in_file>>ver_str)  || ver_str.compare(VERSION_STRING)!=0 ||
       !(in_file>>version) )
   {
      // File type and version info not present. The file must be version 1
      version = 1;
   }
   else
   {
      // Check the file type is as expected
      string expected_string = file_type_strings[file_type];
      if( type_str.compare(expected_string)!=0 )
      {
         stringstream ss;
         ss << "Invalid file type " << expected_string << " when expecting type "
            << type_str;
         throw Seabed_SLAM_IO_Parse_Exception( ss.str() );
      }

      // Check that the version number is sensible
      int max_version = file_type_current_versions[file_type];
      if( version > max_version )
      {
         stringstream ss;
         ss << "Invalid file version "<< version << ". Current version is "
            << max_version;
         throw Seabed_SLAM_IO_Parse_Exception( ss.str() );
      }
   }

   // Return to the start of the file and clear any error state flags
   in_file.seekg( 0, ios::beg );
   in_file.clear();

   return version;
}


//
// Try to read a value, and throw an exception if it fails
//
// FIXME: Extend this to do version checking?
//
template<class T>
static void parse( istream &in, const string &name, T &value )
{
   if( !(in>>value) )
   {
      string err = "Unable to parse " + name;
      throw Seabed_SLAM_IO_Parse_Exception( err );
   }
}

// This is a wrapper for parse() that converts values of -999 to C++ style
// NaNs.
// TODO: this is mainly for compatability with matlab. If we port all the matlab
//       scripts we can ditch this function.
template<class T>
static void parse_999( istream &in, const string &name, T &value )
{
   parse ( in, name, value);
   if (value == -999)
      value = numeric_limits<T>::quiet_NaN();
}


template<class T> 
void read_data( const string &file_name, unsigned int version, 
                istream &in_file, vector<T> &data )
{
   try
   {
      while( !in_file.eof() )
      {
         skip_comments( in_file );
         if( in_file.eof() )
            break;

         pair<unsigned int, T> new_data;
         new_data.first = version;
         if( !(in_file >> new_data ) )
         {
            stringstream ss;
            ss << "Error parsing file '" << file_name << "'";
            throw Seabed_SLAM_IO_Parse_Exception( ss.str() );
         }
         data.push_back( new_data.second );    
      }
   }
   catch( Seabed_SLAM_IO_Parse_Exception &e )
   {
      stringstream ss;
      ss << e.what() << " in file '" << file_name << "'";
      throw Seabed_SLAM_IO_Parse_Exception( ss.str() );
   }
}

// Generic file reading
template<class T>
static vector<T> read_file( const string &file_name,
                            File_Type file_type )
{
   // Open file   
   ifstream in_file;
   open_input_stream( file_name, in_file );
   
   // Get file format version number   
   int version = read_file_version( file_type, in_file );

   // Read the data
   vector<T> data;
   read_data( file_name, version, in_file, data );
   
   return data;
}


//----------------------------------------------------------------------------//
//   Generic File Writing (Private)                                           //
//----------------------------------------------------------------------------//

static void open_output_stream( const string &file_name, ofstream &out_file )
{
   out_file.open( file_name.c_str() );
   if( !out_file )
   {
      stringstream ss;
      ss << "Unable to open output file '" << file_name << "'";
      throw Seabed_SLAM_IO_File_Exception( ss.str() );
   }
   
   // Configure output stream
   out_file << fixed << std::setprecision(PRECISION) << endl;
}


// Write the file header, adding a comment char at the start of each line
static void write_header( const string &header, ostream &out )
{
   stringstream ss;
   ss << header;
   
   string line;
   while( getline(ss,line) )
      out << SEABED_SLAM_FILE_COMMENT_CHAR << " " << line << endl;
}


static void write_file_version_info( File_Type type,
                                     ostream &out )
{
   out << SEABED_SLAM_FILE_COMMENT_CHAR << " "
       << file_type_strings[type] << " "
       << VERSION_STRING << " "
       << file_type_current_versions[type] << endl;
}                                     


// Generic file writing
template<class T>
static void write_file( const string &file_name,
                        File_Type file_type,
                        const string &custom_header,
                        const string &file_info,
                        const vector<T> &data )
{
   // Open file and configure output stream
   ofstream out_file;
   open_output_stream( file_name, out_file );
   
   // Write the file version info
   write_file_version_info( file_type, out_file );
      
   // Write the file header
   write_header( "\n", out_file );
   write_header( custom_header, out_file );
   write_header( "\n", out_file );
   write_header( file_info, out_file );
   write_header( "\n", out_file );

   // Write the data
   for( unsigned int i=0; i<data.size(); i++ )
      out_file << data[i] << endl;
}

#if 0

//----------------------------------------------------------------------------//
//   Vehicle Pose Estimate File                                               //
//----------------------------------------------------------------------------//
//
// Version 1: Initial version
// Version 2: Added altitude
// Version 3: Added latitude and longitude. Local grid coordinate calculation
//            was changed to using a local Transverse Mercator projection 
//            instead of UTM, which may produced large errors when the vehicle
//            is operating a long way from the central meridian. Files with 
//            older version should only be used with caution and understanding
//            of this problem.


#define VEHICLE_POSE_FILE_INFO \
"Each line of this file describes the pose of the vehicle relative to the local\n\
navigation frame. The vehicle poses may have been estimated since they are the\n\
locations at which stereo images or multibeam sonar data were acquired.\n\
\n\
If a pose was estimated because it was the location images were acquired,\n\
additional information for that pose can be found in the file\n\
" STEREO_POSE_EST_FILE_NAME ". The pose identifier can be used to locate matching\n\
poses.\n\
\n\
The X and Y coordinates are produced using a local transverse Mercator \n\
projection using the WGS84 ellipsoid and a central meridian at the origin\n\
latitude. You will probably want to use the provided latitude and longitude to\n\
produce coordinates in what map projection you require.\n\
\n\
The first two lines of the data contain the latitude and longitude of the\n\
origin.\n\
\n\
Each line contains the following items describing the pose of the vehicle:\n\
\n\
1) Pose identifier                   - integer value\n\
2) Timestamp                         - in seconds\n\
3) Latitude                          - in degrees\n\
4) Longitude                         - in degrees\n\
5) X position (Northing)             - in meters, relative to local nav frame\n\
6) Y position (Easting)              - in meters, relative to local nav frame\n\
7) Z position (Depth)                - in meters, relative to local nav frame\n\
8) X-axis Euler angle (Roll)         - in radians, relative to local nav frame\n\
9) Y-axis Euler angle (Pitch)        - in radians, relative to local nav frame\n\
10) Z-axis Euler angle (Yaw/Heading) - in radians, relative to local nav frame\n\
11) Altitude                         - in meters. (0 when unknown)\n"


Vehicle_Pose::Vehicle_Pose( void )
   : pose_est( AUV_NUM_POSE_STATES ),
     altitude( AUV_NO_ALTITUDE )
{

}


// Input operator: first in 'pair' is the file version number, second is the
// datastructure to be initialised.
static istream&
operator>>( istream &in, pair<unsigned int, Vehicle_Pose> &pair )
{
   unsigned int version = pair.first;
   Vehicle_Pose &data = pair.second;

   parse( in, "pose id"  , data.pose_id   );
   parse( in, "pose time", data.pose_time );
   if( version>=3 )
   {
      parse( in, "latitude" , data.latitude  );
      parse( in, "longitude", data.longitude );
   }
   parse( in, "X"        , data.pose_est[AUV_POSE_INDEX_X]     );
   parse( in, "Y"        , data.pose_est[AUV_POSE_INDEX_Y]     );
   parse( in, "Z"        , data.pose_est[AUV_POSE_INDEX_Z]     );
   parse( in, "roll"     , data.pose_est[AUV_POSE_INDEX_PHI]   );
   parse( in, "pitch"    , data.pose_est[AUV_POSE_INDEX_THETA] );
   parse( in, "yaw"      , data.pose_est[AUV_POSE_INDEX_PSI]   );
   if( version>=2 )
      parse( in, "altitude", data.altitude );
   
   return in;
}


static ostream &operator<<( ostream &out, const Vehicle_Pose &data )
{
   assert( data.pose_est.size() == AUV_NUM_POSE_STATES );

   out << data.pose_id                        << " \t"
       << data.pose_time                      << " \t"
       << data.latitude                       << " \t"
       << data.longitude                      << " \t"
       << data.pose_est[AUV_POSE_INDEX_X]     << " \t"
       << data.pose_est[AUV_POSE_INDEX_Y]     << " \t"
       << data.pose_est[AUV_POSE_INDEX_Z]     << " \t"
       << data.pose_est[AUV_POSE_INDEX_PHI]   << " \t"
       << data.pose_est[AUV_POSE_INDEX_THETA] << " \t"
       << data.pose_est[AUV_POSE_INDEX_PSI]   << " \t"
       << data.altitude;

   return out;
}


Vehicle_Pose_File read_vehicle_pose_est_file( const string &file_name )
{
   Vehicle_Pose_File data;

   // Open file   
   ifstream in_file;
   open_input_stream( file_name, in_file );
   
   // Get file format version number   
   int version = read_file_version( FILE_TYPE_VEHICLE_POSE, in_file );

   // Check file version
   if( version < 3 )
   {
      stringstream ss;
      ss << "Obsolete vehicle pose file version " << version << "." << endl
         << endl
         << "This file version contains no geographic (latitude/longitude) "
         << "coordinates, and the inappropriate map projection used by "
         << "seabed_slam to obtain the X/Y coordinates may have introduced "
         << "significant errors in the navigation estimates." << endl 
         << endl
         << "If you are trying to reproduce previous results created using "
         << "this file, the format can be updated to the latest version "
         << "including geographic coordinates using the program "
         << "update_vehicle_pose_file. However, the errors in the navigation "
         << "estimates caused by the poor map projection will remain." << endl
         << endl
         << "Re-run seabed_slam to produce improved navigation estimates." 
         << endl;
      throw Seabed_SLAM_IO_Obsolete_File_Exception( ss.str() ); 
   }

   skip_comments( in_file );

   // Read origin latitude and longitude
   parse_named_value( in_file, "ORIGIN_LATITUDE", data.origin_latitude,
                      "Origin latitude" );
   parse_named_value( in_file, "ORIGIN_LONGITUDE", data.origin_longitude,
                      "Origin longitude" );

   
   // Read the data
   read_data( file_name, version, in_file, data.poses );

   return data;
}


void write_vehicle_pose_est_file( const string &file_name,
                                  const string &custom_header,
                                  const Vehicle_Pose_File &data )
{
   // Open file and configure output stream
   ofstream out_file;
   open_output_stream( file_name, out_file );
   
   // Write the file version info
   write_file_version_info( FILE_TYPE_VEHICLE_POSE, out_file );
      
   // Write the file header
   write_header( "\n", out_file );
   write_header( custom_header, out_file );
   write_header( "\n", out_file );
   write_header( VEHICLE_POSE_FILE_INFO, out_file );
   write_header( "\n", out_file );

   // Write the data
   out_file << "ORIGIN_LATITUDE  " << data.origin_latitude  << endl;
   out_file << "ORIGIN_LONGITUDE " << data.origin_longitude << endl;

   for( unsigned int i=0; i<data.poses.size(); i++ )
      out_file << data.poses[i] << endl;
}                                  


//----------------------------------------------------------------------------//
//   Stereo Pose Estimate File                                                //
//----------------------------------------------------------------------------//
//
// Version 1: Initial version
// Version 2: Added latitude and longitude. Local grid coordinate calculation
//            was changed to using a local Transverse Mercator projection 
//            instead of UTM, which may produced large errors when the vehicle
//            is operating a long way from the central meridian. Files with 
//            older version should only be used with caution and understanding
//            of this problem.
//

#define STEREO_POSE_FILE_INFO \
"Each line of this file describes the pose of the stereo-vision system relative\n\
to the local navigation frame at the time a pair of stereo images were\n\
acquired. The reference frame of the stereo-vision system is defined to be\n\
coincident with the left camera.\n\
\n\
The X and Y coordinates are produced using a local transverse Mercator \n\
projection using the WGS84 ellipsoid and a central meridian at the origin\n\
latitude. You will probably want to use the provided latitude and longitude to\n\
produce coordinates in what map projection you require.\n\
\n\
The first two lines of the data contain the latitude and longitude of the\n\
origin.\n\
\n\
Each line contains the following items describing the pose of the stereo rig:\n\
\n\
1) Pose identifier                   - integer value\n\
2) Timestamp                         - in seconds\n\
3) Latitude                          - in degrees\n\
4) Longitude                         - in degrees\n\
5) X position (North)                - in meters, relative to local nav frame\n\
6) Y position (East)                 - in meters, relative to local nav frame\n\
7) Z position (Depth)                - in meters, relative to local nav frame\n\
8) X-axis Euler angle                - in radians, relative to local nav frame\n\
9) Y-axis Euler angle                - in radians, relative to local nav frame\n\
10) Z-axis Euler angle               - in radians, relative to local nav frame\n\
11) Left image name\n\
12) Right image name\n\
13) Vehicle altitude                   - in meters\n\
14) Approx. bounding image radius      - in meters\n\
15) Likely trajectory cross-over point - 1 for true, 0 for false\n\
\n\
Data items 14 and 15 are used within our 3D mesh building software, and can\n\
safely be ignored in other applications.\n\
\n\
Note: The Euler angles correspond to the orientation of the stereo-rig, and\n\
do not correspond to the roll, pitch and heading of the vehicle. The stereo-\n\
frame is defined such that the positive Z-axis is along the principal ray of\n\
the camera (in the direction the camera is pointed), and the X and Y axes are\n\
aligned with the image axes. The positive X axis is pointing towards the\n\
right of the image, while the positive Y axis points to the bottom of the\n\
image. The Euler angles specify the sequence of rotations in XYZ order, that \n\
align the navigation frame axes (North, East, Down) with the stereo frame."


Stereo_Pose::Stereo_Pose( void )
   : pose_est( AUV_NUM_POSE_STATES ),
     altitude( AUV_NO_ALTITUDE )
{

}

// Input operator: first in 'pair' is the file version number, second is the
// datastructure to be initialised.
static istream &operator>>( istream &in, pair<unsigned int,Stereo_Pose> &pair )
{
   unsigned int version = pair.first;
   Stereo_Pose &data = pair.second;

   parse( in, "pose id"  , data.pose_id   );
   parse( in, "pose time", data.pose_time );
   if( version>=2 )
   {
      parse( in, "latitude" , data.latitude  );
      parse( in, "longitude", data.longitude );
   }
   parse( in, "X"                     , data.pose_est[AUV_POSE_INDEX_X]     );
   parse( in, "Y"                     , data.pose_est[AUV_POSE_INDEX_Y]     );
   parse( in, "Z"                     , data.pose_est[AUV_POSE_INDEX_Z]     );
   parse( in, "roll"                  , data.pose_est[AUV_POSE_INDEX_PHI]   );
   parse( in, "pitch"                 , data.pose_est[AUV_POSE_INDEX_THETA] );
   parse( in, "yaw"                   , data.pose_est[AUV_POSE_INDEX_PSI]   );
   parse( in, "left image name"       , data.left_image_name                );
   parse( in, "right image name"      , data.right_image_name               );
   parse( in, "altitude"              , data.altitude                       );
   parse( in, "image footprint radius", data.image_footprint_radius         );
   parse( in, "likely overlap flag"   , data.likely_overlap                 );

   return in;
}


static ostream &operator<<( ostream &out, const Stereo_Pose &data )
{
   assert( data.pose_est.size() == AUV_NUM_POSE_STATES );

   out << data.pose_id                        << " \t"
       << data.pose_time                      << " \t"
       << data.latitude                       << " \t"
       << data.longitude                      << " \t"
       << data.pose_est[AUV_POSE_INDEX_X]     << " \t"
       << data.pose_est[AUV_POSE_INDEX_Y]     << " \t"
       << data.pose_est[AUV_POSE_INDEX_Z]     << " \t"
       << data.pose_est[AUV_POSE_INDEX_PHI]   << " \t"
       << data.pose_est[AUV_POSE_INDEX_THETA] << " \t"
       << data.pose_est[AUV_POSE_INDEX_PSI]   << " \t"
       << (data.dir_name.empty() ? "" : (data.dir_name +"/"))
       << data.left_image_name                << " \t"
       << (data.dir_name.empty() ? "" : (data.dir_name +"/"))
       << data.right_image_name               << " \t"
       << data.altitude                       << " \t"
       << data.image_footprint_radius         << " \t"
       << data.likely_overlap;

   return out;
}


Stereo_Pose_File read_stereo_pose_est_file( const string &file_name )
{
   Stereo_Pose_File data;

   // Open file   
   ifstream in_file;
   open_input_stream( file_name, in_file );
   
   // Get file format version number   
   int version = read_file_version( FILE_TYPE_STEREO_POSE, in_file );

   // Check file version
   if( version < 2 )
   {
      stringstream ss;
      ss << "Obsolete stereo pose file version " << version << "." << endl
         << endl
         << "This file version contains no geographic (latitude/longitude) "
         << "coordinates, and the inappropriate map projection used by "
         << "seabed_slam to obtain the X/Y coordinates may have introduced "
         << "significant errors in the navigation estimates." << endl 
         << endl
         << "If you are trying to reproduce previous results created using "
         << "this file, the format can be updated to the latest version "
         << "including geographic coordinates using the program "
         << "update_vehicle_pose_file. However, the errors in the navigation "
         << "estimates caused by the poor map projection will remain." << endl
         << endl
         << "Re-run seabed_slam to produce improved navigation estimates." 
         << endl;

      throw Seabed_SLAM_IO_Obsolete_File_Exception( ss.str() ); 
   }

   skip_comments( in_file );

   // Read origin latitude and longitude
   parse_named_value( in_file, "ORIGIN_LATITUDE", data.origin_latitude,
                      "Origin latitude" );
   parse_named_value( in_file, "ORIGIN_LONGITUDE", data.origin_longitude, 
                      "Origin longitude" );
   
   // Read the data
   read_data( file_name, version, in_file, data.poses );

   return data;
}


void write_stereo_pose_est_file( const string &file_name,
                                 const string &custom_header,
                                 const Stereo_Pose_File &data )
{
   // Open file and configure output stream
   ofstream out_file;
   open_output_stream( file_name, out_file );
   
   // Write the file version info
   write_file_version_info( FILE_TYPE_STEREO_POSE, out_file );
      
   // Write the file header
   write_header( "\n", out_file );
   write_header( custom_header, out_file );
   write_header( "\n", out_file );
   write_header( STEREO_POSE_FILE_INFO, out_file );
   write_header( "\n", out_file );

   // Write the data
   out_file << "ORIGIN_LATITUDE  " << data.origin_latitude  << endl;
   out_file << "ORIGIN_LONGITUDE " << data.origin_longitude << endl;

   for( unsigned int i=0; i<data.poses.size(); i++ )
      out_file << data.poses[i] << endl;
}                                  



//----------------------------------------------------------------------------//
//   Sound Speed File                                                         //
//----------------------------------------------------------------------------//
//
// Version 1: Initial version
//

#define SOUND_SPEED_FILE_INFO \
"Each line of this file describes the sound speed measured \n\
at the time indicated.\n\
\n\
On each line of the file are 4 items:\n\
\n\
1) Record identifier                  - integer value\n\
2) Timestamp                        - in seconds\n\
3) Sound speed at vehicle           - in meters/second\n\
4) Mean sound speed beneath vehicle - in meters/second (best guess)\n"


// Input operator: first in 'pair' is the file version number, second is the
// datastructure to be initialised.
static istream &operator>>( istream &in, pair<unsigned int,Sound_Speed> &pair )
{
   //unsigned int version = pair.first;
   Sound_Speed &data = pair.second;

   parse( in, "pose id"  , data.pose_id   );
   parse( in, "pose time", data.pose_time );
   parse( in, "ss head"  , data.ss_head   );
   parse( in, "ss mean"  , data.ss_mean   );

   return in;
}


static ostream &operator<<( ostream &out, const Sound_Speed &data )
{
   out << data.pose_id   << " \t"
       << data.pose_time << " \t"
       << data.ss_head   << " \t"
       << data.ss_mean;

   return out;
}


vector<Sound_Speed> read_sound_speed_file( const string &file_name )
{
   return read_file<Sound_Speed>( file_name, FILE_TYPE_SOUND_SPEED );
}


void write_sound_speed_file( const string &file_name,
                             const string &custom_header,
                             const vector<Sound_Speed> &data )
{
   write_file( file_name, FILE_TYPE_SOUND_SPEED, custom_header, 
               SOUND_SPEED_FILE_INFO, data );
}                                  


//----------------------------------------------------------------------------//
//   Tide Correction File                                                     //
//----------------------------------------------------------------------------//
//
// Version 1: Initial version
//

#define TIDE_CORRECTION_FILE_INFO \
"Each line of this file describes the tidal offset \n\
at the time indicated.\n\
\n\
On each line of the file are 3 items:\n\
\n\
1) Date                        - yyyy/mm/dd\n\
2) Time                        - hours:min (UTC)\n\
3) Tidal offset relative to mean sea level - in meters\n"


// Input operator: first in 'pair' is the file version number, second is the
// datastructure to be initialised.
static istream &operator>>( istream &in, pair<unsigned int,Tide_Correction> &pair )
{
   //unsigned int version = pair.first;
   Tide_Correction &data = pair.second;

   parse( in, "year ", data.year );
   parse( in, "time" , data.time );
   parse( in, "dz"   , data.dz   );

   return in;
}


vector<Tide_Correction> read_tide_correction_file( const string &file_name )
{
    tm tcapTime = {0};
    
    // Read file.
    vector<Tide_Correction> Tides;
    Tides = read_file<Tide_Correction>( file_name, FILE_TYPE_TIDE_CORRECTION );
    
    // Compute unix timestamp.
    for(unsigned int i=0;i<Tides.size();i++){
        sscanf(Tides[i].year.c_str(),"%04u/%02u/%02u",
               &tcapTime.tm_year, &tcapTime.tm_mon, &tcapTime.tm_mday);
        sscanf(Tides[i].time.c_str(),"%02u:%02u",
               &tcapTime.tm_hour, &tcapTime.tm_min);
        tcapTime.tm_year-=1900;
        tcapTime.tm_mon-=1;
        Tides[i].timestamp = (double)mktime(&tcapTime);
    }

    return Tides;
}

//----------------------------------------------------------------------------//
//   Vehicle Pose Estimate and Covariance File                                //
//----------------------------------------------------------------------------//
//
// Version 1: Initial version
//


#define VEHICLE_POSE_COV_FILE_INFO \
"Each line of this file describes the pose of the vehicle relative to the local\n\
navigation frame. The vehicle poses may have been estimated since they are the\n\
locations at which stereo images or multibeam sonar data were acquired.\n\
\n\
If a pose was estimated because it was the location images were acquired,\n\
additional information for that pose can be found in the file\n\
" STEREO_POSE_EST_FILE_NAME ". The pose identifier can be used to locate matching\n\
poses.\n\
\n\
On each line of the file are 20 items:\n\
\n\
1) Pose identifier                  - integer value\n\
2) Timestamp                        - in seconds\n\
3) X position (Northing)            - in meters, relative to local nav frame\n\
4) Y position (Easting)             - in meters, relative to local nav frame\n\
5) Z position (Depth)               - in meters, relative to local nav frame\n\
6) X-axis Euler angle (Roll)        - in radians, relative to local nav frame\n\
7) Y-axis Euler angle (Pitch)       - in radians, relative to local nav frame\n\
8) Z-axis Euler angle (Yaw/Heading) - in radians, relative to local nav frame\n\
9)  XX cov                          - in meters^2\n\
10) XY cov                          - in meters^2\n\
11) XZ cov                          - in meters^2\n\
12) YY cov                          - in meters^2\n\
13) YZ cov                          - in meters^2\n\
14) ZZ cov                          - in meters^2\n\
15) RR cov                          - in meters^2\n\
16) RP cov                          - in meters^2\n\
17) RH cov                          - in meters^2\n\
18) PP cov                          - in meters^2\n\
19) PH cov                          - in meters^2\n\
20) HH cov                          - in meters^2"

Vehicle_Pose_Cov::Vehicle_Pose_Cov( void )
   : pose_est( AUV_NUM_POSE_STATES ),
     pose_cov( AUV_NUM_POSE_STATES, AUV_NUM_POSE_STATES )
{

}


// Input operator: first in 'pair' is the file version number, second is the
// datastructure to be initialised.
static istream&
operator>>( istream &in, pair<unsigned int, Vehicle_Pose_Cov> &pair )
{
   //unsigned int version = pair.first;
   Vehicle_Pose_Cov &data = pair.second;

   parse( in, "pose id"  , data.pose_id                        );
   parse( in, "pose time", data.pose_time                      );
   parse( in, "X"        , data.pose_est(AUV_POSE_INDEX_X)     );
   parse( in, "Y"        , data.pose_est(AUV_POSE_INDEX_Y)     );
   parse( in, "Z"        , data.pose_est(AUV_POSE_INDEX_Z)     );
   parse( in, "roll"     , data.pose_est(AUV_POSE_INDEX_PHI)   );
   parse( in, "pitch"    , data.pose_est(AUV_POSE_INDEX_THETA) );
   parse( in, "yaw"      , data.pose_est(AUV_POSE_INDEX_PSI)   );
   parse( in, "Pxx"      , data.pose_cov(AUV_POSE_INDEX_X, AUV_POSE_INDEX_X));
   parse( in, "Pxy"      , data.pose_cov(AUV_POSE_INDEX_X, AUV_POSE_INDEX_Y));
   parse( in, "Pxz"      , data.pose_cov(AUV_POSE_INDEX_X, AUV_POSE_INDEX_Z));
   parse( in, "Pyy"      , data.pose_cov(AUV_POSE_INDEX_Y, AUV_POSE_INDEX_Y));
   parse( in, "Pyz"      , data.pose_cov(AUV_POSE_INDEX_Y, AUV_POSE_INDEX_Z));
   parse( in, "Pyz"      , data.pose_cov(AUV_POSE_INDEX_Y, AUV_POSE_INDEX_Z));
   parse( in, "Pzz"      , data.pose_cov(AUV_POSE_INDEX_Z, AUV_POSE_INDEX_Z));
   parse( in, "Prr"      , data.pose_cov(AUV_POSE_INDEX_PHI, AUV_POSE_INDEX_PHI));
   parse( in, "Prp"      , data.pose_cov(AUV_POSE_INDEX_PHI, AUV_POSE_INDEX_THETA));
   parse( in, "Prh"      , data.pose_cov(AUV_POSE_INDEX_PHI, AUV_POSE_INDEX_PSI));
   parse( in, "Ppp"      , data.pose_cov(AUV_POSE_INDEX_THETA, AUV_POSE_INDEX_THETA));
   parse( in, "Pph"      , data.pose_cov(AUV_POSE_INDEX_THETA, AUV_POSE_INDEX_PSI));
   parse( in, "Phh"      , data.pose_cov(AUV_POSE_INDEX_PSI, AUV_POSE_INDEX_PSI));

   return in;
}


static ostream &operator<<( ostream &out, const Vehicle_Pose_Cov &data )
{
   assert( data.pose_est.size() == AUV_NUM_POSE_STATES );
   assert( data.pose_cov.size1() == AUV_NUM_POSE_STATES
           && data.pose_cov.size2() == AUV_NUM_POSE_STATES );

   out << data.pose_id                        << " \t"
       << data.pose_time                      << " \t"
       << data.pose_est(AUV_POSE_INDEX_X)     << " \t"
       << data.pose_est(AUV_POSE_INDEX_Y)     << " \t"
       << data.pose_est(AUV_POSE_INDEX_Z)     << " \t"
       << data.pose_est(AUV_POSE_INDEX_PHI)   << " \t"
       << data.pose_est(AUV_POSE_INDEX_THETA) << " \t"
       << data.pose_est(AUV_POSE_INDEX_PSI)   << " \t"
       << data.pose_cov(AUV_POSE_INDEX_X, AUV_POSE_INDEX_X) << "\t"
       << data.pose_cov(AUV_POSE_INDEX_X, AUV_POSE_INDEX_Y) << "\t"
       << data.pose_cov(AUV_POSE_INDEX_X, AUV_POSE_INDEX_Z) << "\t"
       << data.pose_cov(AUV_POSE_INDEX_Y, AUV_POSE_INDEX_Y) << "\t"
       << data.pose_cov(AUV_POSE_INDEX_Y, AUV_POSE_INDEX_Z) << "\t"
       << data.pose_cov(AUV_POSE_INDEX_Z, AUV_POSE_INDEX_Z) << "\t"
       << data.pose_cov(AUV_POSE_INDEX_PHI, AUV_POSE_INDEX_PHI) << "\t"
       << data.pose_cov(AUV_POSE_INDEX_PHI, AUV_POSE_INDEX_THETA) << "\t"
       << data.pose_cov(AUV_POSE_INDEX_PHI, AUV_POSE_INDEX_PSI) << "\t"
       << data.pose_cov(AUV_POSE_INDEX_THETA, AUV_POSE_INDEX_THETA) << "\t"
       << data.pose_cov(AUV_POSE_INDEX_THETA, AUV_POSE_INDEX_PSI) << "\t"
       << data.pose_cov(AUV_POSE_INDEX_PSI, AUV_POSE_INDEX_PSI) << "\t";

   return out;
}


vector<Vehicle_Pose_Cov> read_vehicle_pose_cov_file( const string &file_name )
{
   return read_file<Vehicle_Pose_Cov>( file_name, FILE_TYPE_VEHICLE_POSE );
}


void write_vehicle_pose_cov_file( const string &file_name,
                                  const string &custom_header,
                                  const vector<Vehicle_Pose_Cov> &data )
{
   write_file( file_name, FILE_TYPE_VEHICLE_POSE_COV, custom_header,
               VEHICLE_POSE_COV_FILE_INFO, data );
}




//----------------------------------------------------------------------------//
//   Vehicle Estimate File                                                    //
//----------------------------------------------------------------------------//
//
// Version 1: Initial version
//


#define VEHICLE_EST_FILE_INFO \
"Each line of this file describes the posterior vehicle state estimate at a\n\
particular time.\n\
\n\
On each line of the file are 25 items:\n\
\n\
1)  Timestamp                          - in seconds\n\
2)  X-axis position (Northing)         - in meters, relative to local nav frame\n\
3)  Y-axis position (Easting)          - in meters, relative to local nav frame\n\
4)  Z-axis position (Depth)            - in meters, relative to local nav frame\n\
5)  X-axis Euler angle (Roll)          - in radians, relative to local nav frame\n\
6)  Y-axis Euler angle (Pitch)         - in radians, relative to local nav frame\n\
7)  Z-axis Euler angle (Yaw/Heading)   - in radians, relative to local nav frame\n\
8)  X-axis body velocity               - in meters per second\n\
9)  Y-axis body velocity               - in meters per second\n\
10) Z-axis body velocity               - in meters per second\n\
11) X-axis body rotation rate          - in radians per second\n\
12) Y-axis body rotation rate          - in radians per second\n\
13) Z-axis body rotation rate          - in radians per second\n\
14) X-axis position variance           - in m^2\n\
15) Y-axis position variance           - in m^2\n\
16) Z-axis position variance           - in m^2\n\
17) X-axis Euler angle variance        - in rad^2\n\
18) Y-axis Euler angle variance        - in rad^2\n\
19) Z-axis Euler angle variance        - in rad^2\n\
20) X-axis body velocity variance      - in m^2/s^2\n\
21) Y-axis body velocity variance      - in m^2/s^2\n\
22) Z-axis body velocity variance      - in m^2/s^2\n\
23) X-axis body rotation rate variance - in rad^2/s^2\n\
24) Y-axis body rotation rate variance - in rad^2/s^2\n\
25) Z-axis body rotation rate variance - in rad^2/s^2\n"




// class Vehicle_Est
// {
// public:
//    Vehicle_Est( void );
//    
//    double time;                   // Timestamp
//    libplankton::Vector est;       // Vehicle state estimate 
//    libplankton::Vector cov_diag;  // Diagonal of the covariance matrix
// };  


Vehicle_Est::Vehicle_Est( void )
   : est( 24 ),
     cov_diag( 24 )
{

}

// Input operator: first in 'pair' is the file version number, second is the
// datastructure to be initialised.
static istream&
operator>>( istream &in, pair<unsigned int, Vehicle_Est> &pair )
{
   //unsigned int version = pair.first;
   Vehicle_Est &data = pair.second;

   parse( in, "Time"     , data.time        );
   parse( in, "x"        , data.est(0)       );
   parse( in, "y"        , data.est(1)       );
   parse( in, "z"        , data.est(2)       );
   parse( in, "roll"     , data.est(3)       );
   parse( in, "pitch"    , data.est(4)       );
   parse( in, "yaw"      , data.est(5)       );
   parse( in, "X-vel"    , data.est(6)       );
   parse( in, "Y-vel"    , data.est(7)       );
   parse( in, "Z-vel"    , data.est(8)       );
   parse( in, "p"        , data.est(9)       );
   parse( in, "q"        , data.est(10)      );
   parse( in, "r"        , data.est(11)      );
   parse( in, "x cov"    , data.cov_diag(0)  );
   parse( in, "y cov"    , data.cov_diag(1)  );
   parse( in, "z cov"    , data.cov_diag(2)  );
   parse( in, "roll cov" , data.cov_diag(3)  );
   parse( in, "pitch cov", data.cov_diag(4)  );
   parse( in, "yaw cov"  , data.cov_diag(5)  );
   parse( in, "X-vel cov", data.cov_diag(6)  );
   parse( in, "Y-vel cov", data.cov_diag(7)  );
   parse( in, "Z-vel cov", data.cov_diag(8)  );
   parse( in, "p cov"    , data.cov_diag(9)  );
   parse( in, "q cov"    , data.cov_diag(10) );
   parse( in, "r cov"    , data.cov_diag(11) );

   return in;
}


static ostream &operator<<( ostream &out, const Vehicle_Est &data )
{
   assert( data.est.size()==24 );
   assert( data.cov_diag.size()==24 );

   out << data.time << " \t";
   for( unsigned int i=0; i<12; i++ )
      out << data.est(i) << " \t";
   for( unsigned int i=0; i<12; i++ )
      out << data.cov_diag(i) << " \t";

   return out;
}


vector<Vehicle_Est> read_vehicle_est_file( const string &file_name )
{
   return read_file<Vehicle_Est>( file_name, FILE_TYPE_VEHICLE_EST );
}


void write_vehicle_est_file( const string &file_name,
                             const string &custom_header,
                             const vector<Vehicle_Est> &data )
{
   write_file( file_name, FILE_TYPE_VEHICLE_EST, custom_header,
               VEHICLE_EST_FILE_INFO, data );

}                             


//----------------------------------------------------------------------------//
//   Relative Pose Observation Info File                                      //
//----------------------------------------------------------------------------//

#define REL_POSE_OBS_FILE_INFO \
"This file contains information about relative pose observations applied to\n\
the SLAM filter\n\
\n\
Next line contains the following information on a relative pose observation:\n\
   1) Pose 1 ID\n\
   2) Pose 2 ID\n\
   3) Relative pose source\n\
   4) Relative pose cache key\n\
\n\
The relative pose source values have the following meanings:\n\
   0 - Found during incremental processing for odometry observations\n\
   1 - Found during incremental processing for loop-closure observations\n\
   2 - Found during map optimisation\n\
   3 - Found during a manual loop-closure search\n\
\n\
The cache key can be used to extract the relative pose observation details\n\
from the SLAM pose cache. This information includes the relative pose\n\
estimate, covariance and the coordinates of the image features.\n"


// Input operator: first in 'pair' is the file version number, second is the
// datastructure to be initialised.
static istream&
operator>>( istream &in, pair<unsigned int,Rel_Pose_Obs_Info> &pair )
{
   //int version = pair.first;
   Rel_Pose_Obs_Info &data = pair.second;
   
   int source_int;
   parse( in, "pose 1 ID", data.pose1_id );
   parse( in, "pose 2 ID", data.pose2_id );

   parse( in, "source"   , source_int    );
   data.source = (Rel_Pose_Source)(source_int);

   parse( in, "cache key", data.cache_key );
   size_t len = data.cache_key.length();
   if( data.cache_key[0]!='\"' || data.cache_key[len-1]!='\"' )
      throw Seabed_SLAM_IO_Parse_Exception( "Unable to parse cache key" );
   data.cache_key = data.cache_key.substr(1,len-2);

   return in;
}

static ostream &operator<<( ostream &out, const Rel_Pose_Obs_Info &data )
{
   out << data.pose1_id    << " \t"
       << data.pose2_id    << " \t"
       << (int)data.source << " \t"
       << '\"' << data.cache_key << '\"';
   return out;
}

Rel_Pose_Obs_Info::Rel_Pose_Obs_Info( void )
{

}

Rel_Pose_Obs_Info::Rel_Pose_Obs_Info( unsigned int pose1_id, 
                                      unsigned int pose2_id,
                                      Rel_Pose_Source source,
                                      const string &cache_key )
   : pose1_id(pose1_id), pose2_id(pose2_id), source(source), 
     cache_key(cache_key)
{

}                                      

vector<Rel_Pose_Obs_Info> read_rel_pose_obs_file( const string &file_name )
{
   return read_file<Rel_Pose_Obs_Info>( file_name, FILE_TYPE_REL_POSE_OBS );
}

void write_rel_pose_obs_file( const string &file_name,
                              const string &custom_header,
                              const vector<Rel_Pose_Obs_Info> &data )
{
   write_file( file_name, FILE_TYPE_REL_POSE_OBS, custom_header, 
               REL_POSE_OBS_FILE_INFO, data );
}                                  




//----------------------------------------------------------------------------//
//   Image Feature File                                                       //
//----------------------------------------------------------------------------//
//
// Version 1: Initial version
//

#define IMAGE_FEATURE_FILE_INFO  "..." // TODO

// Input operator: first in 'pair' is the file version number, second is the
// datastructure to be initialised.
static istream &operator>>( istream &in, pair<unsigned int,Image_Feats> &pair )
{
   //unsigned int version = pair.first;
   Image_Feats &data = pair.second;

   parse( in, "pose id"  , data.pose_id   );
   parse( in, "pose time", data.pose_time );
   parse( in, "left image name" , data.left_image_name  );
   parse( in, "right image name", data.right_image_name );
   parse_999( in, "sp_rgsty",  data.sp_rgsty );
   parse_999( in, "sp_slope",  data.sp_slope );
   parse_999( in, "sp_aspct",  data.sp_aspct );
   parse_999( in, "m5_rgsty",  data.m5_rgsty );
   parse_999( in, "m5_slope",  data.m5_slope );
   parse_999( in, "m5_aspct",  data.m5_aspct );
   parse_999( in, "m10_rgsty", data.m10_rgsty );
   parse_999( in, "m10_slope", data.m10_slope );
   parse_999( in, "m10_aspct", data.m10_aspct );
   parse_999( in, "m20_rgsty", data.m20_rgsty );
   parse_999( in, "m20_slope", data.m20_slope );
   parse_999( in, "m20_aspct", data.m20_aspct );
 
   for (unsigned int i=0; i < Image_Feats::LAB_LENGTH; ++i)
      parse_999( in, "meanmod", data.meanmod[i] );

   for (unsigned int i=0; i < Image_Feats::LAB_LENGTH; ++i)
      parse_999( in, "stdmod", data.stdmod[i] );

   parse_999( in, "segsize", data.segsize );

   for (unsigned int i=0; i < Image_Feats::LBP_LENGTH; ++i)
      parse_999( in, "lbp", data.lbp[i] );

   parse_999( in, "stdgray", data.stdgray   );
   parse_999( in, "meangray", data.meangray );
 
   return in;
}

vector<Image_Feats> read_image_feature_file( const string &file_name )
{
   return read_file<Image_Feats>( file_name, FILE_TYPE_IMAGE_FEATURE );
}

#endif
//----------------------------------------------------------------------------//
//   Image Label File                                                         //
//----------------------------------------------------------------------------//
//
// Version 1: Initial version
//

#define IMAGE_LABEL_FILE_INFO \
"Each line of this file lists a single string label for the corresponding image, \n\
generated for instance by an image clustering algorithm.\n\
\n\
On each line of the file are 5 items:\n\
\n\
1) Record identifier                - integer value\n\
2) Timestamp                        - in seconds\n\
3) Left image name                  - string\n\
4) Right image name                 - string\n\
5) Image label                      - integer value\n"


// Input operator: first in 'pair' is the file version number, second is the
// datastructure to be initialised.
static istream &operator>>( istream &in, pair<unsigned int,Image_Label> &pair )
{
   //unsigned int version = pair.first;
   Image_Label &data = pair.second;

   parse( in, "pose id"  , data.pose_id   );
   parse( in, "pose time", data.pose_time );
   parse( in, "left image name"       , data.left_image_name                );
   parse( in, "right image name"      , data.right_image_name               );
   parse( in, "label", data.label );

   return in;
}


static ostream &operator<<( ostream &out, const Image_Label &data )
{
   out << data.pose_id             << " \t"
       << data.pose_time           << " \t"
       << data.left_image_name     << " \t"
       << data.right_image_name    << " \t"
       << data.label;

   return out;
}


vector<Image_Label> read_image_label_file( const string &file_name )
{
   return read_file<Image_Label>( file_name, FILE_TYPE_IMAGE_LABEL );
}


void write_image_label_file( const string &file_name,
                             const string &custom_header,
                             const vector<Image_Label> &data )
{
   write_file( file_name, FILE_TYPE_IMAGE_LABEL, custom_header, 
               IMAGE_LABEL_FILE_INFO, data );
}                                  




//----------------------------------------------------------------------------//
//   GPS Observation File                                                     //
//----------------------------------------------------------------------------//
//
// Version 1: Initial version
//

#define GPS_OBS_FILE_INFO \
"Each line of this file contains information on a GPS observation used by SLAM.\n\
\n\
On each line of the file are 5 items:\n\
\n\
1) Timestamp - in seconds\n\
2) Latitude  - in degrees\n\
3) longitude - in degrees\n\
4) Eastings  - in meters (relative to local nav frame)\n\
5) Northings - in meters (relative to local nav frame)\n\
\n\
The eastings and northings are produced using a local transverse Mercator\n\
projection using the WGS84 ellipsoid and a central meridian at the origin\n\
latitude. You will probably want to use the provided latitude and longitude to\n\
produce coordinates in what map projection you require.\n\
\n"


GPS_Obs::GPS_Obs( )
{

}

GPS_Obs::GPS_Obs( double time, double latitude, double longitude,
                  double northings, double eastings )
   : time( time ),
     latitude( latitude ),
     longitude( longitude ),
     northings( northings ),
     eastings( eastings )
{

}                  

// Input operator: first in 'pair' is the file version number, second is the
// datastructure to be initialised.
static istream &operator>>( istream &in, pair<unsigned int,GPS_Obs> &pair )
{
   //unsigned int version = pair.first;
   GPS_Obs &data = pair.second;

   parse( in, "timestamp", data.time      );
   parse( in, "latitude" , data.latitude  );
   parse( in, "longitude", data.longitude );
   parse( in, "northings", data.northings );
   parse( in, "eastings" , data.eastings  );

   return in;
}


static ostream &operator<<( ostream &out, const GPS_Obs &data )
{
   out << data.time      << " \t"
       << data.latitude  << " \t"
       << data.longitude << " \t"
       << data.northings << " \t"
       << data.eastings;

   return out;
}


vector<GPS_Obs> read_gps_obs_file( const string &file_name )
{
   return read_file<GPS_Obs>( file_name, FILE_TYPE_GPS_OBS );
}


void write_gps_obs_file( const string &file_name,
                         const string &custom_header,
                         const vector<GPS_Obs> &data )
{
   write_file( file_name, FILE_TYPE_GPS_OBS, custom_header, 
               GPS_OBS_FILE_INFO, data );
}                                  






//----------------------------------------------------------------------------//
//   Read Obsolete Stereo Association Files                                   //
//----------------------------------------------------------------------------//
namespace seabed_slam_io_private
{

// Input operator: first in 'pair' is the file version number, second is the
// datastructure to be initialised.
static istream&
operator>>( istream &in, pair<unsigned int,Stereo_Assoc_Data> &pair )
{
   //int version = pair.first;
   Stereo_Assoc_Data &data = pair.second;
   
   parse( in, "pose 1 left image file name" , data.pose1_left_image_name  );
   parse( in, "pose 1 right image file name", data.pose1_right_image_name );
   parse( in, "pose 2 left image file name" , data.pose2_left_image_name  );
   parse( in, "pose 2 right image file name", data.pose2_right_image_name );
   parse( in, "number of features"          , data.num_features           );

   data.pose1_left_coords.resize ( data.num_features*2 );
   data.pose1_right_coords.resize( data.num_features*2 );
   data.pose2_left_coords.resize ( data.num_features*2 );
   data.pose2_right_coords.resize( data.num_features*2 );

   for( unsigned int i=0 ; i<data.num_features ; i++ )
   {
      parse( in, "image coord 1", data.pose1_left_coords [i*2]   );
      parse( in, "image coord 2", data.pose1_left_coords [i*2+1] );
      parse( in, "image coord 3", data.pose1_right_coords[i*2]   );
      parse( in, "image coord 4", data.pose1_right_coords[i*2+1] );
      parse( in, "image coord 5", data.pose2_left_coords [i*2]   );
      parse( in, "image coord 6", data.pose2_left_coords [i*2+1] );
      parse( in, "image coord 7", data.pose2_right_coords[i*2]   );
      parse( in, "image coord 8", data.pose2_right_coords[i*2+1] );
   }

   return in;
}


vector<Stereo_Assoc_Data> read_stereo_assoc_file( const string &file_name )
{
   return read_file<Stereo_Assoc_Data>( file_name, FILE_TYPE_STEREO_ASSOC );
}

} // namespace seabed_slam_io_private

//----------------------------------------------------------------------------//
//   Updating of Obsolete Files                                               //
//----------------------------------------------------------------------------//
#if 0
#include <GeographicConversions/ufRedfearn.h> // for backward compatibility
using namespace UF::GeographicConversions;

#define REDFEARN_ELLIPSOID_NAME "WGS84"
#define REDFEARN_GRID_NAME "UTM"

//
// Read an obsolete vehicle pose estimate file
//
// This reads the obsolete versions 1 and 2 of the vehicle pose file format,
// and fills in the latitude and longitude of each pose by reverting the map
// projection used to obtain the X/Y coordinates.
//
// The errors due to the inappropriate map projection used to produce the 
// results in these files will still remain, and it is recommended you redo
// the SLAM processing to produce better results.
//
Vehicle_Pose_File seabed_slam_io_private::read_obsolete_vehicle_pose_est_file( 
                                                       const string &file_name,
                                                       double origin_latitude,
                                                       double origin_longitude )
{
   Vehicle_Pose_File data;
   
   // Open file   
   ifstream in_file;
   open_input_stream( file_name, in_file );
   
   // Get file format version number   
   int version = read_file_version( FILE_TYPE_VEHICLE_POSE, in_file );

   // Check version number
   if( version>2 )
   {
      stringstream ss;
      ss << "Backward compatibility loading of vehicle pose file version " 
         << version << " not implemented";
      throw Seabed_SLAM_IO_Parse_Exception( ss.str() );   
   }
   
   // Read the data
   data.origin_latitude = origin_latitude;
   data.origin_longitude = origin_longitude;
   read_data( file_name, version, in_file, data.poses );

   // Undo the UTM map projection to fill in geographic coordinates
   Redfearn redfearn( REDFEARN_ELLIPSOID_NAME, REDFEARN_GRID_NAME );
   double origin_easting, origin_northing;
   string origin_zone;
   double grid_convergence, point_scale;
   redfearn.GetGridCoordinates( origin_latitude, origin_longitude, origin_zone,
                                origin_easting, origin_northing,
                                grid_convergence, point_scale );
   for( unsigned int i=0; i<data.poses.size(); i++ )
   {
      double easting = origin_easting+data.poses[i].pose_est[AUV_POSE_INDEX_X];
      double northing = origin_northing+data.poses[i].pose_est[AUV_POSE_INDEX_Y];
      redfearn.GetGeographicCoordinates( origin_zone, easting, northing,
                                         data.poses[i].latitude,
                                         data.poses[i].longitude,
                                         grid_convergence, point_scale );
   }
   return data;
}

//
// Read an obsolete stereo pose estimate file
//
// This reads the obsolete version of the stereo pose file format,
// and fills in the latitude and longitude of each pose by reverting the map
// projection used to obtain the X/Y coordinates.
//
// The errors due to the inappropriate map projection used to produce the 
// results in these files will still remain, and it is recommended you redo
// the SLAM processing to produce better results.
//
Stereo_Pose_File seabed_slam_io_private::read_obsolete_stereo_pose_est_file( 
                                                       const string &file_name,
                                                       double origin_latitude,
                                                       double origin_longitude )
{
   Stereo_Pose_File data;
   
   // Open file   
   ifstream in_file;
   open_input_stream( file_name, in_file );
   
   // Get file format version number   
   int version = read_file_version( FILE_TYPE_STEREO_POSE, in_file );

   // Check version number
   if( version>1 )
   {
      stringstream ss;
      ss << "Backward compatibility loading of stereo pose file version " 
         << version << " not implemented";
      throw Seabed_SLAM_IO_Parse_Exception( ss.str() );   
   }
   
   // Read the data
   data.origin_latitude = origin_latitude;
   data.origin_longitude = origin_longitude;
   read_data( file_name, version, in_file, data.poses );

   // Undo the UTM map projection to fill in geographic coordinates
   Redfearn redfearn( REDFEARN_ELLIPSOID_NAME, REDFEARN_GRID_NAME );
   double origin_easting, origin_northing;
   string origin_zone;
   double grid_convergence, point_scale;
   redfearn.GetGridCoordinates( origin_latitude, origin_longitude, origin_zone,
                                origin_easting, origin_northing,
                                grid_convergence, point_scale );
   for( unsigned int i=0; i<data.poses.size(); i++ )
   {
      double easting = origin_easting +data.poses[i].pose_est[AUV_POSE_INDEX_X];
      double northing = origin_northing+data.poses[i].pose_est[AUV_POSE_INDEX_Y];
      redfearn.GetGeographicCoordinates( origin_zone, easting, northing,
                                         data.poses[i].latitude,
                                         data.poses[i].longitude,
                                         grid_convergence, point_scale );
   }
   return data;
}
#endif
