#if !defined(ANGLENUMANGLEDATETIMECONVERSIONS_H)
#define ANGLENUMANGLEDATETIMECONVERSIONS_H

#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>

#include "ufWin32Header.h"

namespace UF{

//! Conversion of angles to numbers and or strings and vice-versa.
namespace AngleNum
{

  //! Convert degrees minutes and seconds to a string
  /*!
    Take the sign and three numbers representing the degrees minutes and seconds
    of the angle and return a nicely formatted string. You can also set the
    precision of the seconds. Also use this for angles in the range +/- 180 or 0-360 degrees.
    If sign is blank the angle is positive otherwise the angle is negative.

    Usage:
    \code
    std::string s = dms2str(sign, degrees, minutes, seconds)();
    \endcode

    \version 1.0

    \date 23-June-2003

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
  struct /*UF_CONVERSIONS_EXPORT*/ dms2str
  {
    //! Desrees minutes and seconds to string.
    /*!
     * @param sgn : the sign
     * @param d : degrees
     * @param m : minutes
     * @param s : seconds
     * @param prec : precision
     */
    dms2str(char sgn = '+', int d = 0, int m = 0, double s = 0, int prec = 5)
      : _d((sgn == '-')? -d : d), _m(m), _s(s), _prec(prec)
    {}

    //! Return the string corresponding to the degrees minutes and seconds.
    std::string operator () ()
    {
      std::ostringstream os;
      os << std::setw(4) << _d << " " << std::setw(2)  << _m << " ";
      os.flags(std::ios::left);
      if ( _s < 10 )
        os << " " << std::setw(_prec+3) << std::setprecision(_prec) << _s;
      else
        os << std::setw(_prec+4) << std::setprecision(_prec) << _s;

      return os.str();
    }

  private:
    int _d;
    int _m;
    double _s;
    int _prec;
  };

  //! Convert degrees and minutes to a string
  /*!
    Take the sign and two numbers representing degrees, minutes and
    decimals of a minute and return a nicely formatted string. You can also set the
    precision of the minutes.  Also use this for angles in the range +/- 90 degrees.
    If sign is blank the angle is positive otherwise the angle is negative.

    Usage:
    \code
    std::string s = dm2str(sign, degrees, minutes)();
    \endcode

    \version 1.0

    \date 23-June-2003

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
  struct /*UF_CONVERSIONS_EXPORT*/ dm2str
  {
    //! degrees and minutes to string.
    /*!
        @param sgn : the sign
        @param d : degrees
        @param m : minutes
        @param prec : precision
     */
    dm2str(char sgn = '+', int d = 0, double m = 0, int prec = 5)
      : _d((sgn == '-')? -d : d), _m(m), _prec(prec)
    {}

    //! Return the string corresponding to the degrees and minutes.
    std::string operator () ()
    {
      std::ostringstream os;
      os << std::setw(3) << _d << " ";
      os.flags(std::ios::left);
      if ( _m < 10 )
        os << " " << std::setw(_prec+3) << std::setprecision(5) << _m;
      else
        os << std::setw(_prec+4) << std::setprecision(_prec) << _m;

      return os.str();
    }

  private:
    int _d;
    double _m;
    int _prec;
  };


  //! Convert hours minutes and seconds to a string
  /*!
    Take three numbers representing the hours minutes and seconds
    and return a nicely formatted string.
    You can also set the precision of the seconds.

    Usage:
    \code
    std::string s = hms2str(hours, minutes, seconds)();
    \endcode

    \version 1.0

    \date 23-June-2003

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
  struct /*UF_CONVERSIONS_EXPORT*/ hms2str
  {
    //! hours minutes and seconds to string.
    /*!
      @param h : hours
      @param m : minutes
      @param s : seconds
      @param prec : precision
    */
    hms2str (int h = 0, int m = 0, double s = 0, int prec = 6)
      : _h(h), _m(m), _s(s), _prec(prec)
    {}

    //! Return the string corresponding to the hours minutes and seconds.
    std::string operator () ()
    {
      std::ostringstream os;
      os << std::setw(2) << _h << " " << std::setw(2) << _m << " ";
      os.flags(std::ios::left);
      if ( _s < 10 )
        os << " " << std::setw(_prec+3) << std::setprecision(_prec)  << _s;
      else
        os << std::setw(_prec+4) << std::setprecision(_prec) << _s;

      return os.str();
    }

