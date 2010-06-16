//#pragma once

#if !defined(NMEABOD_H)
#define NMEABOD_H

#include "ufNMEASentence.h"

namespace UF {
namespace NMEA {
  //! BOD: Bearing, origin to destination.
  /*!
  CLASS
    BOD


    Introduction

    The BOD message contains information relating to bearing from the
    origin to the destination.
    This sentence is transmitted when a route is active.


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

      The message is output in the format: $PGRMZ,m1,c2,m3,c4,s5,s6*hh

      <table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="679" id="AutoNumber1">
      <tr>
        <td width="88">Parameter</td>
        <td width="390">Description</td>
        <td width="197">Range</td>
      </tr>
      <tr>
        <td width="88">m1</td>
        <td width="390">True Bearing</td>
        <td width="197">000.0 - 359.9</td>
      </tr>
      <tr>
        <td width="88">c2</td>
        <td width="390">True bearing indicator, always T.</td>
        <td width="197">T(rue)</td>
      </tr>
      <tr>
        <td width="88">m3</td>
        <td width="390">Magnetic bearing</td>
        <td width="197">000.0 - 359.9</td>
      </tr>
      <tr>
        <td width="88">c4</td>
        <td width="390">Magnetic bearing indicator, always M.</td>
        <td width="197">M(agnetic)</td>
      </tr>
      <tr>
        <td width="88">s5</td>
        <td width="390">Destination Waypoint Id.</td>
        <td width="197">&nbsp;</td>
      </tr>
      <tr>
        <td width="88">s6</td>
        <td width="390">Origin Waypoint Id.</td>
        <td width="197">&nbsp;</td>
      </tr>
      <tr>
        <td width="88">hh</td>
        <td width="390">Checksum 2-character hex</td>
        <td width="197">&nbsp;</td>
      </tr>
    </table>

    Example 1: $GPBOD,099.3,T,105.6,M,POINTB,*01

    This sentence is transmitted in the GOTO mode, without an active route on your
    GPS.

    WARNING: this is the bearing from the moment you press enter in the GOTO page
    to the destination waypoint and is NOT updated dynamically! To update the
    information, (current bearing to waypoint), you will have to press enter in
    the GOTO page again.

    Example 2: $GPBOD,097.0,T,103.2,M,POINTB,POINTA*52

    It contains the active leg information: origin waypoint "POINTA" and destination
    waypoint "POINTB", bearing between the two points 97.0 True, 103.2 Magnetic.
    It does NOT display  the bearing from current location to destination waypoint!

    WARNING Again this information does not change until you are on the next leg
    of the route. (The bearing from POINTA to POINTB does not change during the
    time you are on this leg.)



    Here is a typical message: $GPBOD,42.3,T,29.9,M,BURNS FARM,AJPM*36


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
  class UF_GPS_PARSER_EXPORT BOD :
      public NMEASentence
  {
  public:
    BOD();
    virtual ~BOD();

    //! Initialise the object.
    void Init()
    {
      this->sentence.clear();
      this->ProprietarySentenceIndicator = false;
      this->ParseStatus = NMEA_NOT_PARSED;

      this->TrueBearing = -1;
      this->NorthReferenceIndicator = 'T';
      this->MagneticBearing = -1;
      this->MagneticNorthReferenceIndicator = 'M';
      this->DestinationWaypoint.clear();
      this->OriginWaypoint.clear();
    }

    //! Assignment
    BOD operator = ( BOD const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->TrueBearing = rhs.TrueBearing;
      this->NorthReferenceIndicator = rhs.NorthReferenceIndicator;
      this->MagneticBearing = rhs.MagneticBearing;
      this->MagneticNorthReferenceIndicator = rhs.MagneticNorthReferenceIndicator;
      this->DestinationWaypoint = rhs.DestinationWaypoint;
      this->OriginWaypoint = rhs.OriginWaypoint;

      return *this;
    }

    //! Constructor
    BOD  ( BOD const & rhs )
    {
      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->TrueBearing = rhs.TrueBearing;
      this->NorthReferenceIndicator = rhs.NorthReferenceIndicator;
      this->MagneticBearing = rhs.MagneticBearing;
      this->MagneticNorthReferenceIndicator = rhs.MagneticNorthReferenceIndicator;
      this->DestinationWaypoint = rhs.DestinationWaypoint;
      this->OriginWaypoint = rhs.OriginWaypoint;

    }

    //! Equality
    bool operator == ( BOD const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
        this->TrueBearing == rhs.TrueBearing &&
        this->NorthReferenceIndicator == rhs.NorthReferenceIndicator &&
        this->MagneticBearing == rhs.MagneticBearing &&
        this->MagneticNorthReferenceIndicator == rhs.MagneticNorthReferenceIndicator &&
        this->DestinationWaypoint == rhs.DestinationWaypoint &&
        this->OriginWaypoint == rhs.OriginWaypoint
        );
    }

    //! Inequality
    bool operator != ( BOD const & rhs )
    {
      return !(*this == rhs);
    }

    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, BOD const & p)
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
    friend std::istream& operator>>(std::istream& is, BOD & p)
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
    //! Set true bearing; referenced to true north.
    SetMacro(TrueBearing,double);
    //! Get true bearing; referenced to true north.
    GetMacro(TrueBearing,double);
    //! Set north reference indicator (always T; true north).
    SetMacro(NorthReferenceIndicator,char);
    //! Get north reference indicator (always T; true north).
    GetMacro(NorthReferenceIndicator,char);

    //! Set magnetic bearing; referenced to true north.
    SetMacro(MagneticBearing,double);
    //! Get magnetic bearing; referenced to true north.
    GetMacro(MagneticBearing,double);
    //! Set north reference indicator (always M; magnetic north).
    SetMacro(MagneticNorthReferenceIndicator,char);
    //! Get north reference indicator (always M; magnetic north).
    GetMacro(MagneticNorthReferenceIndicator,char);

    //! Set the destination waypoint.
    SetMacro(DestinationWaypoint,std::string);
    //! Get the destination waypoint.
    GetMacro(DestinationWaypoint,std::string);
    //! Set the origin waypoint.
    SetMacro(OriginWaypoint,std::string);
    //! Get the origin waypoint.
    GetMacro(OriginWaypoint,std::string);

  protected:
    double TrueBearing;
    char NorthReferenceIndicator;
    double MagneticBearing;
    char MagneticNorthReferenceIndicator;
    std::string DestinationWaypoint;
    std::string OriginWaypoint;
  };

} // Namespace NMEA.
} // Namespace UF.

#endif
