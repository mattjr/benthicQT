#include "ufColour.h"
#include "ufSystemTime.h"
#include "ufLogger.h"

#include <fstream>
#include <iostream>

using namespace UF;
using namespace UF::Log;

//std::string fileName = "ColourNames.html";

struct SColour {
  SColour ( double h, double s, double v, RGBColour::Colour<unsigned char> &colour )
  {
    this->h = h;
    this->s = s;
    this->v = v;
    this->colour = colour;
  }

  double h;
  double s;
  double v;
  RGBColour::Colour<unsigned char> colour;

};

//// Some comparison functions for sorting.
//
//bool lt_h ( SColour const &lhs, SColour const &rhs )
//{
//  return ( lhs.h < rhs.h );
//}
//
//bool lt_s ( SColour const &lhs, SColour const &rhs )
//{
//  return ( lhs.s < rhs.s );
//}
//
//bool lt_v ( SColour const &lhs, SColour const &rhs )
//{
//  return ( lhs.v < rhs.v );
//}
//
//bool ltV ( SColour &lhs, SColour &rhs )
//{
//  return ( lhs.colour.V() < rhs.colour.V() );
//}
//bool ltL ( SColour &lhs, SColour &rhs )
//{
//  return ( lhs.colour.L() < rhs.colour.L() );
//}
//bool ltB ( SColour &lhs, SColour &rhs )
//{
//  return ( lhs.colour.B() < rhs.colour.B() );
//}
//bool ltY ( SColour &lhs, SColour &rhs )
//{
//  return ( lhs.colour.Y() < rhs.colour.Y() );
//}
//bool ltY709 ( SColour &lhs, SColour &rhs )
//{
//  return ( lhs.colour.Y709() < rhs.colour.Y709() );
//}

void WriteHeader ( std::ofstream & f, std::string title1, std::string title2 )
{
  f << "<html>" << std::endl;
  f << "<h1 align=\"center\">" << title1 << "</h1>" << std::endl;
  f << "<p align=\"center\">" << title2 << "</p>" << std::endl;
  f << "<a name=\"Top\"></a>" << std::endl;
}

void WriteIndex (std::ofstream & f, std::string const & title )
{
  f << "<p><a name=\"Index\">Index</a>:<br>" << std::endl;
  f << "<a href=\"#A\">A </a>" << std::endl;
  f << "<a href=\"#B\">B </a>" << std::endl;
  f << "<a href=\"#C\">C </a>" << std::endl;
  f << "<a href=\"#D\">D </a>" << std::endl;
  f << "<a href=\"#E\">E </a>" << std::endl;
  f << "<a href=\"#F\">F </a>" << std::endl;
  f << "<a href=\"#G\">G </a><br>" << std::endl;
  f << "<a href=\"#H\">H </a>" << std::endl;
  f << "<a href=\"#I\">I </a>" << std::endl;
  //f << "<a href=\"#J\">J </a>" << std::endl;
  f << "J " << std::endl;
  f << "<a href=\"#K\">K </a>" << std::endl;
  f << "<a href=\"#L\">L </a>" << std::endl;
  f << "<a href=\"#M\">M </a>" << std::endl;
  f << "<a href=\"#N\">N </a><br>" << std::endl;
  f << "<a href=\"#O\">O </a>" << std::endl;
  f << "<a href=\"#P\">P </a>" << std::endl;
  //f << "<a href=\"#Q\">Q </a>" << std::endl;
  f << "Q " << std::endl;
  f << "<a href=\"#R\">R </a>" << std::endl;
  f << "<a href=\"#S\">S </a>" << std::endl;
  f << "<a href=\"#T\">T </a>" << std::endl;
  f << "<a href=\"#U\">U </a><br>" << std::endl;
  f << "<a href=\"#V\">V </a>" << std::endl;
  f << "<a href=\"#W\">W </a>" << std::endl;
  //f << "<a href=\"#X\">X </a>" << std::endl;
  f << "X " << std::endl;
  f << "<a href=\"#Y\">Y </a>" << std::endl;
  //f << "<a href=\"#Z\">Z </a><br>" << std::endl;
  f << "Z <br>" << std::endl;
  //f << "<a href=\"#HI\">Sorted by Hue and Lightness and Saturation.</a>" << std::endl;
  f << "<a href=\"#HI\">" << title << "</a>" << std::endl;
  f << "<p>&nbsp;</p>" << std::endl;
}

