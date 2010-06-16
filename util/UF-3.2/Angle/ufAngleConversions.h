
#ifdef _MSC_VER
#pragma once
#endif

//#pragma once
#if !defined(ANGLEANGLECONVERSIONS_H)
#define ANGLEANGLECONVERSIONS_H

#include <stdexcept>

#include "ufWin32Header.h"

namespace UF {

namespace Angle {

  //! Angle Conversions.
  /*!
  CLASS
    AngleConversions


    Introduction

    This singleton class facliliates the conversion of angles to/from
    various formats.

    Note that UF::DateTime::TimeConversions also has conversions that are applicable to angles.

    \version 1.0

    \date 12-August-2008

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
 class UF_ANGLE_EXPORT AngleConversions
 {
  public:
    //! Unique point of access for the singleton.
    static AngleConversions* Instance()
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
      static AngleConversions theInstance;
      pInstance_ = &theInstance;
    }
    //! Gets called if a dead reference is detected.
    static void OnDeadReference()
    {
      throw std::runtime_error("Dead Reference Error in AngleConversions.");
    }
    //! Private constructor, disables creation of the object by clients.
    AngleConversions(void){};
    AngleConversions( const AngleConversions & ); // Disable the copy constructor.
    AngleConversions& operator = ( const AngleConversions & ); // Disable self-assignment.
    // Make the destructor private in order to prevent clients
    // holding a pointer from accidentally deleting it.
    virtual ~AngleConversions(void)
    {
      pInstance_ = 0;
      destroyed_ = true;
    };

    //! Pointer to the instance.
    static AngleConversions * pInstance_;
    //! True if destroyed.
    static bool destroyed_;

  public:

    //! Degrees to semicircles. 
    /*!
     * Note that 2^31 semicircles is equal to 180 degrees.
     * North latitudes and East longitudes are indicated with positive numbers.
     * South latitudes and West longitudes are indicated with negative numbers.
     * This representation has a precision of around 1 millisecond.
     *
     * @param degrees : degrees
     *
     * @return int  : the angle in semicircles
     */
    int degrees2semicircles( double const & degrees ) const
    {
      return int((degrees / 180.0 )* 2147483648.0);
    }

    //! Semicircles to degrees. 
    /*!
     * Note that 2^31 semicircles is equal to 180 degrees.
     * North latitudes and East longitudes are indicated with positive numbers.
     * South latitudes and West longitudes are indicated with negative numbers.
     * This representation has a precision of around 1 millisecond.
     *
     * @param semicircles : the angle in semicircles
     *
     * @return double  : the angle degrees
     */
    double semicircles2degrees( double const & semicircles ) const
    {
      return semicircles * 180.0 / 2147483648.0;
    }

 };

} // Namespace Angle
} // Namepsace UF

#endif // ANGLEANGLECONVERSIONS_H
