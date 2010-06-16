//#pragma once

#if !defined(JNSPTPSRATT_H)
#define JNSPTPSRATT_H

#include "ufNMEASentence.h"

namespace UF {
//! All JNS proprietary NMEA Messages.
namespace JNS {
  //! ATT: JNS proprietary NMEA Messages, Attitude Parameters.
  /*!
  CLASS
    ATT


    Introduction

    The ATT message contains information about the attitude parameters.

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

    The message is output in the format: $PTPSR,MsgID,c--c*hh<CR><LF>, the MsgId == ATT.

  <p><b>ATT  Attitude parameters</b><br>
  &nbsp;</p>
  <table border="1" style="border-collapse: collapse" width="94%" id="table2">
    <tr>
      <td><b>Parameter</b></td>
      <td width="113"><b>Format</b></td>
      <td width="503"><b>Description</b></td>
    </tr>
    <tr>
      <td>1</td>
      <td width="113">%C</td>
      <td width="503">UTC time indicator:<br>
      V means that UTC time is valid.<br>
      N means that UTC time is not valid.</td>
    </tr>
    <tr>
      <td>2</td>
      <td width="113">%6.2F</td>
      <td width="503">UTC time of position (the first two digits designate
      hours, the next two digits designate minutes and the rest of the digits
      designate seconds).</td>
    </tr>
    <tr>
      <td>3</td>
      <td width="113">%.3F</td>
      <td width="503">True Heading in degrees.</td>
    </tr>
    <tr>
      <td>4</td>
      <td width="113">%.3F</td>
      <td width="503">Pitch in degrees.</td>
    </tr>
    <tr>
      <td>5</td>
      <td width="113">%C</td>
      <td width="503">Base position type (N  not available, A
      autonomous, D  code differential, F  float, R  fixed).<br>
      This field can be useful provided JPS messages are used for broadcasting
      RTK data.</td>
    </tr>
    <tr>
      <td>6</td>
      <td width="113">*%2X\0D\0A</td>
      <td width="503">Checksum </td>
    </tr>
  </table>
  <p><b>Note</b>: To enable [ATT] message, use the command syntax as shown in
  the following example: em,,nmea/P_ATT</p>


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
  class UF_GPS_PARSER_EXPORT ATT :
    public NMEA::NMEASentence
  {
  public:
    ATT();
    virtual ~ATT();

    //! Initialise the fields in a sentence.
    void Init()
    {
      this->sentence.clear();
      this->ProprietarySentenceIndicator = true;
      this->ParseStatus = NMEA::NMEA_NOT_PARSED;

      this->UTCTimeIndicator = 'V';
      this->UTC = -1;
      this->TrueHeading = -1;
      this->Pitch = -91;
      this->BasePositionType = 'N';
    }

    bool ParsedAllFields()
    {
      return (    this->UTCTimeIndicator != 'V' &&
                  this->UTC != -1 &&
                  this->TrueHeading != -1 &&
                  this->Pitch != -91 &&
                  this->BasePositionType != 'N' );

    }

    //! Assignment
    ATT operator = ( ATT const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->UTCTimeIndicator = rhs.UTCTimeIndicator;
      this->UTC = rhs.UTC;
      this->TrueHeading = rhs.TrueHeading;
      this->Pitch = rhs.Pitch;
      this->BasePositionType = rhs.BasePositionType;

      return *this;
    }

    //! Constructor
    ATT  ( ATT const & rhs )
    {

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->UTCTimeIndicator = rhs.UTCTimeIndicator;
      this->UTC = rhs.UTC;
      this->TrueHeading = rhs.TrueHeading;
      this->Pitch = rhs.Pitch;
      this->BasePositionType = rhs.BasePositionType;

    }

    //! Equality
    bool operator == ( ATT const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
        this->UTCTimeIndicator == rhs.UTCTimeIndicator &&
        this->UTC == rhs.UTC &&
        this->TrueHeading == rhs.TrueHeading &&
        this->Pitch == rhs.Pitch &&
        this->BasePositionType == rhs.BasePositionType
      );
    }

    //! Inequality
    bool operator != ( ATT const & rhs )
    {
      return !(*this == rhs);
    }


    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, ATT const & p)
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
    friend std::istream& operator>>(std::istream& is, ATT & p)
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
    //! Set the UTC Time Indicator.
    SetMacro(UTCTimeIndicator,char);
    //! Get the UTC Time Indicator.
    GetMacro(UTCTimeIndicator,char);
    //! Set UTC.
    SetMacro(UTC,double);
    //! Get UTC.
    GetMacro(UTC,double);
    //! Set the true heading in degrees.
    SetMacro(TrueHeading,double);
    //! Get the true heading in degrees.
    GetMacro(TrueHeading,double);
    //! Set the pitch in degrees.
    SetMacro(Pitch,double);
    //! Get the pitch in degrees.
    GetMacro(Pitch,double);
    //! Set the base position type.
    SetMacro(BasePositionType,char);
    //! Get the base position type.
    GetMacro(BasePositionType,char);

  protected:
    char UTCTimeIndicator;
    double UTC;
    double TrueHeading;
    double Pitch;
    char BasePositionType;
  };

} // Namespace NMEA.
} // Namespace UF.


#endif
