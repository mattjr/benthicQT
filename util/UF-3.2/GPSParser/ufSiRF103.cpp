#include "ufSiRF103.h"

#include <algorithm>

using namespace UF::SiRF;

std::string const SiRF103::MNEMONIC = "SRF103";


SiRF103::SiRF103()
{
  Init();
}

SiRF103::~SiRF103()
{
}

void SiRF103::Parse (bool const & ForceParse)
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
  GetField(1,this->Msg);
  GetField(2,this->Mode);
  GetField(3,this->Rate);
  GetField(4,this->CksumEnable);

  SetParseStatus(NMEA::NMEA_OK);
}

std::string SiRF103::MakeSentence() const
{
  static std::string const COMMA = ",";
  std::ostringstream s;
  s << this->MakeSentenceStart() << this->MNEMONIC << COMMA;
  if ( this->Msg != 255 )
    s << NumConv::n2s<unsigned int>()(this->Msg,0,2,'0');
  else
    s << COMMA;
  s << COMMA;
  if ( this->Mode != 255 )
    s << NumConv::n2s<unsigned int>()(this->Mode,0,2,'0');
  else
    s << COMMA;
  s << COMMA;
  if ( this->Rate < 2 )
    s << NumConv::n2s<unsigned int>()(this->Rate,0,2,'0');
  else
    s << COMMA;
  s << COMMA;
  if ( this->CksumEnable < 2 )
    s << NumConv::n2s<unsigned int>()(this->CksumEnable,0,2,'0');
  else
    s << COMMA;

  SiRF103 nmea;
  nmea.SetSentence(s.str());
  nmea.InsertChecksum();
  return nmea.GetSentence();
}
