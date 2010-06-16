#include "ufDateTime.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>

//#ifdef __QNXNTO__
//#else
//#endif

using namespace UF::Time;

DateTime::DateTime()
{
}

DateTime::~DateTime()
{
}

bool DateTime::IsGregorianDate (int const &Y, int const & M, int const &D) const
{
  if ( Y < JGChangeover::JulianY )
    return false;
  else
    if (( Y == JGChangeover::JulianY ) &&
      ( M < JGChangeover::JulianM))
      return false;
    else
      if (( Y == JGChangeover::JulianY ) &&
        ( M == JGChangeover::JulianM ) &&
        ( D <= JGChangeover::JulianD ))
        return false;
        else
          return true;
}

bool DateTime::IsLeapYear(int const& y) const
{
  if ( y < JGChangeover::JulianY ) // Julian calendar.
    return ( ( y % 4) == 0 );
  else //Gregorian calendar.
    if ( ( y % 100 ) == 0 )
      return ( ( y % 400 ) == 0 ); // Gregorian century leap year.
    else return ( ( y % 4 ) == 0 );
}

bool DateTime::IsValidYMD (int const & y, int const & m, int const & d) const
{
  if (( y < MinYMD::year ) || ( y > MaxYMD::year) ||
    (( y == MaxYMD::year ) && ( m > MaxYMD::month)) ||
    (( y == MaxYMD::year ) && ( m == MaxYMD::month) && ( d > MaxYMD::day )) )
    // The conversion to year month day from julian date will not work.
    return( false );
    else
  {  // Check for nonexistent dates corresponding to the
    // Julian/Gregorian changeover.
    if ( !(
      ( y == JGChangeover::JulianY ) &&
      ( m == JGChangeover::JulianM ) &&
      ( d > JGChangeover::JulianD ) &&
      ( d < JGChangeover::GregorianD )))
    {
  //    !((y==1582)&&(m==10)&&(d>=5)&&(d<15))) {
      switch (m) {
      case  1:
      case  3:
      case  5:
      case  7:
      case  8:
      case 10:
      case 12: return ( ( d > 0 ) && ( d <= 31 ) );
      case  4:
      case  6:
      case  9:
      case 11: return ( (d > 0 ) && ( d <= 30) );
      case 2:
        {
          if ( ( d > 0 ) && ( d <= 29 ) && IsLeapYear( y ) )
            return true;
          else
            if ( ( d > 0 ) && ( d <= 28 ) ) return true;
            else return false;
        }
      default: return false;
      }
    }
    else // Date is nonexistent, ie. between 1582-10-4 and 1582-10-15
    return false;
  }
}

bool DateTime::IsValidJDN ( double const & jdn ) const
{
  if (( jdn >= MinYMD::jdn ) && ( jdn <= MaxYMD::jdn ))
    return true;
  else return false;
}

double DateTime::JulianDayNumber(int const & year, int const & month, int const & day) const
{
  int m;
  int y, A, B;

  // Adjust so that the year runs from Mar to Feb.
  if ( month > 2 )
  {
    m = month;
    y = year;
  }
  else
  { // Treat Jan, Feb as the 13th and 14th month of the previous year.
    m = month + 12;
    y = year - 1;
  }
  // Take into account the Gregorian Calendar Reform.
  if (!IsGregorianDate( year, month, day ))
    B = 0;
  else
  {
    A = y / 100;
    B = 2 - A + A / 4;
  }
//  return (( int ) ( 365.25 * y - ( y < 0 ? 0.75 : 0.0) ) +
//      ( int ) ( 30.6001 * ( m + 1 )) + B + 1720994.5 + day );
  return (( int ) ( 365.25 * (y + 4716L)) +
      ( int ) ( 30.6001 * ( m + 1 )) + day + B - 1524.5 );
}


