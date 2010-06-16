#if !defined(PACKUNPACKPACKUNPACK_H)
#define PACKUNPACKPACKUNPACK_H

#include <cmath>

#include "ufWin32Header.h"

namespace UF{

//! Pack or unpack doubles into or from integer types.
namespace PackUnpack
{
  //! Pack a value into 4 bytes (assumes unsigned int is 4 bytes).
  struct /*UF_CONVERSIONS_EXPORT*/ Pack4Bytes
  {
    unsigned int operator() (double const & Value , double const & Scale = 1 , double const & Origin = 0)
    {
      unsigned int v = (unsigned int)(std::abs(Value - Origin) * Scale);
      if ( (Value - Origin) < 0 )
        v |= 0x80000000;
      return v;
    }
  };

  //! Unpack a value from 4 bytes (assumes unsigned int is 4 bytes).
  struct /*UF_CONVERSIONS_EXPORT*/ Unpack4Bytes
  {
    double operator() (unsigned int const & Value , double const & Scale = 1 , double const & Origin = 0)
    {
      double v = ( double(Value & 0x7fffffff) + Origin) / Scale;
      if ( Value >> 31 )
        v = -v;
      return v;
    }
  };

  //! Pack a value into 2 bytes (assumes unsigned short is 2 bytes).
  struct /*UF_CONVERSIONS_EXPORT*/ Pack2Bytes
  {
    unsigned short operator() (double const & Value , double const & Scale = 1 , double const & Origin = 0)
    {
      unsigned short v = (unsigned short)(std::abs(Value - Origin) * Scale);
      if ( (Value - Origin) < 0 )
        v |= 0x8000;
      return v;
    }
  };

  //! Unpack a value from 2 bytes (assumes unsigned short is 2 bytes).
  struct /*UF_CONVERSIONS_EXPORT*/ Unpack2Bytes
  {
    double operator() (unsigned short const & Value , double const & Scale = 1, double const & Origin = 0)
    {
      double v = ( double(Value & 0x7fff) + Origin) / Scale;
      if ( Value >> 15 )
        v = -v;
      return v;
    }
  };
} // PackUnpack 

} // UF 

#endif // PACKUNPACKPACKUNPACK_H

