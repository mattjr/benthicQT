#include "ufRMB.h"

#include <algorithm>

using namespace UF::NMEA;

std::string const RMB::MNEMONIC = "RMB";


RMB::RMB()
{
  Init();
}

RMB::~RMB()
{
}

void RMB::Parse (bool const & ForceParse)
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
    SetParseStatus(NMEA_WRONG_MNEMONIC);
    return;
  }
  GetField(1,this->Status);
  GetField(2,this->CrossTrackError);
  GetField(3,this->DirectionToSteer);
  GetField(4,this->OriginWaypoint);
  GetField(5,this->DestinationWaypoint);
  GetField(6,this->DestinationLatitude);
  GetField(7,this->DestinationLatitudeSector);
  GetField(8,this->DestinationLongitude);
  GetField(9,this->DestinationLongitudeSector);
  GetField(10,this->DestinationRange);
  GetField(11,this->DestinationBearing);
  GetField(12,this->DestinationClosingVelocity);
  GetField(13,this->ArrivalAlarm);

  int dataFields = NumberOfDataFields();

  if ( dataFields == 16 )
  {
    GetField(dataFields - 2,this->ReceiverStatus);
    if ( this->ReceiverStatus == 'S' )
    {
      SetParseStatus(NMEA_SIMULATING);
      return;
    }
  }
  if ( this->ReceiverStatus == 'V' )
  {
    SetParseStatus(NMEA_NAVIGATION_RECEIVER_WARNING);
    return;
  }
  SetParseStatus(NMEA_OK);
}

std::string RMB::MakeSentence() const
{
  static std::string const COMMA = ",";
  std::ostringstream s;
  s << this->MakeSentenceStart() << this->MNEMONIC << COMMA
    << this->Status << COMMA;
  if ( this->CrossTrackError == -1 )
  {
    s << COMMA;
    s << this->DirectionToSteer << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->CrossTrackError,2) << COMMA;
    s << this->DirectionToSteer << COMMA;
  }
  s << this->OriginWaypoint << COMMA;
  s << this->DestinationWaypoint << COMMA;
  if ( this->DestinationLatitude == -1 )
  {
    s << COMMA;
    s << COMMA;
    //s << this->DestinationLatitudeSector << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->DestinationLatitude,6,11,'0') << COMMA;
    s << this->DestinationLatitudeSector << COMMA;
  }
  if ( this->DestinationLongitude == -1 )
  {
    s << COMMA;
    s << COMMA;
    //s << this->DestinationLongitudeSector << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->DestinationLongitude,6,12,'0') << COMMA;
    s << this->DestinationLongitudeSector << COMMA;
  }
  if ( this->DestinationRange == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<double>()(this->DestinationRange,3) << COMMA;
  }
  if ( this->DestinationBearing == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<double>()(this->DestinationBearing,1,5,'0') << COMMA;
  }
  if ( this->DestinationClosingVelocity == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<double>()(this->DestinationClosingVelocity,1) << COMMA;
  }
  s << this->ArrivalAlarm;

  if ( NumberOfDataFields() == 16 )
  {
    s << COMMA;
    s << this->ReceiverStatus;
  }

  RMB nmea;
  nmea.SetSentence(s.str());
  nmea.InsertChecksum();
  return nmea.GetSentence();
}
