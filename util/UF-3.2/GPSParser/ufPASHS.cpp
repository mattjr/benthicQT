#include "ufPASHS.h"

#include <algorithm>

using namespace UF::ASHTECH;

using std::size_t;

std::string const PASHS::MNEMONIC = "ASHS";


PASHS::PASHS()
{
  Init();
}

PASHS::~PASHS()
{
}

void PASHS::Parse (bool const & ForceParse)
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
  GetField(1,this->Command);
  // The command parameters are the remainder of the sentence.
  // The parameters are comma delimited, we are just extracting
  // them all as one string.
  size_t beg;
  size_t end;
  std::string s = this->GetSentence();
  beg = s.find(this->Command) + this->Command.size() + 1;
  end = s.rfind("*");
  this->CommandParameters = s.substr(beg,end-beg);

  SetParseStatus(NMEA::NMEA_OK);
  return;
}

std::string PASHS::MakeSentence() const
{
  static std::string const COMMA = ",";
    std::ostringstream s;
  s  << this->MakeSentenceStart() << this->MNEMONIC << COMMA;
  if ( !this->Command.empty() )
    s << this->Command;
  else
    s << COMMA;
  s << COMMA;
  if ( !this->CommandParameters.empty() )
    s << this->CommandParameters;

  PASHS nmea;
  nmea.SetSentence(s.str());
  nmea.InsertChecksum();
  return nmea.GetSentence();
}
