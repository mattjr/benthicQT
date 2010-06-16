#include "ufGGA.h"

#include <algorithm>

using namespace UF::NMEA;

std::string const GGA::MNEMONIC = "GGA";

GGA::GGA()
{
  Init();
}

GGA::~GGA()
{
}

void GGA::Parse (bool const & ForceParse)
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
  GetField(1,this->UTC);
  GetField(2,this->Latitude);
  GetField(3,this->LatitudeSector);
  GetField(4,this->Longitude);
  GetField(5,this->LongitudeSector);
  GetField(6,this->PositionFixType);
  GetField(7,this->NumberOfSatellites);
  GetField(8,this->HDOP);
  GetField(9,this->Altitude);
  GetField(10,this->AltitudeUnit);
  GetField(11,this->GeoidalSeparation);
  GetField(12,this->GeoidalSeparationUnit);
  GetField(13,this->AgeOfDiffCorr);
  GetField(14,this->DifferentialBaseStationId);

  SetParseStatus(NMEA::NMEA_OK);
  return;
}

std::string GGA::MakeSentence() const
{
  static std::string const COMMA = ",";
  std::ostringstream s;
  s << this->MakeSentenceStart() << this->MNEMONIC << COMMA;
  if ( this->UTC == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<double>()(this->UTC,2,9,'0') << COMMA;
  }
  if ( this->Latitude == -1 )
  {
    s << COMMA;
    s << this->LatitudeSector << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->Latitude,6,11,'0') << COMMA;
    s << this->LatitudeSector << COMMA;
  }
  if ( this->Longitude == -1 )
  {
    s << COMMA;
    s << this->LongitudeSector << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->Longitude,6,12,'0') << COMMA;
    s << this->LongitudeSector << COMMA;
  }
  if ( this->PositionFixType == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<int>()(this->PositionFixType,0) << COMMA;
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
  if ( this->Altitude == -9999999 )
  {
    s << COMMA;
    s << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->Altitude,2) << COMMA;
    s << this->AltitudeUnit << COMMA;
  }
  if ( this->GeoidalSeparation == -9999999 )
  {
    s << COMMA;
    s << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->GeoidalSeparation,3) << COMMA;
    s << this->GeoidalSeparationUnit << COMMA;
  }
  if ( this->AgeOfDiffCorr == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<double>()(this->AgeOfDiffCorr,3) << COMMA;
  }
  if ( this->DifferentialBaseStationId != -1 )
  {
    s << NumConv::n2s<int>()(this->DifferentialBaseStationId,0);
  }

  GGA nmea;
  nmea.SetSentence(s.str());
  nmea.InsertChecksum();
  return nmea.GetSentence();
}
