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

int DateTimeMain (int argc, char* argv[])
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
  pLog->open("TestDateTime.log",std::ios::out);
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
  *pLog << "TAI - UTC:           " << dt.LeapSecond(dt.JulianDayNumber(y,m,d)) << std::endl;
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
  int UTCOffset = pT->GetUTCOffset();
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
        << dt.DateTimeStr(y,m,d,h,min,s)
        << std::endl;

  // We should test time conversions here.
  Time::TimeConversions *tc = Time::TimeConversions::Instance();
  unsigned int lhs = tc->DateTimeToUI(y,m,d,h,min,s); // Corrected time
  unsigned int rhs = tc->ToUncorrectedUI(lhs);
  *pLog << "Testing time conversions." << std::endl;
  *pLog << "The corrected time is: " << tc->MakeDateTimeString(lhs) << " as unsigned int: " << lhs
      << std::hex << "(" << lhs << std::dec << ")" << std::endl;
  *pLog << "The uncorrected time is: " << tc->MakeDateTimeString(rhs) << " as unsigned int: " << rhs
      << std::hex << "(" << rhs << std::dec << ")" << std::endl;
  if (!tc->IsCorrectedUITime(lhs))
  {
    *pLog << lhs << " should be a corrected time." << std::endl;
    fail |= true;
  }
  if (tc->IsCorrectedUITime(rhs))
  {
    *pLog << rhs << " should be an uncorrected time." << std::endl;
    fail |= true;
  }
  if ( !tc->EQ(lhs,rhs) )
  {
    *pLog << "Time::TimeConversions::EQ() failed." << std::endl;
    fail |= true;
  }
  if ( tc->NE(lhs,rhs) )
  {
    *pLog << "Time::TimeConversions::NE() failed." << std::endl;
    fail |= true;
  }
  if ( !tc->LE(rhs,lhs) )
  {
    *pLog << "Time::TimeConversions::LE() failed." << std::endl;
    fail |= true;
  }
  if ( !tc->GE(lhs,rhs) )
  {
    *pLog << "Time::TimeConversions::GE() failed." << std::endl;
    fail |= true;
  }
  rhs += 1;
  if ( tc->LE(rhs,lhs) )
  {
    *pLog << "Time::TimeConversions::LE() failed." << std::endl;
    fail |= true;
  }
  if ( tc->GE(lhs,rhs) )
  {
    *pLog << "Time::TimeConversions::GE() failed." << std::endl;
    fail |= true;
  }
  if ( tc->LT(rhs,lhs) )
  {
    *pLog << "Time::TimeConversions::LT() failed." << std::endl;
    fail |= true;
  }
  if ( tc->GT(lhs,rhs) )
  {
    *pLog << "Time::TimeConversions::GT() failed." << std::endl;
    fail |= true;
  }

  tc->UIToDateTime(lhs,year,month,day,hour,minute,second);
  if ( year != y || month != m || day != d || hour != h || minute != min || int((second + 0.005) * 10)/10.0 != int(s) )
  {
    *pLog << "Converting corrected time failed." << std::endl;
    *pLog << "Expected: " << tc->DateTimeStr(y,m,d,h,min,int(s)) << std::endl;
    *pLog << "Got:      " << tc->DateTimeStr(year,month,day,hour,minute,second) << std::endl;
    fail |= true;
  }

  --rhs;
  tc->UIToDateTime(rhs,year,month,day,hour,minute,second);
  if ( year != y || month != m || day != d || hour != h || minute != min || int((second + 0.005) * 10)/10.0 != int(s) )
  {
    *pLog << "Converting uncorrected time failed." << std::endl;
    *pLog << "Expected: " << tc->DateTimeStr(y,m,d,h,min,int(s)) << std::endl;
    *pLog << "Got:      " << tc->DateTimeStr(year,month,day,hour,minute,second) << std::endl;
    fail |= true;
  }


  pClock->ISOLocalTime(time);
  *pLog << ((fail)?"Failed.":"Passed") << "\n"
      << "Finished: " << time << "\n" << sep << std::endl;

  delete pLog;

  if ( fail )
    return 1;

  return 0;
}