void DateTime::CalendarDate(double const & jdn,  int & y, int & m, int & d) const
{
  int Z, A, alpha, B, C, D, E;
  double F;

  Z = ( int ) ( jdn  +  0.5 );
  F = jdn + 0.5 - Z;
  // Gregorian/Julian changeover. 2299161 = 1582 Oct 15.5
  if( Z < JGChangeover::GregorianJDN ) A  =  Z;
  else {
    alpha = ( int ) ((( double ) Z - 1867216.25 ) / 36524.25 );
    A = Z + 1 + alpha - ( int ) ( alpha / 4 );
    }
  B = A + 1524L;
  C = ( int ) ( ( B - 122.1 ) / 365.25 );
  D = ( int ) ( C * 365.25 );
  E = ( int ) ( ( B - D ) / 30.6001 );
  d = ( char ) ( B - D - ( int ) ( 30.6001 * E ) );
  m = ( char ) ( ( E < 13.5 ) ?  --E : E - 13 );
  y = ( m > 2.5 ) ? C - 4716 : C - 4715;
}

void DateTime::NormalizeJDFOD ( double &jdn, double &fod ) const
{
  double jd_int, jd_frc;
  double ip, fp; // Integer and fractional parts of fod.

  // Set jd to midnight of the day, add the balance to fod,
  // if fod>1 readjust jdn and subtract 1 from fod,
  // if fod<0 readjust jdn and add 1 to the fod.
  // jd_frc=modf((jdn>=0)?jdn+0.5:jdn-0.5,&jd_int);
  fp = std::modf ( fod, &ip );
  jd_frc = std::modf ( ip + jdn + 0.5, &jd_int );

  // jd_frc=(jd_frc<0)?jd_frc+1:jd_frc;
  jdn = jd_int - 0.5; // 12:00 on the day
  fod = fp;
  fod += jd_frc;
  if ( fod >= 0 ) {
    while ( fod >= 1 )
    { // Next Julian day number.
      ++jdn;
      --fod;
    }
  }
  else {
    while ( fod < 0 )
    { // Previous Julian day number.
      --jdn;
      ++fod;
    }
  }
}

void DateTime::ToHMS( double const & hr, int & hour, int & minute, double & second, int secRounding ) const
{
  bool sgn = hr < 0;
  double dv;
  if ( sgn )
    dv = -hr;
  else
    dv = hr;
  hour = (int)( dv );
  dv = ( dv - hour ) * 60;
  minute = (unsigned char) dv;
  double s =( dv - minute ) * 60;
  // Do a little rounding manipulation.
  if ( secRounding >= 0 && secRounding < 15 )
  {
    double rounding = 1;
    while ( secRounding > 0 )
    {
      rounding *= 10.0;
      --secRounding;
    }
    s+= 5.0 / (10*rounding);
    s = s * rounding + 0.5;
    double y;
    y = std::modf(s,&second);
    second /= rounding;
  }
  else
    second = s;
  if ( second >= 60 )
  {
    ++minute;
    second -= 60;
  }
  if ( minute >= 60)
  {
    ++hour;
    minute -= 60;
  }
  if ( sgn ) hour = -hour;
}

std::string DateTime::DateStr ( int const & year, int const & month, int const & day, char const & sep ) const
{
  char buffer[20];
  buffer[0] = 0;
  const int bufferSize = sizeof(buffer)/sizeof(buffer[0]) - 1;
  int res = snprintf(buffer,bufferSize,"%4d%c%02d%c%02d",year,sep,month,sep,day);
  if ( res < bufferSize )
    return buffer;
  else
    return " Bad date ";
  //std::ostringstream os;

  //os << std::fixed << std::setw(4) << year << sep
  //  << std::fixed << std::setw(2) << std::setfill('0') << month << sep
  //  << std::fixed << std::setw(2) << std::setfill('0') << day;
  //return os.str();
}