void WriteTableHeader( std::ofstream & f, std::string idx )
{
  f << "<div align=\"center\">" << std::endl;
  f << "<center>" << std::endl;
  f << "<table width=\"100%\" cellSpacing=\"0\" cellPadding=\"0\" border=\"5\">" << std::endl;
  f << "<tr bgColor=\"#ffffff\">" << std::endl;
    f << "<th width=\"25%\"><a href=\"#Index\">Index</th>" << std::endl;
  f << "<th colspan=\"3\">RGB</th>" << std::endl;
  f << "<th colspan=\"2\">Colour</th>" << std::endl;
    f << "<th colspan=\"2\">Complement</th>" << std::endl;
  f << "</tr>" << std::endl;
  f << "<tr bgColor=\"#ffffff\">" << std::endl;
     if ( idx.empty() )
    f << "<th>Color Name</th>" << std::endl;
  else
    f << "<th><a name=\"" << idx.c_str() << "\">Color Name </a></th>" << std::endl;
  f << "<th>R</th>" << std::endl;
    f << "<th>G</th>" << std::endl;
    f << "<th>B</th>" << std::endl;
  f << "<th>Hex</th>" << std::endl;
    f << "<th width=\"20%\">Colour</th>" << std::endl;
   f << "<th>Hex</th>" << std::endl;
    f << "<th width=\"20%\">Colour</th>" << std::endl;
  f << "</tr>" << std::endl;
}

void WriteRow ( std::ofstream & f, RGBColour::Colour<unsigned char> & colour )
{
  unsigned char r, g, b, a;

  colour.get_colour_value(r,g,b,a);
  std::string hex_col(colour.colour_value().hex());
  std::string comp_hex_col(colour.colour_value().hex_complement());
  f << "<tr>" << std::endl;
    f << "<td>"<< colour.colour_name() << "</td>" << std::endl;
  f << "<td>" << std::setw(3) << std::setfill('0') << (int)r << "</td>" << std::endl;
  f << "<td>" << std::setw(3) << std::setfill('0') << (int)g << "</td>" << std::endl;
    f << "<td>" << std::setw(3) << std::setfill('0') << (int)b << "</td>" << std::endl;
    f << "<td>" << hex_col << "</td>" << std::endl;
    f << "<td bgColor=\""<< hex_col << "\"><br>&nbsp;</td>" << std::endl;
    f << "<td>" << comp_hex_col << "</td>" << std::endl;
    f << "<td bgColor=\""<< comp_hex_col << "\"><br>&nbsp;</td>" << std::endl;
  f << "</tr>" << std::endl;
}

void WriteEndOfTable ( std::ofstream & f )
{
  f << "</table>" << std::endl;
  f << "</center>" << std::endl;
  f << "</div>" << std::endl;
}

void WriteTail ( std::ofstream & f )
{
  f << "</html>" << std::endl;
}

