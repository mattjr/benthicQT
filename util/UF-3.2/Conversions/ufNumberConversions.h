 
#if !defined(NUMCONVNUMBERCONVERSIONS_H)
#define NUMCONVNUMBERCONVERSIONS_H

#include <string>
#include <sstream>
#include <iomanip>
#include <limits>

#include "ufWin32Header.h"


#ifdef max
#undef max
#endif

namespace UF{

//! Converts a string to a number and vice versa.
namespace NumConv {

  //! Extract a number from a string and return the number.
  /*!
    If a number is not extracted, return the maximum value for that type.

    Usage:
    \code
    // Assume s is a string containing a number.
    double n = s2n<double>()(s);
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
  template < typename T >
  struct /*UF_CONVERSIONS_EXPORT*/ s2n
  {
    //! String to number.
    /*!
     * @param s : the string containing the number
     *
     * @return T : a number
     */
    T operator()( std::string const &s ) const
    {
      std::istringstream is;

      is.str(s);
      T x;

      is >> x;
      if ( is )
        return x;
      else
        return ( std::numeric_limits<T>::max() );
    }
  };

  //! Convert a number to a string and return the string.
  /*!
    If a number is at its maximum value, return blanks.

    Usage:
    \code
      double x = atan(1.0)*4.0;
      std::cout << "Pi is: " << NumConv::n2s<double>()(x,12,7,'*',std::ios::left|std::ios::fixed) << std::endl;
      std::cout << "Pi is: " << NumConv::n2s<double>()(x,12,7,' ',std::ios::right|std::ios::scientific) << std::endl;

    \endcode

    \version 2.0


    \date 11-Nov-2003

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
  template < typename T >
  struct /*UF_CONVERSIONS_EXPORT*/ n2s
  {
    //! Number to string.
    /*!
      @param n : the number.
      @param precision : precision.
      @param width : width.
      @param fill : fill character, usually blank, but can be any character such as 0.
      @param format : format,  series of flags, specifying formatting, usually std::ios.right | std::ios.fixed.

      @return std::string : a string corresponding to a formatted number

      The distinct flag values (elements) are:

      - boolalpha, to insert or extract objects of type bool as names (such as true and false) rather than as numeric values.
      - dec, to insert or extract integer values in decimal format.
      - fixed, to insert floating-point values in fixed-point format (with no exponent field).
      - hex, to insert or extract integer values in hexadecimal format.
      - internal, to pad to a field width as needed by inserting fill characters at a point internal to a generated numeric field.
      - left, to pad to a field width as needed by inserting fill characters at the end of a generated field (left justification).
      - oct, to insert or extract integer values in octal format.
      - right, to pad to a field width as needed by inserting fill characters at the beginning of a generated field (right justification).
      - scientific, to insert floating-point values in scientific format (with an exponent field).
      - showbase, to insert a prefix that reveals the base of a generated integer field.
      - showpoint, to insert a decimal point unconditionally in a generated floating-point field.
      - showpos, to insert a plus sign in a nonnegative generated numeric field.
      - skipws, to skip leading white space before certain extractions.
      - unitbuf, to flush output after each insertion.
      - uppercase, to insert uppercase equivalents of lowercase letters in certain insertions.

      In addition, several useful values are:

      - adjustfield, where internal | left | right
      - basefield, where dec | hex | oct
      - floatfield, where fixed | scientific

      \version 2.0


      \date 11-November-2003

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
    std::string operator()( T const &n, int precision = 6, int width = 1,
      char fill = ' ',
      std::ios_base::fmtflags format = std::ios_base::right | std::ios_base::fixed /*std::ios.right | std::ios.fixed*/ ) const
    {
      std::ostringstream os;

      if ( std::numeric_limits<T>::max() != n )
      {
        std::ios_base::fmtflags oldflgs = os.flags(format);
        os << std::setw(width) << std::setfill(fill) << std::setprecision(precision) << n;
        os.flags(oldflgs);
      }
      else
        os << std::setw(width) << fill;
      return os.str();
    }
  };

} // NumConv

} // UF

#endif // NUMCONVNUMBERCONVERSIONS_H
