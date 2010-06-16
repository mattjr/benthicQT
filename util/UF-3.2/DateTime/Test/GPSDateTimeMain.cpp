#include "ufDateTime.h"
#include "ufSystemTime.h"
#include "ufTimeConversions.h"
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

int GPSDateTimeMain ( int argc, char* argv[] )
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
  pLog->open("TestGPSDateTime.log",std::ios::out);
  std::string time;
  Time::SystemTime *pClock = Time::SystemTime::Instance();
  pClock->ISOLocalTime(time);
  *pLog << sep << "\nStarted:  " << time << std::endl;

  bool fail = false;
  *pLog << "Testing the GPS Date-Time Library" << std::endl;

  Time::SystemTime *pT = Time::SystemTime::Instance();

  Time::DateTime dt;
  Time::MonthNames mn;
  Time::DayNames dn;

  int y;
  int m;
  int d;
  int h;
  int min;
  double s;

  pT->LocalTime(y,m,d,h,min,s);
  *pLog << sep << std::endl;
  *pLog << "Today (Local time):  " << dt.DateStr(y, m, d)
    << " " <<dt.TimeStr(h,min,s) << std::endl;
  *pLog << "Day of the year:     " << dt.DoY(y,m,d) << std::endl;
  *pLog << "Week day:            " << dn.GetDayName((int)dt.DoW(y,m,d)) << std::endl;
  *pLog << "Month Name:          " << mn.GetMonthName(m) << std::endl;
  *pLog << "Julian day number:   " << std::fixed << std::setw(9) << std::setprecision(1) << dt.JulianDayNumber(y,m,d) << std::setprecision(0) << std::endl;
  *pLog << "Fraction of the day: " << std::setprecision(6) << dt.ToFoD(h,min,s)  << std::setprecision(0) << std::endl;

  pT->Time(y,m,d,h,min,s);
  *pLog << sep << std::endl;
  *pLog << "Today (UTC):         " << dt.DateStr(y, m, d)
    << " " <<dt.TimeStr(h,min,s) << std::endl;
  *pLog << "Day of the year:     " << dt.DoY(y,m,d) << std::endl;
  *pLog << "Week day:            " << dn.GetDayName((int)dt.DoW(y,m,d)) << std::endl;
  *pLog << "Month Name:          " << mn.GetMonthName(m) << std::endl;
  *pLog << "Julian day number:   " << std::fixed << std::setw(9) << std::setprecision(1) << dt.JulianDayNumber(y,m,d) << std::setprecision(0) << std::endl;
  *pLog << "Fraction of the day: " << std::setprecision(6) << dt.ToFoD(h,min,s) << std::endl;
  *pLog << "TAI - UTC:           " << dt.LeapSecond(dt.JulianDayNumber(y,m,d)) << std::endl;
  *pLog << sep << std::endl;

  *pLog << sep << std::endl;


  double const secsInDay = 86400; // Seconds in a day.

  // GPS rolllover date. 1980-01-06 00:00:19 TAI.
  double const GPS0 = dt.JulianDayNumber(1980,1,6);
  double const GPS0fod = 19 / secsInDay;

  //double weekNumber = 0;
  //*pLog << "Enter the GPS week number:           ";
  //std::cin >> weekNumber;

  //double weekSeconds = 0;
  //*pLog << "Enter the seconds in the GPS week:   ";
  //std::cin >> weekSeconds;

  double weekNumber = 51;
  double weekSeconds = 80000;
  int weekDay = int(weekSeconds / secsInDay);
  double fod = (weekSeconds - weekDay * secsInDay) / secsInDay;

  double GPSjdn = GPS0 + weekNumber * 7 + weekDay;
  double GPSfod = GPS0fod + fod;
  // Convert to UTC
  double TAI_UTC = dt.LeapSecond(GPSjdn,GPSfod);
  GPSfod -= TAI_UTC/secsInDay;
  dt.NormalizeJDFOD(GPSjdn,GPSfod);


  std::string dts = dt.DateTimeStr(GPSjdn,GPSfod);
  *pLog << "UTC:                                 " << dts  << std::endl;
  *pLog << sep << std::endl;

  //*pLog << "Enter a date yyyy-mm-dd hh:mm:ss.s\nUTC:                                 ";
  //std::cin >> std::ws;
  //std::getline(std::cin,dts);

  dts = "2008-07-15 11:08:12.345";

  dt.ToDateTime(dts,GPSjdn,GPSfod);
  // Convert to TAI
  TAI_UTC = dt.LeapSecond(GPSjdn,GPSfod);
  GPSfod += TAI_UTC/secsInDay;
  dt.NormalizeJDFOD(GPSjdn,GPSfod);
  // Subtract the reference date.
  GPSfod -= GPS0fod;
  GPSjdn -= GPS0;
  // Normalise.
  while (GPSfod < 0)
  {
    GPSfod += 1;
    GPSjdn -= 1;
  }
  while (GPSfod > 1)
  {
    GPSfod -= 1;
    GPSjdn += 1;
  }
  
  weekNumber = int(GPSjdn / 7);
  weekSeconds = (GPSfod + (GPSjdn - weekNumber * 7) ) * 86400;

  *pLog << "GPS Week: " << weekNumber << " " << " GPS seconds: " << weekSeconds
    << " GPS day: " << int(weekSeconds / secsInDay)
    << std::endl;
  *pLog << sep << std::endl;

  pClock->ISOLocalTime(time);
  *pLog << ((fail)?"Failed.":"Passed") << "\n"
      << "Finished: " << time << "\n" << sep << std::endl;

  delete pLog;

  if ( fail )
    return 1;

  return 0;
}
