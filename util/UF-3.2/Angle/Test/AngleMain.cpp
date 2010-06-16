#include "ufSystemTime.h"
#include "ufAngleConversions.h"
#include "ufLogger.h"
#include <iostream>
#include <iomanip>
#include <sstream>

#ifdef _MSC_VER
#include <sys/timeb.h>
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef __QNXNTO__
#include <time.h>
#include <unistd.h>
#else
#include <ctime>
#endif


using namespace UF;
using namespace UF::Log;

bool TestSemicircleConversions( ufLogger *pLog )
{
  double d1 = 647999.0/3600.0; // 179 59 59
  double d2 = -647999.0/3600.0;
  int s1 = 2147480333;
  int s2 = -2147480333;
  double rd1 = Angle::AngleConversions::Instance()->semicircles2degrees(s1);
  double rd2 = Angle::AngleConversions::Instance()->semicircles2degrees(s2);
  int rs1 = Angle::AngleConversions::Instance()->degrees2semicircles(d1);
  int rs2 = Angle::AngleConversions::Instance()->degrees2semicircles(d2);

  bool t1 = std::abs(rd1 - d1) <= 1.0e-6;
  bool t2 = std::abs(rd2 - d2) <= 1.0e-6;
  bool t3 = rs1  - s1 == 0;
  bool t4 = rs2 - s2 == 0;

  *pLog <<  "Semicircles to degrees conversions." << std::endl;
  if ( !t1 )
  {
    *pLog << "Fail, expected: " << d1 << "got: " << rd1 << std::endl;
  }
  if ( !t2 )
  {
    *pLog << "Fail, expected: " << d2 << "got: " << rd2 << std::endl;
  }
  if ( t1 && t2 ) 
  {
    *pLog << "Semicircles to degrees conversions - passed." << std::endl;
  }
  *pLog <<  "Degrees to semicircles conversions." << std::endl;
  if ( !t3 )
  {
    *pLog << "Fail, expected: " << s1 << "got: " << rs1 << std::endl;
  }
  if ( !t4 )
  {
    *pLog << "Fail, expected: " << s2 << "got: " << rs2 << std::endl;
  }
  if ( t3 && t4 ) 
  {
    *pLog << "Degrees to semicircles conversions - passed." << std::endl;
  }

  *pLog <<  "Finished." << std::endl;

  return t1 && t2 && t3 && t4;
}


int AngleMain (int argc, char* argv[])
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
  pLog->open("TestAngle.log",std::ios::out);
  std::string time;
  Time::SystemTime *pClock = Time::SystemTime::Instance();
  pClock->ISOLocalTime(time);
  *pLog << sep << "\nStarted:  " << time << std::endl;

  bool fail = false;
  *pLog << "Testing the Angle Library" << std::endl;

  fail |= !TestSemicircleConversions(pLog);

  pClock->ISOLocalTime(time);
  *pLog << ((fail)?"Failed.":"Passed") << "\n"
      << "Finished: " << time << "\n" << sep << std::endl;

  delete pLog;

  if ( fail )
    return 1;

  return 0;
}
