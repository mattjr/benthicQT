//#pragma once

// Thanks to Kitware for these macros. (vtkSetGet.h)

#if !defined(UFSETGETMACROS_H)
#define UFSETGETMACROS_H

//! Set built-in type.  Creates member Set"name"() (e.g., SetLongitude());
#define SetMacro(name,type) \
virtual void Set##name (type _arg) \
  { \
  if (this->name != _arg) \
    { \
    this->name = _arg; \
    } \
  }

/*
//! Get built-in type.  Creates member Get"name"() (e.g., GetLongitude());
#define GetMacro(name,type) \
virtual type Get##name () { \
  return this->name; \
  }

*/

//! Get built-in type.  Creates member Get"name"() (e.g., GetLongitude());
#define GetMacro(name,type) \
virtual type Get##name () const { \
  return this->name; \
  }


/*
//! Get built-in type.  Creates member Get"name"() (e.g., GetLongitude());
#define GetMacro(name,type) \
virtual type Get##name () { \
  return this->name; \
  } \
  \
virtual type Get##name () const { \
  return this->name; \
  }
*/

//! Set built-in type where value is constrained between min/max limits.
/*!
  Create member Set"name"() (eg., SetRadius()). #defines are a
  convenience for clamping open-ended values.
  The Get"name"MinValue() and Get"name"MaxValue() members return the
  min and max limits.
*/
#define SetClampMacro(name,type,min,max) \
virtual void Set##name (type _arg) \
  { \
  if (this->name != (_arg<min?min:(_arg>max?max:_arg))) \
    { \
    this->name = (_arg<min?min:(_arg>max?max:_arg)); \
    } \
  } \
virtual type Get##name##MinValue () \
  { \
  return min; \
  } \
virtual type Get##name##MaxValue () \
  { \
  return max; \
  }


//! Create members "name"On() and "name"Off() (e.g., DebugOn() DebugOff()).
//
/*!
  Set method must be defined to use this macro.
*/
#define BooleanMacro(name,type) \
virtual void name##On () { this->Set##name((type)1);}; \
virtual void name##Off () { this->Set##name((type)0);}

#endif // UFSETGETMACROS_H
