#include "ufGRMZ.h"

#include <algorithm>

using namespace UF::Garmin;

std::string const GRMZ::MNEMONIC = "GRMZ";


GRMZ::GRMZ()
{
  Init();
}

GRMZ::~GRMZ()
{
}

void GRMZ::Parse (bool const & ForceParse)
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
  GetField(1,this->Altitude);
  GetField(2,this->AltitudeUnits);
  GetField(3,this->Fix);

  SetParseStatus(NMEA::NMEA_OK);
  return;
}

std::string GRMZ::MakeSentence() const
{
  static std::string const COMMA = ",";
  std::ostringstream s;
  s << this->MakeSentenceStart() << this->MNEMONIC << COMMA;
  if ( this->Altitude != -1 )
  {
    s << NumConv::n2s<double>()(this->Altitude,3)
      << COMMA
      << this->AltitudeUnits;
  }
  else
  {
    s << COMMA; s << this->AltitudeUnits;
  }
  if ( this->Fix == 2 || this->Fix == 3 )
  {
    s << COMMA << NumConv::n2s<int>()(this->Fix);
  }
  else
    s<< COMMA;


  GRMZ nmea;
  nmea.SetSentence(s.str());
  nmea.InsertChecksum();
  return nmea.GetSentence();
}
