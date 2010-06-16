//#pragma once

#if !defined(GARMINGRME_H)
#define GARMINGRME_H

#include "ufNMEASentence.h"

namespace UF {
//! Garmin proprietary messages.
namespace Garmin {
  //! GRME: Garmin Proprietary Message, Estimated Position Error (root mean error).
  /*!
  CLASS
    GRME


    Introduction

    The GRME message contains the estimated position error.

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

    The message is output in the format: $GRME,m1,c2,m3,c4,m5,c6*hh

      <table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="679" id="AutoNumber1">
      <tr>
        <td width="88">Parameter</td>
        <td width="390">Description</td>
        <td width="197">Range</td>
      </tr>
      <tr>
        <td width="88">m1</td>
        <td width="390">Estimated horizontal position error (HPE)</td>
        <td width="197">0 - 99.9</td>
      </tr>
      <tr>
        <td width="88">c2</td>
        <td width="390">Horizontal position error units (M - meters).</td>
        <td width="197">M</td>
      </tr>
      <tr>
        <td width="88">m3</td>
        <td width="390">Estimated vertical position error (VPE)</td>
        <td width="197">0 - 99.9</td>
      </tr>
      <tr>
        <td width="88">c4</td>
        <td width="390">Vertical position error units (M - meters).</td>
        <td width="197">M</td>
      </tr>
      <tr>
        <td width="88">m5</td>
        <td width="390">Estimated overall spherical equivalent position
                error (SPE)</td>
        <td width="197">0 - 99.9</td>
      </tr>
      <tr>
        <td width="88">c6</td>
        <td width="390">Overall spherical equivalent position error
                units (M - meters).</td>
        <td width="197">M</td>
      </tr>
      <tr>
        <td width="88">hh</td>
        <td width="390">Checksum 2-character hex</td>
        <td width="197">&nbsp;</td>
      </tr>
    </table>

    Example:

      $GRME,15.0,M,45.0,M,25.0,M*22

    Here is a typical message:

      $GRME,15.0,M,22.5,M,27.0,M*1A


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
  class UF_GPS_PARSER_EXPORT GRME :
    public NMEA::NMEASentence
  {
  public:
    GRME();
    virtual ~GRME();

    //! Initialise the fields in a sentence.
    void Init()
    {
      this->sentence.clear();
      this->ProprietarySentenceIndicator = true;
      this->ParseStatus = NMEA::NMEA_NOT_PARSED;

      this->HPE = -1;
      this->HPEUnits = 'M';
      this->VPE = -1;
      this->VPEUnits = 'M';
      this->SPE = -1;
      this->SPEUnits = 'M';
    }

    //! Assignment
    GRME operator = ( GRME const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->HPE = rhs.HPE;
      this->HPEUnits = rhs.HPEUnits;
      this->VPE = rhs.VPE;
      this->VPEUnits = rhs.VPEUnits;
      this->SPE = rhs.SPE;
      this->SPEUnits = rhs.SPEUnits;

      return *this;
    }

    //! Constructor
    GRME  ( GRME const & rhs )
    {

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->HPE = rhs.HPE;
      this->HPEUnits = rhs.HPEUnits;
      this->VPE = rhs.VPE;
      this->VPEUnits = rhs.VPEUnits;
      this->SPE = rhs.SPE;
      this->SPEUnits = rhs.SPEUnits;

    }

    //! Equality
    bool operator == ( GRME const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
        this->HPE == rhs.HPE &&
        this->HPEUnits == rhs.HPEUnits &&
        this->VPE == rhs.VPE &&
        this->VPEUnits == rhs.VPEUnits &&
        this->SPE == rhs.SPE &&
        this->SPEUnits == rhs.SPEUnits
        );
    }

    //! Inequality
    bool operator != ( GRME const & rhs )
    {
      return !(*this == rhs);
    }


    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, GRME const & p)
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
    friend std::istream& operator>>(std::istream& is, GRME & p)
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
    //! Set the estimated horizontal position error.
    SetMacro(HPE,double);
    //! Get the estimated horizontal position error.
    GetMacro(HPE,double);
    //! Set the horizontal position error units (M - meters).
    SetMacro(HPEUnits,char);
    //! Get the horizontal position error units (M - meters).
    GetMacro(HPEUnits,char);
    //! Set the estimated vertical position error.
    SetMacro(VPE,double);
    //! Get the estimated vertical position error.
    GetMacro(VPE,double);
    //! Set the vertical position error units (M - meters).
    SetMacro(VPEUnits,char);
    //! Get the vertical position error units (M - meters).
    GetMacro(VPEUnits,char);
    //! Set the estimated overall spherical equivalent position error.
    SetMacro(SPE,double);
    //! Get the estimated overall spherical equivalent position error.
    GetMacro(SPE,double);
    //! Set the overall spherical equivalent position error units (M - meters).
    SetMacro(SPEUnits,char);
    //! Get the overall spherical equivalent position error units (M - meters).
    GetMacro(SPEUnits,char);

  protected:
    double HPE;
    char HPEUnits;
    double VPE;
    char VPEUnits;
    double SPE;
    char SPEUnits;
  };

} // Namespace NMEA.
} // Namespace UF.


#endif
