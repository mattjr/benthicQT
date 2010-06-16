#include "ufRTE.h"

#include <algorithm>

typedef std::vector < std::string > ufWaypointsTypeBase;
class UF::NMEA::ufWaypointsType : public ufWaypointsTypeBase
{};

using namespace UF::NMEA;

std::string const RTE::MNEMONIC = "RTE";

RTE::RTE()
{
  this->Waypoints = new ufWaypointsType;
  Init();
}

RTE::~RTE()
{
  delete this->Waypoints;
}

void RTE::Init()
{
  this->sentence.clear();
  this->ProprietarySentenceIndicator = false;
  this->ParseStatus = NMEA_NOT_PARSED;

  this->TotalNumberOfRTEMessages = -1;
  this->MessageNumber = -1;
  this->RouteFlag = 'c';
  this->Waypoints->clear();
}

RTE::RTE ( RTE const & rhs )
{

  this->sentence = rhs.sentence;
  this->ParseStatus = rhs.ParseStatus;
  this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

  this->TotalNumberOfRTEMessages = rhs.TotalNumberOfRTEMessages;
  this->MessageNumber = rhs.MessageNumber;
  this->RouteFlag = rhs.RouteFlag;
  this->Waypoints->clear();
  for ( ufWaypointsType::const_iterator p = rhs.Waypoints->begin(); p != rhs.Waypoints->end(); ++p )
  {
    this->Waypoints->push_back(*p);
  }
}

RTE RTE::operator = ( RTE  const & rhs )
{
  if ( this == &rhs )
    return *this;

  this->sentence = rhs.sentence;
  this->ParseStatus = rhs.ParseStatus;
  this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

  this->TotalNumberOfRTEMessages = rhs.TotalNumberOfRTEMessages;
  this->MessageNumber = rhs.MessageNumber;
  this->RouteFlag = rhs.RouteFlag;
  this->Waypoints->clear();
  for ( ufWaypointsType::const_iterator p = rhs.Waypoints->begin(); p != rhs.Waypoints->end(); ++p )
  {
    this->Waypoints->push_back(*p);
  }

  return *this;
}

bool RTE::operator == ( RTE const & rhs )
{
  NMEASentence *p = this;
  if (
    *p == rhs &&
    this->TotalNumberOfRTEMessages == rhs.TotalNumberOfRTEMessages &&
    this->MessageNumber == rhs.MessageNumber &&
    this->RouteFlag == rhs.RouteFlag)
  {
    ufWaypointsType::const_iterator q = rhs.Waypoints->begin();
    for ( ufWaypointsType::const_iterator p = this->Waypoints->begin(); p != this->Waypoints->end(); ++p )
    {
      if ( *p != * q )
      {
        return false;
      }
    }
    return true;
  }
  return false;
}

std::size_t RTE::WayPointListSz ()
{
  std::size_t commas = Waypoints->size();
  if ( commas != 0 )
  {
    std::size_t chars = 0;
    for ( ufWaypointsTypeBase::const_iterator p = Waypoints->begin(); p != Waypoints->end(); ++p )
      chars += p->size();
    return commas + chars - 1;
  }
  return commas;
}


void RTE::Parse (bool const & ForceParse)
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
  GetField(1,this->TotalNumberOfRTEMessages);
  GetField(2,this->MessageNumber);
  GetField(3,this->RouteFlag);
  if ( this->TotalNumberOfRTEMessages > 0 )
  {
    int dataFields = NumberOfDataFields();

    for ( int i = 4; i < dataFields; ++i )
    {
      std::string s;
      GetField(i,s);
      this->Waypoints->push_back(s);
    }
  }

  SetParseStatus(NMEA_OK);
}

std::string RTE::MakeSentence() const
{
  static std::string const COMMA = ",";
  std::ostringstream s;
  s << this->MakeSentenceStart() << this->MNEMONIC << COMMA;
  if ( this->TotalNumberOfRTEMessages == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<int>()(this->TotalNumberOfRTEMessages,0) << COMMA;
  }
  if ( this->MessageNumber == -1 )
    s << COMMA;
  else
  {
    s << NumConv::n2s<int>()(this->MessageNumber,0) << COMMA;
  }
  s << this->RouteFlag;
  if ( !this->Waypoints->empty() )
    for ( ufWaypointsTypeBase::const_iterator  p = this->Waypoints->begin(); p != this->Waypoints->end(); ++p )
    {
      s << COMMA;
      s << *p;
    }
  else
    s<< COMMA;

  RTE nmea;
  nmea.SetSentence(s.str());
  nmea.InsertChecksum();
  return nmea.GetSentence();
}