std::string DateTime::TimeStr ( int const & hour, int const & minute, double const & second, int const & precision, char const & sep ) const
{
  char buffer[40];
  buffer[0] = 0;
  const int bufferSize = sizeof(buffer)/sizeof(buffer[0]) - 1;
  int res;
  if ( precision > 0 )
    res = snprintf(buffer,bufferSize,"%02d%c%02d%c%0*.*f",hour,sep,minute,sep,precision+3,precision,second);
  else
    res = snprintf(buffer,bufferSize,"%02d%c%02d%c%02.*f",hour,sep,minute,sep,precision,second);
  if ( res < bufferSize )
    return buffer;
  else
    return " Bad time ";
  //std::ostringstream os;

  //if ( precision > 0 )
  //  os << std::fixed << std::setw(2) << std::setfill('0') << hour << sep
  //    << std::fixed << std::setw(2) << std::setfill('0') << minute << sep
  //    << std::fixed << std::setw(precision+3) << std::setfill('0') << std::setprecision(precision) << second;
  //else
  //  os << std::fixed << std::setw(2) << std::setfill('0') << hour << sep
  //    << std::fixed << std::setw(2) << std::setfill('0') << minute << sep
  //    << std::fixed << std::setw(2) << std::setfill('0') << std::setprecision(precision) << second;

  //return os.str();
}

std::string DateTime::DateTimeStr ( double const & jdn, double const & fod,
                                    int const & precision,
                                    char const & datesep, char const & timesep) const
{
  int y;
  int m;
  int d;
  int h;
  int n;
  double s;

  double j = jdn;
  double f = fod;
  this->NormalizeJDFOD(j,f);
  this->CalendarDate(j,y,m,d);
  this->FromFoD(f,h,n,s);

  char buffer[100];
  buffer[0] = 0;
  const int bufferSize = sizeof(buffer)/sizeof(buffer[0]) - 1;
  int res = snprintf(buffer,bufferSize,"%s %s",this->DateStr(y,m,d,datesep).c_str(),this->TimeStr(h,n,s,precision,timesep).c_str());
  if ( res < bufferSize )
    return buffer;
  else
    return " Bad date time ";

  //std::ostringstream os;
  //os << this->DateStr(y,m,d,datesep) << " " << this->TimeStr(h,n,s,precision,timesep);

  //return os.str();
}


std::string DateTime::DateTimeStr ( int const & year, int const & month, int const & day,
                                    int const & hour, int const & minute, double const & second,int const & precision,
                                    char const & datesep, char const & timesep ) const
{
  char buffer[100];
  const int bufferSize = sizeof(buffer)/sizeof(buffer[0]) - 1;
  int res = snprintf(buffer,bufferSize,"%s %s",this->DateStr(year,month,day,datesep).c_str(),this->TimeStr(hour,minute,second,precision,timesep).c_str());
  if ( res < bufferSize )
    return buffer;
  else
    return " Bad date time ";

  //std::ostringstream os;
  //os << this->DateStr(year,month,day,datesep) << " "
  //  << this->TimeStr(hour,minute,second,precision,timesep);

  //return os.str();

}

std::string DateTime::ISODateTimeStr( int const & year, int const & month, int const & day,
                                      int const & hour, int const & minute, double const & second,
                                      int const & UTCOffset,
                                      int const & precision,
                                      char const & dateSep,
                                      char const & timeSep,
                                      char const & dateTimeSep,
                                      char const & UTCMarker) const
{
  // Calculate the offset from UTC as hours and minutes.
  int tz_h = UTCOffset / 3600;
  int tz_min = std::abs(UTCOffset / 60 - tz_h * 60);

  char buffer[200];
  const int bufferSize = sizeof(buffer)/sizeof(buffer[0]) - 1;
  int res = 0;

  std::string dstr = this->DateStr(year,month,day,dateSep);
  std::string tstr = TimeStr(hour,minute,second,precision,timeSep);

  if ( tz_h != 0 || tz_min != 0 )
  {
    res = snprintf(buffer,bufferSize,"%s%c%s%+03d:%02d",
                    dstr.c_str(),
                    dateTimeSep,
                    tstr.c_str(),
                    tz_h, tz_min
                    );
  }
  else
  {
    if ( UTCMarker != 0 )
    {
      if ( UTCMarker == '0' )
      {
        // +00:00 is used.
        res = snprintf(buffer,bufferSize,"%s%c%s+00:00",
                    dstr.c_str(),
                    dateTimeSep,
                    tstr.c_str()
                    );
      }
      else
      {
        // UTCMarker is used, generally a 'Z';
        res = snprintf(buffer,bufferSize,"%s%c%s%c",
                    dstr.c_str(),
                    dateTimeSep,
                    tstr.c_str(),
                    UTCMarker
                    );
      }
    }
    else
    {
      // No UTC marker is used.
      res = snprintf(buffer,bufferSize,"%s%c%s",
                  dstr.c_str(),
                  dateTimeSep,
                  tstr.c_str()
                  );
    }
  }

  if ( res < bufferSize )
  {
    buffer[res] = '\0';
    return buffer;
  }
  else
    return " Bad ISO date time ";
  //return os.str();

}

