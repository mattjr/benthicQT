#include "ufGRMSTO.h"

#include <algorithm>

using std::size_t;

using namespace UF::Garmin;

std::string const GRMSTO::MNEMONIC = "@";


GRMSTO::GRMSTO()
{
  Init();
}

GRMSTO::~GRMSTO()
{
}

void GRMSTO::Parse (bool const & ForceParse)
{
  if ( !ForceParse )
  {
    if ( ValidChecksum() == NMEA::NMEA_INVALID_CHECKSUM )
    {
      SetParseStatus(NMEA::NMEA_INVALID_CHECKSUM);
      return;
    }
  }

  NMEA::TAG_NMEA_STATUS status = NMEA::NMEA_OK;
  std::string s = this->GetSentence();
  if ( s.size() != (size_t)this->MessageLength )
  {
    SetParseStatus(NMEA::NMEA_WRONG_MESSAGE_LENGTH);
    return;
  }
  if ( (s.find(this->MNEMONIC) == std::string::npos ))
  {
    SetParseStatus(NMEA::NMEA_WRONG_MNEMONIC);
    return;
  }

  size_t idx = 1;
  size_t len = 2;
  std::string t = s.substr(idx,len);
  if ( t.find('_') == std::string::npos )
    this->Year = (unsigned char)NumConv::s2n<int>()(t);
  else
    status = NMEA::NMEA_INVALID_FIELD;
  idx += 2;
  t = s.substr(idx,len);
  if ( t.find('_') == std::string::npos )
    this->Month = (unsigned char)NumConv::s2n<int>()(t);
  else
    status = NMEA::NMEA_INVALID_FIELD;
  idx += 2;
  t = s.substr(idx,len);
  if ( t.find('_') == std::string::npos )
    this->Day = (unsigned char)NumConv::s2n<int>()(t);
  else
    status = NMEA::NMEA_INVALID_FIELD;
  idx += 2;
  t = s.substr(idx,len);
  if ( t.find('_') == std::string::npos )
    this->Hour = (unsigned char)NumConv::s2n<int>()(t);
  else
    status = NMEA::NMEA_INVALID_FIELD;
  idx += 2;
  t = s.substr(idx,len);
  if ( t.find('_') == std::string::npos )
    this->Minute = (unsigned char)NumConv::s2n<int>()(t);
  else
    status = NMEA::NMEA_INVALID_FIELD;
  idx += 2;
  t = s.substr(idx,len);
  if ( t.find('_') == std::string::npos )
    this->Second = (unsigned char)NumConv::s2n<int>()(t);
  else
    status = NMEA::NMEA_INVALID_FIELD;
  idx += 2;
  len = 1;
  t = s.substr(idx,len);
  if ( t.find('_') == std::string::npos )
    this->LatitudeSector = t.at(0);
  else
    status = NMEA::NMEA_INVALID_FIELD;
  idx += 1;
  len = 7;
  t = s.substr(idx,len);
  if ( t.find('_') == std::string::npos )
    this->Latitude = NumConv::s2n<double>()(t)/1000.0;
  else
    status = NMEA::NMEA_INVALID_FIELD;
  idx += 7;
  len = 1;
  t = s.substr(idx,len);
  if ( t.find('_') == std::string::npos )
    this->LongitudeSector = t.at(0);
  else
    status = NMEA::NMEA_INVALID_FIELD;
  idx += 1;
  len = 8;
  t = s.substr(idx,len);
  if ( t.find('_') == std::string::npos )
    this->Longitude = NumConv::s2n<double>()(t)/1000.0;
  else
    status = NMEA::NMEA_INVALID_FIELD;
  idx += 8;
  len = 1;
  t = s.substr(idx,len);
  if ( t.find('_') == std::string::npos )
    this->Status = t.at(0);
  else
    status = NMEA::NMEA_INVALID_FIELD;
  idx += 1;
  len = 3;
  t = s.substr(idx,len);
  if ( t.find('_') == std::string::npos )
    this->HorizontalPositionError = NumConv::s2n<int>()(t);
  else
    status = NMEA::NMEA_INVALID_FIELD;
  idx += 3;
  len = 1;
  t = s.substr(idx,len);
  if ( t.find('_') == std::string::npos )
    this->AltitudeSign = t.at(0);
  else
    status = NMEA::NMEA_INVALID_FIELD;
  idx += 1;
  len = 5;
  t = s.substr(idx,len);
  if ( t.find('_') == std::string::npos )
    this->Altitude = NumConv::s2n<double>()(t);
  else
    status = NMEA::NMEA_INVALID_FIELD;
  idx += 5;
  len = 1;
  t = s.substr(idx,len);
  if ( t.find('_') == std::string::npos )
    this->EastWestVelocityDirection = t.at(0);
  else
    status = NMEA::NMEA_INVALID_FIELD;
  idx += 1;
  len = 4;
  t = s.substr(idx,len);
  if ( t.find('_') == std::string::npos )
    this->EastWestVelocity = NumConv::s2n<double>()(t)/10.0;
  else
    status = NMEA::NMEA_INVALID_FIELD;
  idx += 4;
  len = 1;
  t = s.substr(idx,len);
  if ( t.find('_') == std::string::npos )
    this->NorthSouthVelocityDirection = t.at(0);
  else
    status = NMEA::NMEA_INVALID_FIELD;
  idx += 1;
  len = 4;
  t = s.substr(idx,len);
  if ( t.find('_') == std::string::npos )
    this->NorthSouthVelocity = NumConv::s2n<double>()(t)/10.0;
  else
    status = NMEA::NMEA_INVALID_FIELD;
  idx += 4;
  len = 1;
  t = s.substr(idx,len);
  if ( t.find('_') == std::string::npos )
    this->VerticalVelocityDirection = t.at(0);
  else
    status = NMEA::NMEA_INVALID_FIELD;
  idx += 1;
  len = 4;
  t = s.substr(idx,len);
  if ( t.find('_') == std::string::npos )
    this->VerticalVelocity = NumConv::s2n<double>()(t)/100.0;
  else
    status = NMEA::NMEA_INVALID_FIELD;

  if ( status != NMEA::NMEA_INVALID_FIELD )
    if ( this->Status == 'S' )
      status = NMEA::NMEA_SIMULATING;

  SetParseStatus(status);
}