//void make_the_table ( ufLogger *pLog )
//{
//
//  //! Write out a table of colour names, their RGB and Hex values to an html file.
//
//  // Get a vector of colour names.
//  typedef std::vector<std::string> CN;
//  typedef std::vector<std::string>::iterator CN_It;
//
//  typedef std::map < RGBColour::CKey_hsv, RGBColour::Colour<unsigned char>, RGBColour::CKey_hsv_less >  THSVMap;
//  typedef THSVMap::iterator THSVMapIt;
//
//  typedef std::map < RGBColour::CKey_hls, RGBColour::Colour<unsigned char>, RGBColour::CKey_hls_less >  THLSMap;
//  typedef THLSMap::iterator THLSMapIt;
//
//  typedef std::map < RGBColour::CKey_rgb, RGBColour::Colour<unsigned char>, RGBColour::CKey_rgb_less >  TRGBMap;
//  typedef TRGBMap::iterator TRGBMapIt;
//
//  // Declare and open a file stream.
//  std::ofstream f(fileName.c_str());
//  if ( !f )
//  {
//    *pLog << "Unable to create the file " << fileName <<".\nDoes the directory exist?" << std::endl;
//    return;
//  }
//
//  // Here is the colour map.
//  RGBColour::Colour_Map colourMap;
//
//  // Get a vector of colour names.
//  CN colours;
//  colours = colourMap.get_names();
//
//  // Here is a vector of colours.
//  //SCOLV v_col;
//  // A map of sorted colours.
//  THSVMap HSVMap;
//  THLSMap HLSMap;
//  TRGBMap RGBMap;
//
//  // Write out the table.
//  *pLog << "Writing out the names of the colours and their values"
//    << std::endl << " to the file: " << fileName << std::endl;
//  std::string title1 = "RGB to Color Name Mapping(Triplet and Hex)";
//  std::string title2 = "Sorted on name.";
//  WriteHeader(f, title1, title2);
//  WriteIndex(f);
//  char firstChar = '\0';
//  for ( CN_It i = colours.begin(); i != colours.end(); ++i)
//  {
//    double r, g, b, a;
//    unsigned char ur, ug, ub, ua;
//    double h, l, s, v;
//
//    RGBColour::TColour_Map_CItr p = colourMap.colour_map.find(*i);
//    if ( p == colourMap.colour_map.end() ) continue;
//    RGBColour::Colour<unsigned char> colour(p->first,p->second);
//    // r,g,b values are from 0 to 1
//    //colour.get_colour_value_f(r,g,b,a);
//    colour.get_colour_value(ur,ug,ub,ua);
//    r = ur / 255;
//    g = ug / 255;
//    b = ub / 255;
//    a = ua / 255;
//    RGBColour::RGB_HSV()(r,g,b,h,s,v);
//    if ( h >= 0 /*&& !(r == g && r == b && g == b) */)
//    {
//      RGBColour::CKey_hsv key(h,s,v,colour);
//      HSVMap[key] = colour;
//    }
//    RGBColour::RGB_HLS()(r,g,b,h,l,s);
//    if ( h >= 0 /*&& !(r == g && r == b && g == b)*/ )
//    {
//      RGBColour::CKey_hls key(h,l,s,colour);
//      HLSMap[key] = colour;
//    }
//
//    RGBColour::CKey_rgb key(colour);
//    RGBMap[key] = colour;
//
//    //v_col.push_back(SColour(h+colour.Y709(),s,l,colour));
//    if ( i == colours.begin() )
//    {
//      firstChar = (*i)[0];
//      std::string idx;
//      idx.clear();
//      idx.push_back(firstChar);
//      std::locale loc;
//      std::use_facet<std::ctype<char> > ( loc ).toupper( &idx[0], &idx[idx.size() - 1] );
//      WriteTableHeader( f, idx );
//    }
//    if ( firstChar != (*i)[0] )
//    {
//      firstChar = (*i)[0];
//      WriteEndOfTable(f);
//      std::string idx;
//      idx.clear();
//      idx.push_back(firstChar);
//      std::locale loc;
//      std::use_facet<std::ctype<char> > ( loc ).toupper( &idx[0], &idx[idx.size() - 1] );
//      WriteTableHeader( f, idx );
//    }
//
//    WriteRow(f,colour);
//  }
//  WriteEndOfTable(f);
//  WriteTail(f);
//
//  // Sort on brightness then on hue.
//  //std::sort(v_col.begin(),v_col.end(), lt_h );
//  //std::sort(v_col.begin(),v_col.end(), lt_s );
//  //std::sort(v_col.begin(),v_col.end(), lt_v );
//  //std::sort(v_col.begin(),v_col.end(), ltB );
//  //std::sort(v_col.begin(),v_col.end(), ltL );
//  //std::sort(v_col.begin(),v_col.end(), ltV );
//  //std::sort(v_col.begin(),v_col.end(), ltY );
//  //std::sort(v_col.begin(),v_col.end(), ltY709 );
//  //title2 = "Sorted on hue, saturation and value.";
//  title2 = "Sorted on hue, lightness and saturation.";
//  //title2 = "Sorted on red, green and blue.";
//  WriteHeader(f,title1,title2);
//  WriteTableHeader( f, "HI" );
//  //for ( THSVMapIt p = HSVMap.begin(); p != HSVMap.end(); ++p)
//  //{
//  //  WriteRow(f,p->second);
//  //}
//  for ( THLSMapIt p = HLSMap.begin(); p != HLSMap.end(); ++p)
//  {
//    WriteRow(f,p->second);
//  }
//  //for ( TRGBMapIt p = RGBMap.begin(); p != RGBMap.end(); ++p)
//  //{
//  //  WriteRow(f,p->second);
//  //}
//
//  WriteTail(f);
//  WriteEndOfTable(f);
//
//  return;
//
//}
//


