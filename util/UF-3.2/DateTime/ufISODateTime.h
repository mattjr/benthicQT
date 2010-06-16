#ifndef TIMEISODATETIME_H
#define TIMEISODATETIME_H

#include <string>
#include "ufDateTime.h"

#if defined(_MSC_VER) || defined(__BORLANDC__)
 typedef unsigned __int64 uint64;
 typedef signed __int64 int64;
#else
 typedef unsigned long long uint64;
 typedef signed long long int64;
#endif

namespace UF {
//! Some time utilities
namespace Time {


//! Holds the date and time.
/*!
  CLASS
    ISODateTime

    This class stores variables for the date and time. It also provides some
    specific methods for accessing and setting components of the date and time.

    You can input and output the date and time in the ISO format. Internally the
    time is stored as year, month, day, hour, minute, second and UTC offset.

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
class UF_DATE_TIME_EXPORT ISODateTime: public UF::Time::DateTime
{
public:
  ISODateTime(void);
  virtual ~ISODateTime(void);

  //! Constructor.
  ISODateTime( std::string const & date_time);

  //! Copy constructor.
  ISODateTime( ISODateTime const & rhs) : secondsInDay(86400)
  {
    if ( this == &rhs )
      return;

    this->year = rhs.year;
    this->month = rhs.month;
    this->day = rhs.day;
    this->hour = rhs.hour;
    this->minute = rhs.minute;
    this->second = rhs.second;
    this->offset = rhs.offset;

  }

  //! Assignment
  ISODateTime operator = ( ISODateTime const & rhs )
  {
    if ( this == &rhs )
      return *this;

    this->year = rhs.year;
    this->month = rhs.month;
    this->day = rhs.day;
    this->hour = rhs.hour;
    this->minute = rhs.minute;
    this->second = rhs.second;
    this->offset = rhs.offset;

    return *this;
  }

  //! Equality.
  bool operator == ( ISODateTime const & rhs ) const
  {
    // Adjust for any offsets.
    ISODateTime t1(*this);
    ISODateTime t2(rhs);
    t1.ToUTC();
    t2.ToUTC();

    double j1 = t1.GetJDN();
    double j2 = t2.GetJDN();
    if ( j1 != j2 )
    {
      return false;
    }
    double f1 = t1.GetFoD();
    double f2 = t2.GetFoD();
    double dFod = (j2-j1)+(f2-f1);
    if ( dFod == 0  )
    {
      return true;
    }
    return false;
  }

  //! Inequality.
  bool operator != ( ISODateTime const & rhs ) const
  {
    return !(*this == rhs);
  }

  //! Less than.
  bool operator < ( ISODateTime const & rhs ) const
  {
    // Adjust for any offsets.
    ISODateTime t1(*this);
    ISODateTime t2(rhs);
    t1.ToUTC();
    t2.ToUTC();

    double j1 = t1.GetJDN();
    double j2 = t2.GetJDN();
    if ( j1 < j2 )
    {
      return true;
    }
    if ( j2 - j1 > 1 )
    {
      return false;
    }
    double f1 = t1.GetFoD();
    double f2 = t2.GetFoD();
    double dFod = (j2-j1)+(f2-f1);
    if ( dFod > 0  )
    {
      return true;
    }
    return false;
  }

  //! Greater than.
  bool operator > ( ISODateTime const & rhs ) const
  {
    // Adjust for any offsets.
    ISODateTime t1(*this);
    ISODateTime t2(rhs);
    t1.ToUTC();
    t2.ToUTC();

    double j1 = t1.GetJDN();
    double j2 = t2.GetJDN();
    if ( j1 > j2 )
    {
      return true;
    }
    if ( j2 - j1 > 1 )
    {
      return true;
    }
    double f1 = t1.GetFoD();
    double f2 = t2.GetFoD();
    double dFod = (j2-j1)+(f2-f1);
    if ( dFod < 0  )
    {
      return true;
    }
    return false;
  }

  //! Less than or equal to.
  bool operator <= ( ISODateTime const & rhs ) const
  {
    return !(*this > rhs);
  }

  //! Greater than or equal to.
  bool operator >= ( ISODateTime const & rhs ) const
  {
    return !(*this < rhs);
  }

  //! Difference between this time and another.
  /*!
      @param rhs - the other time.
      @param deltaJD - the difference in days.
      @param deltaFoD - the difference in fractions of a day.
  */
  void ISOTimeDelta ( ISODateTime & rhs, double & deltaJD, double & deltaFoD )
  {
    deltaJD = rhs.GetJDN() - this->GetJDN();
    deltaFoD = rhs.GetFoD() - this->GetFoD();
  }

  //! Get the year.
  /*!
   * @return The year.
   */
  int GetYear () const { return this->year; }

  //! Get the months.
  /*!
   * @return The months.
   */
  int GetMonth () const { return this->month; }

  //! Get the days.
  /*!
   * @return The days.
   */
  int GetDay () const { return this->day; }

  //! Get the hours.
  /*!
   * @return The hours.
   */
  int GetHour () const { return this->hour; }

  //! Get the minutes.
  /*!
   * @return The minutes.
   */
  int GetMinute () const { return this->minute; }

  //! Get the seconds.
  /*!
   * @return The seconds.
   */
  double GetSecond () const { return this->second; }

  //! Get the UTC Offset.
  /*!
   * @return The the offset in seconds from UTC, Offset = Local Time - UTC.
   */
  int GetOffset () const { return this->offset; }

  //! Set the year.
  /*!
   * @param  Year the year.
   */
  void SetYear (int const & Year) { this->year = Year; }