  private:
    int _h;
    int _m;
    double _s;
    int _prec;
  };

  //! Convert hours and minutes to a string
  /*!
    Take three numbers representing the hours, minutes and
    decimals of a minute and return a nicely formatted string.
    You can also set the precision of the minutes.

    Usage:
    \code
    std::string s = hms2str(hours, minutes, seconds)();
    \endcode

    \version 1.0

    \date 23-June-2003

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
  struct /*UF_CONVERSIONS_EXPORT*/ hm2str
  {
    //! hours and minutes to string.
    /*!
     * @param h : hours
     * @param m : minutes
     * @param prec : precision
     */
    hm2str (int h = 0, double m = 0, int prec = 6)
      : _h(h), _m(m), _prec(prec)
    {}

    //! Return the string corresponding to the hours minutes and seconds.
    std::string operator () ()
    {
      std::ostringstream os;
      os << std::setw(2) << _h << " " << std::setw(2) << " ";
      os.flags(std::ios::left);
      if ( _m < 10 )
        os << " " << std::setw(_prec+3) << std::setprecision(6) << _m;
      else
        os << std::setw(_prec+4) << std::setprecision(_prec) << _m;

      return os.str();
    }

  private:
    int _h;
    double _m;
    int _prec;
  };

  //! Convert the Right Ascension to a string
  /*!
    Take three numbers representing the hours minutes and seconds
    of the Right Ascension and return a nicely formatted string.
    You can also set the precision of the seconds.

    Usage:
    \code
    std::string s = RAStr(hours, minutes, seconds)();
    \endcode

    \version 1.0

    \date 23-June-2003

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
  struct /*UF_CONVERSIONS_EXPORT*/ RAStr
  {
    //! Right Ascension to string.
    /*!
     * @param h : hours
     * @param m : minutes
     * @param s : seconds
     * @param prec : precision
     */
    RAStr (int h = 0, int m = 0, double s = 0, int prec = 6)
      : _h(h), _m(m), _s(s), _prec(prec)
    {}

    //! Return the string corresponding to the Right Ascension.
    std::string operator () ()
    {
      std::ostringstream os;
      os << std::setw(2) << _h << " " << std::setw(2) << _m << " ";
      os.flags(std::ios::left);
      if ( _s < 10 )
        os << " " << std::setw(_prec+3) << std::setprecision(_prec)  << _s;
      else
        os << std::setw(_prec+4) << std::setprecision(_prec) << _s;

      return os.str();
    }

  private:
    int _h;
    int _m;
    double _s;
    int _prec;
  };

  //! Convert the Declination to a string
  /*!
    Take the sign and three numbers representing the degrees minutes and seconds
    of the declination and return a nicely formatted string. You can also set the
    precision of the seconds. Also use this for angles in the range +/- 90 degrees.
    If sign is blank the angle is positive otherwise the angle is negative.


    Usage:
    \code
    std::string s = DecStr(sign, degrees, minutes, seconds)();
    \endcode

    \version 1.0

    \date 23-June-2003

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
  struct /*UF_CONVERSIONS_EXPORT*/ DecStr
  {
    //! Declination to string.
    /*!
     * @param sgn : the sign
     * @param d : degrees
     * @param m : minutes
     * @param s : seconds
     * @param prec : precision
     */
    DecStr(char sgn = '+', int d = 0, int m = 0, double s = 0, int prec = 5)
      : _d((sgn == '-')? -d : d), _m(m), _s(s), _prec(prec)
    {}

    //! Return the string corresponding to the declination.
    std::string operator () ()
    {
      std::ostringstream os;
      os << std::setw(3) << _d << " " << std::setw(2)  << _m << " ";
      os.flags(std::ios::left);
      if ( _s < 10 )
        os << " " << std::setw(_prec+3) << std::setprecision(_prec) << _s;
      else
        os << std::setw(_prec+4) << std::setprecision(_prec) << _s;

      return os.str();
    }

  private:
    int _d;
    int _m;
    double _s;
    int _prec;
  };

