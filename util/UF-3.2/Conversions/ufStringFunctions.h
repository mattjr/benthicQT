#ifndef _ufStringFunctions_h
#define _ufStringFunctions_h

#include <stdexcept>
#include <string>

#include "ufWin32Header.h"

namespace UF {

namespace StrFmt {

//! Provides some useful string functsions.
/*!
This singletom class provides a series of functions that are useful
for string manipulations.
*/
class /*UF_CONVERSIONS_EXPORT*/ ufStringFns
{
  public:
    //! Unique point of access for the singleton.
    static ufStringFns* Instance()
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
      static ufStringFns theInstance;
      pInstance_ = &theInstance;
    }
    //! Gets called if a dead reference is detected.
    static void OnDeadReference()
    {
      throw std::runtime_error("Dead Reference Error in ufStringFns.");
    }
    //! Private constructor, disables creation of the object by clients.
    ufStringFns()
    {
      // My stuff
    };
    ufStringFns( const ufStringFns & ); // Disable the copy constructor.
    ufStringFns& operator = ( const ufStringFns & ); // Disable self-assignment.
    // Make the destructor private in order to prevent clients
    // holding a pointer from accidentally deleting it.
    virtual ~ufStringFns(void)
    {
      pInstance_ = 0;
      destroyed_ = true;

      // My stuff
    };

    //! Pointer to the instance.
    static ufStringFns * pInstance_;
    //! True if destroyed.
    static bool destroyed_;

  public:
    // Here we put in our specific methods and variables for this class.

    //! Trim unwanted characters from the left of a string.
    /*!
        @param str - the string to be trimmed.
        @param delims - the characters to search for,
        @return The trimmed string.
    */
    std::string TrimLeft(std::string const & str, std::string const & delims = " \t\r\n")
    {
      // Trim leading whitespace.
      std::string::size_type pos = str.find_first_not_of(delims);
      if ( pos != std::string::npos )
        return str.substr(pos);
      else return "";
    }

    //! Trim unwanted characters from the right of a string.
    /*!
        @param str - the string to be trimmed.
        @param delims - the characters to search for,
        @return The trimmed string.
    */
    std::string TrimRight(std::string const & str, std::string const & delims = " \t\r\n")
    {
      // Trim trailing whitespace.
      std::string::size_type pos = str.find_last_not_of(delims);
      if ( pos != std::string::npos )
        return str.substr(0,pos+1);
      else return "";
    }

    //! Trim unwanted characters from the left and right of a string.
    /*!
        @param str - the string to be trimmed.
        @param delims - the characters to search for,
        @return The trimmed string.
    */
    std::string Trim(std::string const & str, std::string const & delims = " \t\r\n")
    {
      return TrimRight(TrimLeft(str,delims),delims);
    }

private:
};

} // StrFmt

} // Namespace UF.

#endif
