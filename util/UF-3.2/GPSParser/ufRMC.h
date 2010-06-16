//#pragma once

#if !defined(NMEARMC_H)
#define NMEARMC_H

#include "ufNMEASentence.h"

namespace UF {
namespace NMEA {
  //! RMC: Recommended Minimum Course
  /*!
  CLASS
    RMC


    Introduction

    This message contains UTC time, date, position status, latitude, longitude, course
    and speed over the ground, and magnetic variation. The RMC message is not output
    unless positions are being computed.

    Output

    The message is output in the format:
    $--RMC,m1,c2,m3,c4,m5,c6,f7,f8,d9,f10,c11,c12*hh

    <table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="753" id="AutoNumber1">
    <tr>
      <td width="88">Parameter</td>
      <td width="483">Description</td>
      <td width="178">Range</td>
    </tr>
    <tr>
      <td width="88">m1</td>
      <td width="483">UTC time of the position fix (hhmmss.ss)</td>
      <td width="178">000000.00 to <br>
      235959.95</td>
    </tr>
    <tr>
      <td width="88">c2</td>
      <td width="483">Status of the position fix (always A)<br>
       A - Position data is valid<br>
       V - Navigation receiver warning</td>
      <td width="178">A, V</td>
    </tr>
    <tr>
      <td width="88">m3</td>
      <td width="483">Latitude (ddmm.mmmmmm)</td>
      <td width="178">0000.000000 to <br>
      8959.999999</td>
    </tr>
    <tr>
      <td width="88">c4</td>
      <td width="483">Latitude sector N(orth),<br>
      S(outh)</td>
      <td width="178">N,S</td>
    </tr>
    <tr>
      <td width="88">m5</td>
      <td width="483">Longitude (dddmm.mmmmmm)</td>
      <td width="178">00000.000000 to<br>
      17959.999999</td>
    </tr>
    <tr>
      <td width="88">c6</td>
      <td width="483">Longitude sector E(ast<br>
      W(est)</td>
      <td width="178">E,W</td>
    </tr>
    <tr>
      <td width="88">f7</td>
      <td width="483">Speed over ground (knots)</td>
      <td width="178">000.0 - 999.9</td>
    </tr>
    <tr>
      <td width="88">f8</td>
      <td width="483">Course over ground (degrees); referenced to true north</td>
      <td width="178">000.0 - 359.9</td>
    </tr>
    <tr>
      <td width="88">d9</td>
      <td width="483">Date (ddmmyy)</td>
      <td width="178">010100 - 123199</td>
    </tr>
    <tr>
      <td width="88">f10</td>
      <td width="483">Magnetic variation (degrees)</td>
      <td width="178">0.0 - 99.9</td>
    </tr>
    <tr>
      <td width="88">c11</td>
      <td width="483">Direction of magnetic variation:<br>
       Easterly variation - subtract this value from true north<br>
      course<br>
       Westerly variation - add this value to the true north<br>
      course</td>
      <td width="178">E (ast)<br>
      W (est)</td>
    </tr>
      <tr>
        <td width="88">c12</td>
        <td width="390">Receiver Status:<br>
         A = Navigating<br>
         S = Simulating<br>
        This field may not be present in the output of some devices.</td>
        <td width="197">A,S</td>
      </tr>
    <tr>
      <td width="88">*hh</td>
      <td width="483">The hexadecimal checksum is computed by exclusive -
      ORing all of the bytes in the message between, but not
      including, the $ and the *. The result is *hh where h is a hex
      character.</td>
      <td width="178">0 to 9 and A through F</td>
    </tr>
    </table>


    Typical RMC message:

    $GPRMC,213357.20,A,3722.410857,N,12159.773686,W,000.3,102.4,290498,15.4,W*43
    Setting

    Thales Navigation

    $PASHS,NME,RMC,x,c,[f]

    Enables/disables the output of the NMEA recommended minimum course message
    which contains time, position, course, and speed data where x is the serial port,
    and c is ON or OFF, and f is the optional message output rate ranging from 0.05
    to 999 seconds depending on the measurement update rate option installed.
    If the command is set without a period, the G12 uses the period set by the
    $PASHS,NME,PER command If the $PASHS,NME,PER command is issued after
    this message period has been set, the period resets to the PER setting.

    Example
    Enter the following command to enable the RMC message on port C.
    $PASHS,NME,RMC,C,ON
    $PASHQ,RMC,[c],
    This command allows you to query the RMC message, where c is the optional port
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
  class UF_GPS_PARSER_EXPORT RMC :
    public NMEASentence
  {
  public:
    RMC();
    virtual ~RMC();

    //! Initialise the fields in a sentence.
    void Init()
    {
      this->sentence.clear();
      this->ProprietarySentenceIndicator = false;
      this->ParseStatus = NMEA_NOT_PARSED;

      this->UTC = -1;
      this->Status = 'A';
      this->Latitude = -1;
      this->LatitudeSector = 'N';
      this->Longitude = -1;
      this->LongitudeSector = 'E';
      this->SpeedOverGround = -1;
      this->CourseOverGround = -1;
      this->Date = -1;
      this->MagneticVariation = -1;
      this->DirectionOfMagneticVariation = 'E';
      this->ReceiverStatus = 'A';
    }

    bool ParsedAllFields()
    {
      return (    this->UTC != -1 &&
                  this->Status != 'A' &&
                  this->Latitude != -1 &&
                  this->LatitudeSector != 'N' &&
                  this->Longitude != -1 &&
                  this->LongitudeSector != 'E' &&
                  this->SpeedOverGround != -1 &&
                  this->CourseOverGround != -1 &&
                  this->Date != -1 &&
                  this->MagneticVariation != -1 &&
                  this->DirectionOfMagneticVariation != 'E' &&
                  this->ReceiverStatus != 'A' );
    }



    //! Assignment
    RMC operator = ( RMC const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->UTC = rhs.UTC;
      this->Status = rhs.Status;
      this->Latitude = rhs.Latitude;
      this->LatitudeSector = rhs.LatitudeSector;
      this->Longitude = rhs.Longitude;
      this->LongitudeSector = rhs.LongitudeSector;
      this->SpeedOverGround = rhs.SpeedOverGround;
      this->CourseOverGround = rhs.CourseOverGround;
      this->Date = rhs.Date;
      this->MagneticVariation = rhs.MagneticVariation;
      this->DirectionOfMagneticVariation = rhs.DirectionOfMagneticVariation;
      this->ReceiverStatus = rhs.ReceiverStatus;

      return *this;
    }

    //! Constructor
    RMC  ( RMC const & rhs )
    {
      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->UTC = rhs.UTC;
      this->Status = rhs.Status;
      this->Latitude = rhs.Latitude;
      this->LatitudeSector = rhs.LatitudeSector;
      this->Longitude = rhs.Longitude;
      this->LongitudeSector = rhs.LongitudeSector;
      this->SpeedOverGround = rhs.SpeedOverGround;
      this->CourseOverGround = rhs.CourseOverGround;
      this->Date = rhs.Date;
      this->MagneticVariation = rhs.MagneticVariation;
      this->DirectionOfMagneticVariation = rhs.DirectionOfMagneticVariation;
      this->ReceiverStatus = rhs.ReceiverStatus;

    }

    //! Equality
    bool operator == ( RMC const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
        this->UTC == rhs.UTC &&
        this->Status == rhs.Status &&
        this->Latitude == rhs.Latitude &&
        this->LatitudeSector == rhs.LatitudeSector &&
        this->Longitude == rhs.Longitude &&
        this->LongitudeSector == rhs.LongitudeSector &&
        this->SpeedOverGround == rhs.SpeedOverGround &&
        this->CourseOverGround == rhs.CourseOverGround &&
        this->Date == rhs.Date &&
        this->MagneticVariation == rhs.MagneticVariation &&
        this->DirectionOfMagneticVariation == rhs.DirectionOfMagneticVariation &&
        this->ReceiverStatus == rhs.ReceiverStatus
      );
    }

    //! Inequality
    bool operator != ( RMC const & rhs )
    {
      return !(*this == rhs);
    }

    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, RMC const & p)
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
    friend std::istream& operator>>(std::istream& is, RMC & p)
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
    //! Set the status of the sentence. A = Data Valid, V = Data Invalid
    SetMacro(Status,char);
    //! Get the status of the sentence. A = Data Valid, V = Data Invalid
    GetMacro(Status,char);
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
    //! Set the speed over ground (knots).
    SetMacro(SpeedOverGround,double);
    //! Get the speed over ground (knots).
    GetMacro(SpeedOverGround,double);
    //! Set the course over ground (degrees - referenced to true north).
    SetMacro(CourseOverGround,double);
    //! Get the course over ground (degrees - referenced to true north).
    GetMacro(CourseOverGround,double);
    //! Set the date.
    SetMacro(Date,int);
    //! Get the date.
    GetMacro(Date,int);
    //! Set the magnetic variation (degrees).
    SetMacro(MagneticVariation,double);
    //! Get the magnetic variation (degrees).
    GetMacro(MagneticVariation,double);
    //! Set the direction of magnetic variation. E = East, W = West.
    SetMacro(DirectionOfMagneticVariation,char);
    //! Get the direction of magnetic variation. E = East, W = West.
    GetMacro(DirectionOfMagneticVariation,char);
    //! Set the receiver status field (A: aquiring real data, S: simulating data).
    SetMacro(ReceiverStatus,char);
    //! Get the receiver status field (A: aquiring real data, S: simulating data).
    GetMacro(ReceiverStatus,char);

protected:
    double UTC;
    char Status;
    double Latitude;
    char LatitudeSector;
    double Longitude;
    char LongitudeSector;
    double SpeedOverGround;
    double CourseOverGround;
    int Date;
    double MagneticVariation;
    char DirectionOfMagneticVariation;
    char ReceiverStatus;
  };

} // Namespace NMEA.
} // Namespace UF.

#endif
