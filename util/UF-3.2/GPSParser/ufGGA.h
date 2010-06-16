//#pragma once

#if !defined(NMEAGGA_H)
#define NMEAGGA_H

#include "ufNMEASentence.h"

namespace UF {
namespace NMEA {
  //! GGA: 3-D GPS Position.
  /*!
  CLASS
    GGA


    Introduction

    The GGA message contains a 3-D position and time of position fix. This message
    is not output unless positions are being computed. In addition to a 3-D position
    (latitude/longitude/altitude), the GGA message contains information on the type of
    position fix (i.e., autonomous or differentially corrected), HDOP, and current time.

    Output

    The message is output in the format: $--GGA,m1,m2,c3,m4,c5,d6,d7,d8,f9,c10,d11,c12,d13,d14*hh

    <table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="811" id="AutoNumber1">
      <tr>
        <td width="87">Parameter </td>
        <td width="406">Description </td>
        <td width="161">Range (FUL) </td>
        <td width="152">Range (STD)</td>
      </tr>
      <tr>
        <td width="87">m1</td>
        <td width="406">UTC time (hhmmss.s) of the position fix </td>
        <td width="161">0 to 235959.99 </td>
        <td width="152">0 to 235959.99</td>
      </tr>
      <tr>
        <td width="87">m2</td>
        <td width="406">Latitude of the position fix (ddmm.mmmmm)</td>
        <td width="161">0 - 90</td>
        <td width="152">0 - 90</td>
      </tr>
      <tr>
        <td width="87">c3</td>
        <td width="406">Latitude sector</td>
        <td width="161">N/S </td>
        <td width="152">N/S</td>
      </tr>
      <tr>
        <td width="87">m4</td>
        <td width="406">Longitude of the position fix (dddmm.mmmmm)</td>
        <td width="161">0 - 180</td>
        <td width="152">0 - 180</td>
      </tr>
      <tr>
        <td width="87">c5</td>
        <td width="406">Longitude sector </td>
        <td width="161">E/W </td>
        <td width="152">E/W</td>
      </tr>
      <tr>
        <td width="87">d6</td>
        <td width="406">Position fix type:<br>
         0 - Invalid position or position not available<br>
         1 - Autonomous position<br>
         2 - Differentially corrected position<br>
         3 - GPS PPS Mode, fix valid</td>
        <td width="161">0/1/2/3</td>
        <td width="152">0/1/2/3</td>
      </tr>
      <tr>
        <td width="87">d7</td>
        <td width="406">Number of satellites used in position computation </td>
        <td width="161">3 -24 </td>
        <td width="152">3 -24</td>
      </tr>
      <tr>
        <td width="87">d8</td>
        <td width="406">HDOP (horizontal dilution of precision) </td>
        <td width="161">00.0 - 99.9 </td>
        <td width="152">00.0 - 99.9</td>
      </tr>
      <tr>
        <td width="87">f9</td>
        <td width="406">Altitude above Mean Sea Level (geoidal height) </td>
        <td width="161">-1000.00 to<br>
        18000.00</td>
        <td width="152">-1000.00 to<br>
        18000.00</td>
      </tr>
      <tr>
        <td width="87">c10</td>
        <td width="406">Altitude unit of measure (always M) </td>
        <td width="161">M</td>
        <td width="152">&nbsp;M</td>
      </tr>
      <tr>
        <td width="87">d11</td>
        <td width="406">Geoidal separation value </td>
        <td width="161">-999.999 -<br>
        999.999</td>
        <td width="152">-99 - 99</td>
      </tr>
      <tr>
        <td width="87">c12</td>
        <td width="406">Geoidal separation unit of measure (always M)</td>
        <td width="161">M </td>
        <td width="152">M</td>
      </tr>
      <tr>
        <td width="87">d13</td>
        <td width="406">Age of differential corrections (seconds)</td>
        <td width="161">0 - 999.999 </td>
        <td width="152">0 -99</td>
      </tr>
      <tr>
        <td width="87">d14</td>
        <td width="406">Differential base station ID number </td>
        <td width="161">0 - 1023 </td>
        <td width="152">0 - 1023</td>
      </tr>
      <tr>
        <td width="87">hh</td>
        <td width="406">Checksum </td>
        <td width="161">2-character hex</td>
        <td width="152">2-character hex</td>
      </tr>
    </table>



    Example full GGA message structure:

    $GPGGA,123456.12,1234.123456,N,12345.123456,W,2,12,12.1,12345.12,M,-12.1,M,123,1234*64

    Example standard GGA message structure (no DGPS):

    $GPGGA,161229.487,3723.2475,N,12158.3416,W,1,07,1.0,9.0,M,,,,0000*18

    Typical GGA message:

    $GPGGA,183805.50,3722.36223,N,12159.82741,W,2,7,2.8, 16.12,M,-31.24,M,005,0001*6F


    Setting

    Thales Navigation

    $PASHS,NME,GGA,x,s,[f]

    Enable/disable NMEA GPS position response message on port x, where x is the
    output port A, B, or C. s is ON or OFF. f is the optional message output rate ranging
    from 0.05 to 999 seconds depending on the measurement update rate option
    installed.

    If the command is set without a period, the G12 uses the period set by the
    $PASHS,NME,PER command If the $PASHS,NME,PER command is issued after
    this message period has been set, the period resets to the PER setting.

    $PASHS,GGA,x,[f]

    This command commands the receiver to output either the entire GGA message
    (x=FUL) or the standard GGA message (x=STD). NMEA standards restricts the
    length of any message to a maximum of 82 characters, including the header,
    checksum and \<CR\>\<LF\> characters. f is the optional message output rate ranging
    from 0.05 to 999 seconds depending on the measurement update rate option
    installed.

    If the command is set without a period, the G12 uses the period set by the
    $PASHS,NME,PER command If the $PASHS,NME,PER command is issued after
    this message period has been set, the period resets to the PER setting.
    A GGA message output in the FUL format may exceed the maximum length of 82
    characters to accommodate the highest level of resolution (recommended in RTK
    mode). Some software and hardware devices which accept NMEA inputs may
    reject or ignore NMEA messages that exceed the maximum length; in those cases,
    the STD format should be selected.

    When using the STD format, leading zeroes in numeric fields and plus signs (+) for
    positive values are eliminated and the geoidal separation value is rounded to the
    nearest meter. This allows the GGA message to comply with the maximum length
    restriction.

    FUL is the default setting.

    Example:

    Enable GGA on port A:

    $PASHS,NME,GGA,A,ON

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
  class UF_GPS_PARSER_EXPORT GGA :
    public NMEASentence
  {
  public:
    GGA();
    virtual ~GGA();

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
      this->PositionFixType = -1;
      this->NumberOfSatellites = -1;
      this->HDOP = -1;
      this->Altitude = -9999999;
      this->AltitudeUnit = 'M';
      this->GeoidalSeparation = -9999999;
      this->GeoidalSeparationUnit = 'M';
      this->AgeOfDiffCorr = -1;
      this->DifferentialBaseStationId = -1;
    }

    bool ParsedAllFields()
    {
      return (    this->UTC != -1 &&
                  this->Latitude != -1 &&
                  this->LatitudeSector != 'N' &&
                  this->Longitude != -1 &&
                  this->LongitudeSector != 'E' &&
                  this->PositionFixType != -1 &&
                  this->NumberOfSatellites != -1 &&
                  this->HDOP != -1 &&
                  this->Altitude != -9999999 &&
                  this->AltitudeUnit != 'M' &&
                  this->GeoidalSeparation != -9999999 &&
                  this->GeoidalSeparationUnit != 'M' &&
                  this->AgeOfDiffCorr != -1 &&
                  this->DifferentialBaseStationId != -1 );
    }

    //! Assignment
    GGA operator = ( GGA const & rhs )
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
      this->PositionFixType = rhs.PositionFixType;
      this->NumberOfSatellites = rhs.NumberOfSatellites;
      this->HDOP = rhs.HDOP;
      this->Altitude = rhs.Altitude;
      this->AltitudeUnit = rhs.AltitudeUnit;
      this->GeoidalSeparation = rhs.GeoidalSeparation;
      this->GeoidalSeparationUnit = 'M';
      this->AgeOfDiffCorr = rhs.AgeOfDiffCorr;
      this->DifferentialBaseStationId = rhs.DifferentialBaseStationId;

      return *this;
    }

    //! Constructor
    GGA  ( GGA const & rhs )
    {
      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->UTC = rhs.UTC;
      this->Latitude = rhs.Latitude;
      this->LatitudeSector = rhs.LatitudeSector;
      this->Longitude = rhs.Longitude;
      this->LongitudeSector = rhs.LongitudeSector;
      this->PositionFixType = rhs.PositionFixType;
      this->NumberOfSatellites = rhs.NumberOfSatellites;
      this->HDOP = rhs.HDOP;
      this->Altitude = rhs.Altitude;
      this->AltitudeUnit = rhs.AltitudeUnit;
      this->GeoidalSeparation = rhs.GeoidalSeparation;
      this->GeoidalSeparationUnit = 'M';
      this->AgeOfDiffCorr = rhs.AgeOfDiffCorr;
      this->DifferentialBaseStationId = rhs.DifferentialBaseStationId;

    }

    //! Equality
    bool operator == ( GGA const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
        this->UTC == rhs.UTC &&
        this->Latitude == rhs.Latitude &&
        this->LatitudeSector == rhs.LatitudeSector &&
        this->Longitude == rhs.Longitude &&
        this->LongitudeSector == rhs.LongitudeSector &&
        this->PositionFixType == rhs.PositionFixType &&
        this->NumberOfSatellites == rhs.NumberOfSatellites &&
        this->HDOP == rhs.HDOP &&
        this->Altitude == rhs.Altitude &&
        this->AltitudeUnit == rhs.AltitudeUnit &&
        this->GeoidalSeparation == rhs.GeoidalSeparation &&
        this->GeoidalSeparationUnit == 'M' &&
        this->AgeOfDiffCorr == rhs.AgeOfDiffCorr &&
        this->DifferentialBaseStationId == rhs.DifferentialBaseStationId
      );
    }

    //! Inequality
    bool operator != ( GGA const & rhs )
    {
      return !(*this == rhs);
    }

    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, GGA const & p)
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
    friend std::istream& operator>>(std::istream& is, GGA & p)
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
    //! Set Position Fix Type.
    SetMacro(PositionFixType,int);
    //! Get Position Fix Type.
    GetMacro(PositionFixType,int);
    //! Set the number of satellites.
    SetMacro(NumberOfSatellites,int);
    //! Get the number of satellites.
    GetMacro(NumberOfSatellites,int);
    //! Set the horizontal dilution of precision.
    SetMacro(HDOP,double);
    //! Get the horizontal dilution of precision.
    GetMacro(HDOP,double);
    //! Set the geoidal height (altitude above mean sea leval).
    SetMacro(Altitude,double);
    //! Get the geoidal height (altitude above mean sea leval).
    GetMacro(Altitude,double);
    //! Set the altitude unit of measure.
    SetMacro(AltitudeUnit,char);
    //! Get the altitude unit of measure.
    GetMacro(AltitudeUnit,char);
    //! Set the geoidal separation value.
    SetMacro(GeoidalSeparation,double);
    //! Get the geoidal separation value.
    GetMacro(GeoidalSeparation,double);
    //! Set the geoidal separation unit of measure.
    SetMacro(GeoidalSeparationUnit,char);
    //! Get the geoidal separation unit of measure.
    GetMacro(GeoidalSeparationUnit,char);
    //! Set the age of the differential corrections.
    SetMacro(AgeOfDiffCorr,double);
    //! Get the age of the differential corrections.
    GetMacro(AgeOfDiffCorr,double);
    //! Set the differential base station Id.
    SetMacro(DifferentialBaseStationId,int);
    //! Get the differential base station Id.
    GetMacro(DifferentialBaseStationId,int);

protected:
    double Latitude;
    char LatitudeSector;
    double Longitude;
    char LongitudeSector;
    double UTC;
    int PositionFixType;
    int NumberOfSatellites;
    double HDOP;
    double Altitude;
    char AltitudeUnit;
    double GeoidalSeparation;
    char GeoidalSeparationUnit;
    double AgeOfDiffCorr;
    int DifferentialBaseStationId;
  };

} // Namespace NMEA.
} // Namespace UF.


#endif
