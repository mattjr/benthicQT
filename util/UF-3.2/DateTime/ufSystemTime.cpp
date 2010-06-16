#include "ufSystemTime.h"
#ifdef _MSC_VER
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif

#ifdef __QNXNTO__
#include <time.h>
#else
#include <ctime>
#endif

//#include <iostream>

#include <cstdlib>

using namespace UF::Time;

SystemTime* SystemTime::pInstance_ = 0;
bool SystemTime::destroyed_ = false;

void SystemTime::CalculateOffset(  double const & jdn, double const & fod )
{

  double sec = 0;

#ifdef _MSC_VER
  struct __timeb64 timebuffer;
  struct tm *sysUTC = new tm;

#if _MSC_VER < 1400 // Visual Studio 7.1 and earlier.
  _ftime64( &timebuffer );
  sysUTC = _gmtime64( &timebuffer.time );
#else
  _ftime64_s( &timebuffer );
  _gmtime64_s( sysUTC, &timebuffer.time );
#endif
  sec = sysUTC->tm_sec + double(timebuffer.millitm) / 1000.0;
  double sysDate = JulianDayNumber(sysUTC->tm_year+1900,(char)sysUTC->tm_mon+1,(char)sysUTC->tm_mday);
  double sysFoD = ToFoD(sysUTC->tm_hour,(char)sysUTC->tm_min,sec);

  delete sysUTC;
#else
  struct timeval ltime;
  gettimeofday(&ltime,NULL);
  // Obtain UTC from the computer.
  struct tm sysUTC;
  gmtime_r( &ltime.tv_sec, &sysUTC );
  sec = sysUTC.tm_sec+ltime.tv_usec/1000000.0;
  double sysDate = JulianDayNumber(sysUTC.tm_year+1900,(char)sysUTC.tm_mon+1,(char)sysUTC.tm_mday);
  double sysFoD = ToFoD(sysUTC.tm_hour,(char)sysUTC.tm_min,sec);
#endif

  this->deltaD = jdn - sysDate;
  this->deltaT = fod - sysFoD;
  this->validOffset = true;

}

void SystemTime::CalculateOffset(
    int const & year, int const & month, int const & day,
    int const & hour, int const & minute, double const & second
  )
{

  double correctDate = JulianDayNumber(year,month,day);
  double correctFoD = ((hour *60.0 + minute) * 60.0 + second)/86400.0; // fraction of the day

  CalculateOffset( correctDate, correctFoD );

}

bool SystemTime::Time ( double & jdn,  double & fod ) const
{
  double sec = 0;

#ifdef _MSC_VER
  struct __timeb64 timebuffer;
  struct tm *sysUTC = new tm;

#if _MSC_VER < 1400 // Visual Studio 7.1 and earlier.
  _ftime64( &timebuffer );
  sysUTC = _gmtime64( &timebuffer.time );
#else
  _ftime64_s( &timebuffer );
  _gmtime64_s( sysUTC, &timebuffer.time );
#endif
  sec = sysUTC->tm_sec + double(timebuffer.millitm) / 1000.0;
  double sysDate = JulianDayNumber(sysUTC->tm_year+1900,(char)sysUTC->tm_mon+1,(char)sysUTC->tm_mday);
  double sysFoD = ToFoD(sysUTC->tm_hour,(char)sysUTC->tm_min,sec);
  delete sysUTC;
#else
  struct timeval ltime;
  gettimeofday(&ltime,NULL);
  // Obtain UTC from the computer.
  struct tm sysUTC;
  gmtime_r( &ltime.tv_sec, &sysUTC );
  sec = sysUTC.tm_sec+ltime.tv_usec/1000000.0;
  double sysDate = JulianDayNumber(sysUTC.tm_year+1900,(char)sysUTC.tm_mon+1,(char)sysUTC.tm_mday);
  double sysFoD = ToFoD(sysUTC.tm_hour,(char)sysUTC.tm_min,sec);
#endif

  if ( validOffset )
  {
    jdn = sysDate + deltaD;
    fod = sysFoD + deltaT;
  }
  else
  {
    jdn = sysDate;
    fod = sysFoD;
  }
  NormalizeJDFOD(jdn,fod);

  if ( validOffset )
    return true;
  else
    return false;
}

