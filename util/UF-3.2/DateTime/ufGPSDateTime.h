#ifndef TIMEGPSDATETIME_H
#define TIMEGPSDATETIME_H


#include <string>
#include "ufDateTime.h"

namespace UF {
namespace Time {


//! Holds the GPS date and time.
/*!
  CLASS
    ISODateTime

    This class stores variables for the GPS date and time. It also provides some
    specific methods for accessing and setting components of the GPS date and time.

    Global Positioning System time, is the atomic time scale implemented by the 
    atomic clocks in the GPS ground control stations and the GPS satellites themselves. 

    GPS time = TAI - 19 seconds.  GPS time matched UTC from 1980-01-06
    to 1981-07-01.  No leap seconds are inserted into GPS time, thus
    GPS time is 13 seconds ahead of UTC on 2000-01-01.  The GPS epoch
    is 00:00 (midnight) UTC on 1980-01-06.

    The differences between GPS Time and International Atomic Time (TAI)
    and Terrestrial Time (TT), also known as Terrestrial Dynamical Time
    (TDT), are constant at the level of some tens of nanoseconds while
    the difference between GPS Time and UTC changes in increments of
    seconds each time a leap second is added to UTC time scale.


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
class  UF_DATE_TIME_EXPORT GPSDateTime: public UF::Time::DateTime
{
public:
  GPSDateTime(void);
  virtual ~GPSDateTime(void);

  //! Copy constructor.
  GPSDateTime( GPSDateTime const & rhs) 
   : weekNumber(-1)
   , secondsInWeek(-1)
   , secondsInDay(86400)
   , GPS0(JulianDayNumber(1980,1,6))
   , GPS0fod(19.0/secondsInDay)
  {
    if ( this == &rhs )
      return;

    this->weekNumber = rhs.weekNumber;
    this->secondsInWeek = rhs.secondsInWeek;

  }

  //! Assignment
  GPSDateTime operator = ( GPSDateTime const & rhs )
  {
    if ( this == &rhs )
      return *this;

    this->weekNumber = rhs.weekNumber;
    this->secondsInWeek = rhs.secondsInWeek;

    return *this;
  }

  //! Get the GPS week number.
  /*!
   * @return GPS Week number.
   */
  int GetWeekNumber () const { return this->weekNumber; }

  //! Get the GPS seconds in the week.
  /*!
   * @return GPS seconds in the week.
   */
  double GetSecondsInWeek () const { return this->secondsInWeek; }

  //! Get the GPS day of the the week.
  /*!
   * @return GPS day of the week.
   */
  int GetDayNumber () const { return int(this->secondsInWeek/this->secondsInDay); }

  //! Get the GPS seconds in the day.
  /*!
   * @return GPS seconds in the day.
   */
  double GetSecondsInDay () const 
  {
    return this->secondsInWeek - int(this->secondsInWeek/this->secondsInDay)*this->secondsInDay;
  }

  //! Get the GPS time.
  /*!
   * @param WeekNumber the GPS Week number.
   * @param DayNumber GPS day number.
   * @param SecondsInDay GPS seconds in the day.
   */
  void GetGPSTime(int & WeekNumber, int & DayNumber, double & SecondsInDay) const 
  {
    WeekNumber = this->weekNumber;
    DayNumber  = int(this->secondsInWeek/this->secondsInDay);
    SecondsInDay = this->secondsInWeek - int(this->secondsInWeek/this->secondsInDay)*this->secondsInDay;;
  }

  //! Get the GPS time.
  /*!
   * @param WeekNumber the GPS Week number.
   * @param SecondsInWeek GPS seconds in the week.
   */
  void GetGPSTime (int & WeekNumber, double & SecondsInWeek) const 
  {
    WeekNumber = this->weekNumber;
    SecondsInWeek = this->secondsInWeek;
  }

  //! Set the GPS week number.
  /*!
   * @param WeekNumber the GPS Week number.
   */
  void SetWeekNumber (int const & WeekNumber) { this->weekNumber = WeekNumber; }

