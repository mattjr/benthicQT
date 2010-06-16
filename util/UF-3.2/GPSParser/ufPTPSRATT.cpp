#include "ufPTPSRATT.h"

#include <algorithm>

using namespace UF::JNS;

std::string const ATT::MNEMONIC = "ATT";


ATT::ATT()
{
  Init();
}

ATT::~ATT()
{
}

void ATT::Parse (bool const & ForceParse)
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
  GetField(1,identifier);
  if ( (identifier.rfind(this->MNEMONIC) == std::string::npos ))
  {
    SetParseStatus(NMEA::NMEA_WRONG_MNEMONIC);
    return;
  }
  GetField(2,this->UTCTimeIndicator);
  GetField(3,this->UTC);
  GetField(4,this->TrueHeading);
  GetField(5,this->Pitch);
  GetField(6,this->BasePositionType);

  SetParseStatus(NMEA::NMEA_OK);
  return;
}

std::string ATT::MakeSentence() const
{
  static std::string const COMMA = ",";
  std::ostringstream s;
  s << "$PTPSR" << COMMA
    << this->MNEMONIC << COMMA
    << this->UTCTimeIndicator << COMMA;
  if ( this->UTC == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<double>()(this->UTC,2,9,'0') << COMMA;
  }
  if ( this->TrueHeading == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<double>()(this->TrueHeading,3,7,'0') << COMMA;
  }
  if ( this->Pitch == -91 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<double>()(this->Pitch,3,6,'0') << COMMA;
  }
  s << this->BasePositionType;

  ATT nmea;
  nmea.SetSentence(s.str());
  nmea.InsertChecksum();
  return nmea.GetSentence();
}
