// Read a file where each line consists of:
// Latitude Longitude Altitude
// and produce a file consisting of:
// Easting Northing Altitude.
// You can optionally have a fourth value on each line
// corresponding to some scalar value.

#include "ufRedfearn.h"
#include "ufVincenty.h"
#include "ufMapGrid.h"
#include "ufMapGrids.h"
#include "ufEllipsoids.h"
#include "ufECEF.h"

#include <fstream>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace UF;

  //! ParseLine - Read a line of three values of type T and three values of type U filtering out comments.
  /*
  * The ParseLine operator scans a string and returns 0 if it finds a 
  * comment symbol. Otherwise it returns 1 and three values corresponding 
  * to the position of the particle (x,y,z) and a scalar value 
  * It is a good idea to set the values of point and colour to some 
  * predefined value before calling the operator.
   */
  template < typename T, typename U > 
    class ParseLine
  {
  public:
    ParseLine (): LookForEndString(false) {};
    int operator () (std::string &s, T point[3], U & scalar)
    {
      // Skip over comment lines.
      std::string::iterator itr;
      std::string tgt("/*");
      itr = std::search(s.begin(),s.end(),tgt.begin(),tgt.end());
      if ( itr != s.end() )
        {
        LookForEndString = true;
        //continue;
        }
      if ( LookForEndString )
        {
        tgt = "*/";
        itr = std::search(s.begin(),s.end(),tgt.begin(),tgt.end());
        if ( itr != s.end() )
          {
          LookForEndString = false;
          }
        //continue;
        return 0;
        }

      tgt = "//";
      itr = std::search(s.begin(),s.end(),tgt.begin(),tgt.end());
      if ( itr != s.end() )
        {
        return 0;
        }
      tgt = "%";
      itr = std::search(s.begin(),s.end(),tgt.begin(),tgt.end());
      if ( itr != s.end() )
        {
        return 0;
        }
      tgt = "#";
      itr = std::search(s.begin(),s.end(),tgt.begin(),tgt.end());
      if ( itr != s.end() )
        {
        return 0;
        }
      // If comma delimited, replace with tab
      std::replace(s.begin(),s.end(),',','\t');

      // We have data.
      std::stringstream ss;
      ss << s << std::ends;
      ss >> point[0] >> point[1] >> point[2] >> scalar;

      return 1;
    }

  private:
    bool LookForEndString;

  };

int main ( int argc, char* argv[] )
{
  if ( argc != 3 ) 
    {
    std::cout << "Need input file and output file." << std::endl;
    return 1;
    }
  std::string ifn = argv[1];
  std::string ofn = argv[2];
  std::cerr << "Input file: " << ifn << std::endl;
  std::cerr << "Output file: " << ofn << std::endl; 
  if ( ifn == ofn )
    {
    std::cerr << "File names cannot be the same." << std::endl;
    return 1;
    }
    
  std::ifstream ifs(ifn.c_str());
  std::ofstream ofs(ofn.c_str());

  if ( !ifs)
    {
    std::cerr << "Unable to open " << ifn << " for reading." << std::endl;
    }
  if ( !ofs)
    {
    std::cerr << "Unable to open " << ofn << " for writing." << std::endl;
    }

  ifs.seekg(0,std::ios::beg);

  GeographicConversions::Redfearn Redfearn("GRS80","MGA");
  if ( !(Redfearn.EllipsoidIsValid() && Redfearn.MapGridIsValid()) )
  {
  std::cerr << "Failed to create the geographic conversions object." << std::endl;
  return 1;
  }

  std::string s;
  int nullScalarValue = -1;

  // Read in the data.
  ParseLine<double,int> pl;

  while ( std::getline(ifs,s,'\n') )
    {
    if ( s.size() != 0 )
      {
      double pt[3];
      pt[0]=pt[1]=pt[2]=0;
      int scalar = nullScalarValue;

      if ( pl(s,pt,scalar) )
        {

        double E, N, GC, PS;
        std::string Z;
        // We are assuming Latitude, Longitude, Altitude and possibly a scalar.
        // Use this if you are sure that data doesn't cover an adjacent zone.
        Redfearn.GetGridCoordinates(pt[1], pt[0], Z, E, N, GC, PS);
        // Use this when you know the zone and if the data covers an adjacent zone.
        // Redfearn.GetZoneGridCoordinates(pt[1], pt[0], "<zone Name>", E, N, GC, PS);

        // Write it out.
        if ( scalar == nullScalarValue )
          {
          ofs << std::ios::fixed << std::setprecision(12) << E << " " << N << " " << pt[2] << std::endl;
          }
        else
          {
          ofs << std::ios::fixed << std::setprecision(12) << E << " " << N << " " << pt[2] << " " << scalar << std::endl;
          }
        }
        }
     }

  return 0;
}
