#include "ufSystemTime.h"
#include "MakeColourTable.h"
#include <iostream>

using namespace UF;
using namespace UF::Log;
using namespace UF::RGBColour;

template < typename T, typename U >
  void test_colour_base ( ufLogger *pLog, T const & scale = 1 )
{

  Colour<T,U> *cb = new Colour<T,U>("White",0,0,0,255);
  Colour<T,U> *cb1 = new Colour<T,U>("Fik",255,63,31,7);
  *pLog << "Should be not equal: ";
  if ( *cb == *cb1 )
    *pLog << "Equal";
  else
    *pLog << "Not Equal";
  *pLog << std::endl;
  *cb = *cb1;
  T rr, gg, bb, aa;
  cb->get_colour_value(rr,gg,bb,aa);
  double V_ = V<T>()(rr,gg,bb,scale);
  double L_ = L<T>()(rr,gg,bb,scale);
  double B_ = B<T>()(rr,gg,bb,scale);
  double Y_ = Y<T>()(rr,gg,bb,scale);
  double Y709_ = Y709<T>()(rr,gg,bb,scale);
  *pLog << "Brightness V: " << V_ << " L: " << L_ << " B: " << B_ << " Y: " << Y_ << " Y709: " << Y709_ << std::endl;
  T r, g, b, a;
  cb->get_colour_value(r,g,b,a);
  *pLog << "Default constructor: " << (double)r << "," << (double)g << "," << (double)b << "," << (double)a << std::endl;
  cb->set_colour_value(1,15,31,255);
  cb->get_colour_value(r,g,b,a);
  *pLog << "Should see 1,15,31,255: " << (double)r << "," << (double)g << "," << (double)b << "," << (double)a << std::endl;
  delete cb;
  cb = new Colour<T,U>("Fik",255,63,31,7);
  *pLog << "Should be equal: ";
  if ( *cb == *cb1 )
    *pLog << "Equal";
  else
    *pLog << "Not Equal";
  *pLog << std::endl;
  cb->get_colour_value(r,g,b,a);
  *pLog << "Constructor, should see 255,63,31,7: " << (double)r << "," << (double)g << "," << (double)b << "," << (double)a << std::endl;
  cb->set_colour_value(1,15,31,255);
  cb->get_colour_value(r,g,b,a);
  *pLog << "Should see 1,15,31,255: " << (double)r << "," << (double)g << "," << (double)b << "," << (double)a << std::endl;
  delete cb;
  delete cb1;
  *pLog << std::endl;
}

template < typename T >
  void test_colour_base1 ( ufLogger *pLog )
{

  Colour<T> *cb = new Colour<T>("White",0,0,0,255);
  Colour<T> *cb1 = new Colour<T>("Fik",255,63,31,7);
  *pLog << "Should be not equal: ";
  if ( *cb == *cb1 )
    *pLog << "Equal";
  else
    *pLog << "Not Equal";
  *pLog << std::endl;
  *cb = *cb1;
  T rr, gg, bb, aa;
  cb->get_colour_value(rr,gg,bb,aa);
  double V_ = V<T>()(rr,gg,bb,255);
  double L_ = L<T>()(rr,gg,bb,255);
  double B_ = B<T>()(rr,gg,bb,255);
  double Y_ = Y<T>()(rr,gg,bb,255);
  double Y709_ = Y709<T>()(rr,gg,bb,255);
  *pLog << "Brightness V: " << V_ << " L: " << L_ << " B: " << B_ << " Y: " << Y_ << " Y709: " << Y709_ << std::endl;
  T r, g, b, a;
  cb->get_colour_value(r,g,b,a);
  *pLog << "Default constructor: " << (double)r << "," << (double)g << "," << (double)b << "," << (double)a << std::endl;
  cb->set_colour_value(1,15,31,255);
  cb->get_colour_value(r,g,b,a);
  *pLog << "Should see 1,15,31,255: " << (double)r << "," << (double)g << "," << (double)b << "," << (double)a << std::endl;
  delete cb;
  cb = new Colour<T>("Fik",255,63,31,7);
  *pLog << "Should be equal: ";
  if ( *cb == *cb1 )
    *pLog << "Equal";
  else
    *pLog << "Not Equal";
  *pLog << std::endl;
  cb->get_colour_value(r,g,b,a);
  *pLog << "Constructor, should see 255,63,31,7: " << (double)r << "," << (double)g << "," << (double)b << "," << (double)a << std::endl;
  cb->set_colour_value(1,15,31,255);
  cb->get_colour_value(r,g,b,a);
  *pLog << "Should see 1,15,31,255: " << (double)r << "," << (double)g << "," << (double)b << "," << (double)a << std::endl;
  delete cb;
  delete cb1;
  *pLog << std::endl;

}

int main(int /*argc*/, char** /*argv[]*/)
{
  std::string sep = "------------------------------------------------------------------------";
  // Set up the log file.
  ufLogger *pLog = new ufLogger;


  pLog->open("EgColour.log",std::ios::out);
  std::string time;
  Time::SystemTime *pClock = Time::SystemTime::Instance();
  pClock->ISOLocalTime(time);
  *pLog << sep << "\nStarted:  " << time << std::endl;

  bool fail = false;
  *pLog << "Testing the Colour Library" << std::endl;

  Colour<double> c;
  *pLog << std::endl
    << "Testing declarations of the form: Colour<T,U> *cb = new Colour<T,U>" << std::endl;
  *pLog << "Double<double,double>" << std::endl;
  test_colour_base<double,double>(pLog);
  *pLog << "Double<double,unsigned char>" << std::endl;
  test_colour_base<double,unsigned char>(pLog, 255);
  *pLog << "Float" << std::endl;
  test_colour_base<float,float>(pLog);
  *pLog << "int" << std::endl;
  test_colour_base<int,int>(pLog, 255);
  *pLog << "unsigned char" << std::endl;
  test_colour_base<unsigned char,unsigned char>(pLog, 255);
  *pLog << std::endl
    << "Testing declarations of the form: Colour<T> *cb = new Colour<T>" << std::endl;
  *pLog << "Double" << std::endl;
  test_colour_base1<double>(pLog);
  *pLog << "Float" << std::endl;
  test_colour_base1<float>(pLog);
  *pLog << "int" << std::endl;
  test_colour_base1<int>(pLog);
  *pLog << "unsigned char" << std::endl;
  test_colour_base1<unsigned char>(pLog);

  //This will exercise the Colour_Map class
  Colour_Map cm1(true);
  Colour_Map cm2;
  if ( cm1 == cm2 )
    *pLog << "Maps are equal" << std::endl;
  else
    *pLog << "Maps are not equal" << std::endl;
  cm1 = cm2;
  if ( cm1 == cm2 )
    *pLog << "Maps are equal" << std::endl;
  else
    *pLog << "Maps are not equal" << std::endl;

  // Write out a colour map.
  make_the_table(pLog);

  Colour<double,double> dc;
  dc.set_colour_value(0.5,0.2,0.1,1);
  Colour<> uc;
  ConvertColourType<double,double>()(dc,uc);
  ConvertColourType<unsigned char,unsigned char,double,double>()(uc,dc,1.0/255.0);

  pClock->ISOLocalTime(time);
  *pLog << ((fail)?"Failed.":"Passed") << "\n"
      << "Finished: " << time << "\n" << sep << std::endl;

  delete pLog;

  if ( fail )
    return 1;

  return 0;
}