int MakeRGBTable(ufLogger *pLog, std::string const & fileName)
{
  //! Write out a table of colour names, their RGB and Hex values to an html file.

  // Get a vector of colour names.
  typedef std::vector<std::string> CN;
  typedef std::vector<std::string>::iterator CN_It;

  typedef std::map < RGBColour::CKey_rgb, RGBColour::Colour<unsigned char>, RGBColour::CKey_rgb_less >  TRGBMap;
  typedef TRGBMap::iterator TRGBMapIt;

  // Declare and open a file stream.
  std::ofstream f(fileName.c_str());
  if ( !f )
  {
    *pLog << "Unable to create the file " << fileName <<".\nDoes the directory exist?" << std::endl;
    return 1;
  }

  // Here is the colour map.
  RGBColour::Colour_Map colourMap;

  // Get a vector of colour names.
  CN colours;
  colours = colourMap.get_names();

  // A map of sorted colours.
  TRGBMap RGBMap;

  // Write out the table.
  *pLog << "Writing out the names of the colours and their values"
    << " to the file: " << fileName << std::endl;
  std::string title1 = "RGB to Color Name Mapping(Triplet and Hex)";
  std::string title2 = "Sorted on name.";
  WriteHeader(f, title1, title2);
  WriteIndex(f,"Sorted by red, green and blue.");
  char firstChar = '\0';
  for ( CN_It i = colours.begin(); i != colours.end(); ++i)
  {
    double r, g, b, a;
    unsigned char ur, ug, ub, ua;
    //double h, l, s, v;

    RGBColour::TColour_Map_CItr p = colourMap.colour_map.find(*i);
    if ( p == colourMap.colour_map.end() ) continue;
    RGBColour::Colour<unsigned char> colour(p->first,p->second);
    // r,g,b values are from 0 to 1
    //colour.get_colour_value_f(r,g,b,a);
    colour.get_colour_value(ur,ug,ub,ua);
    r = ur / 255.0;
    g = ug / 255.0;
    b = ub / 255.0;
    a = ua / 255.0;
    //RGBColour::RGB_HSV()(r,g,b,h,s,v);
    //if ( h >= 0 /*&& !(r == g && r == b && g == b) */)
    //{
    //  RGBColour::CKey_hsv key(h,s,v,colour);
    //  HSVMap[key] = colour;
    //}
    //RGBColour::RGB_HLS()(r,g,b,h,l,s);
    //if ( h >= 0 /*&& !(r == g && r == b && g == b)*/ )
    //{
    //  RGBColour::CKey_hls key(h,l,s,colour);
    //  HLSMap[key] = colour;
    //}

    RGBColour::CKey_rgb key(colour);
    RGBMap[key] = colour;

    //v_col.push_back(SColour(h+colour.Y709(),s,l,colour));
    if ( i == colours.begin() )
    {
      firstChar = (*i)[0];
      std::string idx;
      idx.clear();
      idx.push_back(firstChar);
      std::locale loc;
      std::use_facet<std::ctype<char> > ( loc ).toupper( &idx[0], &idx[idx.size() - 1] );
      WriteTableHeader( f, idx );
    }
    if ( firstChar != (*i)[0] )
    {
      firstChar = (*i)[0];
      WriteEndOfTable(f);
      std::string idx;
      idx.clear();
      idx.push_back(firstChar);
      std::locale loc;
      std::use_facet<std::ctype<char> > ( loc ).toupper( &idx[0], &idx[idx.size() - 1] );
      WriteTableHeader( f, idx );
    }

    WriteRow(f,colour);
  }
  WriteEndOfTable(f);
  WriteTail(f);

  // Sort on brightness then on hue.
  //std::sort(v_col.begin(),v_col.end(), lt_h );
  //std::sort(v_col.begin(),v_col.end(), lt_s );
  //std::sort(v_col.begin(),v_col.end(), lt_v );
  //std::sort(v_col.begin(),v_col.end(), ltB );
  //std::sort(v_col.begin(),v_col.end(), ltL );
  //std::sort(v_col.begin(),v_col.end(), ltV );
  //std::sort(v_col.begin(),v_col.end(), ltY );
  //std::sort(v_col.begin(),v_col.end(), ltY709 );
  //title2 = "Sorted on hue, saturation and value.";
  //title2 = "Sorted on hue, lightness and saturation.";
  title2 = "Sorted on red, green and blue.";
  WriteHeader(f,title1,title2);
  WriteTableHeader( f, "HI" );
  //for ( THSVMapIt p = HSVMap.begin(); p != HSVMap.end(); ++p)
  //{
  //  WriteRow(f,p->second);
  //}
  //for ( THLSMapIt p = HLSMap.begin(); p != HLSMap.end(); ++p)
  //{
  //  WriteRow(f,p->second);
  //}
  for ( TRGBMapIt p = RGBMap.begin(); p != RGBMap.end(); ++p)
  {
    WriteRow(f,p->second);
  }

  WriteTail(f);
  WriteEndOfTable(f);

  return 0;

}