std::string DateTime::ISODateTimeStr( double const & jdn, double const & fod,
                                      int const & UTCOffset,
                                      int const & precision,
                                      char const & dateSep,
                                      char const & timeSep,
                                      char const & dateTimeSep,
                                      char const & UTCMarker) const
{
  double j = jdn;
  double f = fod;
  this->NormalizeJDFOD(j,f);

  int year;
  int month; 
  int day;
  int hour;
  int minute;
  double second;

  this->CalendarDate(j,year,month,day);
  this->FromFoD(f,hour,minute,second,precision);
  return this->ISODateTimeStr(year,month,day,hour,minute,second,UTCOffset,precision,dateSep,timeSep,dateTimeSep,UTCMarker);

}

bool DateTime::Date ( std::string const & date, int & year, int & month, int & day ) const
{
  year = -9999;
  month = -99;
  day = -99;

  if ( date.empty() )
  {
    year = -4712;
    month = 1;
    day = 1;
    return false;
  }

  std::istringstream iss(this->Trim(date));
  char c;
  iss >> year >> c >> month >> c >> day;
  if ( year == -9999 || month == -99 || day == -99 )
  {
    year = -4712;
    month = 1;
    day = 1;
    return false;
  }

  return true;
}

double DateTime::JulianDayNumber ( std::string const & date ) const
{
  int year = 0;
  int month = 0;
  int day = 0;
  this->Date(date,year,month,day);

  return this->JulianDayNumber(year,month,day);
}

bool DateTime::Time ( std::string const & time, int & hour, int & minute, double & second ) const
{
  hour = minute = 99; second = -99;

  if ( time.empty() )
  {
    hour = minute = 0; second = 0;
    return false;
  }

  char c;
  std::istringstream iss(this->Trim(time));
  bool sgn = false;
  c = (char)iss.peek();
  if ( c == '-' || c == '+' )
  {
    if ( c == '-' )
    {
      sgn = true;
    }
    iss >> c;
  }
  iss >> hour >> c;
  c = (char)iss.peek();
  if ( c == '-' || c == '+' )
  {
    if ( c == '-' )
    {
      sgn = true;
    }
    iss >> c;
  }
  iss >> minute >> c;
  c = (char)iss.peek();
  if ( c == '-' || c == '+' )
  {
    if ( c == '-' )
    {
      sgn = true;
    }
    iss >> c;
  }
  iss >> second;
  if ( hour == 99 || minute == 99 || second == -99 )
  {
    hour = minute = 0; second = 0;
    return false;
  }
  if (sgn )
  {
    hour = -hour;
    minute = -minute;
    second = -second;
  }

  return true;
}


double DateTime::FoD ( std::string const & time ) const
{
  int hour = 0;
  int minute = 0;
  double second  = 0;
  this->Time(time,hour,minute,second);

  return this->ToFoD(hour,minute,second);
}

