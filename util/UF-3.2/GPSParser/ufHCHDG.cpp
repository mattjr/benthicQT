#include "ufHCHDG.h"

#include <algorithm>

using namespace UF::NMEA;

std::string const HCHDG::MNEMONIC = "HCHDG";


HCHDG::HCHDG()
{
  Init();
}

HCHDG::~HCHDG()
{
}

void HCHDG::Parse (bool const & ForceParse)
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
  GetField(1,this->MagneticHeading);
  GetField(2,this->MagneticDeviation1);
  GetField(3,this->MagneticDeviation2);
  GetField(4,this->MagneticVariation);
  GetField(5,this->DirectionOfMagneticVariation);

  SetParseStatus(NMEA_OK);
  return;

}

std::string HCHDG::MakeSentence() const
{
  static std::string const COMMA = ",";
  std::ostringstream s;
  s  << "$" << this->MNEMONIC << COMMA;
  if ( this->MagneticHeading == -1 )
  {
    s << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->MagneticHeading,1,5,'0') << COMMA;
  }
  if ( this->MagneticDeviation1 == -1 )
  {
    s << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->MagneticDeviation1,1,5,'0') << COMMA;
  }
  if ( this->MagneticDeviation2 == -1 )
  {
    s << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->MagneticDeviation2,1,5,'0') << COMMA;
  }
  if ( this->MagneticVariation == -1 )
  {
    s << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->MagneticVariation,1,5,'0')
      << COMMA
      << this->DirectionOfMagneticVariation;
  }

  HCHDG nmea;
  nmea.SetSentence(s.str());
  nmea.InsertChecksum();
  return nmea.GetSentence();
}
