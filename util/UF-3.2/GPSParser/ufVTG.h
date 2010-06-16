//#pragma once

#if !defined(NMEAVTG_H)
#define NMEAVTG_H

#include "ufNMEASentence.h"

namespace UF {
namespace NMEA {
  //! VTG: Course and Speed Over Ground.
  /*!
  CLASS
    VTG


    Introduction

    This message is not output unless positions are being computed. The VTG contains
    course over ground referenced to both true and magnetic north, and speed over
    ground in kilometers per hour and nautical miles per hour (knots).

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
      SetCOG(COG);
      GOG = GetCOG();
      \endcode

    Output

    The message is output in the format: $--VTG,f1,c2,f3,c4,f5,c6,f7,c8*hh

        <table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="679" id="AutoNumber1">
      <tr>
        <td width="88">Parameter</td>
        <td width="390">Description</td>
        <td width="197">Range</td>
      </tr>
      <tr>
        <td width="88">f1</td>
        <td width="390">Course over ground; referenced to true north</td>
        <td width="197">000.00 to 359.99</td>
      </tr>
      <tr>
        <td width="88">c2</td>
        <td width="390">North reference indicator (always T; true north)</td>
        <td width="197">T</td>
      </tr>
      <tr>
        <td width="88">f3</td>
        <td width="390">Course over ground; referenced to magnetic north</td>
        <td width="197">000.00 to 359.99</td>
      </tr>
      <tr>
        <td width="88">c4</td>
        <td width="390">North reference indicator (always M; magnetic
                north) </td>
        <td width="197">M</td>
      </tr>
            <tr>
        <td width="88">f5</td>
        <td width="390">Speed over ground (knots)</td>
        <td width="197">000.00 to 999.99</td>
      </tr>
            <tr>
        <td width="88">c6</td>
        <td width="390">Speed unit of measure (always N; nautical miles
                per hour)</td>
        <td width="197">N</td>
      </tr>
            <tr>
        <td width="88">f7</td>
        <td width="390">Speed over ground (kilometers per hour)</td>
        <td width="197">000.00 to 999.99</td>
      </tr>
            <tr>
        <td width="88">c8</td>
        <td width="390">Speed unit of measure (always K; KPH) </td>
        <td width="197">K</td>
      </tr>
      <tr>
        <td width="88">*hh</td>
        <td width="390">Checksum</td>
        <td width="197">2-character hex</td>
      </tr>
    </table>


    Here is a typical message: $GPVTG,179.00,T,193.00,M,000.11,N,000.20,K*3E


    Setting

    Thales Navigation

    $PASHS,NME,VTG,x,c,[f]

    Enable/disable the velocity/course message on port x, where x is the output port,
    and c is ON or OFF, and f is the optional message output rate ranging from 0.05
    to 999 seconds depending upon the measurement update rate option installed.
    The G12 does not output this message unless it computes a position.
    If the command is set without a period, the G12 uses the period set by the
    $PASHS,NME,PER command. If the $PASHS,NME,PER command is issued after
    this message period has been set, the period resets to the PER setting.

    Example

    Enable VTG message or port B.

    $PASHS,NME,VTG,B,ON

    $PASHQ,VTG,[c1]

    This command allows you to query the VTG message, where c1 is the optional port
    designator for the output of the response. If a port is not specified, the receiver
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
  class UF_GPS_PARSER_EXPORT VTG :
    public NMEASentence
  {
  public:
    VTG();
    virtual ~VTG();

    //! Initialise the fields in a sentence.
    void Init()
    {
      this->sentence.clear();
      this->ProprietarySentenceIndicator = false;
      this->ParseStatus = NMEA_NOT_PARSED;

      this->COG = -1;
      this->NorthReferenceIndicator = 'T';
      this->MagneticCOG = -1;
      this->MagneticNorthReferenceIndicator = 'M';
      this->SOG = -1;
      this->SpeedUnitOfMeasure = 'N';
      this->KPH_SOG = -1;
      this->KPH_SpeedUnitOfMeasure = 'K';
    }

    //! Assignment
    VTG operator = ( VTG const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->COG = rhs.COG;
      this->NorthReferenceIndicator = rhs.NorthReferenceIndicator;
      this->MagneticCOG = rhs.MagneticCOG;
      this->MagneticNorthReferenceIndicator = rhs.MagneticNorthReferenceIndicator;
      this->SOG = rhs.SOG;
      this->SpeedUnitOfMeasure = rhs.SpeedUnitOfMeasure;
      this->KPH_SOG = rhs.KPH_SOG;
      this->KPH_SpeedUnitOfMeasure = rhs.KPH_SpeedUnitOfMeasure;

      return *this;
    }

    //! Constructor
    VTG  ( VTG const & rhs )
    {

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->COG = rhs.COG;
      this->NorthReferenceIndicator = rhs.NorthReferenceIndicator;
      this->MagneticCOG = rhs.MagneticCOG;
      this->MagneticNorthReferenceIndicator = rhs.MagneticNorthReferenceIndicator;
      this->SOG = rhs.SOG;
      this->SpeedUnitOfMeasure = rhs.SpeedUnitOfMeasure;
      this->KPH_SOG = rhs.KPH_SOG;
      this->KPH_SpeedUnitOfMeasure = rhs.KPH_SpeedUnitOfMeasure;
    }

    //! Equality
    bool operator == ( VTG const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
        this->COG == rhs.COG &&
        this->NorthReferenceIndicator == rhs.NorthReferenceIndicator &&
        this->MagneticCOG == rhs.MagneticCOG &&
        this->MagneticNorthReferenceIndicator == rhs.MagneticNorthReferenceIndicator &&
        this->SOG == rhs.SOG &&
        this->SpeedUnitOfMeasure == rhs.SpeedUnitOfMeasure &&
        this->KPH_SOG == rhs.KPH_SOG &&
        this->KPH_SpeedUnitOfMeasure == rhs.KPH_SpeedUnitOfMeasure
      );
    }

    //! Inequality
    bool operator != ( VTG const & rhs )
    {
      return !(*this == rhs);
    }


    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, VTG const & p)
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
    friend std::istream& operator>>(std::istream& is, VTG & p)
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

    //! Set course over ground; referenced to true north.
    SetMacro(COG,double);
    //! Get course over ground; referenced to true north.
    GetMacro(COG,double);
    //! Set north reference indicator (always T; true north).
    SetMacro(NorthReferenceIndicator,char);
    //! Get north reference indicator (always T; true north).
    GetMacro(NorthReferenceIndicator,char);

    //! Set course over ground; referenced to magnetic north.
    SetMacro(MagneticCOG,double);
    //! Get course over ground; referenced to magnetic north.
    GetMacro(MagneticCOG,double);
    //! Set north reference indicator (always M; magnetic north).
    SetMacro(MagneticNorthReferenceIndicator,char);
    //! Get north reference indicator (always M; magnetic north).
    GetMacro(MagneticNorthReferenceIndicator,char);

    //! Set speed over ground (knots).
    SetMacro(SOG,double);
    //! Get speed over ground (knots).
    GetMacro(SOG,double);
    //! Set speed unit of measure (always N; nautical miles per hour).
    SetMacro(SpeedUnitOfMeasure,char);
    //! Get speed unit of measure (always N; nautical miles per hour).
    GetMacro(SpeedUnitOfMeasure,char);

    //! Set speed over ground (knots).
    SetMacro(KPH_SOG,double);
    //! Get speed over ground (knots).
    GetMacro(KPH_SOG,double);
    //! Set speed unit of measure (always K; KPH).
    SetMacro(KPH_SpeedUnitOfMeasure,char);
    //! Get speed unit of measure (always K; KPH).
    GetMacro(KPH_SpeedUnitOfMeasure,char);

  protected:
    double COG;
    char NorthReferenceIndicator;
    double MagneticCOG;
    char MagneticNorthReferenceIndicator;
    double SOG;
    char SpeedUnitOfMeasure;
    double KPH_SOG;
    char KPH_SpeedUnitOfMeasure;
  };

} // Namespace NMEA.
} // Namespace UF.


#endif
