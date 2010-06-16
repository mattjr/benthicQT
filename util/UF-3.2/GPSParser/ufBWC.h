//#pragma once

#if !defined(NMEABWC_H)
#define NMEABWC_H

#include "ufNMEASentence.h"
namespace UF {
namespace NMEA {
  //! BWC: Bearing & Distance to Waypoint, Great Circle.
  /*!
  CLASS
    BWC


    Introduction

    The BWC message contains the great circle bearing and distance to a waypoint.

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

    The message is output in the format: $--BWC,m1,m2,c3,m4,c5,m6,c7,m8,c9,m10,c11,s12,c13*hh

      <table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="679" id="AutoNumber1">
      <tr>
        <td width="88">Parameter</td>
        <td width="390">Description</td>
        <td width="197">Range</td>
      </tr>
      <tr>
        <td width="88">m1</td>
        <td width="390">UTC time (hhmmss.ss) of the position fix </td>
        <td width="197">000000.00 to 235959.90</td>
      </tr>
            <tr>
        <td width="88">m2</td>
        <td width="390">Latitude of the waypoint (ddmm.mmmmm) </td>
        <td width="197">0 - 90</td>
      </tr>
            <tr>
        <td width="88">c3</td>
        <td width="390">Latitude sector </td>
        <td width="197">N(orth) or S(outh)</td>
      </tr>
            <tr>
        <td width="88">m4</td>
        <td width="390">Longitude of the waypoint (dddmm.mmmmm)</td>
        <td width="197">0 - 180</td>
      </tr>
            <tr>
        <td width="88">c5</td>
        <td width="390">Longitude sector </td>
        <td width="197">E(ast) or W(est)</td>
      </tr>
      <tr>
        <td width="88">m6</td>
        <td width="390">True Bearing to waypoint.</td>
        <td width="197">000.0 - 359.9</td>
      </tr>
      <tr>
        <td width="88">c7</td>
        <td width="390">True bearing indicator, always T.</td>
        <td width="197">T(rue)</td>
      </tr>
      <tr>
        <td width="88">m8</td>
        <td width="390">Magnetic bearing to waypoint.</td>
        <td width="197">000.0 - 359.9</td>
      </tr>
      <tr>
        <td width="88">c9</td>
        <td width="390">Magnetic bearing indicator, always M.</td>
        <td width="197">M(agnetic)</td>
      </tr>
      <tr>
        <td width="88">m10</td>
        <td width="390">Distance to waypoint.</td>
        <td width="197">&nbsp;</td>
      </tr>
            <tr>
        <td width="88">c11</td>
        <td width="390">Distance units (n: Nautical miles)</td>
        <td width="197">N</td>
      </tr>
      <tr>
        <td width="88">s12</td>
        <td width="390">Waypoint Id.</td>
        <td width="197">&nbsp;</td>
      </tr>
      <tr>
        <td width="88">c13</td>
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

    eg1. $GPBWC,081837,,,,,,T,,M,,N,*13

    eg2. $GPBWC,220516,5130.02,N,00046.34,W,213.8,T,218.0,M,0004.6,N,EGLM*11

    Here is a typical message: $GPBWC,234936,3342.6514,S,15034.2922,E,64.6,T,52.2,M,0.021,N,BURNS FARM,S*02


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
  class UF_GPS_PARSER_EXPORT BWC :
    public NMEASentence
  {
  public:
    BWC();
    virtual ~BWC();

    //! Initialise the fields in a sentence.
    void Init()
    {
      this->sentence.clear();
      this->ProprietarySentenceIndicator = false;
      this->ParseStatus = NMEA_NOT_PARSED;

      this->UTC = -1;
      this->Latitude = -1;
      this->LatitudeSector = 'N';
      this->Longitude = -1;
      this->LongitudeSector = 'E';
      this->TrueBearing = -1;
      this->NorthReferenceIndicator = 'T';
      this->MagneticBearing = -1;
      this->MagneticNorthReferenceIndicator = 'M';
      this->Distance = -1;
      this->DistanceUnit = 'M';
      this->Waypoint.clear();
      this->ReceiverStatus = 'V';
    }

    //! Assignment
    BWC operator = ( BWC const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->UTC = rhs.UTC;
      this->Latitude = rhs.Latitude;
      this->LatitudeSector = rhs.LatitudeSector;
      this->Longitude = rhs.Longitude;
      this->LongitudeSector = rhs.LongitudeSector;
      this->TrueBearing = rhs.TrueBearing;
      this->NorthReferenceIndicator = rhs.NorthReferenceIndicator;
      this->MagneticBearing = rhs.MagneticBearing;
      this->MagneticNorthReferenceIndicator = rhs.MagneticNorthReferenceIndicator;
      this->Distance = rhs.Distance;
      this->DistanceUnit = rhs.DistanceUnit;
      this->Waypoint = rhs.Waypoint;
      this->ReceiverStatus = rhs.ReceiverStatus;

      return *this;
    }

    //! Constructor
    BWC  ( BWC const & rhs )
    {

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->UTC = rhs.UTC;
      this->Latitude = rhs.Latitude;
      this->LatitudeSector = rhs.LatitudeSector;
      this->Longitude = rhs.Longitude;
      this->LongitudeSector = rhs.LongitudeSector;
      this->TrueBearing = rhs.TrueBearing;
      this->NorthReferenceIndicator = rhs.NorthReferenceIndicator;
      this->MagneticBearing = rhs.MagneticBearing;
      this->MagneticNorthReferenceIndicator = rhs.MagneticNorthReferenceIndicator;
      this->Distance = rhs.Distance;
      this->DistanceUnit = rhs.DistanceUnit;
      this->Waypoint = rhs.Waypoint;
      this->ReceiverStatus = rhs.ReceiverStatus;

    }

    //! Equality
    bool operator == ( BWC const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
        this->UTC == rhs.UTC &&
        this->Latitude == rhs.Latitude &&
        this->LatitudeSector == rhs.LatitudeSector &&
        this->Longitude == rhs.Longitude &&
        this->LongitudeSector == rhs.LongitudeSector &&
        this->TrueBearing == rhs.TrueBearing &&
        this->NorthReferenceIndicator == rhs.NorthReferenceIndicator &&
        this->MagneticBearing == rhs.MagneticBearing &&
        this->MagneticNorthReferenceIndicator == rhs.MagneticNorthReferenceIndicator &&
        this->Distance == rhs.Distance &&
        this->DistanceUnit == rhs.DistanceUnit &&
        this->Waypoint == rhs.Waypoint &&
        this->ReceiverStatus == rhs.ReceiverStatus
      );
    }

    //! Inequality
    bool operator != ( BWC const & rhs )
    {
      return !(*this == rhs);
    }

     //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, BWC const & p)
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
    friend std::istream& operator>>(std::istream& is, BWC & p)
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
    //! Set Coordinated Universal Time.
    SetMacro(UTC,double);
    //! Get Coordinated Universal Time.
    GetMacro(UTC,double);
    //! Set the latitude of the next waypoint.
    SetMacro(Latitude,double);
    //! Get the latitude of the next waypoint.
    GetMacro(Latitude,double);
    //! Set the North/South flag N = North, S = South.
    SetMacro(LatitudeSector,char);
    //! Get the North/South flag N = North, S = South.
    GetMacro(LatitudeSector,char);
    //! Set the longitude of the next waypoint.
    SetMacro(Longitude,double);
    //! Get the longitude of the next waypoint.
    GetMacro(Longitude,double);
    //! Set the East/West flag E = East, W = West.
    SetMacro(LongitudeSector,char);
    //! Get the East/West flag E = East, W = West.
    GetMacro(LongitudeSector,char);

    //! Set the bearing to waypoint; referenced to true north.
    SetMacro(TrueBearing,double);
    //! Get the bearing to waypoint; referenced to true north.
    GetMacro(TrueBearing,double);
    //! Set north reference indicator (always T; true north).
    SetMacro(NorthReferenceIndicator,char);
    //! Get north reference indicator (always T; true north).
    GetMacro(NorthReferenceIndicator,char);

    //! Set the magnetic bearing to waypoint; referenced to magnetic north.
    SetMacro(MagneticBearing,double);
    //! Get the magnetic bearing to waypoint; referenced to magnetic north.
    GetMacro(MagneticBearing,double);
    //! Set north reference indicator (always M; magnetic north).
    SetMacro(MagneticNorthReferenceIndicator,char);
    //! Get north reference indicator (always M; magnetic north).
    GetMacro(MagneticNorthReferenceIndicator,char);

    //! Set the distance to the waypoint.
    SetMacro(Distance,double);
    //! Get the distance to the waypoint.
    GetMacro(Distance,double);
    //! Set the distance unit to the waypoint (n: nautical miles).
    SetMacro(DistanceUnit,char);
    //! Get the distance unit to the waypoint (n: nautical miles).
    GetMacro(DistanceUnit,char);

    //! Set the waypoint.
    SetMacro(Waypoint,std::string);
    //! Get the waypoint.
    GetMacro(Waypoint,std::string);

    //! Set the receiver status field (A: aquiring real data, S: simulating data).
    SetMacro(ReceiverStatus,char);
    //! Get the receiver status field (A: aquiring real data, S: simulating data).
    GetMacro(ReceiverStatus,char);

  protected:
    double UTC;
    double Latitude;
    char LatitudeSector;
    double Longitude;
    char LongitudeSector;

    double TrueBearing;
    char NorthReferenceIndicator;
    double MagneticBearing;
    char MagneticNorthReferenceIndicator;

    double Distance;
    char DistanceUnit;

    std::string Waypoint;

    char ReceiverStatus;

  };

} // Namespace NMEA.
} // Namespace UF.


#endif
