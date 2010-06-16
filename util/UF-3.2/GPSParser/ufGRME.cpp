#include "ufGRME.h"

#include <algorithm>

using namespace UF::Garmin;

std::string const GRME::MNEMONIC = "GRME";


GRME::GRME()
{
  Init();
}

GRME::~GRME()
{
}

void GRME::Parse (bool const & ForceParse)
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
  GetField(1,this->HPE);
  GetField(2,this->HPEUnits);
  GetField(3,this->VPE);
  GetField(4,this->VPEUnits);
  GetField(5,this->SPE);
  GetField(6,this->SPEUnits);

  SetParseStatus(NMEA::NMEA_OK);
  return;
}

std::string GRME::MakeSentence() const
{
  static std::string const COMMA = ",";
  std::ostringstream s;
  s  << this->MakeSentenceStart() << this->MNEMONIC << COMMA;
  if ( this->HPE != -1 )
  {
    s << NumConv::n2s<double>()(this->HPE,1);
    s << COMMA; s << this->HPEUnits<< COMMA;
  }
  else
  {
    s << COMMA; s << this->HPEUnits<< COMMA;
  }
  if ( this->VPE != -1 )
  {
    s << NumConv::n2s<double>()(this->VPE,1);
    s << COMMA; s << this->VPEUnits<< COMMA;
  }
  else
  {
    s << COMMA; s << this->VPEUnits<< COMMA;
  }
  if ( this->VPE != -1 )
  {
    s << NumConv::n2s<double>()(this->SPE,1);
    s << COMMA; s << this->SPEUnits;
  }
  else
  {
    s << COMMA; s << this->SPEUnits;
  }

  GRME nmea;
  nmea.SetSentence(s.str());
  nmea.InsertChecksum();
  return nmea.GetSentence();
}