int MakeHLSTable(ufLogger *pLog, std::string const & fileName)
{

  //! Write out a table of colour names, their RGB and Hex values to an html file.

  // Get a vector of colour names.
  typedef std::vector<std::string> CN;
  typedef std::vector<std::string>::iterator CN_It;

  typedef std::map < RGBColour::CKey_hls, RGBColour::Colour<unsigned char>, RGBColour::CKey_hls_less >  THLSMap;
  typedef THLSMap::iterator THLSMapIt;

  typedef std::map < RGBColour::CKey_rgb, RGBColour::Colour<unsigned char>, RGBColour::CKey_rgb_less >  TRGBMap;
  typedef TRGBMap::iterator TRGBMapIt;

  // Declare and open a file stream.
  std::ofstream f(fileName.c_str());
  if ( !f )
  {
    *pLog << "Unable to create the file " << fileName <<".\nDoes the directory exist?" << std::endl;
    return 1;
  }

  // Here is the colour map.
  RGBColour::Colour_Map colourMap;

  // Get a vector of colour names.
  CN colours;
  colours = colourMap.get_names();

  // Here is a vector of colours.
  //SCOLV v_col;
  // A map of sorted colours.
  THLSMap HLSMap;
  TRGBMap RGBMap;

  // Write out the table.
  *pLog << "Writing out the names of the colours and their values"
    << " to the file: " << fileName << std::endl;
  std::string title1 = "RGB to Color Name Mapping(Triplet and Hex)";
  std::string title2 = "Sorted on name.";
  WriteHeader(f, title1, title2);
  WriteIndex(f,"Sorted by hue, lightness and saturation.");
  char firstChar = '\0';
  for ( CN_It i = colours.begin(); i != colours.end(); ++i)
  {
    double r, g, b, a;
    unsigned char ur, ug, ub, ua;
    double h, l, s;

    RGBColour::TColour_Map_CItr p = colourMap.colour_map.find(*i);
    if ( p == colourMap.colour_map.end() ) continue;
    RGBColour::Colour<unsigned char> colour(p->first,p->second);
    // r,g,b values are from 0 to 1
    //colour.get_colour_value_f(r,g,b,a);
    colour.get_colour_value(ur,ug,ub,ua);
    r = ur / 255.0;
    g = ug / 255.0;
    b = ub / 255.0;
    a = ua / 255.0;
    RGBColour::RGB_HLS()(r,g,b,h,l,s);
    if ( h >= 0 /*&& !(r == g && r == b && g == b)*/ )
    {
      RGBColour::CKey_hls key(h,l,s,colour);
      HLSMap[key] = colour;
    }

    RGBColour::CKey_rgb key(colour);
    RGBMap[key] = colour;

    //v_col.push_back(SColour(h+colour.Y709(),s,l,colour));
    if ( i == colours.begin() )
    {
      firstChar = (*i)[0];
      std::string idx;
      idx.clear();
      idx.push_back(firstChar);
      std::locale loc;
      std::use_facet<std::ctype<char> > ( loc ).toupper( &idx[0], &idx[idx.size() - 1] );
      WriteTableHeader( f, idx );
    }
    if ( firstChar != (*i)[0] )
    {
      firstChar = (*i)[0];
      WriteEndOfTable(f);
      std::string idx;
      idx.clear();
      idx.push_back(firstChar);
      std::locale loc;
      std::use_facet<std::ctype<char> > ( loc ).toupper( &idx[0], &idx[idx.size() - 1] );
      WriteTableHeader( f, idx );
    }

    WriteRow(f,colour);
  }
  WriteEndOfTable(f);
  WriteTail(f);

  // Sort on brightness then on hue.
  //std::sort(v_col.begin(),v_col.end(), lt_h );
  //std::sort(v_col.begin(),v_col.end(), lt_s );
  //std::sort(v_col.begin(),v_col.end(), lt_v );
  //std::sort(v_col.begin(),v_col.end(), ltB );
  //std::sort(v_col.begin(),v_col.end(), ltL );
  //std::sort(v_col.begin(),v_col.end(), ltV );
  //std::sort(v_col.begin(),v_col.end(), ltY );
  //std::sort(v_col.begin(),v_col.end(), ltY709 );
  //title2 = "Sorted on hue, saturation and value.";
  title2 = "Sorted on hue, lightness and saturation.";
  //title2 = "Sorted on red, green and blue.";
  WriteHeader(f,title1,title2);
  WriteTableHeader( f, "HI" );
  //for ( THSVMapIt p = HSVMap.begin(); p != HSVMap.end(); ++p)
  //{
  //  WriteRow(f,p->second);
  //}
  for ( THLSMapIt p = HLSMap.begin(); p != HLSMap.end(); ++p)
  {
    WriteRow(f,p->second);
  }
  //for ( TRGBMapIt p = RGBMap.begin(); p != RGBMap.end(); ++p)
  //{
  //  WriteRow(f,p->second);
  //}

  WriteTail(f);
  WriteEndOfTable(f);

  return 0;

}


