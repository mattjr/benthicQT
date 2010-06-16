//#pragma once

#if !defined(GARMINGRMZ_H)
#define GARMINGRMZ_H

#include "ufNMEASentence.h"

namespace UF {
//! Garmin proprietary messages.
namespace Garmin {
  //! GRMZ: Garmin Proprietary Message, Altitude Information.
  /*!
  CLASS
    GRMZ


    Introduction

    The GRMZ message contains information about the altitude.

    Convenience macros

    Most variables that correspond to fields in the message have a set of
      convenience macros to facilitate setting and getting of their values.
      These have the form:
      - SetVariableName()
      - GetVariableName()

      Where Name is the name of the variable. In the documentation you can see the
      macros described as SetMacro() and GetMacro() respectively.

      Usage:
      \code
      SetLongitude(longitude);
      longitude = GetLongitude();
      \endcode

    Output

    The message is output in the format: $GRMZ,m1,c2,m3*hh

      <table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="679" id="AutoNumber1">
      <tr>
        <td width="88">Parameter</td>
        <td width="390">Description</td>
        <td width="197">Range</td>
      </tr>
      <tr>
        <td width="88">m1</td>
        <td width="390">Altitude</td>
        <td width="197"> </td>
      </tr>
      <tr>
        <td width="88">c2</td>
        <td width="390">Altitude units (always f - feet)</td>
        <td width="197">f</td>
      </tr>
      <tr>
        <td width="88">m3</td>
        <td width="390">Position fix (2: 2D (user altitude), 3: 3D</td>
        <td width="197">2,3</td>
      </tr>
      <tr>
        <td width="88">hh</td>
        <td width="390">Checksum 2-character hex</td>
        <td width="197">&nbsp;</td>
      </tr>
    </table>

    Example:

      $GRMZ,246,f,3*1B

      $GRMZ,93,f,3*21

    Here is a typical message: $GRMZ,1076,f,3*2B


    \version 2.0

    \date 07-May-2004

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
  class UF_GPS_PARSER_EXPORT GRMZ :
    public NMEA::NMEASentence
  {
  public:
    GRMZ();
    virtual ~GRMZ();

    //! Initialise the fields in a sentence.
    void Init()
    {
      this->sentence.clear();
      this->ProprietarySentenceIndicator = true;
      this->ParseStatus = NMEA::NMEA_NOT_PARSED;

      this->Altitude = -1;
      this->AltitudeUnits = 'f';
      this->Fix = 0;
    }

    //! Assignment
    GRMZ operator = ( GRMZ const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->Altitude = rhs.Altitude;
      this->AltitudeUnits = rhs.AltitudeUnits;
      this->Fix = rhs.Fix;

      return *this;
    }

    //! Constructor
    GRMZ  ( GRMZ const & rhs )
    {

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->Altitude = rhs.Altitude;
      this->AltitudeUnits = rhs.AltitudeUnits;
      this->Fix = rhs.Fix;

    }

    //! Equality
    bool operator == ( GRMZ const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
        this->Altitude == rhs.Altitude &&
        this->AltitudeUnits == rhs.AltitudeUnits &&
        this->Fix == rhs.Fix
        );
    }

    //! Inequality
    bool operator != ( GRMZ const & rhs )
    {
      return !(*this == rhs);
    }


    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, GRMZ const & p)
    {
      std::ios::fmtflags flags = os.flags();
      os << p.MakeSentence();
      os.flags(flags);
      os.clear();
      return os;
    }

    //! Input the message.
    /*!
        @param is - the input stream.
        @param p - the object into which the data is streamed.

        The data expected is a well formed NMEA string.
    */
    friend std::istream& operator>>(std::istream& is, GRMZ & p)
    {
      std::ios::fmtflags flags = is.flags();
      p.Init();
      std::getline(is,p.sentence,'\n');
      p.sentence += '\n';
      p.Parse();
      is.flags(flags);
      is.clear();
      return is;
    }

    //! Parse the sentence.
    /*!
        @param ForceParse - if true then the checksum is not checked for validity before parsing.
    */
    void Parse(bool const & ForceParse = false);

    //! Make a sentence.
    std::string MakeSentence() const;

    //! The identifying nmemonic for this message.
    static std::string const MNEMONIC;

  public:
    //! Set the altitude.
    SetMacro(Altitude,double);
    //! Get the altitude.
    GetMacro(Altitude,double);
    //! Set the altitude units (f - feet).
    SetMacro(AltitudeUnits,char);
    //! Get the altitude units (f - feet).
    GetMacro(AltitudeUnits,char);
    //! Set the position fix (2: 2D, 3: 3D).
    SetMacro(Fix,int);
    //! Get the position fix (2: 2D, 3: 3D).
    GetMacro(Fix,int);

  protected:
    double Altitude;
    char AltitudeUnits;
    int Fix;
  };

} // Namespace NMEA.
} // Namespace UF.


#endif
