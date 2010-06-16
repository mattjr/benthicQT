//#pragma once

#if !defined(NMEAWPL_H)
#define NMEAWPL_H

#include "ufNMEASentence.h"

namespace UF {
namespace NMEA {
  //! WPL: Waypoint location.
  /*!
  CLASS
    WPL


    Introduction

    The WPL message contains the location and name of the waypoint.

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

    The message is output in the format: $--WPL,m1,c2,m3,c4,s5*hh

    When a route is active, this sentence is sent once for each  waypoint in the route,
    in sequence. When all waypoints have been reported the cycle starts again.

    <table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="679" id="AutoNumber1">
    <tr>
        <td width="88">Parameter</td>
        <td width="390">Description</td>
        <td width="197">Range</td>
      </tr>
      <tr>
        <td width="88">m1</td>
        <td width="390">Latitude of the n<sup>th</sup> waypoint on the
                list (ddmm.mmmmm) </td>
        <td width="197">0 - 90</td>
      </tr>
      <tr>
        <td width="88">c2</td>
        <td width="390">Latitude sector </td>
        <td width="197">N(orth) or S(outh)</td>
      </tr>
      <tr>
        <td width="88">m3</td>
        <td width="390">Longitude of the n<sup>th</sup> waypoint on the
                list (dddmm.mmmmm)</td>
        <td width="197">0 - 180</td>
      </tr>
      <tr>
        <td width="88">c4</td>
        <td width="390">Longitude sector </td>
        <td width="197">E(ast) or W(est)</td>
      </tr>
      <tr>
        <td width="88">s5</td>
        <td width="390">Waypoint Id</td>
        <td width="197">&nbsp;</td>
      </tr>
      <tr>
        <td width="88">hh</td>
        <td width="390">Checksum 2-character hex</td>
        <td width="197">&nbsp;</td>
      </tr>
    </table>

    eg1. $GPWPL,4917.16,N,12310.64,W,003*65

    eg2.  $GPWPL,5128.62,N,00027.58,W,EGLL*59

    Here is a typical message: $GPWPL,3342.6829,S,15034.2577,E,AJPM*47

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
  class UF_GPS_PARSER_EXPORT WPL :
    public NMEASentence
  {
  public:
    WPL();
    virtual ~WPL();

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
      this->Waypoint.clear();
    }

    //! Assignment
    WPL operator = ( WPL const & rhs )
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
      this->Waypoint = rhs.Waypoint;

      return *this;
    }

    //! Constructor
    WPL  ( WPL const & rhs )
    {

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->Latitude = rhs.Latitude;
      this->LatitudeSector = rhs.LatitudeSector;
      this->Longitude = rhs.Longitude;
      this->LongitudeSector = rhs.LongitudeSector;
      this->Waypoint = rhs.Waypoint;

    }

    //! Equality
    bool operator == ( WPL const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
        this->Latitude == rhs.Latitude &&
        this->LatitudeSector == rhs.LatitudeSector &&
        this->Longitude == rhs.Longitude &&
        this->LongitudeSector == rhs.LongitudeSector &&
        this->Waypoint == rhs.Waypoint
        );
    }

    //! Inequality
    bool operator != ( WPL const & rhs )
    {
      return !(*this == rhs);
    }


    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, WPL const & p)
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
    friend std::istream& operator>>(std::istream& is, WPL & p)
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
    //! Set the latitude of the waypoint.
    SetMacro(Latitude,double);
    //! Get the latitude of the waypoint.
    GetMacro(Latitude,double);
    //! Set the North/South flag N = North, S = South.
    SetMacro(LatitudeSector,char);
    //! Get the North/South flag N = North, S = South.
    GetMacro(LatitudeSector,char);
    //! Set the longitude of the waypoint.
    SetMacro(Longitude,double);
    //! Get the longitude of the waypoint.
    GetMacro(Longitude,double);
    //! Set the East/West flag E = East, W = West.
    SetMacro(LongitudeSector,char);
    //! Get the East/West flag E = East, W = West.
    GetMacro(LongitudeSector,char);
    //! Set the waypoint.
    SetMacro(Waypoint,std::string);
    //! Get the waypoint.
    GetMacro(Waypoint,std::string);

  protected:
    double Latitude;
    char LatitudeSector;
    double Longitude;
    char LongitudeSector;
    std::string Waypoint;
  };

} // Namespace NMEA.
} // Namespace UF.

#endif
