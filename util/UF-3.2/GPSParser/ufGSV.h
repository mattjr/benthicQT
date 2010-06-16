//#pragma once

#if !defined(NMEAGSV_H)
#define NMEAGSV_H

#include "ufNMEASentence.h"

namespace UF {
namespace NMEA {
  //! GSV: Satellites in View.
  /*!
  CLASS
    GSV


    Introduction

    The GSV message contains the PRN number, elevation, azimuth, and signal to
    noise ratio for each visible satellite. This message contains data for a maximum
    of four satellites. If seven satellites are visible, two GSV messages are output; if
    ten satellites are visible, three GSV messages are output.

    The total number of messages transmitted and the number of messages transmitted
    are indicated in the first two fields of the first message.The first three messages
    contain GPS satellite information and the second three messages contain
    GLONASS satellite information.

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
      SetPRN1(PRN1);
      PRN1 = GetPRN1();
      \endcode

    Output

    The message is output in the format: $--GSV,d1,d2,d3,d4,d5,d6,f7,d8,d9,d10,f11,d12,d13,d14,f15,d16, d17,d18,f19*hh

    <table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="679" id="AutoNumber1">
      <tr>
        <td width="88">Parameter</td>
        <td width="390">Description</td>
        <td width="197">Range</td>
      </tr>
      <tr>
        <td width="88">d1</td>
        <td width="390">Total number of GSV messages to be output</td>
        <td width="197">1 - 3</td>
      </tr>
      <tr>
        <td width="88">d2</td>
        <td width="390">Message number</td>
        <td width="197">1 - 3</td>
      </tr>
      <tr>
        <td width="88">d3</td>
        <td width="390">Total number of satellites in view</td>
        <td width="197">1 - 12</td>
      </tr>
      <tr>
        <td width="88">d4</td>
        <td width="390">Satellite PRN number</td>
        <td width="197">1 - 32</td>
      </tr>
            <tr>
        <td width="88">d5</td>
        <td width="390">Elevation (degrees)</td>
        <td width="197">0 - 90</td>
      </tr>
            <tr>
        <td width="88">d6</td>
        <td width="390">Azimuth (degrees)</td>
        <td width="197">0 - 359</td>
      </tr>
            <tr>
        <td width="88">d7</td>
        <td width="390">Signal to noise ratio (dbHz)</td>
        <td width="197">30 - 60</td>
      </tr>
            <tr>
        <td width="88">d8</td>
        <td width="390">Satellite PRN number</td>
        <td width="197">1 - 32</td>
      </tr>
            <tr>
        <td width="88">d9</td>
        <td width="390">Elevation (degrees)</td>
        <td width="197">0 - 90</td>
      </tr>
            <tr>
        <td width="88">d10</td>
        <td width="390">Azimuth (degrees)</td>
        <td width="197">0 - 359</td>
      </tr>
            <tr>
        <td width="88">d11</td>
        <td width="390">Signal to noise ratio (dbHz)</td>
        <td width="197">30 - 60</td>
      </tr>
            <tr>
        <td width="88">d12</td>
        <td width="390">Satellite PRN number</td>
        <td width="197">1 - 32</td>
      </tr>
            <tr>
        <td width="88">d13</td>
        <td width="390">Elevation (degrees)</td>
        <td width="197">0 - 90</td>
      </tr>
            <tr>
        <td width="88">d14</td>
        <td width="390">Azimuth (degrees)</td>
        <td width="197">0 - 359</td>
      </tr>
            <tr>
        <td width="88">d15</td>
        <td width="390">Signal to noise ratio (dbHz)</td>
        <td width="197">30 - 60</td>
      </tr>
      <tr>
        <td width="88">d16</td>
        <td width="390">Satellite PRN number</td>
        <td width="197">1 - 32</td>
      </tr>
      <tr>
        <td width="88">d17</td>
        <td width="390">Elevation (degrees)</td>
        <td width="197">0 - 90</td>
      </tr>
      <tr>
        <td width="88">d18</td>
        <td width="390">Azimuth (degrees)</td>
        <td width="197">0 - 359</td>
      </tr>
      <tr>
        <td width="88">d19</td>
        <td width="390">Signal to noise ratio (dbHz)</td>
        <td width="197">30 - 60</td>
      </tr>
      <tr>
        <td width="88">*hh</td>
        <td width="390">Checksum</td>
        <td width="197">2-character hex</td>
      </tr>
    </table>

    Here is a typical message: $GPGSV,2,2,12,08,43,294,47,07,19,062,42,05,49,314,49,02,03,120,29*7E


    Setting

    Thales Navigation

    $PASHS,NME,GSV,x,c,[f]

    This command allows you to enable or disable the GSV response message on the
    output port x, where c is ON or OFF, and f is the optional message output rate
    ranging from 0.05 to 999 seconds depending on the measurement update rate
    option installed.

    If the command is set without a period, the G12 uses the period set by the
    $PASHS,NME,PER command If the $PASHS,NME,PER command is issued after
    this message period has been set, the period resets to the PER setting.

    $PASHQ,GSV,[c1]

    This command allows you to query for the GSV message, where c1 is the optional
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
  class UF_GPS_PARSER_EXPORT GSV :
    public NMEASentence
  {
  public:
    GSV();
    virtual ~GSV();

    //! Initialise the fields in a sentence.
    void Init()
    {
      this->sentence.clear();
      this->ProprietarySentenceIndicator = false;
      this->ParseStatus = NMEA_NOT_PARSED;

      this->TotalNumberOfGSVMessages = 0;
      this->MessageNumber = 0;
      this->TotalNumberOfSatellitesInView = 0;
      this->PRN1 = -1;
      this->Elevation1 = -1;
      this->Azimuth1 = -1;
      this->SN1 = -1;
      this->PRN2 = -1;
      this->Elevation2 = -1;
      this->Azimuth2 = -1;
      this->SN2 = -1;
      this->PRN3 = -1;
      this->Elevation3 = -1;
      this->Azimuth3 = -1;
      this->SN3 = -1;
      this->PRN4 = -1;
      this->Elevation4 = -1;
      this->Azimuth4 = -1;
      this->SN4 = -1;
    }

    //! Assignment
    GSV operator = ( GSV const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->TotalNumberOfGSVMessages = rhs.TotalNumberOfGSVMessages;
      this->MessageNumber = rhs.MessageNumber;
      this->TotalNumberOfSatellitesInView = rhs.TotalNumberOfSatellitesInView;
      this->PRN1 = rhs.PRN1;
      this->Elevation1 = rhs.Elevation1;
      this->Azimuth1 = rhs.Azimuth1;
      this->SN1 = rhs.SN1;
      this->PRN2 = rhs.PRN2;
      this->Elevation2 = rhs.Elevation2;
      this->Azimuth2 = rhs.Azimuth2;
      this->SN2 = rhs.SN2;
      this->PRN3 = rhs.PRN3;
      this->Elevation3 = rhs.Elevation3;
      this->Azimuth3 = rhs.Azimuth3;
      this->SN3 = rhs.SN3;
      this->PRN4 = rhs.PRN4;
      this->Elevation4 = rhs.Elevation4;
      this->Azimuth4 = rhs.Azimuth4;
      this->SN4 = rhs.SN4;

      return *this;
    }

    //! Constructor
    GSV  ( GSV const & rhs )
    {
      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->TotalNumberOfGSVMessages = rhs.TotalNumberOfGSVMessages;
      this->MessageNumber = rhs.MessageNumber;
      this->TotalNumberOfSatellitesInView = rhs.TotalNumberOfSatellitesInView;
      this->PRN1 = rhs.PRN1;
      this->Elevation1 = rhs.Elevation1;
      this->Azimuth1 = rhs.Azimuth1;
      this->SN1 = rhs.SN1;
      this->PRN2 = rhs.PRN2;
      this->Elevation2 = rhs.Elevation2;
      this->Azimuth2 = rhs.Azimuth2;
      this->SN2 = rhs.SN2;
      this->PRN3 = rhs.PRN3;
      this->Elevation3 = rhs.Elevation3;
      this->Azimuth3 = rhs.Azimuth3;
      this->SN3 = rhs.SN3;
      this->PRN4 = rhs.PRN4;
      this->Elevation4 = rhs.Elevation4;
      this->Azimuth4 = rhs.Azimuth4;
      this->SN4 = rhs.SN4;

    }

    //! Equality
    bool operator == ( GSV const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
        this->TotalNumberOfGSVMessages == rhs.TotalNumberOfGSVMessages &&
        this->MessageNumber == rhs.MessageNumber &&
        this->TotalNumberOfSatellitesInView == rhs.TotalNumberOfSatellitesInView &&
        this->PRN1 == rhs.PRN1 &&
        this->Elevation1 == rhs.Elevation1 &&
        this->Azimuth1 == rhs.Azimuth1 &&
        this->SN1 == rhs.SN1 &&
        this->PRN2 == rhs.PRN2 &&
        this->Elevation2 == rhs.Elevation2 &&
        this->Azimuth2 == rhs.Azimuth2 &&
        this->SN2 == rhs.SN2 &&
        this->PRN3 == rhs.PRN3 &&
        this->Elevation3 == rhs.Elevation3 &&
        this->Azimuth3 == rhs.Azimuth3 &&
        this->SN3 == rhs.SN3 &&
        this->PRN4 == rhs.PRN4 &&
        this->Elevation4 == rhs.Elevation4 &&
        this->Azimuth4 == rhs.Azimuth4 &&
        this->SN4 == rhs.SN4
        );
    }

    //! Inequality
    bool operator != ( GSV const & rhs )
    {
      return !(*this == rhs);
    }

    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, GSV const & p)
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
    friend std::istream& operator>>(std::istream& is, GSV & p)
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
    //! Set the total number of GSV messages to be output.
    SetMacro(TotalNumberOfGSVMessages,int);
    //! Get the total number of GSV messages to be output.
    GetMacro(TotalNumberOfGSVMessages,int);
    //! Set the message number.
    SetMacro(MessageNumber,int);
    //! Get the message number.
    GetMacro(MessageNumber,int);
    //! Set the total number of satellites in view.
    SetMacro(TotalNumberOfSatellitesInView,int);
    //! Get the total number of satellites in view.
    GetMacro(TotalNumberOfSatellitesInView,int);

    //! Set the satellite PRN number.
    SetMacro(PRN1,int);
    //! Get the satellite PRN number.
    GetMacro(PRN1,int);
    //! Set the elevation (degrees).
    SetMacro(Elevation1,int);
    //! Get the elevation (degrees).
    GetMacro(Elevation1,int);
    //! Set the azimuth (degrees).
    SetMacro(Azimuth1,int);
    //! Get the azimuth (degrees).
    GetMacro(Azimuth1,int);
    //! Set the signal to noise ratio (dbHz).
    SetMacro(SN1,int);
    //! Get the signal to noise ratio (dbHz).
    GetMacro(SN1,int);

    //! Set the satellite PRN number.
    SetMacro(PRN2,int);
    //! Get the satellite PRN number.
    GetMacro(PRN2,int);
    //! Set the elevation (degrees).
    SetMacro(Elevation2,int);
    //! Get the elevation (degrees).
    GetMacro(Elevation2,int);
    //! Set the azimuth (degrees).
    SetMacro(Azimuth2,int);
    //! Get the azimuth (degrees).
    GetMacro(Azimuth2,int);
    //! Set the signal to noise ratio (dbHz).
    SetMacro(SN2,int);
    //! Get the signal to noise ratio (dbHz).
    GetMacro(SN2,int);

    //! Set the satellite PRN number.
    SetMacro(PRN3,int);
    //! Get the satellite PRN number.
    GetMacro(PRN3,int);
    //! Set the elevation (degrees).
    SetMacro(Elevation3,int);
    //! Get the elevation (degrees).
    GetMacro(Elevation3,int);
    //! Set the azimuth (degrees).
    SetMacro(Azimuth3,int);
    //! Get the azimuth (degrees).
    GetMacro(Azimuth3,int);
    //! Set the signal to noise ratio (dbHz).
    SetMacro(SN3,int);
    //! Get the signal to noise ratio (dbHz).
    GetMacro(SN3,int);

    //! Set the satellite PRN number.
    SetMacro(PRN4,int);
    //! Get the satellite PRN number.
    GetMacro(PRN4,int);
    //! Set the elevation (degrees).
    SetMacro(Elevation4,int);
    //! Get the elevation (degrees).
    GetMacro(Elevation4,int);
    //! Set the azimuth (degrees).
    SetMacro(Azimuth4,int);
    //! Get the azimuth (degrees).
    GetMacro(Azimuth4,int);
    //! Set the signal to noise ratio (dbHz).
    SetMacro(SN4,int);
    //! Get the signal to noise ratio (dbHz).
    GetMacro(SN4,int);

  protected:
    int TotalNumberOfGSVMessages;
    int MessageNumber;
    int TotalNumberOfSatellitesInView;
    int PRN1;
    int Elevation1;
    int Azimuth1;
    int SN1;
    int PRN2;
    int Elevation2;
    int Azimuth2;
    int SN2;
    int PRN3;
    int Elevation3;
    int Azimuth3;
    int SN3;
    int PRN4;
    int Elevation4;
    int Azimuth4;
    int SN4;
  };

} // Namespace NMEA.
} // Namespace UF.


#endif