  //! Set the GPS seconds in the week.
  /*!
   * @param SecondsInWeek GPS seconds in the week.
   */
  void SetSecondsInWeek ( double const & SecondsInWeek)
  {
    this->secondsInWeek = SecondsInWeek;
  }

  //! Set the GPS seconds in the week.
  /*!
   * @param DayNumber GPS day number.
   * @param SecondsInDay GPS seconds in the day.
   */
  void SetSecondsInWeek (int const & DayNumber, double const & SecondsInDay)
  {
    this->secondsInWeek = DayNumber * this->secondsInDay + SecondsInDay;
  }

  //! Set the GPS day of the the week.
  /*!
   * @param DayNumber GPS day number.
   */
  void SetDayNumber (int const & DayNumber) 
  {
    this->secondsInWeek = DayNumber * this->secondsInDay + this->secondsInWeek - int(this->secondsInWeek/this->secondsInDay)*this->secondsInDay;
  }

  //! Set the seconds in a GPS day of the the week.
  /*!
   * @param SecondsInDay GPS seconds in the day.
   */
  void SetSecondsInDay (double const & SecondsInDay) 
  {
    this->secondsInWeek = SecondsInDay + int(this->secondsInWeek/this->secondsInDay)*this->secondsInDay;
  }

  //! Set the GPS time.
  /*!
   * @param WeekNumber the GPS Week number.
   * @param DayNumber GPS day number.
   * @param SecondsInDay GPS seconds in the day.
   */
  void SetGPSTime (int const & WeekNumber, int const & DayNumber, double const & SecondsInDay)
  {
    this->weekNumber = WeekNumber;
    this->secondsInWeek = DayNumber * this->secondsInDay + SecondsInDay;
  }

  //! Set the GPS time.
  /*!
   * @param WeekNumber the GPS Week number.
   * @param SecondsInWeek GPS seconds in the week.
   */
  void SetGPSTime (int const & WeekNumber, double const & SecondsInWeek)
  {
    this->weekNumber = WeekNumber;
    this->secondsInWeek = SecondsInWeek;
  }

  //! Get the date and time formatted as an ISO date time.
  /*!
   *
   * @param UTCOffset The difference between local time and utc in seconds,
   * @param UTCOffset = Local Time - UTC.
   * @return date and time formatted as an ISO date time.
   */
  std::string GetISODateTime (int const & UTCOffset = 0) const;

  //! Set the date and time formatted as an ISO date time.
  /*!
   * @param DateTime the date and time formatted as an ISO date time.
   *
   * @return false if the parse was unsuccessful, true otherwise.
   */
  bool SetISODateTime (std::string const & DateTime);

  //! Set the GPS time.
  /*!
   * @param jdn the Julian day number.
   * @param fod fraction of the day.
   * @param UTCOffset = Local Time - UTC.
   */
  void SetGPSTime (double const & jdn, double const & fod, int const & UTCOffset);

  //! Get the GPS time as UTC.
  /*!
   * @param jdn the Julian day number.
   * @param fod fraction of the day.
   */
  void GetGPSTimeAsUTC (double & jdn, double & fod) const;

  //! True if the GPS time is valid.
  /*!
   * @return true if the GPS week number and the seconds in the week are both positive.
   */
  bool IsValidGPSTime() const
  {
    return this->weekNumber >= 0 && this->secondsInWeek >= 0;
  }

  //! Get the TAI-UTC.
  /*!
   * @return The difference between TAI and UTC.
   */
  double GetTAIUTCDiff() const;

  //! Get the GPS-UTC.
  /*!
   * @return The difference between GPS and UTC.
   */
  double GetGPSUTCDiff() const { return GetTAIUTCDiff() - 19; }

protected:
  //! The GPS week number
  int weekNumber;
  //! Seconds in the GPS week.
  double secondsInWeek;
  //! Number of seconds in a day.
  const int secondsInDay;
  // GPS epoch beginning. 1980-01-06 00:00:19 TAI.
  double const GPS0;
  //! TAI-UTC at the GPS epoch.
  double const GPS0fod;

};

} // Namespace Time
} // Namepsace UF
#endif // TIMEGPSDATETIME_H