bool DateTime::ISODateTime( std::string const & date_time, int & year, int & month, int & day, int & hour, int & minute, double & second, int & UTCOffset ) const
{

  std::string date;
  std::string time;
  std::string offset;

  this->SplitDateTime(date_time,date,time,offset);

  int offset_hour = 0;
  int offset_min = 0;
  char c;
  if ( !offset.empty() )
  {
    std::istringstream iss(offset);
    iss >> offset_hour >> c >> offset_min;
  }
  bool dateOk = this->Date(date,year,month,day);;
  bool timeOk = this->Time(time,hour,minute,second);;
  UTCOffset = ( offset_hour*60 + ((offset_hour>= 0)?offset_min:-offset_min) ) * 60;
  return dateOk & timeOk;
}

bool DateTime::ISODateTime( std::string const & date_time, double & jdn, double & fod, int & UTCOffset ) const
{
  // ISO Dates are in the form:
  // 2004-02-23T14:46:22.356+11:00, or the equivalent instant
  // in UTC as 2004-02-23T14:46:22.356Z.

  int year = 0;
  int month = 0;
  int day = 0;
  int hour = 0;
  int minute = 0;
  double second = 0;

  bool Ok = this->ISODateTime(date_time,year,month,day,hour,minute,second,UTCOffset);

  jdn = this->JulianDayNumber(year,month,day);
  fod = this->ToFoD(hour,minute,second);

  return Ok;
}

void DateTime::ToDateTime ( std::string const & date_time,
                            int & year, int & month, int & day,
                            int & hour, int & minute, double & second) const
{
  std::string date;
  std::string time;
  std::string offset;

  this->SplitDateTime(date_time,date,time,offset);

  this->Date(date,year,month,day);
  this->Time(time,hour,minute,second);
}

void DateTime::ToDateTime ( std::string const & date_time, double & jdn, double & fod ) const
{
  int year = 0;
  int month = 0;
  int day = 0;
  int hour = 0;
  int minute = 0;
  double second = 0;

  this->ToDateTime(date_time,year,month,day,hour,minute,second);

  jdn = this->JulianDayNumber(year,month,day);
  fod = this->ToFoD(hour,minute,second);
}

