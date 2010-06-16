
#ifndef TIMEDATETIME_H
#define TIMEDATETIME_H

#include <cmath>
#include <string>
#include <map>

#include "ufWin32Header.h"

namespace UF {
//! Some time utilities
namespace Time {

  //! A map for month names.
  typedef std::map < int, std::string > TMonthNames;

  //! Month names.
  /*!
  CLASS
    MonthNames

    This class provides a convenient way of accessing either month names
    or abbreviated month names based on the index on the month. Months are
    indexed from 1 to 12, with 1 being the first month of the year (January)
    and  12 being the last month of the year (December).
    The language used is English,
    however you can set the month names to other languages.

    The original date time classes were  developed by the
    author in 1982.

    \version 10.0

    \date 20-November-2003

    \author Andrew Maclean

    \par license
    This software is distributed WITHOUT ANY WARRANTY; without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    YOU, as the USER, take SOLE RESPONSIBILITY for, and are responsible for
    determining the FITNESS and APPLICABILITY of this software for a particular
    purpose. The author or any other person or entity are not liable in any way
    for the use or misuse of any program or software using this software.

    \todo

    \bug
  */
  class UF_DATE_TIME_EXPORT MonthNames
  {
  public:

    MonthNames ( void )
    {
      this->monthNames = new TMonthNames;

      (*this->monthNames)[1] = "January";
      (*this->monthNames)[2] = "February";
      (*this->monthNames)[3] = "March";
      (*this->monthNames)[4] = "April";
      (*this->monthNames)[5] = "May";
      (*this->monthNames)[6] = "June";
      (*this->monthNames)[7] = "July";
      (*this->monthNames)[8] = "August";
      (*this->monthNames)[9] = "September";
      (*this->monthNames)[10] = "October";
      (*this->monthNames)[11] = "November";
      (*this->monthNames)[12] = "December";
    }

    virtual ~MonthNames ( void ) { delete this->monthNames; }

    //! Get the month name.
    /*!
        An empty string is returned if the index of the month
        lies outside the range 1...12.
    */
    std::string GetMonthName(int month) const
    {
      if ( month > 0 && month < 13 )
        return (*this->monthNames)[month];
      else
        return "";
    }

    //! Get the abbreviated month name.
    /*!
        An empty string is returned if the index of the month
        lies outside the range 1...12.

        For other languages you may need to select a different
        number of letters to ensure a unique abbreviation. For
        English, three letters are sufficient.

        size is the number of letters used to form the abbreviation.
    */
    std::string GetAbbreviatedMonthName(int month, std::size_t const & size = 3) const
    {
      if ( month > 0 && month < 13 )
        if (size <= (*this->monthNames)[month].size() )
          return (*this->monthNames)[month].substr(0,size);
        else
          return (*this->monthNames)[month];
      else
        return "";
    }

    //! Set the month name. Useful when using a different language.
    /*
        There are 12 months and the first month starts at 1.
        No updates are performed if the index of the month
        lies outside the range 1...12.
    */
    void SetMonthName ( int month, std::string MonthName )
    {
      if ( month > 0 && month < 13 )
        (*this->monthNames)[month] = MonthName;
    }

  private:
    TMonthNames *monthNames;
  };

  //! A map for day names.
  typedef std::map < int, std::string > TDayNames;


  //! Day names.
  /*!
  CLASS
    DayNames

    This class provides a convenient way of accessing either day names
    or abbreviated day names based on the index on the day. Days are
    indexed from 0 to 6, with 0 being Sunday and day 6 being Saturday.
    The language used is English,
    however you can set the day names to other languages.

    The original date time classes were  developed by the
    author in 1982.

    \version 11.0

    \date 8-June-2006

    \author Andrew Maclean

    \par license
    This software is distributed WITHOUT ANY WARRANTY; without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    YOU, as the USER, take SOLE RESPONSIBILITY for, and are responsible for
    determining the FITNESS and APPLICABILITY of this software for a particular
    purpose. The author or any other person or entity are not liable in any way
    for the use or misuse of any program or software using this software.

    \todo

    \bug
  */
  class UF_DATE_TIME_EXPORT DayNames
  {
  public:

    DayNames ( void )
    {
      this->dayNames = new TDayNames;

      (*this->dayNames)[0] = "Sunday";
      (*this->dayNames)[1] = "Monday";
      (*this->dayNames)[2] = "Tuesday";
      (*this->dayNames)[3] = "Wednesday";
      (*this->dayNames)[4] = "Thursday";
      (*this->dayNames)[5] = "Friday";
      (*this->dayNames)[6] = "Saturday";
    }

    virtual ~DayNames ( void ) { delete this->dayNames; }

    //! Get the day name.
    /*!
        An empty string is returned if the index of the day
        lies outside the range 0...6.
    */
    std::string GetDayName(int day) const
    {
      if ( day > -1 && day < 7 )
        return (*this->dayNames)[day];
      else
        return "";
    }

    //! Get the abbreviated day name.
    /*!
        An empty string is returned if the index of the day
        lies outside the range 0...6.

        For other languages you may need to select a different
        number of letters to ensure a unique abbreviation. For
        English, two letters are sufficient, but three are used.

        size is the number of letters used to form the abbreviation.
    */
    std::string GetAbbreviatedDayName(int const & day, std::size_t const & size = 3) const
    {
      if ( day > -1 && day < 7 )
        if (size <= (*this->dayNames)[day].size() )
          return (*this->dayNames)[day].substr(0,size);
        else
          return (*this->dayNames)[day];
      else
        return "";
    }

    //! Set the day name. Useful when using a different language.
    /*
        There are 7 days and the first day starts at 0.
        No updates are performed if the index of the day
        lies outside the range 0...6.
    */
    void SetDayName ( int const & day, std::string const & DayName )
    {
      if ( day > 0 && day < 6 )
        (*this->dayNames)[day] = DayName;
    }

  private:
    //TDayNames dayNames;
    TDayNames *dayNames;
  };

  //! Julian/Gregorian Changeover date.
  /*!
  CLASS
    JGChangeover

    Default values are:
    The last Julian calendar day was Thu, 1582 Oct 4. The following
    day was  the first Gregorian calendar day, Fri, 1582 Oct 15, so 10
    calendar days were cancelled.

    Some countries persisted using the Julian Calendar right up into
    the early 1900's.

    If you want to use a calendar for a different country where the
    changeover date was different, then just change these values.

    For example:

    In US, Great Britain and Colonies the parameters for the changeover are
    {1752, 9, 2, 2361221, 1752, 9, 14}.  Note that the difference
    is 11 days between the last Julian date and the first Gregorian date.

    Warning:

    If you decide to alter values, be aware of the conditions
    for the Julian to Gregorian changeover as outlined in the
    introduction to the DateTime class. Changing these values can lead
    to unexpected behaviour in the DateTime class with regard to
    calendrical values.

    Irrespective of the changeover date, the Julian Day Numbers on or
    after the date when the Gregorian Calendar comes into operation
    should show a smooth progression from the Julian to the Gregorian
    calendar. In other words Julian Day numbers  are continuous even
    though the dates are not. This also means that there is no
    discontinuity in the week days.

    Thus we have, in 1582:

    The last day of the Julian Calendar being
    Thursday 1582 Oct 04 (Julian Day Number: 299159.5 )
    with first day of the Gregorian Calendar being
    Friday 1582 Oct 15 (Julian Day Number: 299160.5 ).

    The Julian Day Numbers are continuous and the weekdays
    are continuous.

    The original date time classes were developed by the
    author in 1982.

    \version 11.0

    \date 06-June-2006

    \author Andrew Maclean

    \par license
    This software is distributed WITHOUT ANY WARRANTY; without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    YOU, as the USER, take SOLE RESPONSIBILITY for, and are responsible for
    determining the FITNESS and APPLICABILITY of this software for a particular
    purpose. The author or any other person or entity are not liable in any way
    for the use or misuse of any program or software using this software.

    \todo

    \bug
  */
  class UF_DATE_TIME_EXPORT JGChangeover {
  public:
    JGChangeover ( void ) :
      JulianY(1582)
    , JulianM(10)
    , JulianD(4)
    , GregorianJDN(2299161L)
    , GregorianY(1582)
    , GregorianM(10)
    , GregorianD(15)
    {}
    virtual ~JGChangeover ( void ) {}
  public:
    //! The last year of the Julian Calendar.
    int JulianY;
    //! The last month of the Julian Calendar.
    int JulianM;
    //! The last day of the Julian Calendar.
    int JulianD;
    //! The julian day number of midday of the first day of the Gregorian Calendar. 2299161 = 1582 Oct 15.5
    int GregorianJDN;
    //! The first year of the Gregorian Calendar.
    int GregorianY;
    //! The first month of the Gregorian Calendar.
    int GregorianM;
    //! The first day of the Gregorian Calendar.
    int GregorianD;

  }; // { 1582, 10, 4, 2299161L, 1582, 10, 15 };