bool SystemTime::Time (
  int & year, int & month, int & day,
  int & hour, int & minute, double & second
) const
{
  double correctDate;
  double correctFoD;

  Time(correctDate,correctFoD);

  int hr;
  FromFoD(correctFoD,hr,minute,second);
  if ( hr > 23 )
  {
    while ( hr > 0 )
    {
      ++correctDate;
      hr -= 24;
    }
  }
  if ( hr < -23 )
  {
    while ( hr < 0 )
    {
      --correctDate;
      hr += 24;
    }
  }
  hour = (unsigned char)hr;
  int y;
  int m;
  int d;
  CalendarDate(correctDate,y,m,d);
  year = y;
  month = m;
  day = d;

  if ( validOffset )
    return true;
  else
    return false;
}

bool SystemTime::Time ( std::string & time,  int const & precision,
                        char const & datesep, char const & timesep ) const
{
  int year;
  int month;
  int day;
  int hour;
  int minute;
  double second;
  bool status = this->Time(year, month, day, hour, minute, second);
  std::string tick = DateStr (year, month, day, datesep );
  std::string tock = TimeStr ( hour, minute, second, precision, timesep );
  time = tick + " " + tock;
  return status;
}

unsigned int SystemTime::Time () const
{
  double correctDate;
  double correctFoD;

  Time(correctDate,correctFoD);

  correctDate = correctDate - Jd2000;

  if ( correctDate < 0 || correctDate > DayRange )
    return 0xffffffff;
  else
  {
    unsigned int period = (unsigned int)(( correctDate + correctFoD) * SecsInDay);
    if ( validOffset )
      return period;
    else
      return period + 0xE0000000;
  }
}

int SystemTime::GetUTCOffset () const
{
  int offset = 0;
#if _MSC_VER >= 1400 // Visual Studio 8.0 or greater.
  // localtime corrects for the local time zone if the user first sets the 
  // global environment variable TZ. When TZ is set, three other environment
  // variables (_timezone, _daylight, and _tzname) are automatically set as 
  // well. If the TZ variable is not set, localtime attempts to use the time 
  // zone information specified in the Date/Time application in Control Panel. 
  // If this information cannot be obtained, PST8PDT, which signifies the 
  // Pacific Time Zone, is used by default. See _tzset for a description of 
  // these variables. TZ is a Microsoft extension and not part of the 
  // ANSI standard definition of localtime.

  // Use the control panel.
  struct tm newtime;
  __time64_t long_time;
  _time64( &long_time );// Get time as 64-bit integer.
  // Convert to local time.
  /* int err = */ _localtime64_s( &newtime, &long_time );
  // Now get the TZ information using _tzset()
  _tzset();

  // In XP _daylight seems to be always true. In Vista this is not the case.
  // So we will use newtime.tm_isdst to determine if DST is in effect. 
  //std::cout << "Difference between UTC and LST: " << _timezone << "seconds." << std::endl;
  //std::cout << "Is DST in effect? " << std::boolalpha << bool(_daylight != 0) << std::endl;
  //std::cout << "The DST Bias: " << _dstbias << std::endl;
  //std::cout << "Local standard timezone code is: " << _tzname[0] << std::endl; 
  //std::cout << "Local alternnate timezone code is: " << _tzname[1] << std::endl; 
  if ( newtime.tm_isdst > 0 )
  {
    offset = -(_timezone + _dstbias);
  }
  else
  {
    offset = -_timezone;
  }
#else
  tzset();
  timeval now;
  gettimeofday(&now,NULL);
  struct tm localTime;
  localtime_r(&now.tv_sec,&localTime);
  offset = localTime.tm_gmtoff;
#endif
  return offset;
}

