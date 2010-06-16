#include "ufGSA.h"

#include <algorithm>

using namespace UF::NMEA;

std::string const GSA::MNEMONIC = "GSA";


GSA::GSA()
{
  Init();
}

GSA::~GSA()
{
}

void GSA::Parse(bool const & ForceParse)
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
  GetField(1,this->ModeIndicator);
  GetField(2,this->PositionModeIndicator);
  GetField(3,this->Channel01);
  GetField(4,this->Channel02);
  GetField(5,this->Channel03);
  GetField(6,this->Channel04);
  GetField(7,this->Channel05);
  GetField(8,this->Channel06);
  GetField(9,this->Channel07);
  GetField(10,this->Channel08);
  GetField(11,this->Channel09);
  GetField(12,this->Channel10);
  GetField(13,this->Channel11);
  GetField(14,this->Channel12);
  GetField(15,this->PDOP);
  GetField(16,this->HDOP);
  GetField(17,this->VDOP);

  SetParseStatus(NMEA_OK);
  return;
}

std::string GSA::MakeSentence() const
{
  static std::string const COMMA = ",";
  std::ostringstream s;
  s << this->MakeSentenceStart() << this->MNEMONIC << COMMA
    << this->ModeIndicator << COMMA
    << this->PositionModeIndicator << COMMA;
  if(this->Channel01 == -1) s << COMMA; else { s << NumConv::n2s<int>()(this->Channel01,2,2,'0') << COMMA; }
  if(this->Channel02 == -1) s << COMMA; else { s << NumConv::n2s<int>()(this->Channel02,2,2,'0') << COMMA; }
  if(this->Channel03 == -1) s << COMMA; else { s << NumConv::n2s<int>()(this->Channel03,2,2,'0') << COMMA; }
  if(this->Channel04 == -1) s << COMMA; else { s << NumConv::n2s<int>()(this->Channel04,2,2,'0') << COMMA; }
  if(this->Channel05 == -1) s << COMMA; else { s << NumConv::n2s<int>()(this->Channel05,2,2,'0') << COMMA; }
  if(this->Channel06 == -1) s << COMMA; else { s << NumConv::n2s<int>()(this->Channel06,2,2,'0') << COMMA; }
  if(this->Channel07 == -1) s << COMMA; else { s << NumConv::n2s<int>()(this->Channel07,2,2,'0') << COMMA; }
  if(this->Channel08 == -1) s << COMMA; else { s << NumConv::n2s<int>()(this->Channel08,2,2,'0') << COMMA; }
  if(this->Channel09 == -1) s << COMMA; else { s << NumConv::n2s<int>()(this->Channel09,2,2,'0') << COMMA; }
  if(this->Channel10 == -1) s << COMMA; else { s << NumConv::n2s<int>()(this->Channel10,2,2,'0') << COMMA; }
  if(this->Channel11 == -1) s << COMMA; else { s << NumConv::n2s<int>()(this->Channel11,2,2,'0') << COMMA; }
  if(this->Channel12 == -1) s << COMMA; else { s << NumConv::n2s<int>()(this->Channel12,2,2,'0') << COMMA; }
  if(this->PDOP == -1) s << COMMA; else { s << NumConv::n2s<float>()(this->PDOP,1,4,'0') << COMMA; }
  if(this->HDOP == -1) s << COMMA; else { s << NumConv::n2s<float>()(this->HDOP,1,4,'0') << COMMA; }
  if(this->VDOP == -1) s << COMMA; else { s << NumConv::n2s<float>()(this->VDOP,1,4,'0'); }

  GSA nmea;
  nmea.SetSentence(s.str());
  nmea.InsertChecksum();
  return nmea.GetSentence();
}