  //! Minimum valid date.
  /*!
  CLASS
    MinYMD

    The range of valid dates is from Monday -4712 January 01 to
    Wednesday 5874773 August 15

    Do not change anything here ... unless you really know what
    you are doing!

    The original date time classes were  developed by the
    author in 1982.

    \version 10.0

    \date 18-November-2003

    \author Andrew Maclean

    \par license
    This software is distributed WITHOUT ANY WARRANTY; without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    YOU, as the USER, take SOLE RESPONSIBILITY for, and are responsible for
    determining the FITNESS and APPLICABILITY of this software for a particular
    purpose. The author or any other person or entity are not liable in any way
    for the use or misuse of any program or software using this software.

    \todo

    \bug
   */
  class UF_DATE_TIME_EXPORT MinYMD
  {
  public:
    MinYMD( void ) :
      year(-4712)
    , month(1)
    , day(1)
    , jdn(-0.5)
    {}
    virtual ~MinYMD ( void ) {}

  public:
    //! Minimum valid year.
    int year;
    //! Minimum valid month of that year.
    int month;
    //! Minimum valid day of that month.
    int day;
    //! Minimum valid Julian Day Number.
    double jdn;
  }; // = { -4712, 1, 1, -0.5 };

  //! Maximum valid date.
  /*!
  CLASS
    MaxYMD

    The range of valid dates is from Monday -4712 January 01 to
    Wednesday 5874773 August 15.

    Do not change anything here ... unless you really know what
    you are doing!

    The original date time classes were  developed by the
    author in 1982.

    \version 10.0

    \date 18-November-2003

    \author Andrew Maclean

    \par license
    This software is distributed WITHOUT ANY WARRANTY; without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    YOU, as the USER, take SOLE RESPONSIBILITY for, and are responsible for
    determining the FITNESS and APPLICABILITY of this software for a particular
    purpose. The author or any other person or entity are not liable in any way
    for the use or misuse of any program or software using this software.

    \todo

    \bug
   */
  class UF_DATE_TIME_EXPORT MaxYMD
  {
  public:
    MaxYMD ( void ) :
      year(5874773)
    , month(8)
    , day(15)
    , jdn(2147438063.5)
    {}
    virtual ~MaxYMD ( void ) {}
  public:
    //! Maximum valid year.
    int year;
    //! Maximum valid month of that year.
    int month;
    //! Maximum valid day of that month.
    int day;
    //! Maximum valid Julian Day Number.
    double jdn;
  }; // = { 5874773, 8, 15, 2147438063.5 };