  //! Set the months.
  /*!
   * @param Month the months.
   */
  void SetMonth (int const & Month) { this->month = Month; }

  //! Set the days.
  /*!
   * @param Day the days.
   */
  void SetDay (int const & Day) { this->day = Day; }

  //! Set the hours.
  /*!
   * @param Hour the hours.
   */
  void SetHour (int const & Hour) { this->hour = Hour; }

  //! Set the minutes.
  /*!
   * @param Minute the minutes.
   */
  void SetMinute (int const & Minute) { this->minute = Minute; }

  //! Set the seconds.
  /*!
   * @param Second the seconds.
   */
  void SetSecond (int const & Second) { this->second = Second; }

  //! Set the UTC Offset.
  /*!
   * @param  Offset the offset in seconds from UTC, Offset = Local Time - UTC.
   */
  void SetOffset (int const & Offset) { this->offset = Offset; }

  //! Get the date and time formatted as an ISO date time.
  /*!
   * There is provision through the following parameters to change the
   * precision and formatting:

   * @param precision number of decimal places in the seconds
   * @param dateSep the date separator
   * @param timeSep the time separator
   * @param dateTimeSep the separator character used between the data and time, often 'T' is used.
   * @param UTCMarker the character used to indicate UTC time, often 'Z' is used. If this is '0'
   *        then +00:00 is used. If this is 0 then no UTC marker is used. Otherwise, whatever 
   *        character you specify is used.
   * @return date and time formatted as an ISO date time.
   */
  std::string GetISODateTime (int const & precision = 3,
                              char const & dateSep = '-',
                              char const & timeSep = ':',
                              char const & dateTimeSep = ' ',
                              char const & UTCMarker = '0') const; 

  //! Set the date and time formatted as an ISO date time.
  /*!
   * If the parse failed, a valid date an time is generated, namely
   * -4712-0-0 00:00:00, however false is returned.
   *
   * @param DateTime the date and tme formatted as an ISO date time.
   *
   * @return true if the parse was successful, false otherwise.
   */
  bool SetISODateTime (std::string const & DateTime);

  //! Get the Julian Day Number.
  /*!
   * @return the Julian Day Number.
   */
  double GetJDN();

  //! Get the fraction of the day.
  /*!
   * @return the fraction of the day.
   */
  double GetFoD();

  //! Set the Julian Day Number.
  /*!
   * @param jdn the Julian Day Number.
   */
  void SetJDN(double const & jdn);

  //! Set the fraction of the day.
  /*!
   * @param fod the fraction of the day.
   * @param secondsPrecision The precision in seconds required, the 
                             default value should work for most conversions.
   */
  void SetFoD(double const & fod, int const & secondsPrecision = 11);

  //! Get the day of the week.
  /*!
   * @return the day of the week.
   */
  int GetDOW();

  //! Get the day of the year.
  /*!
   * @return the day of the year.
   */
  int GetDOY();

  //! Convert the time from Local Time to UTC.
  void ToUTC();
  //! Convert the time from UTC to Local time.
  /*!
   *  @param Offset in seconds, positive if East of the Greenwich Meridian.
  */
  void ToLocalTime(int const & Offset);

  //! Check if the UTC offset is valid.
  /*!
   * @return true if the UTC offset is valid.
   */
  bool IsValidOffset();

  //! Check if the date and time is valid.
  /*!
   * @return true if the date and time is valid.
   */
  bool IsValidDateTime();

  //! The number of seconds in a day.
  /*!
   * @return the number of seconds in a day.
   */
  int GetSecondsInDay() const
  {
    return this->secondsInDay;
  }

  //! Get the epoch, defined as the number of seconds corresponding to the Julian/Gregorian Date.
  /*!
   * @return the epoch, the number of seconds corresponding to the Julian/Gregorian Date.
  */
  double GetEpoch()
  {
    return (this->GetJDN() + this->GetFoD()) * this->secondsInDay; 
  }

  //! Set the epoch, defined as the number of seconds corresponding to the Julian/Gregorian Date.
  /*!
   * @param epoch, the number of seconds corresponding to the Julian/Gregorian Date.
  */
  void SetEpoch(double epoch)
  {
    int64 jdn = int64(epoch / this->secondsInDay);
    double fod = epoch / this->secondsInDay - jdn;
    double j = (double)jdn;
    this->NormalizeJDFOD(j,fod);
    this->SetJDN(j);
    this->SetFoD(fod);
  }


protected:
  //! Years
  int year;
  //! Months
  int month;
  //! Days
  int day;
  //! Hours
  int hour;
  //! Minutes
  int minute;
  //! Seconds
  double second;
  //! Offset in seconds from UTC (positive if East of the prime meridian).
  int offset;
  //! Number of seconds in a day.
  const int secondsInDay;

};

// STL Helpers.

//! Weak ordering.
struct ltISODateTime
{
  bool operator()(const UF::Time::ISODateTime &lhs, const UF::Time::ISODateTime &rhs) const
  {
    // Adjust for any offsets.
    ISODateTime t1(lhs);
    ISODateTime t2(rhs);
    t1.ToUTC();
    t2.ToUTC();

    double j1 = t1.GetJDN();
    double j2 = t2.GetJDN();
    if ( j1 < j2 )
    {
      return true;
    }
    if ( j2 - j1 > 1 )
    {
      return false;
    }
    double f1 = t1.GetFoD();
    double f2 = t2.GetFoD();
    double dFod = (j2-j1)+(f2-f1);
    if ( dFod > 0  )
    {
      return true;
    }
    return false;
  }
};


} // Namespace Time
} // Namepsace UF
#endif //TIMEISODATETIME_H
