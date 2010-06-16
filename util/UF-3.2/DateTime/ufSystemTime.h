//#pragma once
#if !defined(TIMESYSTEMTIME_H)
#define TIMESYSTEMTIME_H

#include "ufDateTime.h"
#include <stdexcept>

#include "ufWin32Header.h"

namespace UF {

namespace Time {

  //! System time.
  /*!
  CLASS
    SystemTime


    Introduction

    This singleton class obtains the time from the system,
    and applies an offset, if known, to calculate the UTC.

    The steps are:
    - When you have the time from a known correct source, use it to calculate the offset.
    - From then on, call Time() to get the corrected time.

    Usage:
    \code
      // Instead of using a fixed date we would get the date from a known
      // correct source, say a GPS to get the class to calculate an offset.
      SystemTime::Instance()->CalculateOffset( 3, 11, 18, 2, 33, 59.990);
      _sleep(5000);
      int y;
      int m;
      int d;
      int h;
      int min;
      double s;
      // Whenever we call Time(...), it will return the system time
      // modified by the previously calculated offset.
      SystemTime::Instance->Time(y,m,d,h,min,s);
    \endcode

    \version 3.0

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
  class UF_DATE_TIME_EXPORT SystemTime : private DateTime
  {
  public:
    //! Unique point of access for the singleton.
    static SystemTime* Instance()
    {
      if ( !pInstance_ )
      {
        // Check for a dead reference.
        if ( destroyed_ )
          OnDeadReference();
        else // First call initialise.
          Create();
      }
      return pInstance_;
    }

  private:

    //! Create a new singleton and store a pointer to it in pInstance_
    static void Create()
    {
      // Task: Initialise pInstance_
      static SystemTime theInstance;
      pInstance_ = &theInstance;
    }
    //! Gets called if a dead reference is detected.
    static void OnDeadReference()
    {
      throw std::runtime_error("Dead Reference Error in SystemTime.");
    }
    SystemTime( const SystemTime & ); // Disable the copy constructor.
    SystemTime& operator = ( const SystemTime & ); // Disable self-assignment.
    //! Private constructor, disables creation of the object by clients.
    SystemTime(void) :
     validOffset(false)
     , DayRange(41983-1) // Use one day less than the maximun range
     , Jd2000(2451544.5)
     , SecsInDay(86400.0)
    {};
    // Make the destructor private in order to prevent clients
    // holding a pointer from accidentally deleting it.
    virtual ~SystemTime(void)
    {
      pInstance_ = 0;
      destroyed_ = true;
    };

    //! Pointer to the instance.
    static SystemTime * pInstance_;
    //! True if destroyed.
    static bool destroyed_;

  public:

    //! Calculate the difference in time and date, relative to the system time.
    /*!
     * This difference is then used to correct the time as provided by the system.
     * @param jdn Julian Day Number
     * @param fod Fraction of the day
     */
    void CalculateOffset(  double const & jdn, double const & fod );

    //! Calculate the difference in time and date, relative to the system time.
    /*!
     * This difference is then used to correct the time as provided by the system.
     * @param year year
     * @param month month
     * @param day day
     * @param hour hour
     * @param minute minute
     * @param second second
     */
    void CalculateOffset(
      int const & year, int const & month, int const & day,
      int const & hour, int const & minute, double const & second
    );

    //! Do we have a valid offset for the system time?
    /*!
     * @return true if valid.
     */
    bool IsOffsetValid ( void ) const { return validOffset; }

    //! Clear the offset. This renders all times uncorrected from this point on.
    void ClearOffset ( void )
    {
      validOffset = false;
      deltaT = 0;
      deltaD = 0;
    }

    //! Return the difference in time relative to the system time expressed in seconds.
    /*!
    */
    double GetOffset ( void ) const
    {
      return deltaT * 86400.0 + deltaD * 86400.0;
    }

    //! Get the time.
    /*!
     * If the offset is not valid, then just the system time is returned.
     * False is returned to indicate that the uncorrected system time is being used.
     * You should check the returned time to ensure that it is valid.
     *
     * @param time a string representing the time
     * @param precision number of decimal places in the seconds
     * @param datesep the date separator
     * @param timesep the time separator
     * @return true if the time is corrected.
     */
    bool Time ( std::string & time, int const & precision = 3,
                char const & datesep = '-', char const & timesep = ':') const;

