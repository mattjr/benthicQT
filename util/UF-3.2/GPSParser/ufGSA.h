//#pragma once

#if !defined(NMEAGSA_H)
#define NMEAGSA_H

#include "ufNMEASentence.h"

namespace UF {
namespace NMEA {
  //! GSA: DOP and Active Satellites.
  /*!
  CLASS
    GSA


    Introduction

    The GSA message contains indicators for current position mode. A list of the
    satellites being used to compute position, and  the values for PDOP, HDOP, and VDOP.
    This message is not output until positions are being computed.

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
      SetHDOP(HDOP);
      HDOP = GetHDOP();
      \endcode

    Output

    The message is output in the format: $--GSA,c1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13,d14,d15,d16,d17*hh

        <table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="679" id="AutoNumber1">
      <tr>
        <td width="88">Parameter</td>
        <td width="390">Description</td>
        <td width="197">Range</td>
      </tr>
      <tr>
        <td width="88">c1</td>
        <td width="390">Position mode indicator:<br>
                 A - Automatic mode<br>
                 M - Manual mode</td>
        <td width="197">A, M</td>
      </tr>
      <tr>
        <td width="88">d2</td>
        <td width="390">Position mode indicator:<br>
                 2 - 2D mode<br>
                 3 - 3D mode</td>
        <td width="197">2, 3</td>
      </tr>
      <tr>
        <td width="88">d3 - d14</td>
        <td width="390">These twelve fields represent the receivers
                twelve channels listed in ascending order. The number 17
                appearing in field d5 indicates that G12 channel 3 is locked on
                satellite 17. If a given channel is not locked on a satellite,
                the corresponding
                field will be empty.</td>
        <td width="197">1 - 32</td>
      </tr>
      <tr>
        <td width="88">d15</td>
        <td width="390">Current PDOP value</td>
        <td width="197">0 - 99.9</td>
      </tr>
            <tr>
        <td width="88">d16</td>
        <td width="390">Current HDOP value</td>
        <td width="197">0 - 99.9</td>
      </tr>
            <tr>
        <td width="88">d17</td>
        <td width="390">Current VDOP value</td>
        <td width="197">0 - 99.9</td>
      </tr>
      <tr>
        <td width="88">*hh</td>
        <td width="390">Checksum</td>
        <td width="197">2-character hex</td>
      </tr>
    </table>


    Here is a typical message: $GPGSA,A,3,31,29,23,26,,21,17,09,08,,03,,01.7,01.0,01.4*0B


    Setting

    Thales Navigation

    $PASHS,NME,GSA,x,s,[f]

    Enable/disable DOP and active satellite message to be sent out to the serial port,
    where x is the output port, and s is ON or OFF, and f is the optional message output
    rate ranging from 0.05 to 999 seconds depending on the measurement update rate
    option installed.

    The receiver does not output this message if a position is not computed.
    If the command is set without a period, the G12 uses the period set by the
    $PASHS,NME,PER command If the $PASHS,NME,PER command is issued after
    this message period has been set, the period resets to the PER setting.

    Example

    Enter the following command to enable GSA message on port B.

    $PASHS,NME,GSA,B,ON

    $PASHQ,GSA,[c1]

    This command allows you to query for the GSA message, where c1 is the optional
    port designator for the output of the response. If a port is not specified, the receiver
    sends the response to the current port.

    SiRF Proprietary NMEA Input Messages

    Query/RateControl

    This command is used to control the output of standard NMEA messages GGA, GLL,
    GSA, GSV, RMC, and VTG. Using this command message, standard NMEA
    messages may be polled once, or setup for periodic output. Checksums may also be
    enabled or disabled depending on the needs of the receiving program. NMEA
    message settings are saved in battery-backed memory for each entry when the
    message is accepted.

    $PSRF103,Msg,Mode,Rate,CkSumEnable,CkSum

    - Msg one of:
      - 0 GGA
      - 1 GLL
      - 2 GSA
      - 3 GSV
      - 4 RMC
      - 5 VTG

    - Mode 0 - set rate, 1 - query
    - Rate (0-255) 0 - output off, maxumim value - 255 seconds.
    - CkSumEnable 0: disable, 1: enable
    - CkSum checksum.

    Some examples:
    -# Query the GGA message with checksum enabled:$PSRF103,00,01,00,01*25
    -# Enable VTG message for a 1 Hz constant output with checksum enabled: $PSRF103,05,00,01,01*20
    -# Disable VTG message: $PSRF103,05,00,00,01*21

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
  class UF_GPS_PARSER_EXPORT GSA :
    public NMEASentence
  {
  public:
    GSA();
    virtual ~GSA();

    //! Initialise the fields in a sentence.
    void Init()
    {
      this->sentence.clear();
      this->ProprietarySentenceIndicator = false;
      this->ParseStatus = NMEA_NOT_PARSED;

      this->ModeIndicator = 'A';
      this->PositionModeIndicator = 2;
      this->Channel01 = -1;
      this->Channel02 = -1;
      this->Channel03 = -1;
      this->Channel04 = -1;
      this->Channel05 = -1;
      this->Channel06 = -1;
      this->Channel07 = -1;
      this->Channel08 = -1;
      this->Channel09 = -1;
      this->Channel10 = -1;
      this->Channel11 = -1;
      this->Channel12 = -1;
      this->PDOP = -1;
      this->HDOP = -1;
      this->VDOP = -1;
    }

    //! Assignment
    GSA operator = ( GSA const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->ModeIndicator = rhs.ModeIndicator;
      this->PositionModeIndicator = rhs.PositionModeIndicator;
      this->Channel01 = rhs.Channel01;
      this->Channel02 = rhs.Channel02;
      this->Channel03 = rhs.Channel03;
      this->Channel04 = rhs.Channel04;
      this->Channel05 = rhs.Channel05;
      this->Channel06 = rhs.Channel06;
      this->Channel07 = rhs.Channel07;
      this->Channel08 = rhs.Channel08;
      this->Channel09 = rhs.Channel09;
      this->Channel10 = rhs.Channel10;
      this->Channel11 = rhs.Channel11;
      this->Channel12 = rhs.Channel12;
      this->PDOP = rhs.PDOP;
      this->HDOP = rhs.HDOP;
      this->VDOP = rhs.VDOP;

      return *this;
    }

    //! Constructor
    GSA  ( GSA const & rhs )
    {

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->ModeIndicator = rhs.ModeIndicator;
      this->PositionModeIndicator = rhs.PositionModeIndicator;
      this->Channel01 = rhs.Channel01;
      this->Channel02 = rhs.Channel02;
      this->Channel03 = rhs.Channel03;
      this->Channel04 = rhs.Channel04;
      this->Channel05 = rhs.Channel05;
      this->Channel06 = rhs.Channel06;
      this->Channel07 = rhs.Channel07;
      this->Channel08 = rhs.Channel08;
      this->Channel09 = rhs.Channel09;
      this->Channel10 = rhs.Channel10;
      this->Channel11 = rhs.Channel11;
      this->Channel12 = rhs.Channel12;
      this->PDOP = rhs.PDOP;
      this->HDOP = rhs.HDOP;
      this->VDOP = rhs.VDOP;
    }

    //! Equality
    bool operator == ( GSA const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
        this->ModeIndicator == rhs.ModeIndicator &&
        this->PositionModeIndicator == rhs.PositionModeIndicator &&
        this->Channel01 == rhs.Channel01 &&
        this->Channel02 == rhs.Channel02 &&
        this->Channel03 == rhs.Channel03 &&
        this->Channel04 == rhs.Channel04 &&
        this->Channel05 == rhs.Channel05 &&
        this->Channel06 == rhs.Channel06 &&
        this->Channel07 == rhs.Channel07 &&
        this->Channel08 == rhs.Channel08 &&
        this->Channel09 == rhs.Channel09 &&
        this->Channel10 == rhs.Channel10 &&
        this->Channel11 == rhs.Channel11 &&
        this->Channel12 == rhs.Channel12 &&
        this->PDOP == rhs.PDOP &&
        this->HDOP == rhs.HDOP &&
        this->VDOP == rhs.VDOP
        );
    }

    //! Inequality
    bool operator != ( GSA const & rhs )
    {
      return !(*this == rhs);
    }


    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, GSA const & p)
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
    friend std::istream& operator>>(std::istream& is, GSA & p)
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
    //! Set the mode indicator.
    SetMacro(ModeIndicator,char);
    //! Get the mode indicator.
    GetMacro(ModeIndicator,char);
    //! Set the position mode indicator.
    SetMacro(PositionModeIndicator,int);
    //! Get the position mode indicator.
    GetMacro(PositionModeIndicator,int);
    //! Set the satellite number being used by this channel.
    SetMacro(Channel01,int);
    //! Get the satellite number being used by this channel.
    GetMacro(Channel01,int);
    //! Set the satellite number being used by this channel.
    SetMacro(Channel02,int);
    //! Get the satellite number being used by this channel.
    GetMacro(Channel02,int);
    //! Set the satellite number being used by this channel.
    SetMacro(Channel03,int);
    //! Get the satellite number being used by this channel.
    GetMacro(Channel03,int);
    //! Set the satellite number being used by this channel.
    SetMacro(Channel04,int);
    //! Get the satellite number being used by this channel.
    GetMacro(Channel04,int);
    //! Set the satellite number being used by this channel.
    SetMacro(Channel05,int);
    //! Get the satellite number being used by this channel.
    GetMacro(Channel05,int);
    //! Set the satellite number being used by this channel.
    SetMacro(Channel06,int);
    //! Get the satellite number being used by this channel.
    GetMacro(Channel06,int);
    //! Set the satellite number being used by this channel.
    SetMacro(Channel07,int);
    //! Get the satellite number being used by this channel.
    GetMacro(Channel07,int);
    //! Set the satellite number being used by this channel.
    SetMacro(Channel08,int);
    //! Get the satellite number being used by this channel.
    GetMacro(Channel08,int);
    //! Set the satellite number being used by this channel.
    SetMacro(Channel09,int);
    //! Get the satellite number being used by this channel.
    GetMacro(Channel09,int);
    //! Set the satellite number being used by this channel.
    SetMacro(Channel10,int);
    //! Get the satellite number being used by this channel.
    GetMacro(Channel10,int);
    //! Set the satellite number being used by this channel.
    SetMacro(Channel11,int);
    //! Get the satellite number being used by this channel.
    GetMacro(Channel11,int);
    //! Set the satellite number being used by this channel.
    SetMacro(Channel12,int);
    //! Get the satellite number being used by this channel.
    GetMacro(Channel12,int);
    //! Set the PDOP.
    SetMacro(PDOP,float);
    //! Get the PDOP.
    GetMacro(PDOP,float);
    //! Set the HDOP.
    SetMacro(HDOP,float);
    //! Get the HDOP.
    GetMacro(HDOP,float);
    //! Set VDOP.
    SetMacro(VDOP,float);
    //! Get the VDOP.
    GetMacro(VDOP,float);

  protected:
    char ModeIndicator;
    int PositionModeIndicator;
    int Channel01;
    int Channel02;
    int Channel03;
    int Channel04;
    int Channel05;
    int Channel06;
    int Channel07;
    int Channel08;
    int Channel09;
    int Channel10;
    int Channel11;
    int Channel12;
    float PDOP;
    float HDOP;
    float VDOP;
  };

} // Namespace NMEA.
} // Namespace UF.


#endif
