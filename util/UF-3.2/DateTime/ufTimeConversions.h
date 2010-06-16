
#ifdef _MSC_VER
#pragma once
#endif

//#pragma once
#if !defined(TIMETIMECONVERSIONS_H)
#define TIMETIMECONVERSIONS_H

#include "ufDateTime.h"
#include <stdexcept>

#include "ufWin32Header.h"

namespace UF {

namespace Time {

  //! Time Conversions.
  /*!
  CLASS
    TimeConversions


    Introduction

    This singleton class facliliates the conversion of time to/from
    various formats.

    \version 3.0

    \date 23-May-2006

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
 class UF_DATE_TIME_EXPORT TimeConversions : public DateTime
 {
  public:
    //! Unique point of access for the singleton.
    static TimeConversions* Instance()
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
      static TimeConversions theInstance;
      pInstance_ = &theInstance;
    }
    //! Gets called if a dead reference is detected.
    static void OnDeadReference()
    {
      throw std::runtime_error("Dead Reference Error in TimeConversions.");
    }
    //! Private constructor, disables creation of the object by clients.
    TimeConversions(void);
    TimeConversions( const TimeConversions & ); // Disable the copy constructor.
    TimeConversions& operator = ( const TimeConversions & ); // Disable self-assignment.
    // Make the destructor private in order to prevent clients
    // holding a pointer from accidentally deleting it.
    virtual ~TimeConversions(void)
    {
      pInstance_ = 0;
      destroyed_ = true;
    };

    //! Pointer to the instance.
    static TimeConversions * pInstance_;
    //! True if destroyed.
    static bool destroyed_;

  public:

    //! Convert the GPS time expressed as a double in the format hhmmss.s to the fraction of a day.
    /*!
        The range for a valid fraction of a day is from 0 to 1.
        If it is < 0 then the time is invalid.

        @param GPSTime

        @return the fraction of the day
    */
    double GPSTimeToFoD ( double const & GPSTime ) const
    {
      double fod = HMSToSeconds(GPSTime);
      if ( fod < 0 ) return -1;
      else return fod/this->SecsInDay;
    }

    //! From the GPS date expressed as ddmmyy calculate the Julian Day Number.
    /*!
      Note that the year is double digit, so we are assuming 2000 as the
      base year.

      @param GPSDate the GPS date

      @return the julian day number
    */
    double GPSDateToJDN ( double GPSDate ) const;

    //! Convert the date and time to the unsigned int format.
    /*!
      If the time is not valid, we set the returned value to 0xffffffff.
      If the offset is not valid we add the mask 0xE0000000 to the time value.
      This implies that we are using the uncorrected system time.
      This means that times range from 0 to 0xDFFFFFFF.
      If the base date 2000-Jan-01 then the range is 0 to 3758096383 seconds.
      So the maximum possible date in this system is: 2122-December-21 11h 39m 43s

      So if the returned value == 0xFFFFFFFF -> Invalid time.
      If the returned time is > 0xDFFFFFFF -> time is uncorrected.
      In this case subtract 0xE0000000 to get the time.

      @param jdn Julian Day Number
      @param fod fraction of the day

      @return the unsigned int representation of the date marked as corrected.

    */
    unsigned int DateTimeToUI ( double const &  jdn, double const &  fod ) const;

    //! Convert the date and time to the unsigned int format.
    /*!
      If the time is not valid, we set the returned value to 0xffffffff.
      If the offset is not valid we add the mask 0xE0000000 to the time value.
      This implies that we are using the uncorrected system time.
      This means that times range from 0 to 0xDFFFFFFF.
      If the base date 2000-Jan-01 then the range is 0 to 3758096383 seconds.
      So the maximum possible date in this system is: 2122-December-21 11h 39m 43s

      So if the returned value == 0xFFFFFFFF -> Invalid time.
      If the returned time is > 0xDFFFFFFF -> time is uncorrected.
      In this case subtract 0xE0000000 to get the time.

      @param year year
      @param month month
      @param day day
      @param hour hour
      @param minute minute
      @param second second

      @return the unsigned int representation of the date, marked as corrected.

    */
    unsigned int DateTimeToUI ( int const & year, int const & month, int const & day,
      int const &  hour, int const &  minute, double const & second ) const;


