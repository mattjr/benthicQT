#include "ufGRMM.h"

#include <algorithm>

using namespace UF::Garmin;

std::string const GRMM::MNEMONIC = "GRMM";


GRMM::GRMM()
{
  Init();
}

GRMM::~GRMM()
{
}

void GRMM::Parse (bool const & ForceParse)
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
  GetField(1,this->MapDatum);

  SetParseStatus(NMEA::NMEA_OK);
  return;
}

std::string GRMM::MakeSentence() const
{
  static std::string const COMMA = ",";
  std::ostringstream s;
  s  << this->MakeSentenceStart() << this->MNEMONIC << COMMA
    << this->MapDatum;

  GRMM nmea;
  nmea.SetSentence(s.str());
  nmea.InsertChecksum();
  return nmea.GetSentence();
}
