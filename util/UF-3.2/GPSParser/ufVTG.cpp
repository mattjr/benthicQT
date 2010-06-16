#include "ufVTG.h"

#include <algorithm>

using namespace UF::NMEA;

std::string const VTG::MNEMONIC = "VTG";


VTG::VTG()
{
  Init();
}

VTG::~VTG()
{
}

void VTG::Parse (bool const & ForceParse)
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
  GetField(1,this->COG);
  GetField(2,this->NorthReferenceIndicator);
  GetField(3,this->MagneticCOG);
  GetField(4,this->MagneticNorthReferenceIndicator);
  GetField(5,this->SOG);
  GetField(6,this->SpeedUnitOfMeasure);
  GetField(7,this->KPH_SOG);
  GetField(8,this->KPH_SpeedUnitOfMeasure);

  SetParseStatus(NMEA_OK);
}

std::string VTG::MakeSentence() const
{
  static std::string const COMMA = ",";
  std::ostringstream s;
  s << this->MakeSentenceStart() << this->MNEMONIC << COMMA;
  if ( this->COG == -1 )
  {
    s << COMMA;
    s << this->NorthReferenceIndicator << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->COG,2,6,'0') << COMMA;
    s << this->NorthReferenceIndicator << COMMA;
  }
  if ( this->MagneticCOG == -1 )
  {
    s << COMMA;
    s << this->MagneticNorthReferenceIndicator << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->MagneticCOG,2,6,'0') << COMMA;
    s << this->MagneticNorthReferenceIndicator << COMMA;
  }
  if ( this->SOG == -1 )
  {
    s << COMMA;
    s << this->SpeedUnitOfMeasure << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->SOG,2,6,'0') << COMMA;
    s << this->SpeedUnitOfMeasure << COMMA;
  }
  if ( this->KPH_SOG == -1 )
  {
    s << COMMA;
    s << this->KPH_SpeedUnitOfMeasure;
  }
  else
  {
    s << NumConv::n2s<double>()(this->KPH_SOG,2,6,'0') << COMMA;
    s << this->KPH_SpeedUnitOfMeasure;
  }

  VTG nmea;
  nmea.SetSentence(s.str());
  nmea.InsertChecksum();
  return nmea.GetSentence();
}
