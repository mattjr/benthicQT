// main.cpp : Defines the entry point for the console application.
//

//#include "ufAngleDateTimeConversions.h"
#include "ufNumberConversions.h"
#include "ufStringFormat.h"
#include "ufRotateVector.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>

#include "ufSystemTime.h"
#include "ufLogger.h"

using namespace UF;
using namespace UF::Log;

// Change the type here.
typedef float T;

template < typename T >
void Normalize ( T const v1[3], T v2[3] )
{
  T s = 0;
  for ( int i = 0 ; i < 3; ++ i ) s += v1[i] * v1[i];
  s = std::sqrt(s);
  if ( s != 0 )
    for ( int i = 0; i < 3; ++i ) v2[i] = v1[i] / s;
  else
    for ( int i = 0; i < 3; ++i ) v2[i] = 0;
}

//void TestTimeDate ( ufLogger *pLog )
//{
//  int h, m;
//  double s;
//
//  h = 12;
//  m = 2;
//  s = 1.23456;
//
//  *pLog << "Time: " << AngleNum::time2s()(h,m,s,2,':') << std::endl;
//  s = 21.23456;
//  *pLog << "Time: " << AngleNum::time2s()(h,m,s,4,':') << std::endl;
//  h = 0;
//  m = 0;
//  s = -0.1;
//  *pLog << "Time: " << AngleNum::time2s()(h,m,s) << std::endl;
//
//  int year, month;
//  double day;
//  year = 2005;
//  month = 12;
//  day = 1;
//  *pLog << "Date: " << AngleNum::date2s()(year,month,day,0,'/') << std::endl;
//  day = 31.1419;
//  *pLog << "Date: " << AngleNum::date2s()(year,month,day,3,0) << std::endl;
//  month = 1;
//  year = 0;
//  *pLog << "Date: " << AngleNum::date2s()(year,month,day,0,'/') << std::endl;
//  month = -1;
//  *pLog << "Date: " << AngleNum::date2s()(year,month,day,0,'/') << std::endl;
//  year = -4717;
//  *pLog << "Date: " << AngleNum::date2s()(year,month,day,0,'/') << std::endl;
//}

template <typename T>
  void TestRotationMatrix( ufLogger *pLog )
{
  T from[3];
  T to[3];
  T rotMat[3][3];

  CoordinateConversions::RotateVector<T> R;

  from[0] = 1; from[1] = 0; from[2] = 0;
  *pLog << "From: "
    << from[0] << " " << from[1] << " " << from[2] << std::endl;
  to[0] = 0; to[1] = -1; to[2] = 0;
  *pLog << "To: "
    << to[0] << " " << to[1] << " " << to[2]<< std::endl;

  // Normalise
  Normalize(from,from);
  Normalize(to,to);

  R.fromToRotation(from,to,rotMat);

  *pLog << "Normalized." << std::endl;
  *pLog << "From: ";
  for ( int i = 0; i < 3; ++i) *pLog << from[i] << " ";
  *pLog << std::endl;
  *pLog << "To:   ";
  for ( int i = 0; i < 3; ++i) *pLog << to[i] << " ";
  *pLog << std::endl;
  *pLog << "Rotation Matrix:" << std::endl;
  for ( int i = 0; i < 3; ++i )
  {
    for ( int j = 0; j < 3; ++j ) *pLog << rotMat[i][j] << " ";
    *pLog << std::endl;
  }

  T r[3]; // the result.
  for ( int i = 0; i < 3; ++i ) r[i] = 0;
  for ( int i = 0; i < 3; ++i )
  {
    T s = 0;
    for ( int j = 0; j < 3; ++j )
      s += rotMat[i][j] * from[j];
    r[i] = s;
  }
  *pLog << "Result (== To): ";
  for ( int i = 0; i < 3; ++i) *pLog << r[i] << " ";
  *pLog << std::endl;

}

void TestReplaceCRLF( ufLogger *pLog )
{
  std::string s;

  s = "Here is a string with two CRLF's. \r\nIts size should be: 60\r\n";
  *pLog << s << std::endl;
  *pLog << "After replace_crlf() the size will be 58." << std::endl;
  StrFmt::Replace_CRLF_LF()(s);
  *pLog << "The size is in fact:" << (unsigned int)s.size()
    << std::endl << "and the string is:"  << std::endl << s << std::endl;

}

//void TestAngleStuff( ufLogger *pLog )
//{
//  //AngleNum::CAngle angle;
//  AngleNum::Angle angle('-',15,12,3);
//  angle.d(90.123);
//  angle.d(' ',15.2345123456789);
//  char sgn;
//  int d;
//  double m;
//  double sec;
//  angle.dm(sgn,d,m);
//  std::string s = StrFmt::make_string() << "The angle is:" << angle.get_d();
//  std::string str = AngleNum::hm2str(d,m)();
//  *pLog << s << " or " << str;
//  angle.dms(sgn,d,m,sec);
//  str = AngleNum::hms2str(d,(int)m,sec)();
//  *pLog << " or " << str << std::endl;
//}

void TestNumConv( ufLogger *pLog )
{
  double x = std::atan(1.0)*4.0;
  *pLog << "Pi is: " << NumConv::n2s<double>()(x,7,12,'*',std::ios::left|std::ios::fixed) << std::endl;
  *pLog << "Pi is: " << NumConv::n2s<double>()(x,5,15,'*',std::ios::right|std::ios::scientific) << std::endl;
}

int main()
{
  std::string sep = "------------------------------------------------------------------------";
  // Set up the log file.
  ufLogger *pLog = new ufLogger;


  pLog->open("EgConversions.log",std::ios::out);
  std::string time;
  Time::SystemTime *pClock = Time::SystemTime::Instance();
  pClock->ISOLocalTime(time);
  *pLog << sep << "\nStarted:  " << time << std::endl;

  bool fail = false;
  *pLog << "Testing the Conversions" << std::endl;

  //TestAngleStuff(pLog);
  TestNumConv(pLog);
  TestRotationMatrix<float>(pLog);
  TestReplaceCRLF(pLog);
  //TestTimeDate(pLog);
  //bool res = TestSemicircleConversions(pLog);
  //if ( !res ) 
  //{
  //  *pLog << "TestSemicircleConversions() failed." << std::endl;
  //}

  pClock->ISOLocalTime(time);
  *pLog << ((fail)?"Failed.":"Passed") << "\n"
      << "Finished: " << time << "\n" << sep << std::endl;

  delete pLog;

  if ( fail )
    return 1;

  return 0;
}

