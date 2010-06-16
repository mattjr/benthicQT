//#pragma once

#if !defined(GARMINGRMSTO_H)
#define GARMINGRMSTO_H

#include "ufNMEASentence.h"

namespace UF {
//! Garmin proprietary messages.
namespace Garmin {
  //! GRMSTO: Garmin Simple Text Output format.
  /*!
  CLASS
    GRMSTO


    Introduction

    The GRMSTO message contains the simple text output format used by Garmins.

    \code
        Simple Text Output Format:

        The simple text (ASCII) output contains time, position, and velocity data in
        the fixed width fields (not delimited) defined in the following table:

            FIELD DESCRIPTION:      WIDTH:  NOTES:
            ----------------------- ------- ------------------------
            Sentence start          1       Always '@'
            ----------------------- ------- ------------------------
          / Year                    2       Last two digits of UTC year
          | ----------------------- ------- ------------------------
          | Month                   2       UTC month, "01".."12"
        T | ----------------------- ------- ------------------------
        i | Day                     2       UTC day of month, "01".."31"
        m | ----------------------- ------- ------------------------
        e | Hour                    2       UTC hour, "00".."23"
          | ----------------------- ------- ------------------------
          | Minute                  2       UTC minute, "00".."59"
          | ----------------------- ------- ------------------------
          \ Second                  2       UTC second, "00".."59"
            ----------------------- ------- ------------------------
          / Latitude hemisphere     1       'N' or 'S'
          | ----------------------- ------- ------------------------
          | Latitude position       7       WGS84 ddmmmmm, with an implied
          |                                 decimal after the 4th digit
          | ----------------------- ------- ------------------------
          | Longitude hemishpere    1       'E' or 'W'
          | ----------------------- ------- ------------------------
          | Longitude position      8       WGS84 dddmmmmm with an implied
        P |                                 decimal after the 5th digit
        o | ----------------------- ------- ------------------------
        s | Position status         1       'd' if current 2D differential GPS position
        i |                                 'D' if current 3D differential GPS position
        t |                                 'g' if current 2D GPS position
        i |                                 'G' if current 3D GPS position
        o |                                 'S' if simulated position
        n |                                 '_' if invalid position
          | ----------------------- ------- ------------------------
          | Horizontal posn error   3       EPH in meters
          | ----------------------- ------- ------------------------
          | Altitude sign           1       '+' or '-'
          | ----------------------- ------- ------------------------
          | Altitude                5       Height above or below mean
          \                                sea level in meters
            ----------------------- ------- ------------------------
          / East/West velocity      1       'E' or 'W'
          |     direction
          | ----------------------- ------- ------------------------
          | East/West velocity      4       Meters per second in tenths,
          |     magnitude                   ("1234" = 123.4 m/s)
        V | ----------------------- ------- ------------------------
        e | North/South velocity    1       'N' or 'S'
        l |     direction
        o | ----------------------- ------- ------------------------
        c | North/South velocity    4       Meters per second in tenths,
        i |     magnitude                   ("1234" = 123.4 m/s)
        t | ----------------------- ------- ------------------------
        y | Vertical velocity       1       'U' (up) or 'D' (down)
          |     direction
          | ----------------------- ------- ------------------------
          | Vertical velocity       4       Meters per second in hundredths,
          \    magnitude                   ("1234" = 12.34 m/s)
            ----------------------- ------- ------------------------
            Sentence end            2       Carriage return, '0x0D', and
                                            line feed, '0x0A'
            ----------------------- ------- ------------------------

        If a numeric value does not fill its entire field width, the field is padded
        with leading '0's (eg. an altitude of 50 meters above MSL will be output as
        "+00050").

        Any or all of the data in the text sentence (except for the sentence start
        and sentence end fields) may be replaced with underscores to indicate
        invalid data.
      \endcode


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
  class UF_GPS_PARSER_EXPORT GRMSTO :
    public NMEA::NMEASentence
  {
  public:
    GRMSTO();
    virtual ~GRMSTO();

    //! Initialise the fields in a sentence.
    void Init()
    {
      this->sentence.clear();
      this->ProprietarySentenceIndicator = true;
      this->ParseStatus = NMEA::NMEA_NOT_PARSED;

      this->Year = 0;
      this->Month = 0;
      this->Day = 0;
      this->Hour = 0;
      this->Minute = 0;
      this->Second = 0;
      this->LatitudeSector = '_';
      this->Latitude = -1;
      this->LongitudeSector = '_';
      this->Longitude = -1;
      this->Status = '_';
      this->HorizontalPositionError = 0;
      this->AltitudeSign = '_';
      this->Altitude = 0;
      this->EastWestVelocityDirection = '_';
      this->EastWestVelocity = 0;
      this->NorthSouthVelocityDirection = '_';
      this->NorthSouthVelocity = 0;
      this->VerticalVelocityDirection = '_';
      this->VerticalVelocity = 0;
      this->MessageLength = 57;
    }

    //! Assignment
    GRMSTO operator = ( GRMSTO const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->Year = rhs.Year;
      this->Month = rhs.Month;
      this->Day = rhs.Day;
      this->Hour = rhs.Hour;
      this->Minute = rhs.Minute;
      this->Second = rhs.Second;
      this->LatitudeSector = rhs.LatitudeSector;
      this->Latitude = rhs.Latitude;
      this->LongitudeSector = rhs.LongitudeSector;
      this->Longitude = rhs.Longitude;
      this->Status = rhs.Status;
      this->HorizontalPositionError = rhs.HorizontalPositionError;
      this->AltitudeSign = rhs.AltitudeSign;
      this->Altitude = rhs.Altitude;
      this->EastWestVelocityDirection = rhs.EastWestVelocityDirection;
      this->EastWestVelocity = rhs.EastWestVelocity;
      this->NorthSouthVelocityDirection = rhs.NorthSouthVelocityDirection;
      this->NorthSouthVelocity = rhs.NorthSouthVelocity;
      this->VerticalVelocityDirection = rhs.VerticalVelocityDirection;
      this->VerticalVelocity = rhs.VerticalVelocity;
      this->MessageLength = rhs.MessageLength;

      return *this;
    }

    //! Constructor
    GRMSTO  ( GRMSTO const & rhs )
    {

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->Year = rhs.Year;
      this->Month = rhs.Month;
      this->Day = rhs.Day;
      this->Hour = rhs.Hour;
      this->Minute = rhs.Minute;
      this->Second = rhs.Second;
      this->LatitudeSector = rhs.LatitudeSector;
      this->Latitude = rhs.Latitude;
      this->LongitudeSector = rhs.LongitudeSector;
      this->Longitude = rhs.Longitude;
      this->Status = rhs.Status;
      this->HorizontalPositionError = rhs.HorizontalPositionError;
      this->AltitudeSign = rhs.AltitudeSign;
      this->Altitude = rhs.Altitude;
      this->EastWestVelocityDirection = rhs.EastWestVelocityDirection;
      this->EastWestVelocity = rhs.EastWestVelocity;
      this->NorthSouthVelocityDirection = rhs.NorthSouthVelocityDirection;
      this->NorthSouthVelocity = rhs.NorthSouthVelocity;
      this->VerticalVelocityDirection = rhs.VerticalVelocityDirection;
      this->VerticalVelocity = rhs.VerticalVelocity;
      this->MessageLength = rhs.MessageLength;
    }

    //! Equality
    bool operator == ( GRMSTO const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
        this->Year == rhs.Year &&
        this->Month == rhs.Month &&
        this->Day == rhs.Day &&
        this->Hour == rhs.Hour &&
        this->Minute == rhs.Minute &&
        this->Second == rhs.Second &&
        this->LatitudeSector == rhs.LatitudeSector &&
        this->Latitude == rhs.Latitude &&
        this->LongitudeSector == rhs.LongitudeSector &&
        this->Longitude == rhs.Longitude &&
        this->Status == rhs.Status &&
        this->HorizontalPositionError == rhs.HorizontalPositionError &&
        this->AltitudeSign == rhs.AltitudeSign &&
        this->Altitude == rhs.Altitude &&
        this->EastWestVelocityDirection == rhs.EastWestVelocityDirection &&
        this->EastWestVelocity == rhs.EastWestVelocity &&
        this->NorthSouthVelocityDirection == rhs.NorthSouthVelocityDirection &&
        this->NorthSouthVelocity == rhs.NorthSouthVelocity &&
        this->VerticalVelocityDirection == rhs.VerticalVelocityDirection &&
        this->VerticalVelocity == rhs.VerticalVelocity &&
        this->MessageLength == rhs.MessageLength
        );
    }

    //! Inequality
    bool operator != ( GRMSTO const & rhs )
    {
      return !(*this == rhs);
    }


    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, GRMSTO const & p)
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
    friend std::istream& operator>>(std::istream& is, GRMSTO & p)
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
    SetMacro(Year,unsigned char);
    GetMacro(Year,unsigned char);
    SetMacro(Month,unsigned char);
    GetMacro(Month,unsigned char);
    SetMacro(Day,unsigned char);
    GetMacro(Day,unsigned char);
    SetMacro(Hour,unsigned char);
    GetMacro(Hour,unsigned char);
    SetMacro(Minute,unsigned char);
    GetMacro(Minute,unsigned char);
    SetMacro(Second,unsigned char);
    GetMacro(Second,unsigned char);
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
    //! Set the status of the sentence.
    SetMacro(Status,char);
    //! Get the status of the sentence.
    GetMacro(Status,char);
    SetMacro(HorizontalPositionError,int);
    GetMacro(HorizontalPositionError,int);
    //! Set the altitude sign.
    SetMacro(AltitudeSign,char);
    //! Get the altitude sign.
    GetMacro(AltitudeSign,char);
    //! Set the altitude.
    SetMacro(Altitude,double);
    //! Get the altitude.
    GetMacro(Altitude,double);

    SetMacro(EastWestVelocityDirection,char);
    GetMacro(EastWestVelocityDirection,char);
    SetMacro(EastWestVelocity,double);
    GetMacro(EastWestVelocity,double);
    SetMacro(NorthSouthVelocityDirection,char);
    GetMacro(NorthSouthVelocityDirection,char);
    SetMacro(NorthSouthVelocity,double);
    GetMacro(NorthSouthVelocity,double);
    SetMacro(VerticalVelocityDirection,char);
    GetMacro(VerticalVelocityDirection,char);
    SetMacro(VerticalVelocity,double);
    // The length of the message including the @ and CRLF.
    GetMacro(VerticalVelocity,double);

    GetMacro(MessageLength,int);

    //! Return the date as ddmmyy.
    double GetDate()
    {
      return (this->Day*100 + this->Month)*100 + this->Year;
    }

    //! Return the time as hhmmss.
    double GetUTC()
    {
      return (this->Hour*100 + this->Minute)*100 + this->Second;
    }

  protected:
    unsigned char Year;
    unsigned char Month;
    unsigned char Day;
    unsigned char Hour;
    unsigned char Minute;
    unsigned char Second;
    double Latitude;
    char LatitudeSector;
    double Longitude;
    char LongitudeSector;
    char Status;
    int HorizontalPositionError;
    char AltitudeSign;
    double Altitude;
    char EastWestVelocityDirection;
    double EastWestVelocity;
    char NorthSouthVelocityDirection;
    double NorthSouthVelocity;
    char VerticalVelocityDirection;
    double VerticalVelocity;
    //! This is a fixed length message.
    int MessageLength;
  };

} // Namespace NMEA.
} // Namespace UF.


#endif
