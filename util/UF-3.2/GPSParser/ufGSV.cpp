#include "ufGSV.h"

#include <algorithm>

using namespace UF::NMEA;

std::string const GSV::MNEMONIC = "GSV";


GSV::GSV()
{
  Init();
}

GSV::~GSV()
{
}

void GSV::Parse (bool const & ForceParse)
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
  GetField(1,this->TotalNumberOfGSVMessages);
  GetField(2,this->MessageNumber);
  GetField(3,this->TotalNumberOfSatellitesInView);
  GetField(4,this->PRN1);
  GetField(5,this->Elevation1);
  GetField(6,this->Azimuth1);
  GetField(7,this->SN1);
  GetField(8,this->PRN2);
  GetField(9,this->Elevation2);
  GetField(10,this->Azimuth2);
  GetField(11,this->SN2);
  GetField(12,this->PRN3);
  GetField(13,this->Elevation3);
  GetField(14,this->Azimuth3);
  GetField(15,this->SN3);
  GetField(16,this->PRN4);
  GetField(17,this->Elevation4);
  GetField(18,this->Azimuth4);
  GetField(19,this->SN4);

  SetParseStatus(NMEA_OK);
  return;
}

std::string GSV::MakeSentence() const
{
  static std::string const COMMA = ",";
  std::ostringstream s;
  s << this->MakeSentenceStart() << this->MNEMONIC << COMMA
    << this->TotalNumberOfGSVMessages << COMMA
    << this->MessageNumber << COMMA
    << this->TotalNumberOfSatellitesInView << COMMA;
  if ( this->PRN1 == -1 ) s << COMMA; else { s << NumConv::n2s<int>()(this->PRN1,2,2,'0'); s <<  COMMA; }
  if ( this->Elevation1 == -1 ) s << COMMA; else { s << NumConv::n2s<int>()(this->Elevation1); s <<  COMMA; }
  if ( this->Azimuth1 == -1 ) s << COMMA; else { s << NumConv::n2s<int>()(this->Azimuth1,3,3,'0'); s <<  COMMA; }
  if ( this->SN1 == -1 ) s << COMMA; else { s << NumConv::n2s<int>()(this->SN1); s <<  COMMA; }
  if ( this->PRN2 == -1 ) s << COMMA; else { s << NumConv::n2s<int>()(this->PRN2,2,2,'0'); s <<  COMMA; }
  if ( this->Elevation2 == -1 ) s << COMMA; else { s << NumConv::n2s<int>()(this->Elevation2); s <<  COMMA; }
  if ( this->Azimuth2 == -1 ) s << COMMA; else { s << NumConv::n2s<int>()(this->Azimuth2,3,3,'0'); s <<  COMMA; }
  if ( this->SN2 == -1 ) s << COMMA; else { s << NumConv::n2s<int>()(this->SN2); s <<  COMMA; }
  if ( this->PRN3 == -1 ) s << COMMA; else { s << NumConv::n2s<int>()(this->PRN3,2,2,'0'); s <<  COMMA; }
  if ( this->Elevation3 == -1 ) s << COMMA; else { s << NumConv::n2s<int>()(this->Elevation3); s <<  COMMA; }
  if ( this->Azimuth3 == -1 ) s << COMMA; else { s << NumConv::n2s<int>()(this->Azimuth3,3,3,'0'); s <<  COMMA; }
  if ( this->SN3 == -1 ) s << COMMA; else { s << NumConv::n2s<int>()(this->SN3); s <<  COMMA; }
  if ( this->PRN4 == -1 ) s << COMMA; else { s << NumConv::n2s<int>()(this->PRN4,2,2,'0'); s <<  COMMA; }
  if ( this->Elevation4 == -1 ) s << COMMA; else { s << NumConv::n2s<int>()(this->Elevation4); s <<  COMMA; }
  if ( this->Azimuth4 == -1 ) s << COMMA; else { s << NumConv::n2s<int>()(this->Azimuth4,3,3,'0'); s <<  COMMA; }
  if ( this->SN4 != -1 ) { s << NumConv::n2s<int>()(this->SN4);}

  GSV nmea;
  nmea.SetSentence(s.str());
  nmea.InsertChecksum();
  return nmea.GetSentence();
}