  //! Convert the Longitude to a string
  /*!
    Take the sign and three numbers representing the degrees minutes and seconds
    of the longitude and return a nicely formatted string. You can also set the
    precision of the seconds. Also use this for angles in the range +/- 180 or 0-360 degrees.
    If sign is blank the angle is positive otherwise the angle is negative.

    Usage:
    \code
    std::string s = LongStr(sign, degrees, minutes, seconds)();
    \endcode

    \version 1.0

    \date 23-June-2003

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
  struct /*UF_CONVERSIONS_EXPORT*/ LongStr
  {
    //! Longitude to string.
    /*!
     * @param sgn : the sign
     * @param d : degrees
     * @param m : minutes
     * @param s : seconds
     * @param prec : precision
     */
    LongStr(char sgn = '+', int d = 0, int m = 0, double s = 0, int prec = 5)
      : _d((sgn == '-')? -d : d), _m(m), _s(s), _prec(prec)
    {}

    //! Return the string corresponding to the longitude.
    std::string operator () ()
    {
      std::ostringstream os;
      os << std::setw(4) << _d << " " << std::setw(2)  << _m << " ";
      os.flags(std::ios::left);
      if ( _s < 10 )
        os << " " << std::setw(_prec+3) << std::setprecision(_prec) << _s;
      else
        os << std::setw(_prec+4) << std::setprecision(_prec) << _s;

      return os.str();
    }

  private:
    int _d;
    int _m;
    double _s;
    int _prec;
  };

  //! Convert the Latitude to a string
  /*!
    Take the sign and three numbers representing the degrees minutes and seconds
    of the latitude and return a nicely formatted string. You can also set the
    precision of the seconds.  Also use this for angles in the range +/- 90 degrees.
    If sign is blank the angle is positive otherwise the angle is negative.

    Usage:
    \code
    std::string s = LatStr(sign, degrees, minutes, seconds)();
    \endcode

    \version 1.0

    \date 23-June-2003

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
  struct /*UF_CONVERSIONS_EXPORT*/ LatStr
  {
    //! Latitude to string.
    /*!
     * @param sgn : the sign
     * @param d : degrees
     * @param m : minutes
     * @param s : seconds
     * @param prec : precision
     */
    LatStr(char sgn = '+', int d = 0, int m = 0, double s = 0, int prec = 5)
      : _d((sgn == '-')? -d : d), _m(m), _s(s), _prec(prec)
    {}

    //! Return the string corresponding to the latitude.
    std::string operator () ()
    {
      std::ostringstream os;
      os << std::setw(3) << _d << " " << std::setw(2)  << _m << " ";
      os.flags(std::ios::left);
      if ( _s < 10 )
        os << " " << std::setw(_prec+3) << std::setprecision(_prec) << _s;
      else
        os << std::setw(_prec+4) << std::setprecision(_prec) << _s;

      return os.str();
    }

  private:
    int _d;
    int _m;
    double _s;
    int _prec;
  };

  //! Convert the Longitude to a string
  /*!
    Take the sign and two numbers representing the degrees and minutes
    of the longitude and return a nicely formatted string. You can also set the
    precision of the minutes.
    If sign is blank the angle is positive otherwise the angle is negative.

    Usage:
    \code
    std::string s = Long_mStr(sign, degrees, minutes)();
    \endcode

    \version 1.0

    \date 23-June-2003

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
  struct /*UF_CONVERSIONS_EXPORT*/ Long_mStr
  {
    //! Longitude to string.
    /*!
     * @param sgn : the sign
     * @param d : degrees
     * @param m : minutes
     * @param prec : precision
     */
    Long_mStr(char sgn = '+', int d = 0, double m = 0, int prec = 5)
      : _d((sgn == '-')? -d : d), _m(m), _prec(prec)
    {}

    //! Return the string corresponding to the longitude.
    std::string operator () ()
    {
      std::ostringstream os;
      os << std::setw(4) << _d << " ";
      os.flags(std::ios::left);
      if ( _m < 10 )
        os << " " << std::setw(_prec+3) << std::setprecision(_prec) << _m;
      else
        os << std::setw(_prec+4) << std::setprecision(_prec) << _m;

      return os.str();
    }

  private:
    int _d;
    double _m;
    int _prec;
  };

  //! Convert the Latitude to a string
  /*!
    Take the sign and two numbers representing the degrees and minutes
    of the latitude and return a nicely formatted string. You can also set the
    precision of the minutes.  Also use this for angles in the range +/- 90 degrees.
    If sign is blank the angle is positive otherwise the angle is negative.

    Usage:
    \code
    std::string s = Lat_mStr(sign, degrees, minutes)();
    \endcode

    \version 1.0

    \date 23-June-2003

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
  struct /*UF_CONVERSIONS_EXPORT*/ Lat_mStr
  {
    //! Latitude to string.
    /*!
     * @param sgn : the sign
     * @param d : degrees
     * @param m : minutes
     * @param prec : precision
     */
    Lat_mStr(char sgn = '+', int d = 0, double m = 0, int prec = 5)
      : _d((sgn == '-')? -d : d), _m(m), _prec(prec)
    {}