int MakeHSVTable(ufLogger *pLog, std::string const & fileName)
{

  //! Write out a table of colour names, their RGB and Hex values to an html file.

  // Get a vector of colour names.
  typedef std::vector<std::string> CN;
  typedef std::vector<std::string>::iterator CN_It;

  typedef std::map < RGBColour::CKey_hsv, RGBColour::Colour<unsigned char>, RGBColour::CKey_hsv_less >  THSVMap;
  typedef THSVMap::iterator THSVMapIt;

  typedef std::map < RGBColour::CKey_rgb, RGBColour::Colour<unsigned char>, RGBColour::CKey_rgb_less >  TRGBMap;
  typedef TRGBMap::iterator TRGBMapIt;

  // Declare and open a file stream.
  std::ofstream f(fileName.c_str());
  if ( !f )
  {
    *pLog << "Unable to create the file " << fileName <<".\nDoes the directory exist?" << std::endl;
    return 1;
  }

  // Here is the colour map.
  RGBColour::Colour_Map colourMap;

  // Get a vector of colour names.
  CN colours;
  colours = colourMap.get_names();

  // Here is a vector of colours.
  //SCOLV v_col;
  // A map of sorted colours.
  THSVMap HSVMap;
  TRGBMap RGBMap;

  // Write out the table.
  *pLog << "Writing out the names of the colours and their values"
    << " to the file: " << fileName << std::endl;
  std::string title1 = "RGB to Color Name Mapping(Triplet and Hex)";
  std::string title2 = "Sorted on name.";
  WriteHeader(f, title1, title2);
  WriteIndex(f,"Sorted by hue, saturation and value.");
  char firstChar = '\0';
  for ( CN_It i = colours.begin(); i != colours.end(); ++i)
  {
    double r, g, b, a;
    unsigned char ur, ug, ub, ua;
    double h, s, v;

    RGBColour::TColour_Map_CItr p = colourMap.colour_map.find(*i);
    if ( p == colourMap.colour_map.end() ) continue;
    RGBColour::Colour<unsigned char> colour(p->first,p->second);
    // r,g,b values are from 0 to 1
    //colour.get_colour_value_f(r,g,b,a);
    colour.get_colour_value(ur,ug,ub,ua);
    r = ur / 255.0;
    g = ug / 255.0;
    b = ub / 255.0;
    a = ua / 255.0;
    RGBColour::RGB_HSV()(r,g,b,h,s,v);
    if ( h >= 0 /*&& !(r == g && r == b && g == b)*/ )
    {
      RGBColour::CKey_hsv key(h,s,v,colour);
      HSVMap[key] = colour;
    }

    RGBColour::CKey_rgb key(colour);
    RGBMap[key] = colour;

    //v_col.push_back(SColour(h+colour.Y709(),s,l,colour));
    if ( i == colours.begin() )
    {
      firstChar = (*i)[0];
      std::string idx;
      idx.clear();
      idx.push_back(firstChar);
      std::locale loc;
      std::use_facet<std::ctype<char> > ( loc ).toupper( &idx[0], &idx[idx.size() - 1] );
      WriteTableHeader( f, idx );
    }
    if ( firstChar != (*i)[0] )
    {
      firstChar = (*i)[0];
      WriteEndOfTable(f);
      std::string idx;
      idx.clear();
      idx.push_back(firstChar);
      std::locale loc;
      std::use_facet<std::ctype<char> > ( loc ).toupper( &idx[0], &idx[idx.size() - 1] );
      WriteTableHeader( f, idx );
    }

    WriteRow(f,colour);
  }
  WriteEndOfTable(f);
  WriteTail(f);

  // Sort on brightness then on hue.
  //std::sort(v_col.begin(),v_col.end(), lt_h );
  //std::sort(v_col.begin(),v_col.end(), lt_s );
  //std::sort(v_col.begin(),v_col.end(), lt_v );
  //std::sort(v_col.begin(),v_col.end(), ltB );
  //std::sort(v_col.begin(),v_col.end(), ltL );
  //std::sort(v_col.begin(),v_col.end(), ltV );
  //std::sort(v_col.begin(),v_col.end(), ltY );
  //std::sort(v_col.begin(),v_col.end(), ltY709 );
  //title2 = "Sorted on hue, saturation and value.";
  title2 = "Sorted on hue, saturation and value.";
  WriteHeader(f,title1,title2);
  WriteTableHeader( f, "HI" );
  for ( THSVMapIt p = HSVMap.begin(); p != HSVMap.end(); ++p)
  {
    WriteRow(f,p->second);
  }

  WriteTail(f);
  WriteEndOfTable(f);

  return 0;

}



