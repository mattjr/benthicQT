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

int main ( void )
{
  std::string sep = "------------------------------------------------------------------------";
  // Set up the log file.
  ufLogger *pLog = new ufLogger;
  pLog->open("EgDateTime.log",std::ios::out);
  std::string time;
  Time::SystemTime *pClock = Time::SystemTime::Instance();
  pClock->ISOLocalTime(time);
  *pLog << sep << "\nStarted:  " << time << std::endl;

  bool fail = false;
  *pLog << "Testing the Date-Time Library" << std::endl;

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
  *pLog << sep << std::endl;

#if _MSC_VER >= 1400 // Visual Studio 8.0 or greater.
  _tzset();
  // In XP _daylight seems to be always true. In Vista this is not the case.
  // So we will use newtime.tm_isdst to determine if DST is in effect. 
  *pLog << "Difference between UTC and LST: " << _timezone << "seconds." << std::endl;
  *pLog << "Is DST in effect? " << std::boolalpha << bool(_daylight != 0) << std::endl;
  *pLog << "The DST Bias: " << _dstbias << std::endl;
  *pLog << "Local standard timezone code is: " << _tzname[0] << std::endl; 
  *pLog << "Local alternnate timezone code is: " << _tzname[1] << std::endl; 

#else
  timeval now;
  gettimeofday(&now,NULL);
  struct tm localTime;
  localtime_r(&now.tv_sec,&localTime);
  *pLog << "Time Zone:           " << timezone << "(" << timezone / 3600 << "h) " << std::endl;
  *pLog << "DST:                 " << std::boolalpha << bool(daylight != 0) <<  " DST Bias: " << -localTime.tm_gmtoff - timezone << "(" << (-localTime.tm_gmtoff - timezone) / 3600 << "h)"  << std::endl;
  *pLog << "UTC Offset:          " << localTime.tm_gmtoff  << "(" << localTime.tm_gmtoff / 3600 << "h)"  << std::endl;
  *pLog << "Local standard timezone code is: " << tzname[0] << std::endl; 
  *pLog << "Local alternnate timezone code is: " << tzname[1] << std::endl; 
  //UTCOffset = localTime.tm_gmtoff;
#endif

  *pLog << sep << std::endl;
  // ISO Date tests.
  std::string ISOTimeUTC = dt.ISODateTimeStr(y,m,d,h,min,s);
  *pLog << "ISO Time is: " << ISOTimeUTC << std::endl;
  double ISOjdn = 0;
  double ISOfod = 0;
  int UTCOffset1 = 0;
  dt.ISODateTime(ISOTimeUTC,ISOjdn,ISOfod,UTCOffset1);
  int y1 = 0;
  int m1 = 0;
  int d1 = 0;
  int h1 = 0;
  int min1 = 0;
  double s1 = 0;
  dt.ISODateTime(ISOTimeUTC,y1,m1,d1,h1,min1,s1,UTCOffset1);
  if ( !(y1 == y && m1 == m && d1 == d && h1 == h && min1 == min1 && int(s1 * 1000)/1000.0 == int(s * 1000)/1000.0) )
  {
    *pLog << sep << std::endl;
    *pLog << "Iso time conversion fail:\n Expected: ";
    *pLog << y << "-" << m << "-" << d << "T" << h << ":" << min << ":" << s << std::endl;
    *pLog << "      Got: ";
    *pLog << y1 << "-" << m1 << "-" << d1 << "T" << h1 << ":" << min1 << ":" << s1 << std::endl;
    *pLog << sep << std::endl;
    fail |= false;
  }
  int UTCOffset = 0;
  UTCOffset = pT->GetUTCOffset();
  pT->LocalTime(y,m,d,h,min,s);
  std::string ISOTimeLocal = dt.ISODateTimeStr(y,m,d,h,min,s,UTCOffset);
  *pLog << "or:          " << ISOTimeLocal << std::endl;
  double ISOjdn1 = 0;
  double ISOfod1 = 0;
  dt.ISODateTime(ISOTimeLocal,ISOjdn1,ISOfod1,UTCOffset1);
  dt.ISODateTime(ISOTimeLocal,y1,m1,d1,h1,min1,s1,UTCOffset1);
  if ( !(y1 == y && m1 == m && d1 == d && h1 == h && min1 == min1 && int(s1 * 1000)/1000.0 == int(s * 1000)/1000.0 && UTCOffset1 == UTCOffset) )
  {
    *pLog << sep << std::endl;
    *pLog << "Iso time conversion fail:\n Expected: ";
    *pLog << y << "-" << m << "-" << d << "T" << h << ":" << min << ":" << s << " UTCOffset " << UTCOffset << std::endl;
    *pLog << "      Got: ";
    *pLog << y1 << "-" << m1 << "-" << d1 << "T" << h1 << ":" << min1 << ":" << s1 << " UTCOffset1 " << UTCOffset1 << std::endl;
    fail |= false;
  }
  *pLog << sep << std::endl;


  // Testing date strings and time strings.
  pT->Time(y,m,d,h,min,s);
  std::string dateStr = dt.DateStr(y,m,d);
  std::string timeStr = dt.TimeStr(h,min,s);
  std::string dateTimeStr = dateStr + " " + timeStr;
  *pLog << "The time is (UTC): " << dateTimeStr << std::endl;
  dt.ToDateTime(dateTimeStr,y1,m1,d1,h1,min1,s1);
  if ( !(y1 == y && m1 == m && d1 == d && h1 == h && min1 == min1 && int(s1 * 1000)/1000.0 == int(s * 1000)/1000.0 ) )
  {
    *pLog << sep << std::endl;
    *pLog << "Date time string failed:\n Expected: ";
    *pLog << y << "-" << m << "-" << d << " " << h << ":" << min << ":" << s << std::endl;
    *pLog << "      Got: ";
    *pLog << y1 << "-" << m1 << "-" << d1 << " " << h1 << ":" << min1 << ":" << s1 << std::endl;
    fail |= false;
  }

  *pLog << sep << std::endl;



  *pLog << std::endl << "Adding 5s to a time:" << std::endl;
  y = 2003;
  m = 11;
  d = 18;
  h = 2;
  min = 33;
  s = 59.990;
  // Instead of using a fixed date we would get the date from a known
  // correct source, say a GPS to get the class to calculate an offset.
  pT->CalculateOffset(y, m, d, h, min, s);
  *pLog << "Original time:      " << dt.DateStr(y, m, d)
    << " " << dt.TimeStr(h,min,s) << std::endl;

#ifdef _MSC_VER
  Sleep(5000);
#else
#ifdef __QNXNTO__
  delay(5000);
#else
  sleep(5);
#endif
#endif

  // Whenever we call Time(...), it will return the system time
  // modified by the previously calculated offset.
  *pLog << "OK after 5 seconds." << std::endl;
  pT->Time(y,m,d,h,min,s);
  *pLog << "Five seconds later: " << dt.DateStr(y,m,d)
    << " " << dt.TimeStr(h,min,s) << std::endl;

  int year = 2004;
  if ( dt.IsLeapYear(year) )
    *pLog << year << " is a leap year." << std::endl;
  else
  {
    *pLog << year << " is not a leap year." << std::endl;
    fail |= true;
  }
  year = 1900;
  if ( dt.IsLeapYear(year) )
  {
    *pLog << year << " is a leap year." << std::endl;
    fail |= true;
  }
  else
    *pLog << year << " is not a leap year." << std::endl;

  int month = 13;
  int day = 1;

  if ( dt.IsValidYMD(year,month,day) )
  {
    *pLog << dt.DateStr(year,month,day) << " is a valid date." << std::endl;
    fail |= true;
  }
  else
    *pLog << dt.DateStr(year,month,day) << " is not a valid date." << std::endl;
  year = 2004; month = 2; day = 29;
  if ( dt.IsValidYMD(year,month,day) )
    *pLog << dt.DateStr(year,month,day) << " is a valid date." << std::endl;
  else
  {
    *pLog << dt.DateStr(year,month,day) << " is not a valid date." << std::endl;
    fail |= true;
  }
  year = 1582; month = 10; day = 15;
  if ( dt.IsGregorianDate(year,month,day) )
    *pLog << dt.DateStr(year,month,day) << " is a Gregorian date." << std::endl;
  else
  {
    *pLog << dt.DateStr(year,month,day) << " is not a Gregorian date." << std::endl;
    fail |= true;
  }
  year = 1582; month = 10; day = 4;
  if ( dt.IsGregorianDate(year,month,day) )
  {
    *pLog << dt.DateStr(year,month,day) << " is a Gregorian date." << std::endl;
    fail |= true;
  }
  else
    *pLog << dt.DateStr(year,month,day) << " is not a Gregorian date." << std::endl;
  year = 1582; month = 10; day = 5;
  if ( dt.IsValidYMD(year,month,day) )
  {
    *pLog << dt.DateStr(year,month,day) << " is a valid date." << std::endl;
    fail |= true;
  }
  else
    *pLog << dt.DateStr(year,month,day) << " is not a valid date." << std::endl;
  year = 1582; month = 10; day = 15;
  if ( dt.IsValidYMD(year,month,day) )
    *pLog << dt.DateStr(year,month,day) << " is a valid date." << std::endl;
  else
  {
    *pLog << dt.DateStr(year,month,day) << " is not a valid date." << std::endl;
    fail |= true;
  }
  double jdn = 0;
  if ( dt.IsValidJDN(jdn) )
  {
    dt.CalendarDate(jdn,year,month,day);
    *pLog << dt.DateStr(year,month,day) << " is a valid date." << std::endl;
  }
  else
  {
    *pLog << "Invalid julian date: " << jdn << std::endl;
    fail |= true;
  }
  year = 2003; month = 11; day = 19;
  *pLog << "Week day: " << dt.DoW(year,month,day) << std::endl;
  double jdn1 = dt.JulianDayNumber(year,month,day);
  dt.CalendarDate(jdn1,year,month,day);
  jdn = dt.JulianDayNumber(year,month,day);
  if ( jdn != jdn1 )
  {
    *pLog << "Mismatch!" << std::endl;
    fail |= true;
  }
  else
    *pLog << "Ok!" << std::endl;
  *pLog << "Seeing if the changeover to Gregorian calendar is handled OK." << std::endl;
  year = 1582; month = 10; day = 4;
  *pLog << "Week day: " << dt.DoW(year,month,day) << std::endl;
  *pLog << "Julian day number: " << std::fixed << std::setw(9) << std::setprecision(1) << dt.JulianDayNumber(year,month,day) << std::setprecision(0) << std::endl;
  year = 1582; month = 10; day = 15;
  *pLog << "Week day: " << dt.DoW(year,month,day) << std::endl;
  *pLog << "Julian day number: " << std::fixed << std::setw(9) << std::setprecision(1) << dt.JulianDayNumber(year,month,day) << std::setprecision(0) << std::endl;

  *pLog << "Fraction of day to/from time - note the loss of precision in the seconds." << std::endl;
  double fod = dt.ToFoD(23,(unsigned char)59,59.1234567890123);
  *pLog << "Entered time: " << dt.TimeStr(23,(unsigned char)59,59.1234567890123,13,':') << std::endl;
  int hour;
  int minute;
  double second;
  dt.FromFoD(fod,hour,minute,second,13);
  *pLog << "From fod:     " << dt.TimeStr(hour,minute,second,13,':') << std::endl;
  fod = dt.ToFoD(23,(unsigned char)59,59.99999999999);
  *pLog << "Entered time: " << dt.TimeStr(23,(unsigned char)59,59.99999999999,13,':') << std::endl;
  dt.FromFoD(fod,hour,minute,second,13);
  *pLog << "From fod:     " << dt.TimeStr(hour,minute,second,13,':') << std::endl;
  *pLog << "It looks like 11 decimal digits is the precision for seconds." << std::endl;

  for ( int i = 1 ; i < 12; ++i )
    *pLog << mn.GetMonthName(i) << " ";
  *pLog << std::endl;
  for ( int i = 0 ; i < 7; ++i )
    *pLog << dn.GetDayName(i) << " ";
  *pLog << std::endl;
  for ( int i = 1 ; i < 12; ++i )
    *pLog << mn.GetAbbreviatedMonthName(i) << " ";
  *pLog << std::endl;
  for ( int i = 1 ; i < 12; ++i )
    *pLog << mn.GetAbbreviatedMonthName(i,4) << " ";
  *pLog << std::endl;
  for ( int i = 0 ; i < 7; ++i )
    *pLog << dn.GetAbbreviatedDayName(i,1) << " ";
  *pLog << std::endl;
  // Change a name
  dn.SetDayName(3,"Miercoles");
  for ( int i = 0 ; i < 7; ++i )
    *pLog << dn.GetDayName(i) << " ";
  *pLog << std::endl;
  mn.SetMonthName(1,"Enero");
  for ( int i = 1 ; i < 12; ++i )
    *pLog << mn.GetMonthName(i) << " ";
  *pLog << std::endl;

  year = 2000; month = 1; day = 1;
  *pLog << "Week day: " << dt.DoW(year,month,day) << std::endl;
  *pLog << "Julian day number: " << std::fixed << std::setw(9) << std::setprecision(1) << dt.JulianDayNumber(year,month,day) << std::setprecision(0) << std::endl;
  jdn = -0.5;
  dt.CalendarDate(jdn,year,month,day);
  *pLog << "JDN = " << std::setprecision(1) << jdn << " is: " << dt.DateStr(year,month,day) << std::endl;
  if ( dt.IsValidJDN(jdn) )
    *pLog << "JDN is valid. ";
  else
  {
    *pLog << "JDN is not valid. ";
    fail |= true;
  }
  if ( dt.IsValidYMD(year,month,day) )
    *pLog << "Year, Month and Day are valid.";
  else
  {
    *pLog << "Year, Month and Day are not valid.";
    *pLog << std::endl;
    fail |= true;
  }

  // Just get the current system time.
  pT->ClearOffset();
  pT->Time(y,m,d,h,min,s);
  *pLog << "\nIt is now:            "
        << dt.DateTimeStr(y, m, d, h, min, s)
        << std::endl;

  pClock->ISOLocalTime(time);
  *pLog << ((fail)?"Failed.":"Passed") << "\n"
      << "Finished: " << time << "\n" << sep << std::endl;

  // We should test time conversions here.
  //Time::TimeConversions *tc = Time::TimeConversions::Instance();

  delete pLog;

  if ( fail )
    return 1;

  return 0;
}
