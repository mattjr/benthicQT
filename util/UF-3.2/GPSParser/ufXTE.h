//#pragma once

#if !defined(NMEAXTE_H)
#define NMEAXTE_H

#include "ufNMEASentence.h"

namespace UF {
namespace NMEA {
  //! XTE: Cross-Track Error, Measured.
  /*!
  CLASS
    XTE


    Introduction

    The XTE message contains the measured cross-track error.

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

    The message is output in the format: $--XTE,c1,c2,d3,c4,c5,c6*hh


    <table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="679" id="AutoNumber1">
      <tr>
        <td width="88">Parameter</td>
        <td width="390">Description</td>
        <td width="197">Range</td>
      </tr>
      <tr>
        <td width="88">c1</td>
        <td width="390">Status of the position fix:<br>
         A = Valid<br>
         V = Warning, Loran-C Blink or SNR warning</td>
        <td width="197">A,V</td>
      </tr>
      <tr>
        <td width="88">c2</td>
        <td width="390">Loran-C cycle lock flag (not used for GPS):<br>
         A = Valid<br>
         V = Invalid</td>
        <td width="197">A,V</td>
      </tr>
      <tr>
        <td width="88">d3</td>
        <td width="390">Cross-track error distance.</td>
        <td width="197">&nbsp;</td>
      </tr>
      <tr>
        <td width="88">c4</td>
        <td width="390">Direction to steer to correct the error:<br>
         L = Steer left<br>
         R = Steer right</td>
        <td width="197">L,R</td>
      </tr>
      <tr>
        <td width="88">c5</td>
        <td width="390">Distance units, N: Nautical Miles</td>
        <td width="197">N</td>
      </tr>
      <tr>
        <td width="88">c6</td>
        <td width="390">Receiver Status:<br>
         A = Navigating<br>
         S = Simulating<br>
        This field may not be present in the output of some devices.</td>
        <td width="197">A,S</td>
      </tr>
      <tr>
        <td width="88">hh</td>
        <td width="390">Checksum 2-character hex</td>
        <td width="197">&nbsp;</td>
      </tr>
    </table>

    eg1. $GPXTE,A,A,0.67,L,N

    eg2. $GPXTE,A,A,4.07,L,N*6D

    Here is a typical message: $GPXTE,V,V,0.01,R,N,S*0E


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
  class UF_GPS_PARSER_EXPORT XTE :
    public NMEASentence
  {
  public:
    XTE();
    virtual ~XTE();

    //! Initialise the fields in a sentence.
    void Init()
    {
      this->sentence.clear();
      this->ProprietarySentenceIndicator = false;
      this->ParseStatus = NMEA_NOT_PARSED;

      this->Status = 'A';
      this->CycleLock = 'A';
      this->CrossTrackError = -1;
      this->DirectionToSteer = 'L';
      this->CrossTrackUnits = 'N';
      this->ReceiverStatus = 'A';
    }

    //! Assignment
    XTE operator = ( XTE const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->Status = rhs.Status;
      this->CycleLock = rhs.CycleLock;
      this->CrossTrackError = rhs.CrossTrackError;
      this->DirectionToSteer = rhs.DirectionToSteer;
      this->CrossTrackUnits = rhs.CrossTrackUnits;
      this->ReceiverStatus = rhs.ReceiverStatus;

      return *this;
    }

    //! Constructor
    XTE  ( XTE const & rhs )
    {

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->Status = rhs.Status;
      this->CycleLock = rhs.CycleLock;
      this->CrossTrackError = rhs.CrossTrackError;
      this->DirectionToSteer = rhs.DirectionToSteer;
      this->CrossTrackUnits = rhs.CrossTrackUnits;
      this->ReceiverStatus = rhs.ReceiverStatus;

    }

    //! Equality
    bool operator == ( XTE const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
        this->Status == rhs.Status &&
        this->CycleLock == rhs.CycleLock &&
        this->CrossTrackError == rhs.CrossTrackError &&
        this->DirectionToSteer == rhs.DirectionToSteer &&
        this->CrossTrackUnits == rhs.CrossTrackUnits &&
        this->ReceiverStatus == rhs.ReceiverStatus
      );
    }

    //! Inequality
    bool operator != ( XTE const & rhs )
    {
      return !(*this == rhs);
    }


    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, XTE const & p)
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
    friend std::istream& operator>>(std::istream& is, XTE & p)
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
    //! Set the status of the sentence. A = Data Valid, V = warning (Loran-C Blink or SNR warning)
    SetMacro(Status,char);
    //! Get the status of the sentence. A = Data Valid, V = warning (Loran-C Blink or SNR warning)
    GetMacro(Status,char);

    //! Set the cycle lock - A = Data Valid, V = warning. Not used for GPS (Loran-C cycle lock flag).
    SetMacro(CycleLock,char);
    //! Get the cycle lock - A = Data Valid, V = warning. Not used for GPS (Loran-C cycle lock flag).
    GetMacro(CycleLock,char);

    //! Set the cross-track error.
    SetMacro(CrossTrackError,double);
    //! Get the cross-track error.
    GetMacro(CrossTrackError,double);
    //! Set the direction to steer to correct the error (L: left, R: right).
    SetMacro(DirectionToSteer,char);
    //! Get the direction to steer to correct the error (L: left, R: right).
    GetMacro(DirectionToSteer,char);
    //! Set the units for the cross-track error (N: nautical miles).
    SetMacro(CrossTrackUnits,char);
    //! Get the units for the cross-track error (N: nautical miles).
    GetMacro(CrossTrackUnits,char);

    //! Set the receiver status field (A: aquiring real data, S: simulating data).
    SetMacro(ReceiverStatus,char);
    //! Get the receiver status field (A: aquiring real data, S: simulating data).
    GetMacro(ReceiverStatus,char);

  protected:
    char Status;
    char CycleLock;
    double CrossTrackError;
    char DirectionToSteer;
    char CrossTrackUnits;
    char ReceiverStatus;
  };

} // Namespace NMEA.
} // Namespace UF.

#endif