double DateTime::LeapSecond ( double const & jdn, double const & fod  ) const
{
  // The Modified Julian Day (MJD) is the number of days (with decimal fraction of the day)
  // that have elapsed since midnight at the beginning of Wednesday November 17, 1858.
  // In terms of the Julian day:
  double MJD = jdn + fod - 2400000.5;

  // A table of leap seconds.
  // See: http://maia.usno.navy.mil/ser7/tai-utc.dat
  /*
  1961 JAN  1 =JD 2437300.5  TAI-UTC=   1.4228180 S + (MJD - 37300.) X 0.001296 S
  1961 AUG  1 =JD 2437512.5  TAI-UTC=   1.3728180 S + (MJD - 37300.) X 0.001296 S
  1962 JAN  1 =JD 2437665.5  TAI-UTC=   1.8458580 S + (MJD - 37665.) X 0.0011232S
  1963 NOV  1 =JD 2438334.5  TAI-UTC=   1.9458580 S + (MJD - 37665.) X 0.0011232S
  1964 JAN  1 =JD 2438395.5  TAI-UTC=   3.2401300 S + (MJD - 38761.) X 0.001296 S
  1964 APR  1 =JD 2438486.5  TAI-UTC=   3.3401300 S + (MJD - 38761.) X 0.001296 S
  1964 SEP  1 =JD 2438639.5  TAI-UTC=   3.4401300 S + (MJD - 38761.) X 0.001296 S
  1965 JAN  1 =JD 2438761.5  TAI-UTC=   3.5401300 S + (MJD - 38761.) X 0.001296 S
  1965 MAR  1 =JD 2438820.5  TAI-UTC=   3.6401300 S + (MJD - 38761.) X 0.001296 S
  1965 JUL  1 =JD 2438942.5  TAI-UTC=   3.7401300 S + (MJD - 38761.) X 0.001296 S
  1965 SEP  1 =JD 2439004.5  TAI-UTC=   3.8401300 S + (MJD - 38761.) X 0.001296 S
  1966 JAN  1 =JD 2439126.5  TAI-UTC=   4.3131700 S + (MJD - 39126.) X 0.002592 S
  1968 FEB  1 =JD 2439887.5  TAI-UTC=   4.2131700 S + (MJD - 39126.) X 0.002592 S
  1972 JAN  1 =JD 2441317.5  TAI-UTC=  10.0       S + (MJD - 41317.) X 0.0      S
  1972 JUL  1 =JD 2441499.5  TAI-UTC=  11.0       S + (MJD - 41317.) X 0.0      S
  1973 JAN  1 =JD 2441683.5  TAI-UTC=  12.0       S + (MJD - 41317.) X 0.0      S
  1974 JAN  1 =JD 2442048.5  TAI-UTC=  13.0       S + (MJD - 41317.) X 0.0      S
  1975 JAN  1 =JD 2442413.5  TAI-UTC=  14.0       S + (MJD - 41317.) X 0.0      S
  1976 JAN  1 =JD 2442778.5  TAI-UTC=  15.0       S + (MJD - 41317.) X 0.0      S
  1977 JAN  1 =JD 2443144.5  TAI-UTC=  16.0       S + (MJD - 41317.) X 0.0      S
  1978 JAN  1 =JD 2443509.5  TAI-UTC=  17.0       S + (MJD - 41317.) X 0.0      S
  1979 JAN  1 =JD 2443874.5  TAI-UTC=  18.0       S + (MJD - 41317.) X 0.0      S
  1980 JAN  1 =JD 2444239.5  TAI-UTC=  19.0       S + (MJD - 41317.) X 0.0      S
  1981 JUL  1 =JD 2444786.5  TAI-UTC=  20.0       S + (MJD - 41317.) X 0.0      S
  1982 JUL  1 =JD 2445151.5  TAI-UTC=  21.0       S + (MJD - 41317.) X 0.0      S
  1983 JUL  1 =JD 2445516.5  TAI-UTC=  22.0       S + (MJD - 41317.) X 0.0      S
  1985 JUL  1 =JD 2446247.5  TAI-UTC=  23.0       S + (MJD - 41317.) X 0.0      S
  1988 JAN  1 =JD 2447161.5  TAI-UTC=  24.0       S + (MJD - 41317.) X 0.0      S
  1990 JAN  1 =JD 2447892.5  TAI-UTC=  25.0       S + (MJD - 41317.) X 0.0      S
  1991 JAN  1 =JD 2448257.5  TAI-UTC=  26.0       S + (MJD - 41317.) X 0.0      S
  1992 JUL  1 =JD 2448804.5  TAI-UTC=  27.0       S + (MJD - 41317.) X 0.0      S
  1993 JUL  1 =JD 2449169.5  TAI-UTC=  28.0       S + (MJD - 41317.) X 0.0      S
  1994 JUL  1 =JD 2449534.5  TAI-UTC=  29.0       S + (MJD - 41317.) X 0.0      S
  1996 JAN  1 =JD 2450083.5  TAI-UTC=  30.0       S + (MJD - 41317.) X 0.0      S
  1997 JUL  1 =JD 2450630.5  TAI-UTC=  31.0       S + (MJD - 41317.) X 0.0      S
  1999 JAN  1 =JD 2451179.5  TAI-UTC=  32.0       S + (MJD - 41317.) X 0.0      S
  2006 JAN  1 =JD 2453736.5  TAI-UTC=  33.0       S + (MJD - 41317.) X 0.0      S
  2009 JAN  1 =JD 2454832.5  TAI-UTC=  34.0       S + (MJD - 41317.) X 0.0      S
  */
  static const double factors[][4] = {
  {2437300.5,  1.4228180, -37300.0, 0.001296},
  {2437512.5,  1.3728180, -37300.0, 0.001296},
  {2437665.5,  1.8458580, -37665.0, 0.0011232},
  {2438334.5,  1.9458580, -37665.0, 0.0011232},
  {2438395.5,  3.2401300, -38761.0, 0.001296},
  {2438486.5,  3.3401300, -38761.0, 0.001296},
  {2438639.5,  3.4401300, -38761.0, 0.001296},
  {2438761.5,  3.5401300, -38761.0, 0.001296},
  {2438820.5,  3.6401300, -38761.0, 0.001296},
  {2438942.5,  3.7401300, -38761.0, 0.001296},
  {2439004.5,  3.8401300, -38761.0, 0.001296},
  {2439126.5,  4.3131700, -39126.0, 0.002592},
  {2439887.5,  4.2131700, -39126.0, 0.002592},
  {2441317.5, 10.0      , -41317.0, 0.0},
  {2441499.5, 11.0      , -41317.0, 0.0},
  {2441683.5, 12.0      , -41317.0, 0.0},
  {2442048.5, 13.0      , -41317.0, 0.0},
  {2442413.5, 14.0      , -41317.0, 0.0},
  {2442778.5, 15.0      , -41317.0, 0.0},
  {2443144.5, 16.0      , -41317.0, 0.0},
  {2443509.5, 17.0      , -41317.0, 0.0},
  {2443874.5, 18.0      , -41317.0, 0.0},
  {2444239.5, 19.0      , -41317.0, 0.0},
  {2444786.5, 20.0      , -41317.0, 0.0},
  {2445151.5, 21.0      , -41317.0, 0.0},
  {2445516.5, 22.0      , -41317.0, 0.0},
  {2446247.5, 23.0      , -41317.0, 0.0},
  {2447161.5, 24.0      , -41317.0, 0.0},
  {2447892.5, 25.0      , -41317.0, 0.0},
  {2448257.5, 26.0      , -41317.0, 0.0},
  {2448804.5, 27.0      , -41317.0, 0.0},
  {2449169.5, 28.0      , -41317.0, 0.0},
  {2449534.5, 29.0      , -41317.0, 0.0},
  {2450083.5, 30.0      , -41317.0, 0.0},
  {2450630.5, 31.0      , -41317.0, 0.0},
  {2451179.5, 32.0      , -41317.0, 0.0},
  {2451179.5, 32.0      , -41317.0, 0.0},
  {2453736.5, 33.0      , -41317.0, 0.0},
  {2454832.5, 34.0      , -41317.0, 0.0}
  };

  if ( jdn < factors[0][0] )
    return 0;
  std::size_t rows = sizeof(factors) / (4 * sizeof(double));
  std::size_t last_row = --rows;
  for ( std::size_t i = 0; i < last_row; ++i )
  {
    if ( jdn >= factors[i][0] && jdn < factors[i+1][0] )
      return factors[i][1] + (MJD + factors[i][2]) * factors[i][3];
  }
  return factors[last_row][1] + (MJD + factors[last_row][2]) * factors[last_row][3];

}

