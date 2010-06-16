#include "ufWPL.h"

#include <algorithm>

using namespace UF::NMEA;

std::string const WPL::MNEMONIC = "WPL";


WPL::WPL()
{
  Init();
}

WPL::~WPL()
{
}

void WPL::Parse (bool const & ForceParse)
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
  GetField(1,this->Latitude);
  GetField(2,this->LatitudeSector);
  GetField(3,this->Longitude);
  GetField(4,this->LongitudeSector);
  GetField(5,this->Waypoint);

  SetParseStatus(NMEA_OK);
}

std::string WPL::MakeSentence() const
{
  static std::string const COMMA = ",";
  std::ostringstream s;
  s << this->MakeSentenceStart() << this->MNEMONIC << COMMA;
  if ( this->Latitude == -1 )
  {
    s << COMMA;
    s << COMMA;
    //s << this->LatitudeSector << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->Latitude,6) << COMMA;
    s << this->LatitudeSector << COMMA;
  }
  if ( this->Longitude == -1 )
  {
    s << COMMA;
    s << COMMA;
    //s << this->LongitudeSector << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->Longitude,6) << COMMA;
    s << this->LongitudeSector << COMMA;
  }
  s << this->Waypoint;

  WPL nmea;
  std::string t;

  nmea.SetSentence(s.str());
  nmea.InsertChecksum();
  return nmea.GetSentence();
}
