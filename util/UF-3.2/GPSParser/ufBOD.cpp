#include "ufBOD.h"

#include <algorithm>
#include <sstream>

using namespace UF::NMEA;

std::string const BOD::MNEMONIC = "BOD";


BOD::BOD()
{
  Init(); //Initialise
}

BOD::~BOD()
{
  Init();
}

void BOD::Parse(bool const & ForceParse)
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
  GetField(1,this->TrueBearing);
  GetField(2,this->NorthReferenceIndicator);
  GetField(3,this->MagneticBearing);
  GetField(4,this->MagneticNorthReferenceIndicator);
  GetField(5,this->DestinationWaypoint);
  GetField(6,this->OriginWaypoint);

  SetParseStatus(NMEA::NMEA_OK);
}

std::string BOD::MakeSentence() const
{
  static std::string const COMMA = ",";
  std::ostringstream s;
  s << this->MakeSentenceStart() << this->MNEMONIC << COMMA;
  if ( this->TrueBearing != -1 )
  {
    s << NumConv::n2s<double>()(this->TrueBearing,2) << COMMA;
    s << this->NorthReferenceIndicator << COMMA;
  }
  else
  {
    s << COMMA;
    s << this->NorthReferenceIndicator << COMMA;
  }
  if ( this->MagneticBearing != -1 )
  {
    s << NumConv::n2s<double>()(this->MagneticBearing,2) << COMMA;
    s << this->MagneticNorthReferenceIndicator << COMMA;
  }
  else
  {
    s << COMMA;
    s << this->MagneticNorthReferenceIndicator << COMMA;
  }
  s << this->DestinationWaypoint;
  s << COMMA;
  s << this->OriginWaypoint;

  BOD nmea;
  nmea.SetSentence(s.str());
  nmea.InsertChecksum();
  return nmea.GetSentence();
}
