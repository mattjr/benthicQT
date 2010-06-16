#include "ufCASRG.h"

#include <algorithm>

using namespace UF::CAS;

std::string const CASRG::MNEMONIC = "CASRG";


CASRG::CASRG()
{
}

CASRG::~CASRG()
{
}

void CASRG::Parse(bool const & ForceParse)
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
  if ( (identifier.rfind(this->MNEMONIC) == std::string::npos ) )
  {
    SetParseStatus(NMEA::NMEA_WRONG_MNEMONIC);
    return;
  }
  GetField(1,this->Date);
  GetField(2,this->UTC);
  GetField(3,this->Latitude);
  GetField(4,this->LatitudeSector);
  GetField(5,this->Longitude);
  GetField(6,this->LongitudeSector);
  GetField(7,this->Altitude);
  GetField(8,this->GeoidalSeparation);
  GetField(9,this->SpeedOverGround);
  GetField(10,this->CourseOverGround);
  GetField(11,this->NumberOfSatellites);
  GetField(12,this->HDOP);
  GetField(13,this->AgeOfDiffCorr);
  GetField(14,this->PositionFixType);
  GetField(15,this->Status);

  if ( this->Status == 'V' )
  {
    SetParseStatus(NMEA::NMEA_NAVIGATION_RECEIVER_WARNING);
    return;
  }
  SetParseStatus(NMEA::NMEA_OK);
  return;
}

std::string CASRG::MakeSentence() const
{
  static std::string const COMMA = ",";
  std::ostringstream s;
  s << this->MakeSentenceStart() << this->MNEMONIC << COMMA;

  if ( this->Date == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<int>()(this->Date,0,6,'0') << COMMA;
  }
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
  }
  else
  {
    s << NumConv::n2s<double>()(this->Longitude,6,12,'0') << COMMA;
    s << this->LongitudeSector << COMMA;
  }
  if ( this->Altitude == -9999999 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<double>()(this->Altitude,4) << COMMA;
  }
  if ( this->GeoidalSeparation == -9999999 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<double>()(this->GeoidalSeparation,4) << COMMA;
  }
  if ( this->SpeedOverGround == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<double>()(this->SpeedOverGround,3) << COMMA;
  }
  if ( this->CourseOverGround == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<double>()(this->CourseOverGround,3,7,'0') << COMMA;
  }
  if ( this->NumberOfSatellites == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<int>()(this->NumberOfSatellites,0) << COMMA;
  }
  if ( this->HDOP == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<double>()(this->HDOP,1) << COMMA;
  }
  if ( this->AgeOfDiffCorr == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<double>()(this->AgeOfDiffCorr,3) << COMMA;
  }
  if ( this->PositionFixType == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<int>()(this->PositionFixType,0) << COMMA;
  }
  s << this->Status;

  CASRG nmea;
  nmea.SetSentence(s.str());
  nmea.InsertChecksum();
  return nmea.GetSentence();
}

