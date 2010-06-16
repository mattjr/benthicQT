//#pragma once

#if !defined(RMB_H)
#define RMB_H

#include "ufNMEASentence.h"

namespace UF {
namespace NMEA {
  //! RMB: Recommended minimum navigation information when following a route.
  /*!
  CLASS
    RMB


    Introduction

    The RMB message contains the recommended minimum navigation information
    (sent by the navigational receiver when a destination waypoint is active).

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

    The message is output in the format: $--RMB,c1,d2,c3,s4,s5,m6,c7,m8,c9,d10,d11,d12,c13,c14*hh

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
         V = Invalid</td>
        <td width="197">A,V</td>
      </tr>
      <tr>
        <td width="88">d2</td>
        <td width="390">Cross-track error (nautical miles, 9.9 max.)</td>
        <td width="197">0...9.9</td>
      </tr>
      <tr>
        <td width="88">c3</td>
        <td width="390">Direction to steer, Left to correct (or R =
                Right)</td>
        <td width="197">L,R</td>
      </tr>
      <tr>
        <td width="88">s4</td>
        <td width="390">Origin waypoint ID</td>
        <td width="197">&nbsp;</td>
      </tr>
            <tr>
        <td width="88">s5</td>
        <td width="390">Destination waypoint ID</td>
        <td width="197">&nbsp;</td>
      </tr>
            <tr>
        <td width="88">m6</td>
        <td width="390">Destination waypoint latitude (ddmm.mmmmm) </td>
        <td width="197">0 - 90</td>
      </tr>
            <tr>
        <td width="88">c7</td>
        <td width="390">Latitude sector </td>
        <td width="197">N(orth) or S(outh)</td>
      </tr>
            <tr>
        <td width="88">m8</td>
        <td width="390">Destination waypoint longitude</td>
        <td width="197">0 - 180</td>
      </tr>
            <tr>
        <td width="88">c9</td>
        <td width="390">Longitude sector </td>
        <td width="197">E(ast) or W(est)</td>
      </tr>
            <tr>
        <td width="88">d10</td>
        <td width="390">Range to destination, nautical miles</td>
        <td width="197">&nbsp;</td>
      </tr>
            <tr>
        <td width="88">d11</td>
        <td width="390">True bearing to destination</td>
        <td width="197">0 - 359</td>
      </tr>
            <tr>
        <td width="88">d12</td>
        <td width="390">Velocity towards destination, knots</td>
        <td width="197">&nbsp;</td>
      </tr>
            <tr>
        <td width="88">c13</td>
        <td width="390">Arrival Alarm:<br>
         A = Arrived<br>
         V = Not Arrived</td>
        <td width="197">A,V</td>
      </tr>
      <tr>
        <td width="88">c14</td>
        <td width="390">Receiver Status:<br>
         A = Navigating<br>
         S = Simulating<br>
        This field may not be present in the output of some devices.</td>
        <td width="197">A,S</td>
      </tr>
      <tr>
        <td width="88">*hh</td>
        <td width="390">Checksum</td>
        <td width="197">2-character hex</td>
      </tr>
    </table>

    eg1. $GPRMB,A,0.66,L,003,004,4917.24,N,12309.57,W,001.3,052.5,000.5,V*0B

    eg2. $GPRMB,A,4.08,L,EGLL,EGLM,5130.02,N,00046.34,W,004.6,213.9,122.9,A*3D

    Here is a typical message: $GPRMB,V,0.01,R,AJPM,BURNS F,3342.6514,S,15034.2922,E,0.027,61.2,10.6,A,S*6C


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
  class UF_GPS_PARSER_EXPORT RMB :
    public NMEASentence
  {
  public:
    RMB();
    virtual ~RMB();

    //! Initialise the fields in a sentence.
    void Init()
    {
      this->sentence.clear();
      this->ProprietarySentenceIndicator = false;
      this->ParseStatus = NMEA_NOT_PARSED;

      this->Status = 'A';
      this->CrossTrackError = -1;
      this->DirectionToSteer = 'L';
      this->OriginWaypoint.erase();
      this->DestinationWaypoint.erase();
      this->DestinationLatitude = -1;
      this->DestinationLatitudeSector = 'N';
      this->DestinationLongitude = -1;
      this->DestinationLongitudeSector = 'E';
      this->DestinationRange = -1;
      this->DestinationBearing = -1;
      this->DestinationClosingVelocity = -1;
      this->ArrivalAlarm = 'V';
      this->ReceiverStatus = 'S';
    }

    //! Assignment
    RMB operator = ( RMB const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->Status = rhs.Status;
      this->CrossTrackError = rhs.CrossTrackError;
      this->DirectionToSteer = rhs.DirectionToSteer;
      this->OriginWaypoint = rhs.OriginWaypoint;
      this->DestinationWaypoint = rhs.DestinationWaypoint;
      this->DestinationLatitude = rhs.DestinationLatitude;
      this->DestinationLatitudeSector = rhs.DestinationLatitudeSector;
      this->DestinationLongitude = rhs.DestinationLongitude;
      this->DestinationLongitudeSector = rhs.DestinationLongitudeSector;
      this->DestinationRange = rhs.DestinationRange;
      this->DestinationBearing = rhs.DestinationBearing;
      this->DestinationClosingVelocity = rhs.DestinationClosingVelocity;
      this->ArrivalAlarm = rhs.ArrivalAlarm;
      this->ReceiverStatus = rhs.ReceiverStatus;

      return *this;
    }

    //! Constructor
    RMB  ( RMB const & rhs )
    {

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->Status = rhs.Status;
      this->CrossTrackError = rhs.CrossTrackError;
      this->DirectionToSteer = rhs.DirectionToSteer;
      this->OriginWaypoint = rhs.OriginWaypoint;
      this->DestinationWaypoint = rhs.DestinationWaypoint;
      this->DestinationLatitude = rhs.DestinationLatitude;
      this->DestinationLatitudeSector = rhs.DestinationLatitudeSector;
      this->DestinationLongitude = rhs.DestinationLongitude;
      this->DestinationLongitudeSector = rhs.DestinationLongitudeSector;
      this->DestinationRange = rhs.DestinationRange;
      this->DestinationBearing = rhs.DestinationBearing;
      this->DestinationClosingVelocity = rhs.DestinationClosingVelocity;
      this->ArrivalAlarm = rhs.ArrivalAlarm;
      this->ReceiverStatus = rhs.ReceiverStatus;

    }

    //! Equality
    bool operator == ( RMB const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
        this->Status == rhs.Status &&
        this->CrossTrackError == rhs.CrossTrackError &&
        this->DirectionToSteer == rhs.DirectionToSteer &&
        this->OriginWaypoint == rhs.OriginWaypoint &&
        this->DestinationWaypoint == rhs.DestinationWaypoint &&
        this->DestinationLatitude == rhs.DestinationLatitude &&
        this->DestinationLatitudeSector == rhs.DestinationLatitudeSector &&
        this->DestinationLongitude == rhs.DestinationLongitude &&
        this->DestinationLongitudeSector == rhs.DestinationLongitudeSector &&
        this->DestinationRange == rhs.DestinationRange &&
        this->DestinationBearing == rhs.DestinationBearing &&
        this->DestinationClosingVelocity == rhs.DestinationClosingVelocity &&
        this->ArrivalAlarm == rhs.ArrivalAlarm &&
        this->ReceiverStatus == rhs.ReceiverStatus
      );
    }

    //! Inequality
    bool operator != ( RMB const & rhs )
    {
      return !(*this == rhs);
    }


    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, RMB const & p)
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
    friend std::istream& operator>>(std::istream& is, RMB & p)
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
    //! Set the status of the sentence. A = Data Valid, V = Data Invalid
    SetMacro(Status,char);
    //! Get the status of the sentence. A = Data Valid, V = Data Invalid
    GetMacro(Status,char);
    //! Set the crosstrack error in nautical miles
    SetMacro(CrossTrackError,double);
    //! Get the crosstrack error in nautical miles
    GetMacro(CrossTrackError,double);
    //! Set the direction to steer L (left) or R (right) to correct the crosstrack error.
    SetMacro(DirectionToSteer,char);
    //! Get the direction to steer L (left) or R (right) to correct the crosstrack error.
    GetMacro(DirectionToSteer,char);
    //! Set the origin waypoint.
    SetMacro(OriginWaypoint,std::string);
    //! Get the origin waypoint.
    GetMacro(OriginWaypoint,std::string);
    //! Set the destination waypoint.
    SetMacro(DestinationWaypoint,std::string);
    //! Get the destination waypoint.
    GetMacro(DestinationWaypoint,std::string);
    //! Set the latitude.
    SetMacro(DestinationLatitude,double);
    //! Get the latitude.
    GetMacro(DestinationLatitude,double);
    //! Set the North/South flag N = North, S = South.
    SetMacro(DestinationLatitudeSector,char);
    //! Get the North/South flag N = North, S = South.
    GetMacro(DestinationLatitudeSector,char);
    //! Set the longitude.
    SetMacro(DestinationLongitude,double);
    //! Get the longitude.
    GetMacro(DestinationLongitude,double);
    //! Set the East/West flag E = East, W = West.
    SetMacro(DestinationLongitudeSector,char);
    //! Get the East/West flag E = East, W = West.
    GetMacro(DestinationLongitudeSector,char);

    //! Set range to destination, nautical miles.
    SetMacro(DestinationRange,double);
    //! Get range to destination, nautical miles.
    GetMacro(DestinationRange,double);
    //! Set true bearing to destination.
    SetMacro(DestinationBearing,double);
    //! Get true bearing to destination.
    GetMacro(DestinationBearing,double);
    //! Set velocity towards destination, knots.
    SetMacro(DestinationClosingVelocity,double);
    //! Get velocity towards destination, knots.
    GetMacro(DestinationClosingVelocity,double);
    //! Set the arrival alarm  A = arrived, V = not arrived.
    SetMacro(ArrivalAlarm,char);
    //! Get the arrival alarm  A = arrived, V = not arrived.
    GetMacro(ArrivalAlarm,char);
    //! Set the receiver status field (A: aquiring real data, S: simulating data).
    SetMacro(ReceiverStatus,char);
    //! Get the receiver status field (A: aquiring real data, S: simulating data).
    GetMacro(ReceiverStatus,char);

  protected:
    char Status;
    double CrossTrackError;
    char DirectionToSteer;
    std::string OriginWaypoint;
    std::string DestinationWaypoint;

    double DestinationLatitude;
    char DestinationLatitudeSector;
    double DestinationLongitude;
    char DestinationLongitudeSector;

    double DestinationRange;
    double DestinationBearing;
    double DestinationClosingVelocity;
    char ArrivalAlarm;
    char ReceiverStatus;
  };

} // Namespace NMEA.
} // Namespace UF.


#endif