    //! Get the Julian Day Number from the unsigned int time.
    /*!
       @param dt unsigned int time
       @return jdn Julian Day Number
   */
    double UIToJDN ( unsigned int const & dt ) const
    {
      // Make the time corrected.
      unsigned int d = dt & ~(this->UncorrectedTimeFlag);
      unsigned int days = (unsigned int)(d / this->SecsInDay);
      return Jd2000 + days;
    }

    //! Get the fraction of the day from the unsigned int time.
    /*!
       @param dt unsigned int time
       @return fod fraction of the day
   */
    double UIToFoD ( unsigned int const & dt ) const
    {
      unsigned int d = dt & ~(this->UncorrectedTimeFlag);
      double days = (unsigned int)(d / this->SecsInDay);
      return (d - days * this->SecsInDay) / this->SecsInDay;
    }

    //! Convert the unsigned int time to Julian Day Number and fraction of a day.
    /*!
       @param dt unsigned int time
       @param jdn Julian Day Number
       @param fod fraction of the day
   */
    void UIToDateTime ( unsigned int const & dt, double & jdn, double & fod ) const
    {
      jdn = this->UIToJDN(dt);
      fod = this->UIToFoD(dt);
      this->NormalizeJDFOD(jdn,fod);
    }

    //! Convert the unsigned int time to a date and time.
    /*!
       @param dt unsigned int time
       @param year year
       @param month month
       @param day day
       @param hour hour
       @param minute minute
       @param second second
   */
    void UIToDateTime ( unsigned int const & dt, int & year, int & month, int & day,
      int & hour, int & minute, double & second ) const
    {
      double jdn;
      double fod;

      UIToDateTime(dt,jdn,fod);
      this->CalendarDate(jdn,year,month,day);
      this->SecondsToHMS(fod*this->SecsInDay,hour,minute,second);
    }

    //! Convert the GPS date and time to the unsigned int format.
    /*!
        GPS date is formatted as yymmdd
        GPS time is formatted as hhmmss.s

        @param GPSDate
        @param GPSTime

        @return the unsigned int representation of the date

    */
    unsigned int GPSToUI ( double GPSDate, double GPSTime ) const
    {
      double jdn = GPSDateToJDN(GPSDate);
      double fod = GPSTimeToFoD(GPSTime);
      return DateTimeToUI(jdn,fod);
    }

    //! Convert the unsigned int time to an uncorrected time.
    /*!
        Useful when comparing times.
    */
    unsigned int ToUncorrectedUI ( unsigned int const & t ) const
    {
      if ( t == this->InvalidTimeFlag )
      {
        return this->InvalidTimeFlag;
      }
      return t | this->UncorrectedTimeFlag;
    }

    //! Convert the unsigned int time to a corrected time.
    /*!
        Useful when comparing times.
    */
    unsigned int ToCorrectedUI ( unsigned int const & t ) const
    {
      if ( t == InvalidTimeFlag )
      {
        return this->InvalidTimeFlag;
      }
      return t & ~(this->UncorrectedTimeFlag);
    }

    //! Compare two times for ==.
    /*!
        Times are compared on the basis of both being uncorrected.
        Note that if either of the times are invalid, false is returned.
    */
    bool EQ ( unsigned int const & lhs, unsigned int const & rhs ) const
    {
      if ( lhs == this->InvalidTimeFlag || rhs == this->InvalidTimeFlag )
      {
        return false;
      }
      unsigned int l = lhs | this->UncorrectedTimeFlag;
      unsigned int r = rhs | this->UncorrectedTimeFlag;
      return l == r;
    }