void SystemTime::GetUTCOffset (int & hours, int & minutes) const
{
  int offset = GetUTCOffset();
  // Calculate the offset from UTC as hours and minutes.
  hours = offset / 3600;
  minutes = offset / 60 - hours * 60;
  // If hours are 0 then -30 minutes is a valid timezone.
  if ( hours < 0 )
    minutes = std::abs(minutes);
}

bool SystemTime::LocalTime ( std::string & time,
                 int const & precision,
                 char const & datesep, char const & timesep,
                 const bool & Use_UTC_offset,
                 const int & UTC_offset) const
{
  int year;
  int month;
  int day;
  int hour;
  int minute;
  double second;

  bool status = this->LocalTime(year, month, day, hour, minute, second, Use_UTC_offset, UTC_offset);
  std::string tick = DateStr (year, month, day, datesep );
  std::string tock = TimeStr ( hour, minute, second, precision, timesep );
  time = tick + " " + tock;
  return status;
}

bool SystemTime::ISOLocalTime ( std::string & time,
                        const bool & Use_UTC_offset,
                        int const & UTC_Offset,
                        int const & precision,
                        char const & dateSep,
                        char const & timeSep,
                        char const & dateTimeSep,
                        char const & UTCMarker ) const
{
  int year;
  int month;
  int day;
  int hour;
  int minute;
  double second;

  bool status = this->LocalTime(year, month, day, hour, minute, second, Use_UTC_offset, UTC_Offset);

  int UTCOffset = 0;
  if (Use_UTC_offset)
  {
    UTCOffset = UTC_Offset;
  }
  else
  {
    UTCOffset = GetUTCOffset();
  }

  time = this->ISODateTimeStr(year, month, day, hour, minute, second, UTCOffset, precision, dateSep, timeSep, dateTimeSep, UTCMarker);
  return status;
}

 bool SystemTime::LocalTime ( double & jdn,  double & fod,
                              const bool & Use_UTC_offset,
                              const int & UTC_offset) const
{
  int UTCOffset = 0;
  if (Use_UTC_offset)
  {
    UTCOffset = UTC_offset;
  }
  else
  {
    UTCOffset = GetUTCOffset();
  }

  // Get the system time.
  bool status = Time(jdn,fod);
  // Now add the offset to it.
  fod += (double)UTCOffset / 86400.0;
  NormalizeJDFOD(jdn,fod);

  return status;

}

bool SystemTime::LocalTime (
  int & year, int & month, int & day,
  int & hour, int & minute, double & second,
  const bool & Use_UTC_offset,
  const int & UTC_offset) const
{
  double correctDate;
  double correctFoD;

  bool status = LocalTime(correctDate,correctFoD,Use_UTC_offset,UTC_offset);

  int hr;
  FromFoD(correctFoD,hr,minute,second);
  if ( hr > 23 )
  {
    while ( hr > 0 )
    {
      ++correctDate;
      hr -= 24;
    }
  }
  if ( hr < -23 )
  {
    while ( hr < 0 )
    {
      --correctDate;
      hr += 24;
    }
  }
  hour = (unsigned char)hr;
  int y;
  int m;
  int d;
  CalendarDate(correctDate,y,m,d);
  year = y;
  month = m;
  day = d;

  return status;
}

unsigned int SystemTime::LocalTime (const bool & Use_UTC_offset,
                                    const int & UTC_offset) const
{
  double correctDate;
  double correctFoD;

  LocalTime(correctDate,correctFoD,Use_UTC_offset,UTC_offset);

  correctDate = correctDate - Jd2000;

  if ( correctDate < 0 || correctDate > DayRange )
    return 0xffffffff;
  else
  {
    unsigned int period = (unsigned int)(( correctDate + correctFoD) * SecsInDay);
    if ( validOffset )
      return period;
    else
      return period + 0xE0000000;
  }
}
