#if !defined(NUMSTRNUMBERSTRINGCONVERSIONS_H)
#define NUMSTRNUMBERSTRINGCONVERSIONS_H

#include <string>
#include <algorithm>

#include "ufWin32Header.h"

namespace UF{

//! Performs number to byte conversions.
namespace NumStr {

  //! Performs number to byte conversions.
  /*!
  CLASS
    CNumStr

    To perform endian conversions, reverse the string before passing it
    to the str function.

    \version 1.2

    \date 15-Aug-2004

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
  template <typename T >
  class /*UF_CONVERSIONS_EXPORT*/ CNumStr
  {
    union number_string_union
    {
      // The numeric value
      T _num;
      // The ASCII representation of the value
      char _str[sizeof(T)];
    } _value;

  public:
    //! Constructor
    CNumStr( void ) { _value._num = 0; }
    //! Constructor
    CNumStr( T const n ) { _value._num = n; }
    //! Constructor
    CNumStr( std::string const & s ) { str(s); }

    //! Copy constructor
    inline CNumStr ( CNumStr const &rhs )
    {
      _value._num = rhs._value._num;
    }
    ~CNumStr(void) { }

    //! Assignment
    inline  CNumStr &operator = ( CNumStr const &rhs )
      {
        if ( this == &rhs )
          return *this;

        _value = rhs._value;
        return *this;
      }

    //! Assigment
    inline  CNumStr &operator = ( T const &rhs )
      {
        //if ( this == &rhs )
        //  return *this;

        _value._num = rhs;
        return *this;
      }

    //! Return the numeric value
    inline T num(void) const { return _value._num; }
    //! Set the numeric value
    inline void num(T const n) { _value._num = n; }


    //! Return a string representing the value
    inline std::string str(void) const
    {
      std::string s;

      for ( unsigned int i = 0; i < sizeof(T); ++i )
        s += _value._str[i];
      return s;
    }

    //! Set a value from a string representing the value
    inline void str(std::string const &s)
    {
      if ( s.size() != sizeof(T) ) // Do nothing
        return;

      int i = 0;
      for ( std::string::const_iterator p = s.begin(); p != s.end(); ++p)
      {
        _value._str[i] = *p;
        ++i;
      }
    }

    //! Reverse the bytes. Often used to change the endianess.
    inline void reverse ( void )
    {
      std::string s = this->str();
      std::reverse(s.begin(),s.end());
      this->str(s);
    }


  };

} // NumStr

} // UF 

#endif