    //! Get the time.
    /*!
     * If the offset is not valid, then just the system time is returned.
     * False is returned to indicate that the uncorrected system time is being used.
     * You should check the returned time to ensure that it is valid.
     *
     * @param jdn Julian Day Number
     * @param fod Fraction of the day
     * @return true if the time is corrected.
    */
    bool Time ( double & jdn,  double & fod ) const;

    //! Get the time.
    /*!
     * If the offset is not valid, then just the system time is returned.
     * False is returned to indicate that the uncorrected system time is being used.
     * You should check the returned time to ensure that it is valid.
     *
     * @param year year
     * @param month month
     * @param day day
     * @param hour hour
     * @param minute minute
     * @param second second
     * @return true if the time is corrected.
    */
    bool Time (
      int & year, int & month, int & day,
      int & hour, int & minute, double & second
    ) const;

    //! Get the time.
    /*!
     * If the time is not valid, we set the returned value to 0xffffffff.
     * If the offset is not valid we add the mask 0xE0000000 to the time value.
     * This implies that we are using the uncorrected system time.
     * This means that times range from 0 to 0xDFFFFFFF.
     * If the base date 2000-Jan-01 then the range is 0 to 3758096383 seconds.
     * So the maximum possible date in this system is: 2122-December-21 11h 39m 43s
     *
     * So if the returned value == 0xFFFFFFF -> Invalid time.
     * If the returned time is > 0xDFFFFFF -> time is uncorrected.
     * In this case subtract 0xE0000000 to get the time.
     *
     * @return the time as unsigned int.
    */
    unsigned int Time () const;

    //! Get the UTC offset.
    /*!
     * This depends upon knowing the difference between local time and UTC.
     * So it assumes that the system time is UTC and that the difference between
     * system time and UTC is known. It also assumes that the time, time zone
     * and daylight saving is set correctly in the system.
     *
     * @return The UTC offset relative to UTC (East positive) in seconds.
    */
    int GetUTCOffset () const;

    //! Get the UTC offset.
    /*!
     * This depends upon knowing the difference between local time and UTC.
     * So it assumes that the system time is UTC and that the difference between
     * system time and UTC is known. It also assumes that the time, time zone
     * and daylight saving is set correctly in the system.
     *
     * The offset is relative to UTC and is positive if the local time is
     * east of the UTC meridian.
     *
     * @param hours the offset in hours
     * @param minutes the offset in minutes
    */
    void GetUTCOffset (int & hours, int & minutes) const;

    //! Get the local time.
    /*!
     * This depends upon knowing the difference between local time and UTC.
     * So it assumes that the system time is UTC and that the difference between
     * system time and UTC is known. It also assumes that the time, time zone
     * and daylight saving is set correctly in the system.
     *
     * If the offset is not valid, then just the system time converted to local time
     * is returned.
     * False is returned to indicate that the uncorrected local time is being used.
     * You should check the returned time to ensure that it is valid.
     *
     * UTC offset is defined as follows:
     * if the offset is zero then the local time is equivalent to UTC,
     * if the offset > 0 then the local time is East of the meridian,
     * if the offset < 0 then the local time is West of the meridian.
     * E.g.
     * if the local time is UTC + 10h then the offset is 36000,
     * if the local time is UTC - 5h then the offset is -18000.
     *
     * @param time a string representing the time
     * @param precision number of decimal places in the seconds
     * @param datesep the date separator
     * @param timesep the time separator
     * @return true if the time is corrected.
     */
    bool LocalTime ( std::string & time,
                     int const & precision = 3,
                     char const & datesep = '-', char const & timesep = ':',
                     const bool & Use_UTC_offset = false,
                     const int & UTC_offset = 0 ) const;