    //! Return the string corresponding to the latitude.
    std::string operator () ()
    {
      std::ostringstream os;
      os << std::setw(3) << _d << " ";
      os.flags(std::ios::left);
      if ( _m < 10 )
        os << " " << std::setw(_prec+3) << std::setprecision(5) << _m;
      else
        os << std::setw(_prec+4) << std::setprecision(_prec) << _m;

      return os.str();
    }

  private:
    int _d;
    double _m;
    int _prec;
  };

  //! Convert degrees minutes and seconds to degrees.
  /*!
    Converts degrees and minutes or degrees minutes and seconds to decimal degrees.
    If sign is blank the angle is positive otherwise the angle is negative.

    Usage:
    \code
    double angle = dms2d()(sign, degrees, minutes, seconds);
    \endcode

    \version 1.0

    \date 23-June-2003

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
  struct /*UF_CONVERSIONS_EXPORT*/ dms2d
  {
    //! Degrees minutes seconds to degrees and decimals of a degree.
    /*!
     * @param sign : the sign
     * @param degrees : degrees
     * @param minutes : minutes
     * @param seconds : seconds
     *
     * @return double  : the angle in degrees and decimals of a degree
     */
    double operator()( char const sign = ' ', int degrees = 0, double minutes = 0, double seconds = 0 ) const
    {

      double angle = ( std::abs(seconds) * 60.0 + std::abs(minutes) ) * 60.0 + std::abs(degrees);

      if ( sign == ' ' )
        return angle;
      else
        return -angle;
    }

  };

  //! Convert hours minutes and seconds to degrees.
  /*!
    Converts hours and minutes or hours minutes and seconds to decimal degrees.
    Hours vary from 0 to 23.

    Usage:
    \code
    double angle = hms2d()(hours, minutes, seconds);
    \endcode

    \version 1.0

    \date 23-June-2003

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
  struct /*UF_CONVERSIONS_EXPORT*/ hms2d
  {
    //! Hours minutes seconds to degrees and decimals of a degree.
    /*!
     * @param hours : hours
     * @param minutes : minutes
     * @param seconds : seconds
     *
     * @return double  : the angle in degrees and decimals of a degree
     */
    double operator()( int hours = 0, double minutes = 0, double seconds = 0 ) const
    {

      return dms2d()( '+', hours * 24, minutes, seconds );

    }

  };

  //! Convert latitudes in the format [ddmm.mmmm][N|S] to decimals of a degree.
  /*!
    Convert latitudes in the format [ddmm.mmmm][N|S] to decimals of a degree.

    Usage:
    \code
    double latitude = GPSLat2Lat()(GPSlatitude, GPSlatitudeSector);
    \endcode

    \version 1.0

    \date 03-May-2004

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
  struct /*UF_CONVERSIONS_EXPORT*/ GPSLat2Lat
  {

    //! Convert [ddmm.mmmm][N|S] to d.dddd, North latitudes are positive.
    /*!
     * @param latitude : positive latitude in the form ddmm.mmm.
     * @param latitudeSector : the sector, 'N' (north) or 'S' (south).
     *
     * @return double  : the latitude in degrees and decimals of a degree. North latitude is positive.
     */
    double operator()(double const & latitude, char const latitudeSector)
    {
      double d_lat = (int)(latitude/100.0);
      d_lat = d_lat + (latitude - d_lat * 100.0)/60.0;

      if ( latitudeSector == 'N' || latitudeSector == 'n' )
        return d_lat;
      return -d_lat;
    }

  };

  //! Convert latitudes in the format decimals of a degree to [ddmm.mmmm][N|S].
  /*!
    Convert latitudes in the format decimals of a degree to [ddmm.mmmm][N|S].

    Usage:
    \code
    double GPSlatitude;
    char GPSlatitudeSector;
    double latitude;
    // ...
    Lat2GPSLat()(latitude, GPSlatitude, GPSlatitudeSector);
    \endcode

    \version 1.0

    \date 03-May-2004

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
  struct /*UF_CONVERSIONS_EXPORT*/ Lat2GPSLat
  {