    //! Compare two times for <.
    /*!
        Times are compared on the basis of both being uncorrected.
        Note that if either of the times are invalid, false is returned.
    */
    bool LT ( unsigned int const & lhs, unsigned int const & rhs ) const
    {
      if ( lhs == this->InvalidTimeFlag || rhs == this->InvalidTimeFlag )
      {
        return false;
      }
      unsigned int l = lhs | this->UncorrectedTimeFlag;
      unsigned int r = rhs | this->UncorrectedTimeFlag;
      return l < r ;
    }

    //! Compare two times for >.
    /*!
        Times are compared on the basis of both being uncorrected.
        Note that if either of times are invalid false is returned.
    */
    bool GT ( unsigned int const & lhs, unsigned int const & rhs ) const
    {
      if ( lhs == this->InvalidTimeFlag || rhs == this->InvalidTimeFlag )      {
        return false;
      }
      unsigned int l = lhs | this->UncorrectedTimeFlag;
      unsigned int r = rhs | this->UncorrectedTimeFlag;
      return ( l > r );
    }

    //! Compare two times !=.
    /*!
        Times are compared on the basis of both being uncorrected.
        Note that if either of times are invalid false is returned.
    */
    bool NE ( unsigned int const & lhs, unsigned int const & rhs ) const
    {
      if ( lhs == this->InvalidTimeFlag || rhs == this->InvalidTimeFlag )
      {
        return false;
      }
      return !EQ(lhs,rhs);
    }

    //! Compare two times <=.
    /*!
        Times are compared on the basis of both being uncorrected.
        Note that if either of times are invalid false is returned.
    */
    bool LE ( unsigned int const & lhs, unsigned int const & rhs ) const
    {
      if ( lhs == this->InvalidTimeFlag || rhs == this->InvalidTimeFlag )
      {
        return false;
      }
      return !GT(lhs,rhs);
    }

    //! Compare two times >=.
    /*!
        Times are compared on the basis of both being uncorrected.
        Note that if either of times are invalid false is returned.
    */
    bool GE ( unsigned int const & lhs, unsigned int const & rhs ) const
    {
      if ( lhs == this->InvalidTimeFlag || rhs == this->InvalidTimeFlag )
      {
        return false;
      }
      return !LT(lhs,rhs);
    }

    //! Returns true if the time is valid.
    /*!
     *
     * @param time the time.
     *
     * @return true if the time is valid.
     */
    bool IsValidUITime ( unsigned int time ) const
    {
      return time != this->InvalidTimeFlag;
    }

    //! Returns true if the time is uncorrected.
    /*!
     * Check for invalid time before using this function.
     *
     * @param time the time.
     *
     * @return true if the time is uncorrected.
     */
    bool IsUncorrectedUITime ( unsigned int time ) const
    {
      if ( !IsValidUITime(time) )
      {
        return false;
      }
      return time >= this->UncorrectedTimeFlag;
    }

    //! Returns true if the time is corrected.
    /*!
     * Check for invalid time before using this function.
     *
     * @param time the time.
     *
     * @return true if the time is corrected.
     */
    bool IsCorrectedUITime ( unsigned int time ) const
    {
      if ( !IsValidUITime(time) )
      {
        return false;
      }
      return !IsUncorrectedUITime(time);
    }

    //! Convert a time in hhmmss.s format to seconds.
    /*!
     *
     * @param time the time.
     *
     * @return the seconds corresponding to the time.
     */
    double HMSToSeconds ( double const & time ) const
    {
      double t;
      if ( time < 0 )
      {
        t = -t;
      }
      int hh = int(t / 10000);
      int mm = int((t - hh * 10000)/100.0);
      double ss = t - hh * 10000 - mm * 100;
      double result = (hh * 60.0 + mm)*60.0 + ss;
      if ( time < 0 )
      {
        result = -result;
      }
      return result;
    }