std::string GRMSTO::MakeSentence() const
{
  static char const CR = '\r';
  static char const LF = '\n';
  std::ostringstream s;
  s << this->MNEMONIC;
  if ( this->Year == 0 )
    s << "__";
  else
    s << std::fixed << std::setw(2) << std::setfill('0') << (int)this->Year;
  if ( this->Month == 0 )
    s << "__";
  else
    s << std::fixed << std::setw(2) << std::setfill('0') << (int)this->Month;
  if ( this->Day == 0 )
    s << "__";
  else
    s << std::fixed << std::setw(2) << std::setfill('0') << (int)this->Day;
  s << std::fixed << std::setw(2) << std::setfill('0') << (int)this->Hour;
  s << std::fixed << std::setw(2) << std::setfill('0') << (int)this->Minute;
  s << std::fixed << std::setw(2) << std::setfill('0') << (int)this->Second;
  s << this->LatitudeSector;
  if ( this->LatitudeSector == '_' )
    s << "_______";
  else
    s << std::fixed << std::setw(7) << std::setfill('0') << (int)(this->Latitude*1000);
  s << this->LongitudeSector;
  if ( this->LongitudeSector == '_' )
    s << "________";
  else
    s << std::fixed << std::setw(8) << std::setfill('0') << (int)(this->Longitude*1000);
  s << this->Status;
  s << std::fixed << std::setw(3) << std::setfill('0') << (int)this->HorizontalPositionError;
  s << this->AltitudeSign;
  if ( this->AltitudeSign == '_' )
    s << "_____";
  else
    s << std::fixed << std::setw(5) << std::setfill('0') << (int)(this->Altitude);
  s << this->EastWestVelocityDirection;
  if ( this->EastWestVelocityDirection == '_' )
    s << "____";
  else
    s << std::fixed << std::setw(4) << std::setfill('0') << (int)(this->EastWestVelocity*10.0);
  s << this->NorthSouthVelocityDirection;
  if ( this->NorthSouthVelocityDirection == '_' )
    s << "____";
  else
    s << std::fixed << std::setw(4) << std::setfill('0') << (int)(this->NorthSouthVelocity*10.0);
  s << this->VerticalVelocityDirection;
  if ( this->VerticalVelocityDirection == '_' )
    s << "____";
  else
    s << std::fixed << std::setw(4) << std::setfill('0') << (int)(this->VerticalVelocity*100.0);

  s << CR << LF;
  return s.str();

}
