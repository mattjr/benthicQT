//#pragma once

#if !defined(SIRFSIRF103_H)
#define SIRFSIRF103_H

#include "ufNMEASentence.h"

namespace UF {
//! SiRF Proprietary Messages.
namespace SiRF {
  //! SiRF103: SiRF NMEA input message. Query standard NMEA message and/or set output rate.
  /*!
  CLASS
    SiRF103


    Introduction

    This command is used to control the output of standard NMEA messages GGA, GLL,
    GSA, GSV, RMC, and VTG. Using this command message, standard NMEA
    messages may be polled once, or setup for periodic output. Checksums may also be
    enabled or disabled depending on the needs of the receiving program. NMEA
    message settings are saved in battery-backed memory for each entry when the
    message is accepted.

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

    <p>Query/Rate Control Data Format<br>
    &nbsp;</p>
    <table border="1" style="border-collapse: collapse" width="62%" id="table2">
      <tr>
        <td width="109"><b>Name </b></td>
        <td width="87"><b>Example </b></td>
        <td width="64"><b>Units </b></td>
        <td width="358"><b>Description</b></td>
      </tr>
      <tr>
        <td width="109">Message ID </td>
        <td width="87">$PSRF103</td>
        <td width="64">&nbsp;</td>
        <td width="358">PSRF103 protocol header</td>
      </tr>
      <tr>
        <td width="109">Msg </td>
        <td width="87">00 </td>
        <td width="64">&nbsp;</td>
        <td width="358">See Table 1-16</td>
      </tr>
      <tr>
        <td width="109">Mode </td>
        <td width="87">01</td>
        <td width="64">&nbsp;</td>
        <td width="358">0=SetRate, 1=Query</td>
      </tr>
      <tr>
        <td width="109">Rate </td>
        <td width="87">00 </td>
        <td width="64">seconds </td>
        <td width="358">Output off=0, max=255</td>
      </tr>
      <tr>
        <td width="109">CksumEnable </td>
        <td width="87">01</td>
        <td width="64">&nbsp;</td>
        <td width="358">0=Disable Checksum, 1=Enable Checksum</td>
      </tr>
      <tr>
        <td width="109">Checksum </td>
        <td width="87">*25</td>
        <td width="64">&nbsp;</td>
        <td width="358">&nbsp;</td>
      </tr>
      <tr>
        <td width="109">&lt;CR&gt; &lt;LF&gt; </td>
        <td width="87">&nbsp;</td>
        <td width="64">&nbsp;</td>
        <td width="358">End of message termination</td>
      </tr>
    </table>
    <p><br>
    &nbsp;</p>
    <p>Messages</p>
    <table border="1" style="border-collapse: collapse" width="149" id="table1">
      <tr>
        <td width="47"><b>Value</b></td>
        <td width="95"><b>Description</b></td>
      </tr>
      <tr>
        <td width="47">0</td>
        <td width="95">GGA</td>
      </tr>
      <tr>
        <td width="47">1</td>
        <td width="95">GLL</td>
      </tr>
      <tr>
        <td width="47">2</td>
        <td width="95">GSA</td>
      </tr>
      <tr>
        <td width="47">3</td>
        <td width="95">GSV</td>
      </tr>
      <tr>
        <td width="47">4</td>
        <td width="95">RMC</td>
      </tr>
      <tr>
        <td width="47">5</td>
        <td width="95">VTG</td>
      </tr>
    </table>

    1. Query the GGA message with checksum enabled

    $PSRF103,00,01,00,01*25

    2. Enable VTG message for a 1 Hz constant output with checksum enabled

    $PSRF103,05,00,01,01*20

    3. Disable VTG message

    $PSRF103,05,00,00,01*21

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
  class UF_GPS_PARSER_EXPORT SiRF103 :
    public NMEA::NMEASentence
  {
  public:
    SiRF103();
    virtual ~SiRF103();

    //! Initialise the fields in a sentence.
    void Init()
    {
      this->sentence.clear();
      this->ProprietarySentenceIndicator = true;
      this->ParseStatus = NMEA::NMEA_NOT_PARSED;

      this->Msg = 255;
      this->Mode = 255;
      this->Rate = 1;
      this->CksumEnable = 1;
    }

    //! Assignment
    SiRF103 operator = ( SiRF103 const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->Msg = rhs.Msg;
      this->Mode = rhs.Mode;
      this->Rate = rhs.Rate;
      this->CksumEnable = rhs.CksumEnable;

      return *this;
    }

    //! Constructor
    SiRF103  ( SiRF103 const & rhs )
    {

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->Msg = rhs.Msg;
      this->Mode = rhs.Mode;
      this->Rate = rhs.Rate;
      this->CksumEnable = rhs.CksumEnable;
    }

    //! Equality
    bool operator == ( SiRF103 const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
        this->Msg == rhs.Msg &&
        this->Mode == rhs.Mode &&
        this->Rate == rhs.Rate &&
        this->CksumEnable == rhs.CksumEnable
        );
    }

    //! Inequality
    bool operator != ( SiRF103 const & rhs )
    {
      return !(*this == rhs);
    }


    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, SiRF103 const & p)
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
    friend std::istream& operator>>(std::istream& is, SiRF103 & p)
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
    SetMacro(Msg,unsigned char);
    GetMacro(Msg,unsigned char);
    SetMacro(Mode,unsigned char);
    GetMacro(Mode,unsigned char);
    SetMacro(Rate,unsigned char);
    GetMacro(Rate,unsigned char);
    SetMacro(CksumEnable,unsigned char);
    GetMacro(CksumEnable,unsigned char);

  protected:
    unsigned char Msg;
    unsigned char Mode;
    unsigned char Rate;
    unsigned char CksumEnable;
  };

} // Namespace NMEA.
} // Namespace UF.

#endif