    //! Convert a time in hours minutes and seconds to seconds.
    /*!
     * @param hour the hours.
     * @param minute the minutes.
     * @param second the seconds.
     *
     * @return the seconds corresponding to the time.
     */
    double HMSToSeconds ( int const & hour, int const & minute, double const & second ) const
    {
      return (hour * 60.0 + minute)*60.0 + second;
    }

    //! Convert a time in seconds to hhmmss.s
    /*!
     *
     * @param time the time.
     * @return the time.
    */
    double SecondsToHMS ( double const & time ) const
    {
      int hh;
      int mm;
      double ss;
      SecondsToHMS(time,hh,mm,ss);
      return ( hh * 10000 + mm * 100 + ss );
    }

    //! Convert a time in seconds to hours minutes and seconds
    /*!
     * @param time the time.
     * @param hour the hours.
     * @param minute the minutes.
     * @param second the seconds.
     */
    void SecondsToHMS ( double const & time, int & hour, int & minute, double & second ) const
    {
      double t = time;
      if ( time < 0 )
      {
        t = -t;
      }
      hour = (int)(t / 3600.0);
      second = (t - hour * 3600)/60;
      minute = (int)second;
      second = (second - minute) * 60.0;
      if ( time < 0 )
      {
        hour = -hour;
        minute = -minute;
        second = -second;
      }
    }

  //! Generate a string representing the date and time.
  /*!
    * @param dt date and time
    * @param DispCorrected add a marker if the time is uncorrected, a space otherwise.
    * @param DateSep date separator.
    * @param TimeSep time separator.
    *
    * @return the date and time.
  */
  std::string MakeDateTimeString(unsigned int const & dt, bool const & DispCorrected = true, char const & DateSep = '-', char const & TimeSep = ':' ) const
  {

    int year;
    int month;
    int day;
    int hour;
    int minute;
    double second;

    std::string s;
    if ( dt != this->InvalidTimeFlag )
    {
      UIToDateTime(dt,year,month,day,hour,minute,second);
      s = DateStr(year,month,day,DateSep);
      s += " ";
      s += TimeStr(hour,minute,second,0,TimeSep);
      if ( IsUncorrectedUITime(dt) && DispCorrected )
        s += " u";
    }
    else
      s = "Invalid Time";

    return s;
  }

  //! Generate a string representing the date.
  /*!
    * @param dt date and time
    * @param DispCorrected add a marker if the time is uncorrected, a space otherwise.
    * @param sep date separator.
    *
    * @return the date.
  */
  std::string MakeDateString(unsigned int const & dt, bool const & DispCorrected = false, char const & sep = '-') const
  {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    double second;

    UIToDateTime(dt,year,month,day,hour,minute,second);
    std::string s = DateStr(year,month,day,sep);
    if ( IsUncorrectedUITime(dt) && DispCorrected )
      s += " u";

    return s;
  }

  //! Generate a string representing the time.
  /*!
    * @param dt date and time
    * @param DispCorrected add a marker if the time is uncorrected, a space otherwise.
    * @param sep time separator.
    *
    * @return the time.
  */
  std::string MakeTimeString(unsigned int const & dt, bool const & DispCorrected = true, char const & sep = ':') const
  {
    Time::TimeConversions * pTC = Time::TimeConversions::Instance();

    int year;
    int month;
    int day;
    int hour;
    int minute;
    double second;

    pTC->UIToDateTime(dt,year,month,day,hour,minute,second);
    std::string s = TimeStr(hour,minute,second,0,sep);
    if ( IsUncorrectedUITime(dt) && DispCorrected )
      s += " u";

    return s;
  }


  private:
    //! Maximum number of days that can be represented.
    double DayRange;
    //! Julian Day Number of 2000-Jan-01
    double Jd2000;
    //! Number of seconds in a day.
    double SecsInDay;
    //! Invalid time
    unsigned int InvalidTimeFlag;
    //! Uncorrected time, this is added to the time to indicate that it is uncorrected.
    unsigned int UncorrectedTimeFlag;
  };

} // Namespace Time
} // Namepsace UF

#endif // TIMETIMECONVERSIONS_H
