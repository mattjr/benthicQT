#include "ufRMC.h"

#include <algorithm>

using namespace UF::NMEA;

std::string const RMC::MNEMONIC = "RMC";

RMC::RMC()
{
  Init();
}

RMC::~RMC()
{
}

void RMC::Parse (bool const & ForceParse)
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
  if ( (identifier.rfind(this->MNEMONIC) == std::string::npos ) )
  {
    SetParseStatus(NMEA_WRONG_MNEMONIC);
    return;
  }
  GetField(1,this->UTC);
  GetField(2,this->Status);
  GetField(3,this->Latitude);
  GetField(4,this->LatitudeSector);
  GetField(5,this->Longitude);
  GetField(6,this->LongitudeSector);
  GetField(7,this->SpeedOverGround);
  GetField(8,this->CourseOverGround);
  GetField(9,this->Date);
  GetField(10,this->MagneticVariation);
  GetField(11,this->DirectionOfMagneticVariation);

  int dataFields = NumberOfDataFields();

  if ( dataFields == 14 )
  {
    GetField(dataFields - 2,this->ReceiverStatus);
    if ( this->ReceiverStatus == 'S' )
    {
      SetParseStatus(NMEA_SIMULATING);
      return;
    }
  }
  if ( this->ReceiverStatus == 'V' )
  {
    SetParseStatus(NMEA_NAVIGATION_RECEIVER_WARNING);
    return;
  }
  SetParseStatus(NMEA_OK);
  return;
}

std::string RMC::MakeSentence() const
{
  static std::string const COMMA = ",";
  std::ostringstream s;
  s << this->MakeSentenceStart() << this->MNEMONIC << COMMA;
  if ( this->UTC == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<double>()(this->UTC,2,9,'0') << COMMA;
  }
  s << this->Status << COMMA;
  if ( this->Latitude == -1 )
  {
    s << COMMA;
    s << COMMA;
    //s << this->LatitudeSector << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->Latitude,6,11,'0') << COMMA;
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
    s << NumConv::n2s<double>()(this->Longitude,6,12,'0') << COMMA;
    s << this->LongitudeSector << COMMA;
  }
  if ( this->SpeedOverGround == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<double>()(this->SpeedOverGround,1) << COMMA;
  }
  if ( this->CourseOverGround == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<double>()(this->CourseOverGround,1,5,'0') << COMMA;
  }
  if ( this->Date == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<int>()(this->Date,0,6,'0') << COMMA;
  }
  if ( this->MagneticVariation == -1 )
  {
    s << COMMA;
    s << this->DirectionOfMagneticVariation;
  }
  else
  {
    s << NumConv::n2s<double>()(this->MagneticVariation,1) << COMMA;
    s << this->DirectionOfMagneticVariation;
  }
    if ( NumberOfDataFields() == 14 )
    {
      s << COMMA;
      s << this->ReceiverStatus;
    }

  RMC nmea;
  nmea.SetSentence(s.str());
  nmea.InsertChecksum();
  return nmea.GetSentence();
}
