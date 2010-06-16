//#pragma once

#if !defined(NMEAGLL_H)
#define NMEAGLL_H

#include "ufNMEASentence.h"

namespace UF {
namespace NMEA {
  //! GLL: 2-D Position.
  /*!
  CLASS
    GLL


    Introduction

    The GLL message contains a 2-D position (latitude/longitude only) and time of position fix.

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

    The message is output in the format: $--GLL,m1,c2,m3,c4,m5,c6,c7*hh

        <table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="679" id="AutoNumber1">
      <tr>
        <td width="88">Parameter</td>
        <td width="390">Description</td>
        <td width="197">Range</td>
      </tr>
      <tr>
        <td width="88">m1</td>
        <td width="390">Latitude of the position fix (ddmm.mmmmm) </td>
        <td width="197">0 - 90</td>
      </tr>
      <tr>
        <td width="88">c2</td>
        <td width="390">Latitude sector </td>
        <td width="197">N(orth) or S(outh)</td>
      </tr>
      <tr>
        <td width="88">m3</td>
        <td width="390">Longitude of the position fix (dddmm.mmmmm)</td>
        <td width="197">0 - 180</td>
      </tr>
      <tr>
        <td width="88">c4</td>
        <td width="390">Longitude sector </td>
        <td width="197">E(ast) or W(est)</td>
      </tr>
      <tr>
        <td width="88">m5</td>
        <td width="390">UTC time (hhmmss.ss) of the position fix </td>
        <td width="197">000000.00 to 235959.90</td>
      </tr>
      <tr>
        <td width="88">c6</td>
        <td width="390">Status of the position fix (always A):<br>
         A = Valid<br>
         V = Invalid</td>
        <td width="197">A, V</td>
      </tr>
      <tr>
        <td width="88">c7</td>
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


    Here is a typical message: $GPGLL,3722.36223,N,12159.82741,W,170003.00,A*17


    Setting

    Thales Navigation

    $PASHS,NME,GLL,x,s,[f]
    Enable/disable NMEA latitude/longitude response message on port x, where x is
    the output port, s is ON or OFF, and f is the optional message output rate ranging
    from 0.05 to 999 seconds depending upon the measurement update rate option
    installed.

    If the command is set without a period, the G12 uses the period set by the
    $PASHS,NME,PER command. If the $PASHS,NME,PER command is issued after
    this message period has been set, the period resets to the PER setting.

    Example

    Enter the following command to enable GLL message on port A.
    $PASHS,NME,GLL,A,ON
    $PASHQ,GLL,[c1]

    This command allows you to query for the GLL position message, where c1 is the
    optional port designator for the output of the response. If a port is not specified, the
    receiver sends the response to the current port.

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
  class UF_GPS_PARSER_EXPORT GLL :
    public NMEASentence
  {
  public:
    GLL();
    virtual ~GLL();

    //! Initialise the fields in a sentence.
    void Init()
    {
      this->sentence.clear();
      this->ProprietarySentenceIndicator = false;
      this->ParseStatus = NMEA_NOT_PARSED;

      this->Latitude = -1;
      this->LatitudeSector = 'N';
      this->Longitude = -1;
      this->LongitudeSector = 'E';
      this->UTC = -1;
      this->Status = 'V';
      this->ReceiverStatus = 'V';
    }

    //! Assignment
    GLL operator = ( GLL const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->Latitude = rhs.Latitude;
      this->LatitudeSector = rhs.LatitudeSector;
      this->Longitude = rhs.Longitude;
      this->LongitudeSector = rhs.LongitudeSector;
      this->UTC = rhs.UTC;
      this->Status = rhs.Status;
      this->ReceiverStatus = rhs.ReceiverStatus;

      return *this;
    }

    //! Constructor
    GLL  ( GLL const & rhs )
    {
      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->Latitude = rhs.Latitude;
      this->LatitudeSector = rhs.LatitudeSector;
      this->Longitude = rhs.Longitude;
      this->LongitudeSector = rhs.LongitudeSector;
      this->UTC = rhs.UTC;
      this->Status = rhs.Status;
      this->ReceiverStatus = rhs.ReceiverStatus;

    }

    //! Equality
    bool operator == ( GLL const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
        this->Latitude == rhs.Latitude &&
        this->LatitudeSector == rhs.LatitudeSector &&
        this->Longitude == rhs.Longitude &&
        this->LongitudeSector == rhs.LongitudeSector &&
        this->UTC == rhs.UTC &&
        this->Status == rhs.Status &&
        this->ReceiverStatus == rhs.ReceiverStatus
      );
    }

    //! Inequality
    bool operator != ( GLL const & rhs )
    {
      return !(*this == rhs);
    }

    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, GLL const & p)
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
    friend std::istream& operator>>(std::istream& is, GLL & p)
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
    //! Set the latitude.
    SetMacro(Latitude,double);
    //! Get the latitude.
    GetMacro(Latitude,double);
    //! Set the North/South flag N = North, S = South.
    SetMacro(LatitudeSector,char);
    //! Get the North/South flag N = North, S = South.
    GetMacro(LatitudeSector,char);
    //! Set the longitude.
    SetMacro(Longitude,double);
    //! Get the longitude.
    GetMacro(Longitude,double);
    //! Set the East/West flag E = East, W = West.
    SetMacro(LongitudeSector,char);
    //! Get the East/West flag E = East, W = West.
    GetMacro(LongitudeSector,char);
    //! Set Coordinated Universal Time.
    SetMacro(UTC,double);
    //! Get Coordinated Universal Time.
    GetMacro(UTC,double);
    //! Set the status of the sentence. A = Data Valid, V = Data Invalid
    SetMacro(Status,char);
    //! Get the status of the sentence. A = Data Valid, V = Data Invalid
    GetMacro(Status,char);
    //! Set the receiver status field (A: aquiring real data, S: simulating data).
    SetMacro(ReceiverStatus,char);
    //! Get the receiver status field (A: aquiring real data, S: simulating data).
    GetMacro(ReceiverStatus,char);

  protected:
    double Latitude;
    char LatitudeSector;
    double Longitude;
    char LongitudeSector;
    double UTC;
    char Status;
    char ReceiverStatus;
  };

} // Namespace NMEA.
} // Namespace UF.


#endif
