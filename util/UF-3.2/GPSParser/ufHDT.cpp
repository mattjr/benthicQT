#include "ufHDT.h"

#include <algorithm>

using namespace UF::NMEA;

std::string const HDT::MNEMONIC = "HDT";


HDT::HDT()
{
  Init();
}

HDT::~HDT()
{
}

void HDT::Parse (bool const & ForceParse)
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
  GetField(1,this->TrueHeading);
  GetField(2,this->HeadingIndicator);

  SetParseStatus(NMEA_OK);
  return;

}

std::string HDT::MakeSentence() const
{
  static std::string const COMMA = ",";
  std::ostringstream s;
  s  << this->MakeSentenceStart() << this->MNEMONIC << COMMA;
  if ( this->TrueHeading == -1 )
  {
    s << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->TrueHeading,3,7,'0') << COMMA;
  }
  s << this->HeadingIndicator;

  HDT nmea;
  nmea.SetSentence(s.str());
  nmea.InsertChecksum();
  return nmea.GetSentence();
}