    //! Extract the latitude in [ddmm.mm][N|S] format from the dd.d format. North is positive.
    /*!
      @param ilatitude - the latitude to be converted.
      @param latitude in ddmm.m.
      @param latitudeSector - latitude sector either 'N' or 'S'.
    */
    void operator()(double const & ilatitude, double & latitude, char & latitudeSector)
    {
      double lat = std::abs(ilatitude);
      int ilat = (int)lat;
      lat = (lat - ilat)*60.0;
      latitude = ilat * 100 + lat;
      if (ilatitude < 0)
        latitudeSector = 'S';
      else
        latitudeSector = 'N';
    }

  };


  //! Convert logitudes in the format [dddmm.mmmm][E|W] to decimals of a degree.
  /*!
    Convert logitudes in the format [dddmm.mmmm][E|W] to decimals of a degree.

    Usage:
    \code
    double longitude = GPSLong2Dec()(GPSlongitude, GPSlongitudeSector);
    \endcode

    \version 1.0

    \date 03-May-2004

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
  struct /*UF_CONVERSIONS_EXPORT*/ GPSLong2Long
  {
    //! Convert dddmm.m to d.dddd, East longitudes are positive.
    /*!
     * @param longitude : positive longitude in the form dddmm.mmm.
     * @param longitudeSector : the sector, 'E' (east) or 'W' (west).
     *
     * @return double  : the longititude in degrees and decimals of a degree. East longitude is positive.
     */
    double operator() (double const & longitude, char const longitudeSector)
    {
      double d_long = (int)(longitude/100.0);
      d_long = d_long + (longitude - d_long * 100)/60.0;

      if ( longitudeSector == 'E' || longitudeSector == 'e' )
        return d_long;
      return -d_long;
    }
  };

  //! Convert longitudes in the format decimals of a degree to [dddmm.mmmm][E|W].
  /*!
    Convert longitudes in the format decimals of a degree to [dddmm.mmmm][E|W].

    Usage:
    \code
    double GPSlongitude;
    char GPSlongitudeSector;
    double longitude;
    // ...
    Long2GPSLong()(longitude, GPSlongitude, GPSlongitudeSector);
    \endcode

    \version 1.0

    \date 03-May-2004

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
  struct /*UF_CONVERSIONS_EXPORT*/ Long2GPSLong
  {

    //! Extract the longitude in [dddmm.mmmm][E|W] format from the dd.d format. East is positive.
    /*!
      @param ilongitude - the longitude to be converted.
      @param longitude - positive in ddmm.m.
      @param longitudeSector - longitude sector either 'E' or 'W'.
    */
    void operator()(double const & ilongitude, double & longitude, char & longitudeSector)
    {
      double dlong = std::abs(ilongitude);
      int ilong = (int)dlong;
      dlong = (dlong - ilong)*60.0;
      longitude = ilong * 100 + dlong;
      if (ilongitude < 0)
        longitudeSector = 'W';
      else
        longitudeSector = 'E';
    }

  };

  //! A class for manipulating angles.
  /*!
    This is a class that manipulates angles. It is a convenience class providing
    ready conversion between various angle formats.

    \version 1.0

    \date 23-June-2003

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
  class /*UF_CONVERSIONS_EXPORT*/ Angle
  {
  public:

    //! Default constructor.
    Angle ( void ):
    _sign('+'), _degrees(0), _minutes(0), _seconds(0)
    {};

    //! Constructor for degrees minutes and seconds.
    /*!
     * @param degrees : degrees
     * @param minutes : minutes
     * @param seconds : seconds
     */
    Angle ( int const& degrees, double const& minutes = 0, double const& seconds = 0 )
    {
      dms( degrees, minutes, seconds);
    };

    //! Constructor for degrees minutes and seconds.
    /*!
     * @param sign : sign
     * @param degrees : degrees
     * @param minutes : minutes
     * @param seconds : seconds
     */
    Angle ( char const& sign, int const& degrees, double const& minutes = 0, double const& seconds = 0 )
    {
      dms( sign, degrees, minutes, seconds);
    };

    //! Constructor for degrees.
    /*!
     * @param degrees : degrees
     */
    Angle ( double const& degrees )
    {
      d(degrees);
    };

    //! Constructor for degrees.
    /*!
     * @param sign : sign
     * @param degrees : degrees
     */
    Angle ( char const& sign, double const& degrees )
    {
      d(sign,degrees);
    };