  //! Date and time manipulation routines.
  /*!
  CLASS
    DateTime


    Introduction

    This class performs year month day and fraction of a day
    calculations. In general you can represent a date as a double
    presision value corresponding to a Julian Day Number and the
    time of the day as a double precision number corresponding to the
    fraction of the day. When this is done, this class will conver to/from
    these representations.

    The intent of this class is to provide a series of operations on dates
    that are valid over a very large time-span and consistent with
    astronomical usage. The range of valid dates is from
    Monday -4712 January 01 to Wednesday 5874773 August 15.
    The Proleptic Julian Calendar is
    used for dates not in the Gregorian Calendar.
    The class handles the Julian and Gregorian calendar
    transition automatically. A date on or less than
    1582 Oct 4 is treated as a Julian Calendar date,
    whilst those on or later than 1582 Oct 15 are treated
    as Gregorian Calendar dates. This can be changed by the
    user to suit local circumstances.

    Input and output to the class is in the form of year, month and day and
    as Julian Day Numbers. Julian Day Numbers are the count of continuous
    days and fractions of a day from the year -4712. The Julian Day,
    by tradition, begins at Greenwich Mean Noon (12hUT). Thus midnight
    on -4712 January 1 corresponds to the Julian Day Number of -0.5.
    The fundamental unit in the date class is the Julian Day Number with
    the Year, Month and Day being displayed in either the Julian or
    Gregorian Calendar systems. The Gregorian Calendar reform is taken
    into account in the date class. The day following
    Thursday, 4 October 1582 (Julian Calendar) is
    Friday, 15 October 1582 (Gregorian Calendar).
    However the user may alter this to suit local circumstances.

    The Gregorian Calendar reform was not officially adopted by all
    countries at the same time. Thus civil calendars varied between
    countries and this fact must be kept in mind when undertaking
    historical research. In Great Britain the change was made on
    1752 September 3/14. Whilst in Turkey the change was undertaken
    on 1927 January 1. Provision is made for the user to change the
    Julian Gregorian Changeover date.

    The Julian Calendar was established in the year 46BC (-45) by
    Julius Caesar and reached its final form about the year 8.
    This was done by revising the ancient local calendar of the city of
    Rome, with the advice of the Alexandrian astronomer Soisgenes.

    Until year 8 there was some confusion over which years were leap
    or intercalary years. The intercalations were incorrectly made until
    the error was rectified in 8BC (-8) by Augustus, who omitted further
    intercalations until 8AD (8). This variation in intercalations in
    this period is ignored in this date object with an intercalary day
    being added every four years.

    For convenience, for dates less than this we will use the Julian
    Calendar  even though the calendar was yet still to be conceived!

    The Julian calendar remained in general use in Europe until 1582
    when the Gregorian Calendar began to replace it. The need for a
    replacement arose because the mean length of the Julian calendar
    was 365.25 days which exceeds the length of the Tropical Year by
    about 11m 14s. This amounts to about 3 days every 400 years and meant
    that by 1582  that the gradual change in calendar dates of the seasons
    was having a noticeable effect on the date of Easter, which was fixed
    by rules intended to keep it near the Vernal Equinox.

    The Gregorian Calendar was instituted in 1582 by by Pope Gregory XIII,
    primarily as a basis for regulating Easter and hence the
    ecclesiastical calendar.

    The Gregorian reform of the Julian calendar consisted of:
    - Omitting 10 days from the calendar reckoning, the day next after
    1582 October 4 being designated 1582 October 15, for the purpose of
    restoring the date of the actual vernal equinox to March 21;
    - Adopting a different rule for leap year, by omitting the intercalary
    day in centurial years that are not divisible by 400,
    such as 1700, 1800, 1900, and 2100, in order to correct the error
    of the Julian calendar where an intercalary day is inserted every
    four years;
    - Fixing rules for determining the date of Easter in the revised
    calendar.
    - The week was not modified in any way; special provision was made
    that the sequence of the days of the week was not broken.

    The mean length of the Gregorian calendar year is 365.2425 days.
    At the completion of a 400-year calendar cycle, the cumulative
    discrepancy with the tropical year is only a few hours.

    A note on the treatment of negative years:  The method used here is
    the only one suitable for arithmetical purposes. Historically, years
    previous to 1 have been termed BC years. I.e. the historical method of
    counting years is as follows:  ... 5BC, 4BC, 3BC, 2BC, 1BC, 1AD, 2AD,
    3AD, 4AD... . In this class the years are counted as follows: ... -4,
    -3, -2, -1, 0, 1, 2,  3,  4, .... So xBC == -x+1. This means that by
    using this methodology, regular arithmetic can be performed on the
    dates and also the rule of divisibility regarding leap years applies
    i.e. 0, -4, -8, -12, ... are leap years, corresponding to 1BC, 5BC,
    9BC, 13BC, ... .

    Further reading:
    - "Explanatory Supplement to the Astronomical Almanac and
    The American Ephemeris and Nautical Almanac",
    Her Majesty's Stationary Office, London, Third Impression 1974.
    ISBN 0 11 880578 9*.
    - Jan Meeus, "Astronomical Algorithms", Willmann-Bell Inc., 1991.
    ISBN 0-943396-35-2.
    - W.M. O'Neil "Time and Calendars", Sydney University Press, 1975.
    ISBN 0 424 00003 2.

    The original date time classes were  developed by the
    author in 1982.

    \version 10.0

    \date 18-November-2003

    \author Andrew Maclean

    \par license
    This software is distributed WITHOUT ANY WARRANTY; without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    YOU, as the USER, take SOLE RESPONSIBILITY for, and are responsible for
    determining the FITNESS and APPLICABILITY of this software for a particular
    purpose. The author or any other person or entity are not liable in any way
    for the use or misuse of any program or software using this software.

    \todo

    \bug
  */
  class UF_DATE_TIME_EXPORT DateTime : public
    JGChangeover,
    private MinYMD, MaxYMD
  {
  public:

    DateTime();
    virtual ~DateTime();

public:

  //! Tests if the date is a Gregorian date.
  /*!
   *
   * @param Y year
   * @param M month
   * @param D day
   * @return true if the date is gregorian.
   */
  bool IsGregorianDate (int const &Y, int const & M, int const &D) const;

  //! Test for a leap year.
  /*!
   * @param y year
   * @return true if it is a leap year.
   */
  bool IsLeapYear(int const& y) const;

  //! Check to ensure that the date is a valid one.
  /*!
   * @param y year
   * @param m month
   * @param d day
   * @return true if the date is valid.
   */
  bool IsValidYMD (int const & y, int const & m, int const & d) const;

  //! Check to ensure that the date is a valid one.
  /*!
   * @param jdn Julian Day Number
   * @return true if the date is valid.
   */
  bool IsValidJDN ( double const & jdn ) const;

  //! Calculate the Julian Day Number.
  /*!
   * Based on: Jean Meeus, "Astronomical Formulae for Calculators",
   * Second Edition, Willmann-Bell Inc., 1982.
   *
   * @param year year
   * @param month month
   * @param day day
   * @return the julian day number.
  */
  double JulianDayNumber(int const & year, int const & month, int const & day) const;


  //! Calculate the Year, Month, Day from the Julian date, discard the fraction of the day.
  /*!
   * Based on: Jean Meeus, "Astronomical Formulae for Calculators",
   * Second Edition, Willmann-Bell Inc., 1982.
   *
   * @param jdn Julian Day Number
   * @param y year
   * @param m month
   * @param d day
  */
  void CalendarDate(double const & jdn, int & y, int & m, int & d) const;

  //! Calculate the day of the week.
  /*!
    * @return the day of the week.
    */
  double DoW ( double const & jdn ) const { return std::fmod( ( jdn + 1.5 ) ,7 );}

  //! Calculate the day of the week. Sunday = 0.
  /*!
   * @param year year
   * @param month month
   * @param day day
   * @return the day of the week.
   */
  double DoW ( int const & year, int const & month, int const & day ) const
  {
    double jdn = JulianDayNumber(year,month,day);
    return std::fmod( ( jdn + 1.5 ) ,7 );
  }

  //! Calculate the day of the year. January 01 = 1.
  /*!
   * @param year year
   * @param month month
   * @param day day
   * @return the day of the year.
   */
  double DoY ( int const & year, int const & month, int const & day ) const
  {
    double jdn = JulianDayNumber(year,month,day);
    double jdn0 = JulianDayNumber(year,1,1);
    return jdn - jdn0 + 1;
  }

  //! Adjust jdn and fod such that 0 <= fod < 1.
  /*!
   * @param jdn Julian Day Number
   * @param fod Fraction of the day
   */
  void NormalizeJDFOD ( double &jdn, double &fod ) const;

  //! From the decimal representation hh.hhhh calculate the hour (or degree), minute, second.
  /*!
      secRounding controls the rounding of the seconds value. If rounding is to occur it
      should be positive and be >= 0 and < 15 inclusive. If it is any other value,
      no rounding occurs. E.g if secRounding = 3, then seconds are rounded to the
      nearest millisecond.
        It seems that the precision for seconds is around 11 decimal digits.
      On return check the hour value and adjust the date accordingly.
  */
  void ToHMS( double const & hr, int & hour, int & minute, double & second, int secRounding = 3 ) const;

  //! From the decimal representation hh.hhhh corrsponding to the hour (or degree), minute, second.
  /*!
    * @return the hour.
    */
  double FromHMS( int const & hour, int const & minute, double const & second ) const
  {
    bool sgn = hour < 0;

    int hr;

    if ( sgn )
      hr = -hour;
    else
      hr = hour;

    double res = (second / 60.0 + minute) / 60.0 + hr;

    if ( sgn )
      return -res;
    else
      return res;

  }

  //! Calculate the fraction of the day from the hours, minutes and seconds.
  /*!
    * @return the fraction of the day.
    */
  double ToFoD ( int const & hour, int const & minute, double const & second ) const
  {
    return FromHMS(hour,minute,second) / 24.0;;
  }

  //! Get the hour, minute and second from the fraction of the day.
  /*!
   * secRounding controls the rounding of the seconds value. If rounding is to occur it
   * should be positive and be >= 0 and < 15 inclusive. If it is any other value,
   * no rounding occurs. E.g if secRounding = 3, then seconds are rounded to the
   * nearest millisecond.
   *
   * It seems that the precision for seconds is around 11 decimal digits.
   * On return check the hour value and adjust the date accordingly.
   *
  */
  void FromFoD ( double const & fod,  int & hour, int & minute, double & second, int const & secRounding = 3 ) const
  {
    ToHMS( fod * 24.0, hour, minute, second, secRounding);
  }

  //! Convert a date string to a date.
  /*!
    * If the parse was not successful a valid date, namely
    * -4712-00-00, is set and false is returned.
    *
    * @param date date is yyyy-mm-dd format
    * @param year year.
    * @param month month.
    * @param day day.
    *
    * @return true if the time was parsed successfully, false otherwise.
    */
  bool Date ( std::string const & date, int & year, int & month, int & day ) const;

  //! Return the Julian Day Number.
  /*!
    * @param date date is yyyy-mm-dd format
    * @return the Julian Day Number.
    */
  double JulianDayNumber ( std::string const & date ) const;

  //! Convert a time string to a time.
  /*!
    * If the parse was not successful a valid time, namely
    * 00:00:00, is set and false is returned.
    * Formats allowed are:
    * (+|-)hh:mm:ss.s, 00:(+|-)mm:ss.s, 00:00:(+|-)ss.s, (+|-)00:00:ss.s
    * If a negative sign is detected then h,m,s are all returned as negative.
    *
    * @param time time is hh:mm:ss.s format
    * @param hour hour.
    * @param minute minute.
    * @param second second.
    *
    * @return true if the time was parsed successfully, false otherwise.
    */
  bool Time ( std::string const & time, int & hour, int & minute, double & second ) const;

  //! Return the fraction of the day.
  /*!
    * @param time time is hh:mm:ss format
    * @return the fraction of the day.
    */
  double FoD ( std::string const & time ) const;

  //! Convert a string representing the date and time.
  /*!
    * @param date_time  date and time in yyyy-mm-dd hh:mm:ss.s format.
    * @param jdn Julian Day Number
    * @param fod Fraction of the day
    */
  void ToDateTime ( std::string const & date_time, double & jdn, double & fod ) const;

  //! Convert a string representing the date and time.
  /*!
    * @param date_time  date and time in yyyy-mm-dd hh:mm:ss.s format.
    * @param year year
    * @param month month
    * @param day day
    * @param hour hour
    * @param minute minute
    * @param second second
    */
  void ToDateTime ( std::string const & date_time,
    int & year, int & month, int & day,
    int & hour, int & minute, double & second) const;

  //! Split ISO date and time into its constituents.
  /*!
    *
    * UTC offset is defined as follows:
    * if the offset is zero then the local time is equivalent to UTC,
    * if the offset > 0 then the local time is East of the meridian,
    * if the offset < 0 then the local time is West of the meridian.
    * E.g.
    * if the local time is UTC + 10h then the offset is 36000,
    * if the local time is UTC - 5h then the offset is -18000.
    *
    * @param date_time ISO Date
    * @param year year
    * @param month month
    * @param day day
    * @param hour hour
    * @param minute minute
    * @param second second
    * @param UTCOffset UTC Offset expressed as seconds.
    *
    * @return true if the parse was successful, false otherwise.
    */
  bool ISODateTime( std::string const & date_time,
    int & year, int & month, int & day,
    int & hour, int & minute, double & second,
    int & UTCOffset ) const;

  //! Return the Julian Day Number and fraction of the day.
  /*!
    *
    * If you convert jdn and fod to UTC by subtracting UTCOffset/86400.0 from fod
    * remember to call NormaliseJDFOD() after doing this.
    *
    * UTC offset is defined as follows:
    * if the offset is zero then the local time is equivalent to UTC,
    * if the offset > 0 then the local time is East of the meridian,
    * if the offset < 0 then the local time is West of the meridian.
    * E.g.
    * if the local time is UTC + 10h then the offset is 36000,
    * if the local time is UTC - 5h then the offset is -18000.
    *
    * @param date_time ISO Date
    * @param jdn Julian Day Number
    * @param fod Fraction of the day
    * @param UTCOffset UTC Offset expressed as seconds. Subtract this from fod to get UTC.
    *
    * @return true if the parse was successful, false otherwise.
    */
  bool ISODateTime( std::string const & date_time, double & jdn, double & fod, int & UTCOffset ) const;

  //! Return a formatted string representing the date.
  /*!
    * @param year year
    * @param month month
    * @param day day
    * @param sep the date separator
    * @return the date as a string.
    */
  std::string DateStr ( int const & year, int const & month, int const & day, char const & sep = '-' ) const;

  //! Return a formatted string representing the time.
  /*!
    * @param hour hour
    * @param minute minute
    * @param second second
    * @param precision the number of decimal places in the seconds
    * @param sep the time separator
    * @return the time as a string.
    */
  std::string TimeStr ( int const & hour, int const & minute, double const & second,
                        int const & precision = 3, char const & sep = ':') const;

  //! Return a formatted string representing the date and time.
  /*!
    * @param jdn Julian Day Number
    * @param fod Fraction of the day
    * @param precision number of decimal places in the seconds
    * @param datesep the date separator
    * @param timesep the time separator
    * @return the date and time as a string.
    */
  std::string DateTimeStr ( double const & jdn, double const & fod = 0,
                            int const & precision = 3,
                            char const & datesep = '-', char const & timesep = ':' ) const;

  //! Return a formatted string representing the date and time.
  /*!
    * @param year year
    * @param month month
    * @param day day
    * @param hour hour
    * @param minute minute
    * @param second second
    * @param precision number of decimal places in the seconds
    * @param datesep the date separator
    * @param timesep the time separator
    * @return the date and time as a string.
    */
  std::string DateTimeStr ( int const & year, int const & month, int const & day,
                            int const & hour, int const & minute, double const & second,
                            int const & precision = 3,
                            char const & datesep = '-', char const & timesep = ':') const;


  //! Return a formatted string representing the ISO date and time.
  /*!
  *
  * The time that you pass to this function is already assumed to 
  * be in local time, i.e. the LT-UTC offset has already been 
  * applied to the time.
  *
  * A string like this corresponding to the local time is returned:
  * 2004-02-23T14:46:22.356+11:00, or the equivalent instant
  * in UTC:2004-02-23T14:46:22.356Z.
  *
  * UTC offset is defined as follows:
  * if the offset is zero then the local time is equivalent to UTC,
  * if the offset > 0 then the local time is East of the meridian,
  * if the offset < 0 then the local time is West of the meridian.
  * E.g.
  * if the local time is UTC + 10h then the offset is 36000,
  * if the local time is UTC - 5h then the offset is -18000.
  *
  * @param year year
  * @param month month
  * @param day day
  * @param hour hour
  * @param minute minute
  * @param second second
  * @param UTCOffset Offset between UTC and local time expressed as seconds
  * @param precision number of decimal places in the seconds
  * @param dateSep the date separator
  * @param timeSep the time separator
  * @param dateTimeSep the separator character used between the data and time, often 'T' is used.
  * @param UTCMarker the character used to indicate UTC time, often 'Z' is used. If this is '0'
  *        then +00:00 is used. If this is 0 then no UTC marker is used. Otherwise, whatever 
  *        character you specify is used.
  *
  * It is the responsibility of the user to ensure the offset is correct.
  */
  std::string ISODateTimeStr ( int const & year, int const & month, int const & day,
                                int const & hour, int const & minute, double const & second,
                                int const & UTCOffset = 0,
                                int const & precision = 3,
                                char const & dateSep = '-',
                                char const & timeSep = ':',
                                char const & dateTimeSep = ' ',
                                char const & UTCMarker = '0' ) const;

  //! Return a formatted string representing the ISO date and time.
  /*!
  *
  * The time that you pass to this function is already assumed to 
  * be in local time, i.e. the LT-UTC offset has already been 
  * applied to the time.
  *
  * A string like this corresponding to the local time is returned:
  * 2004-02-23T14:46:22.356+11:00, or the equivalent instant
  * in UTC:2004-02-23T14:46:22.356Z.
  *
  * UTC offset is defined as follows:
  * if the offset is zero then the local time is equivalent to UTC,
  * if the offset > 0 then the local time is East of the meridian,
  * if the offset < 0 then the local time is West of the meridian.
  * E.g.
  * if the local time is UTC + 10h then the offset is 36000,
  * if the local time is UTC - 5h then the offset is -18000.
  *
  * @param jdn Julian Day Number
  * @param fod Fraction of the day
  * @param UTCOffset Offset between UTC and local time expressed as seconds
  * @param precision number of decimal places in the seconds
  * @param dateSep the date separator
  * @param timeSep the time separator
  * @param dateTimeSep the separator character used between the data an time
  * @param UTCMarker the character used to indicate UTC time
  *
  * It is the responsibility of the user to ensure the offset is correct.
  */
  std::string ISODateTimeStr (  double const & jdn, double const & fod = 0,
                                int const & UTCOffset = 0,
                                int const & precision = 3,
                                char const & dateSep = '-',
                                char const & timeSep = ':',
                                char const & dateTimeSep = 'T',
                                char const & UTCMarker = 'Z' ) const;

  //! Calculate leap seconds.
  /*!
   * This function returns TAI - UTC.
   * See See http://tycho.usno.navy.mil/leapsec.html or
   * http://www.boulder.nist.gov/timefreq/pubs/bulletin/leapsecond.htm
   * A leap second is a second added to Coordinated Universal Time (UTC) to make it agree with astronomical time to within 0.9 second. UTC is an atomic time scale, based on the performance of atomic clocks. Astronomical time is based on the rate of rotation of the earth. Since atomic clocks are more stable than the rate at which the earth rotates, leap seconds are needed to keep the two time scales in agreement.
   *
   * The first leap second was added on June 30, 1972. Since then, they have occurred at an average rate of less than one per year. Although it is possible to have a negative leap second (a second removed from UTC), so far, all leap seconds have been positive (an extra second has been added to UTC). Based on what we know about the earth's rotation, it seems unlikely that we will ever have a negative leap second.
   *
   * @param jdn Julian Day Number
   * @param fod Fraction of the day
   *
   * @return TAI - UTC in seconds.
   */
  double LeapSecond ( double const & jdn, double const & fod = 0 ) const;

  //! Trim unwanted characters from the left of a string.
  /*!
      @param str - the string to be trimmed.
      @param delims - the characters to search for,
      @return The trimmed string.
  */
  std::string TrimLeft(std::string const & str, std::string const & delims = " \t\r\n") const
  {
    // Trim leading whitespace.
    std::string::size_type pos = str.find_first_not_of(delims);
    if ( pos != std::string::npos )
      return str.substr(pos);
    else return "";
  }

  //! Trim unwanted characters from the right of a string.
  /*!
      @param str - the string to be trimmed.
      @param delims - the characters to search for,
      @return The trimmed string.
  */
  std::string TrimRight(std::string const & str, std::string const & delims = " \t\r\n") const
  {
    // Trim trailing whitespace.
    std::string::size_type pos = str.find_last_not_of(delims);
    if ( pos != std::string::npos )
      return str.substr(0,pos+1);
    else return "";
  }

  //! Trim unwanted characters from the left and right of a string.
  /*!
      @param str - the string to be trimmed.
      @param delims - the characters to search for,
      @return The trimmed string.
  */
  std::string Trim(std::string const & str, std::string const & delims = " \t\r\n") const
  {
    return TrimRight(TrimLeft(str,delims),delims);
  }


  //! Split a date and time string into date, time and offset (if any).
  /*!
      Take a string in the format yyyy-mm-dd[T| ][[hh:mm:ss.s][[+|-]hh:mm]]
      and split it into yyyy-mm-dd and optionally hh:mm:ss.s and hh:mm if the time exists.

      @param date_time the date and time string to be split.
      @param date the date.
      @param time the time.
      @param offset the offset from UTC to the time.
  */
  void SplitDateTime ( std::string const & date_time, std::string & date, std::string & time, std::string & offset) const;

  };

} // Namespace Time
} // Namepsace UF
#endif // DATE_TIME_H