    //! Get the local time in ISO format.
    /*!
     * This depends upon knowing the difference between local time and UTC.
     * So it assumes that the system time is UTC and that the difference between
     * system time and UTC is known. It also assumes that the time, time zone
     * and daylight saving is set correctly in the system.
     *
     * If the offset is not valid, then just the system time converted to local time
     * is returned.
     * False is returned to indicate that the uncorrected local time is being used.
     * You should check the returned time to ensure that it is valid.
     *
     * UTC offset is defined as follows:
     * if the offset is zero then the local time is equivalent to UTC,
     * if the offset > 0 then the local time is East of the meridian,
     * if the offset < 0 then the local time is West of the meridian.
     * E.g.
     * if the local time is UTC + 10h then the offset is 36000,
     * if the local time is UTC - 5h then the offset is -18000.
     *
     * @param time a string representing the time
     * @param Use_UTC_offset Use the supplied UTC offset.
     * @param UTCOffset Offset between UTC and local time expressed as seconds.
     * @param precision number of decimal places in the seconds
     * @return true if the time is corrected.
     * @param dateSep the date separator
     * @param timeSep the time separator
     * @param dateTimeSep the separator between the date and time.
     * @param UTCMarker the character used to indicate UTC time
     */
    bool ISOLocalTime ( std::string & time,
                        const bool & Use_UTC_offset = false,
                        int const & UTCOffset = 0,
                        int const & precision = 3,
                        char const & dateSep = '-',
                        char const & timeSep = ':',
                        char const & dateTimeSep = 'T',
                        char const & UTCMarker = 'Z' ) const;

    //! Get the local time.
    /*!
     * This depends upon knowing the difference between local time and UTC.
     * So it assumes that the system time is UTC and that the difference between
     * system time and UTC is known. It also assumes that the time, time zone
     * and daylight saving is set correctly in the system.
     *
     * If the offset is not valid, then just the system time converted to local time
     * is returned.
     * False is returned to indicate that the uncorrected local time is being used.
     * You should check the returned time to ensure that it is valid.
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
     * @param Use_UTC_offset Use the supplied UTC offset.
     * @param UTC_offset Offset between UTC and local time expressed as seconds.
     * @return true if the time is corrected.
    */
    bool LocalTime ( double & jdn,  double & fod,
                     const bool & Use_UTC_offset = false,
                     const int & UTC_offset = 0 ) const;

    //! Get the local time.
    /*!
     * This depends upon knowing the difference between local time and UTC.
     * So it assumes that the system time is UTC and that the difference between
     * system time and UTC is known. It also assumes that the time, time zone
     * and daylight saving is set correctly in the system.
     *
     * If the offset is not valid, then just the system time converted to local time
     * is returned.
     * False is returned to indicate that the uncorrected local time is being used.
     * You should check the returned time to ensure that it is valid.
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
     * @param Use_UTC_offset Use the supplied UTC offset.
     * @param UTC_offset Offset between UTC and local time expressed as seconds.
     * @return true if the time is corrected.
     */
    bool LocalTime (
      int & year, int & month, int & day,
      int & hour, int & minute, double & second,
      const bool & Use_UTC_offset = false,
      const int & UTC_offset = 0 ) const;

    //! Get the local time.
    /*!
     * This depends upon knowing the difference between local time and UTC.
     * So it assumes that the system time is UTC and that the difference between
     * system time and UTC is known. It also assumes that the time, time zone
     * and daylight saving is set correctly in the system.
     *
     * If the time is not valid, we set the returned value to 0xffffffff.
     * If the offset is not valid we add the mask 0xE0000000 to the time value.
     * This implies that we are using the uncorrected system time.
     * This means that times range from 0 to 0xDFFFFFFF.
     * If the base date 2000-Jan-01 then the range is 0 to 3758096383 seconds.
     * So the maximum possible date in this system is: 2122-December-21 11h 39m 43s
     *
     * So if the returned value == 0xFFFFFFF -> Invalid time.
     * If the returned time is > 0xDFFFFFF -> time is uncorrected.
     * In this case subtract 0xE0000000 to get the time.
     *
     * @param Use_UTC_offset Use the supplied UTC offset.
     * @param UTC_offset Offset between UTC and local time expressed as seconds.
     * @return the time as unsigned int.
     */
    unsigned int LocalTime (
                     const bool & Use_UTC_offset = false,
                     const int & UTC_offset = 0 ) const;

  private:

    //! The difference in time relative to the system time expressed as fraction of a day.
    double deltaT;
    //! The difference in date relative to the system date expressed in days.
    double deltaD;
    //! True of the offset has been calculated.
    bool validOffset;

    //! Maximum number of days that can be represented.
    double DayRange;
    //! Julian Day Number of 2000-Jan-01
    double Jd2000;
    //! Number of seconds in a day.
    double SecsInDay;
  };

} // Namespace Time
} // Namepsace UF

#endif // TIMESYSTEMTIME_H
