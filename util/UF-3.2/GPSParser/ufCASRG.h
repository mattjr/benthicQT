#if !defined(CASCASRG_H)
#define CASCASRG_H

#include "ufGGA.h"
#include "ufRMC.h"

namespace UF {
//! Centre for Autonomous Systems (CAS), proprietary messages.
namespace CAS {
  //! CASRG: Provides a combination of data from the RMC and GGA messages.
  /*!
  CLASS
    CASRG


    Introduction

    A combination of RMC data and GGA data is provided so that a position, including
    speed, course, altitude and GPS costellation status is output.

    Output

    The message is output in the format:
    $CASRG,d1,m2,m3,c4,m5,c6,f7,f8,f9,f10,d11,f12,f13,d14,c15*hh<CR><LF>

    <table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="753" id="AutoNumber1">
    <tr>
      <td width="88">Parameter</td>
      <td width="483">Description</td>
      <td width="178">Range</td>
    </tr>
    <tr>
      <td width="88">d1</td>
      <td width="483">Date (ddmmyy)</td>
      <td width="178">010100 - 123199</td>
    </tr>
    <tr>
      <td width="88">m2</td>
      <td width="483">UTC time of the position fix (hhmmss.ss)</td>
      <td width="178">000000.00 to <br>
      235959.95</td>
    </tr>
    <tr>
      <td width="88">m3</td>
      <td width="483">Latitude (ddmm.mmmmmmm)</td>
      <td width="178">0000.0000000 to <br>
      8959.9999999</td>
    </tr>
    <tr>
      <td width="88">c4</td>
      <td width="483">Latitude sector N(orth),S(outh)</td>
      <td width="178">N,S</td>
    </tr>
    <tr>
      <td width="88">m5</td>
      <td width="483">Longitude (dddmm.mmmmmmm)</td>
      <td width="178">00000.0000000 to<br>
      17959.9999999</td>
    </tr>
    <tr>
      <td width="88">c6</td>
      <td width="483">Longitude sector E(ast),W(est)</td>
      <td width="178">E,W</td>
    </tr>
    <tr>
        <td width="88">f7</td>
        <td width="483">Altitude above Mean Sea Level (geoidal height) ,
        in meters.</td>
        <td width="178">-1000.0000 to<br>
        18000.0000</td>
    </tr>
    <tr>
        <td width="88">f8</td>
        <td width="483">Geoidal separation: the difference between the
        earth ellipsoid and geoid defined by the reference datum
        [meters]</td>
        <td width="178">-999.9999 -<br>
        999.9999</td>
    </tr>
    <tr>
      <td width="88">f9</td>
      <td width="483">Speed over ground (knots)</td>
      <td width="178">000.0000 - 999.9999</td>
    </tr>
    <tr>
      <td width="88">f10</td>
      <td width="483">Course over ground (degrees); referenced to true north</td>
      <td width="178">000.000 - 359.999</td>
    </tr>
    <tr>
        <td width="88">d11</td>
        <td width="483">Number of satellites used in position computation </td>
        <td width="178">3 -24 </td>
    </tr>
    <tr>
        <td width="88">f12</td>
        <td width="483">HDOP (horizontal dilution of precision) </td>
        <td width="178">00.00 - 99.99 </td>
    </tr>
    <tr>
        <td width="88">f13</td>
        <td width="483">Age of differential corrections (seconds)</td>
        <td width="178">0 - 999.9 </td>
    </tr>
    <tr>
        <td width="88">d14</td>
        <td width="483">Position fix type:<br>
         0 - Invalid position or fix not available.<br>
         1 - Autonomous position, GPS SPS Mode (single point mode), fix
        valid.<br>
         2 - Differentially corrected position, GPS, SPS Mode, fix
        valid.<br>
         3 - GPS PPS Mode (single point mode), fix valid<br>
         4 - RTK fixed.<br>
         5 - RTK float.<br>
         6 - Estimated (dead reckoning) mode.<br>
         7 - manual input mode.<br>
         8 - Simulator Mode.</td>
        <td width="178">0 - 8 </td>
    </tr>
    <tr>
      <td width="88">c15</td>
      <td width="483">Status of the position fix:<br>
       A - Autonomous. Satellite system used in non-differential mode in
      position fix.<br>
       D - Differential. Satellite system used in differential mode in
      position fix.<br>
       E - Estimated (dead reckoning) mode.<br>
       M - Manual input mode.<br>
       S- Simulator mode.<br>
       N - Data not valid.<br>
       V - Navigation receiver warning.</td>
      <td width="178">A, D, E, M, S, N, V</td>
    </tr>
    <tr>
      <td width="88">*hh</td>
      <td width="483">The hexadecimal checksum is computed by exclusive -
      ORing all of the bytes in the message between, but not including,
      the $ and the *. The result is *hh where h is a hex character.</td>
      <td width="178">0 to 9 and A through F</td>
    </tr>
    </table>

    \version 2.0

    \date 20-April-2003

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
  class UF_GPS_PARSER_EXPORT CASRG :
    public NMEA::NMEASentence
  {
  public:
    CASRG();
    virtual ~CASRG();

    //! Initialise the fields in a sentence.
    void Init()
    {
      this->sentence.clear();
      this->ProprietarySentenceIndicator = true;
      this->ParseStatus = NMEA::NMEA_NOT_PARSED;

      this->UTC = -1;
      this->Date = -1;
      this->Latitude = -1;
      this->LatitudeSector = 'N';
      this->Longitude = -1;
      this->LongitudeSector = 'E';
      this->Altitude = -9999999;
      this->GeoidalSeparation = -9999999;
      this->SpeedOverGround = -1;
      this->CourseOverGround = -1;
      this->NumberOfSatellites = -1;
      this->HDOP = -1;
      this->AgeOfDiffCorr = -1;
      this->PositionFixType = -1;
      this->Status = 'A';
    }

    //! Assignment
    CASRG operator = ( CASRG const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->Date = rhs.Date;
      this->UTC = rhs.UTC;
      this->Latitude = rhs.Latitude;
      this->LatitudeSector = rhs.LatitudeSector;
      this->Longitude = rhs.Longitude;
      this->LongitudeSector = rhs.LongitudeSector;
      this->Altitude = rhs.Altitude;
      this->GeoidalSeparation = rhs.GeoidalSeparation;
      this->SpeedOverGround = rhs.SpeedOverGround;
      this->CourseOverGround = rhs.CourseOverGround;
      this->NumberOfSatellites = rhs.NumberOfSatellites;
      this->HDOP = rhs.HDOP;
      this->AgeOfDiffCorr = rhs.AgeOfDiffCorr;
      this->PositionFixType = rhs.PositionFixType;
      this->Status = rhs.Status;

      return *this;
    }

    //! Partial Assignment from GGA.
    CASRG operator = ( NMEA::GGA const & rhs )
    {
      this->UTC = rhs.GetUTC();
      this->Latitude = rhs.GetLatitude();
      this->LatitudeSector = rhs.GetLatitudeSector();
      this->Longitude = rhs.GetLongitude();
      this->LongitudeSector = rhs.GetLongitudeSector();
      this->Altitude = rhs.GetAltitude();
      this->GeoidalSeparation = rhs.GetGeoidalSeparation();
      this->NumberOfSatellites = rhs.GetNumberOfSatellites();
      this->HDOP = rhs.GetHDOP();
      this->AgeOfDiffCorr = rhs.GetAgeOfDiffCorr();
      this->PositionFixType = rhs.GetPositionFixType();

      return *this;
    }

    //! Partial Assignment from RMC.
    CASRG operator = ( NMEA::RMC const & rhs )
    {
      this->Date = rhs.GetDate();
      this->UTC = rhs.GetUTC();
      this->Latitude = rhs.GetLatitude();
      this->LatitudeSector = rhs.GetLatitudeSector();
      this->Longitude = rhs.GetLongitude();
      this->LongitudeSector = rhs.GetLongitudeSector();
      this->SpeedOverGround = rhs.GetSpeedOverGround();
      this->CourseOverGround = rhs.GetCourseOverGround();
      this->Status = rhs.GetStatus();

      return *this;
    }

    //! Copy constructor
    CASRG ( CASRG const & rhs )
    {
      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->UTC = rhs.UTC;
      this->Latitude = rhs.Latitude;
      this->LatitudeSector = rhs.LatitudeSector;
      this->Longitude = rhs.Longitude;
      this->LongitudeSector = rhs.LongitudeSector;
      this->Altitude = rhs.Altitude;
      this->GeoidalSeparation = rhs.GeoidalSeparation;
      this->SpeedOverGround = rhs.SpeedOverGround;
      this->CourseOverGround = rhs.CourseOverGround;
      this->NumberOfSatellites = rhs.NumberOfSatellites;
      this->HDOP = rhs.HDOP;
      this->AgeOfDiffCorr = rhs.AgeOfDiffCorr;
      this->PositionFixType = rhs.PositionFixType;
      this->Status = rhs.Status;

    }

    //! Equality
    bool operator == ( CASRG const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
        this->Date == rhs.Date &&
        this->UTC == rhs.UTC &&
        this->Latitude == rhs.Latitude &&
        this->LatitudeSector == rhs.LatitudeSector &&
        this->Longitude == rhs.Longitude &&
        this->LongitudeSector == rhs.LongitudeSector &&
        this->Altitude == rhs.Altitude &&
        this->GeoidalSeparation == rhs.GeoidalSeparation &&
        this->SpeedOverGround == rhs.SpeedOverGround &&
        this->CourseOverGround == rhs.CourseOverGround &&
        this->NumberOfSatellites == rhs.NumberOfSatellites &&
        this->HDOP == rhs.HDOP &&
        this->AgeOfDiffCorr == rhs.AgeOfDiffCorr &&
        this->PositionFixType == rhs.PositionFixType &&
        this->Status == rhs.Status
      );
    }

    //! Inequality
    bool operator != ( CASRG const & rhs )
    {
      return !(*this == rhs);
    }


    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, CASRG const & p)
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
    friend std::istream& operator>>(std::istream& is, CASRG & p)
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
      //! Set the date.
      SetMacro(Date,int);
      //! Get the date.
      GetMacro(Date,int);
      //! Set Coordinated Universal Time.
      SetMacro(UTC,double);
      //! Get Coordinated Universal Time.
      GetMacro(UTC,double);
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
      //! Set the geoidal height (altitude above mean sea leval).
      SetMacro(Altitude,double);
      //! Get the geoidal height (altitude above mean sea leval).
      GetMacro(Altitude,double);
      //! Set the geoidal separation value.
      SetMacro(GeoidalSeparation,double);
      //! Get the geoidal separation value.
      GetMacro(GeoidalSeparation,double);
      //! Set the speed over ground (knots).
      SetMacro(SpeedOverGround,double);
      //! Get the speed over ground (knots).
      GetMacro(SpeedOverGround,double);
      //! Set the course over ground (degrees - referenced to true north).
      SetMacro(CourseOverGround,double);
      //! Get the course over ground (degrees - referenced to true north).
      GetMacro(CourseOverGround,double);
      //! Set the number of satellites.
      SetMacro(NumberOfSatellites,int);
      //! Get the number of satellites.
      GetMacro(NumberOfSatellites,int);
      //! Set the horizontal dilution of precision.
      SetMacro(HDOP,double);
      //! Get the horizontal dilution of precision.
      GetMacro(HDOP,double);
      //! Set the age of the differential corrections.
      SetMacro(AgeOfDiffCorr,double);
      //! Get the age of the differential corrections.
      GetMacro(AgeOfDiffCorr,double);
      //! Set Position Fix Type.
      SetMacro(PositionFixType,int);
      //! Get Position Fix Type.
      GetMacro(PositionFixType,int);
      //! Set the status of the sentence. A = Data Valid, V = Data Invalid
      SetMacro(Status,char);
      //! Get the status of the sentence. A = Data Valid, V = Data Invalid
      GetMacro(Status,char);

    protected:
      int Date; // RMC:
      double UTC; // RMC:
      double Latitude; // RMC: ddmm.mmm resolution 0.0000001 minute
      char LatitudeSector; // RMC:
      double Longitude; //RMC: dddmm.mmm resolution 0.0000001 minute
      char LongitudeSector; // RMC:
      double Altitude; // GGA: In meters, resolution 0.0001
      double GeoidalSeparation; // GGA: In meters resolution 0.0001
      double SpeedOverGround; // RMC: Knots resolution 0.0001
      double CourseOverGround; // RMC: Degrees resolution 0.001
      int NumberOfSatellites; // GGA
      double HDOP; // GGA: xx.xx
      double AgeOfDiffCorr; // GGA: Age in seconds, resolution 0.1s
      int PositionFixType; // GGA: (0-10)
      char Status; // RMC:

  };

} // Namespace NMEA.
} // Namespace UF.


#endif