    ~Angle(){};

    //! Get the value in degrees.
    /*!
     * @return double  : the angle in degrees and decimals of a degree
     */
    double get_d ( void )
    {
      double temp = ( _seconds / 60.0 + _minutes ) / 60.0 + _degrees;
      if ( _sign == '-' )
        return -temp;
      else
        return temp;
    }

    //! Get the value in degrees.
    /*!
     * @param d  : the angle in degrees and decimals of a degree
     */
    void d ( double& d )
    {
      d = ( _seconds / 60.0 + _minutes ) / 60.0 + _degrees;
      if ( _sign == '-' )
        d = -d;
    }

    //! Set the value in degrees.
    /*!
     * @param degrees : degrees
     */
    void d ( double const& degrees )
    {
      if ( degrees >= 0 )
        _sign = '+';
      else
        _sign = '-';
      _degrees = (int) std::abs(degrees);
      _minutes = ( std::abs(degrees) - _degrees ) * 60;
      _seconds = ( _minutes - int(_minutes) ) * 60;
      _minutes = (int) _minutes;
    }

    //! Set the value in degrees.
    /*!
     * @param sign : sign
     * @param degrees : degrees
     */
    void d ( char const& sign = '+', double const& degrees = 0 )
    {
      if ( sign == '-' )
        _sign = sign;
      else
        _sign = '+';
      _degrees = (int) std::abs(degrees);
      _minutes = ( std::abs(degrees) - _degrees ) * 60;
      _seconds = ( _minutes - int(_minutes) ) * 60;
      _minutes = (int) _minutes;
    }

    //! Set the value in degrees, minutes and seconds.
    /*!
     * @param degrees : degrees
     * @param minutes : minutes
     * @param seconds : seconds
     */
    void dms ( int const& degrees = 0, double const& minutes = 0, double const& seconds = 0 )
    {
      if ( degrees >= 0 )
        _sign = '+';
      else
        _sign = '-';
      _degrees = std::abs(degrees);
      _minutes = std::abs(minutes);
      _seconds = std::abs(seconds) + ( _minutes - (int) _minutes ) * 60.0;
      _minutes = (int) _minutes;
      while ( _seconds >= 60 )
      {
        _seconds -= 60;
        _minutes++;
      }
      while ( _minutes >= 60 )
      {
        _minutes -= 60;
        _degrees++;
      }
    }

    //! Set the value in degrees, minutes and seconds.
    /*!
     * @param sign : sign
     * @param degrees : degrees
     * @param minutes : minutes
     * @param seconds : seconds
     */
    void dms ( char const& sign = '+', int const& degrees = 0, double const& minutes = 0, double const& seconds = 0 )
    {
      if ( sign == '-' )
        _sign = sign;
      else
        _sign = '+';
      _degrees = std::abs(degrees);
      _minutes = std::abs(minutes);
      _seconds = std::abs(seconds) + ( _minutes - (int) _minutes ) * 60.0;
      _minutes = (int) _minutes;
      while ( _seconds >= 60 )
      {
        _seconds -= 60;
        _minutes++;
      }
      while ( _minutes >= 60 )
      {
        _minutes -= 60;
        _degrees++;
      }
    }

    //! Get the value in degrees, minutes and seconds.
    /*!
     * @param degrees : degrees
     * @param minutes : minutes
     * @param seconds : seconds
     */
    void dms ( int &degrees, double &minutes, double &seconds )
    {
      if ( _sign == '-' )
        degrees = -_degrees;
      else
        degrees = _degrees;
      minutes = _minutes;
      seconds = _seconds;
    }

    //! Get the value in degrees, minutes and seconds.
    /*!
     * @param sign : sign
     * @param degrees : degrees
     * @param minutes : minutes
     * @param seconds : seconds
     */
    void dms ( char &sign, int &degrees, double &minutes, double &seconds )
    {
      sign = _sign;
      degrees = _degrees;
      minutes = _minutes;
      seconds = _seconds;
    }

    //! Get the value in degrees and minutes.
    /*!
     * @param degrees : degrees
     * @param minutes : minutes
     */
    void dm ( int &degrees, double &minutes )
    {
      if ( _sign == '-' )
        degrees = -_degrees;
      else
        degrees = _degrees;
      minutes = _minutes + _seconds / 60.0;
    }

