#include "ufXTE.h"

#include <algorithm>

using namespace UF::NMEA;

std::string const XTE::MNEMONIC = "XTE";


XTE::XTE()
{
  Init();
}

XTE::~XTE()
{
}

void XTE::Parse (bool const & ForceParse)
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
  GetField(1,this->Status);
  GetField(2,this->CycleLock);
  GetField(3,this->CrossTrackError);
  GetField(4,this->DirectionToSteer);
  GetField(5,this->CrossTrackUnits);

  int dataFields = NumberOfDataFields();

  if ( dataFields == 8 )
  {
    GetField(dataFields - 2,this->ReceiverStatus);
    if ( this->ReceiverStatus == 'S' )
    {
      SetParseStatus(NMEA_SIMULATING);
      return;
    }
  }

  if ( Status == 'V' )
  {
    SetParseStatus(NMEA_NAVIGATION_RECEIVER_WARNING);
    return;
  }

  SetParseStatus(NMEA_OK);
}

std::string XTE::MakeSentence() const
{
  static std::string const COMMA = ",";
  std::ostringstream s;
  s << this->MakeSentenceStart() << this->MNEMONIC << COMMA
    << this->Status << COMMA
    << this->CycleLock << COMMA;
  if ( this->CrossTrackError == -1 )
  {
    s << COMMA;
  }
  else
  {
    s << NumConv::n2s<double>()(this->CrossTrackError,2) << COMMA;
  }
  s << this->DirectionToSteer;
  s << COMMA;
  s << this->CrossTrackUnits;
  if ( NumberOfDataFields() == 8 )
  {
    s << COMMA;
    s << this->ReceiverStatus;
  }

  XTE nmea;
  nmea.SetSentence(s.str());
  nmea.InsertChecksum();
  return nmea.GetSentence();
}
