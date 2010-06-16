#include "ufCASDTC.h"

#include <algorithm>

using namespace UF::CAS;

std::string const CASDTC::MNEMONIC = "CASDTC";

CASDTC::CASDTC()
{
  Init();
}

CASDTC::~CASDTC()
{
}

void CASDTC::Parse(bool const & ForceParse)
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
    this->SetParseStatus(NMEA::NMEA_WRONG_MNEMONIC);
    return;
  }
  GetField(1,this->Year);
  GetField(2,this->Month);
  GetField(3,this->Day);
  GetField(4,this->Hour);
  GetField(5,this->Minute);
  GetField(6,this->Second);
  GetField(7,this->Corrected);
  GetField(8,this->Comment);

  this->SetParseStatus(NMEA::NMEA_OK);
  return;
}

std::string CASDTC::MakeSentence() const
{
  static std::string const COMMA = ",";
  std::ostringstream s;
  s << this->MakeSentenceStart() << this->MNEMONIC << COMMA;
    s << NumConv::n2s<int>()(this->Year,4) << COMMA;
    if ( this->Month > 0 && this->Month < 13 )
      s << NumConv::n2s<int>()(this->Month,2,0,'0');
    s << COMMA;
    if ( this->Day > 0 && this->Day < 31 )
      s << NumConv::n2s<int>()(this->Day,2,0,'0');
    s << COMMA;
    if ( this->Hour >= 0 && this->Hour < 24 )
      s << NumConv::n2s<int>()(this->Hour,2,0,'0');
    s << COMMA;
    if ( this->Minute >= 0 && this->Minute < 60 )
      s << NumConv::n2s<int>()(this->Minute,2,0,'0');
    s << COMMA;
    if ( this->Second >= 0 && this->Second < 60 )
      s << NumConv::n2s<double>()(this->Second,3,6,'0');
    s << COMMA;
    if ( this->Corrected == 'u' || this->Corrected == 'U' )
      s << this->Corrected;
    s << COMMA;
    s << this->Comment;

    CASDTC nmea;
    nmea.SetSentence(s.str());
    nmea.InsertChecksum();
    return nmea.GetSentence();
}