    //! Get the value in degrees and minutes.
    /*!
     * @param sign : sign
     * @param degrees : degrees
     * @param minutes : minutes
     */
    void dm ( char &sign, int &degrees, double &minutes )
    {
      sign = _sign;
      degrees = _degrees;
      minutes = _minutes + _seconds / 60.0;
    }

  private:
    char _sign;
    int _degrees;
    double _minutes;
    double _seconds;
  };

  //! Convert a date to a string and return the string.
  struct /*UF_CONVERSIONS_EXPORT*/ date2s
  {
      //! Convert a date to a string and return the string.
      /*!
      Usage:
      \code
      std::string s = date2s()(year,month,day);
      \endcode

      No checking is done for valid dates.
      @param year : the year
      @param month : the month (1..12)
      @param day : the day (1..31), fractions of a day are allowed.
      @param precision: precision of the day
      @param separator: The separator, if it is zero it indicates that none is used.

      @return std::string : a string corresponding to a formatted date (yyyymmdd)

      \version 3.0

      \date 20-Feb-2006

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
    std::string operator()( int year, int month, double day, int precision = 0, char separator = 0 ) const
    {
      std::ostringstream os;

      bool sgn = false;
      if (year < 0 || month < 0 || day < 0)
        sgn = true;

      int y = std::abs(year);
      int m = std::abs(month);
      double p10 = std::pow(10.0,precision);
      double d = int(std::abs(day)*p10+0.5)/p10;

      if ( sgn )
        os << "-";
      else
        os << " ";

      if ( separator != 0 )
        os << std::fixed << std::setw(2) << std::setfill('0') << y << separator
        << std::fixed << std::setw(2) << std::setfill('0') << m << separator;
     else
      {
        os << std::fixed << std::setw(2) << std::setfill('0') << y
          << std::fixed << std::setw(2) << std::setfill('0') << m;
      }
      if ( precision == 0 )
        os << std::fixed << std::setw(2+precision) << std::setfill('0') << std::setprecision(precision) << d;
      else
        os << std::fixed << std::setw(3+precision) << std::setfill('0') << std::setprecision(precision) << d;

      return os.str();
    }
  };


  //! Convert a time to a string and return the time as a string.
  struct /*UF_CONVERSIONS_EXPORT*/ time2s
  {
    //! Convert a time to a string and return the time as a string.
    /*!
      Usage:
      \code
      std::string s = time2s()(hour,minute,second);
      \endcode

      No checking is done for valid times.

      @param hour : the hour (0..59)
      @param minute : the minute (0..59)
      @param second : the second (0..59)
      @param precision: precision of the seconds
      @param separator: The separator, if it is zero it indicates that none is used.

      @return std::string : a string corresponding to a formatted time (hhmmss.sss)

      \version 3.0

      \date 20-Feb-2006

      \author Andrew Maclean

      \par license
      This software is distributed WITHOUT ANY WARRANTY; without even the
      implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
      YOU, as the USER, take SOLE RESPONSIBILITY for, and are responsible for
      determining the FITNESS and APPLICABILITY of this software for a particular
      purpose. The author or any other person or entity are not liable in any way
      for the use or misuse of any program or software using this software.

      \todo
    */
    std::string operator()( int hour, int minute, double second, int precision = 3, char separator = 0) const
    {
      std::ostringstream os;

      bool sgn = false;
      if (hour < 0 || minute < 0 || second < 0)
        sgn = true;

      int h = std::abs(hour);
      int m = std::abs(minute);
      double p10 = std::pow(10.0,precision);
      double s = int(std::abs(second)*p10+0.5)/p10;

      if ( sgn )
        os << "-";
      else
        os << " ";

      if ( separator != 0 )
      {
      os << std::fixed << std::setw(2) << std::setfill('0') << h << separator
        << std::fixed << std::setw(2) << std::setfill('0') << m << separator;
      }
      else
      {
      os << std::fixed << std::setw(2) << std::setfill('0') << h
        << std::fixed << std::setw(2) << std::setfill('0') << m;
      }
      if (precision == 0)
        os << std::fixed << std::setw(2+precision) << std::setfill('0') << std::setprecision(precision) << s;
      else
        os << std::fixed << std::setw(3+precision) << std::setfill('0') << std::setprecision(precision) << s;

      return os.str();
    }
  };

} // AngleNum

} // namespace UF
 
#endif // ANGLENUMANGLEDATETIMECONVERSIONS_H