int MakeColourTable(int argc, char* argv[])
{
  // Get rid of the argc, argv unreferenced formal parameter warnings.
  int numargs;
  std::string programName;
  numargs = argc;
  if ( argc != 0 )
  {
    programName = argv[0];
  }

  std::string sep = "------------------------------------------------------------------------";
  // Set up the log file.
  ufLogger *pLog = new ufLogger;


  pLog->open("MakeColourtable.log",std::ios::out);
  std::string time;
  Time::SystemTime *pClock = Time::SystemTime::Instance();
  pClock->ISOLocalTime(time);
  *pLog << sep << "\nStarted:  " << time << std::endl;

  int fail = false;
  int result;
  *pLog << "Testing the Colour Tables" << std::endl;
  fail = MakeRGBTable(pLog, "ColoursRGB.html");
  *pLog << "RGB Colour Table " << ((fail)?"failed.":"passed") << "\n";
  result = fail;
  fail = MakeHLSTable(pLog, "ColoursHLS.html");
  *pLog << "HLS Colour Table " << ((fail)?"failed.":"passed") << "\n";
  result |= fail;
  fail = MakeHSVTable(pLog, "ColoursHSV.html");
  *pLog << "HSV Colour Table " << ((fail)?"failed.":"passed") << "\n";
  result |= fail;

  pClock->ISOLocalTime(time);
  *pLog << ((result)?"Some tests failed.":"All tests passed.") << "\n"
     << "Finished: " << time << "\n" << sep << std::endl;
  delete pLog;
  if ( result )
    return result;

  return result;
}
