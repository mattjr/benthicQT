#include "ufBWC.h"

#include <algorithm>

using namespace UF::NMEA;

std::string const BWC::MNEMONIC = "BWC";

BWC::BWC()
{
  Init();
}

BWC::~BWC()
{
}

void BWC::Parse (bool const & ForceParse)
{
  if ( !ForceParse )
  {
    if ( ValidChecksum() == NMEA::NMEA_INVALID_CHECKSUM )
    {
      SetParseStatus(NMEA::NMEA_INVALID_CHECKSUM);
      return;
    }
  }
  std::string identifier;
  GetField(0,identifier);
  if ( (identifier.rfind(this->MNEMONIC) == std::string::npos ))
  {
    SetParseStatus(NMEA::NMEA_WRONG_MNEMONIC);
    return;
  }
  GetField(1,this->UTC);
  GetField(2,this->Latitude);
  GetField(3,this->LatitudeSector);
  GetField(4,this->Longitude);
  GetField(5,this->LongitudeSector);
  GetField(6,this->TrueBearing);
  GetField(7,this->NorthReferenceIndicator);
  GetField(8,this->MagneticBearing);
  GetField(9,this->MagneticNorthReferenceIndicator);
  GetField(10,this->Distance);
  GetField(11,this->DistanceUnit);
  GetField(12,this->Waypoint);

  int dataFields = NumberOfDataFields();
  if ( dataFields == 15 )
  {
    GetField(dataFields - 2,this->ReceiverStatus);
    if ( this->ReceiverStatus == 'S' )
    {
      SetParseStatus(NMEA::NMEA_SIMULATING);
      return;
    }
  }
  if ( this->ReceiverStatus == 'V' )
  {
    SetParseStatus(NMEA::NMEA_NAVIGATION_RECEIVER_WARNING);
    return;
  }
  SetParseStatus(NMEA::NMEA_OK);
  return;
}

std::string BWC::MakeSentence() const
{
  static std::string const COMMA = ",";
  std::ostringstream s;
  s  << this->MakeSentenceStart() << this->MNEMONIC << COMMA;
  if ( this->UTC == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<double>()(this->UTC,2,9,'0') << COMMA;
  }
  if ( this->Latitude == -1 )
  {
    s << COMMA;
    s << COMMA;
    //s << this->LatitudeSector << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->Latitude,6,11,'0') << COMMA;
    s << this->LatitudeSector << COMMA;
  }
  if ( this->Longitude == -1 )
  {
    s << COMMA;
    s << COMMA;
    //s << this->LongitudeSector << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->Longitude,6,12,'0') << COMMA;
    s << this->LongitudeSector << COMMA;
  }
  if ( this->TrueBearing == -1 )
  {
    s << COMMA;
    s << this->NorthReferenceIndicator << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->TrueBearing,2,6,'0'); s << COMMA;
    s << this->NorthReferenceIndicator << COMMA;
  }
  if ( this->MagneticBearing == -1 )
  {
    s << COMMA;
    s << this->MagneticNorthReferenceIndicator << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->MagneticBearing,2,6,'0'); s << COMMA;
    s << this->MagneticNorthReferenceIndicator << COMMA;
  }
  if ( this->Distance == -1 )
  {
    s << COMMA;
    s << this->DistanceUnit << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->Distance,3) << COMMA;
    s << this->DistanceUnit << COMMA;
  }
  s << this->Waypoint;
  if ( NumberOfDataFields() == 15 )
  {
    s << COMMA;
    s << this->ReceiverStatus;
  }

  BWC nmea;
  nmea.SetSentence(s.str());
  nmea.InsertChecksum();
  return nmea.GetSentence();
}