void DateTime::SplitDateTime ( std::string const & date_time, std::string & date, std::string & time, std::string & offset) const
{
  std::string dts = this->Trim(date_time);
  std::replace_if(dts.begin(),dts.end(),std::bind2nd(std::equal_to<char>(),'T'),' ');
  std::replace_if(dts.begin(),dts.end(),std::bind2nd(std::equal_to<char>(),'Z'),' ');
  std::replace_if(dts.begin(),dts.end(),std::bind2nd(std::equal_to<char>(),'t'),' ');
  std::replace_if(dts.begin(),dts.end(),std::bind2nd(std::equal_to<char>(),'z'),' ');

  std::size_t pos = dts.find_first_of(' ');
  if ( pos != std::string::npos )
  {
    date = dts.substr(0,pos);
    time = dts.substr(pos+1,date_time.size()-1);
    offset.clear();
    pos = time.rfind("+");
    if ( pos != 0 && pos != std::string::npos )
    {
      offset = time.substr(pos+1,time.size()-1);
      time = time.substr(0,pos);
    }
    else
    {
      pos = time.rfind("-");
      if ( pos != 0 && pos != std::string::npos )
      {
        offset = time.substr(pos,time.size()-1);
        time = time.substr(0,pos);
      }
    }
  }
  else
  {
    date = dts;
    time.clear();
    offset.clear();
  }

}



