#include "ufNMEASentence.h"

#include <locale>
#include <iomanip>

using namespace UF;

NMEA::byte const NMEA::NMEASentence::CR = '\r';
NMEA::byte const NMEA::NMEASentence::LF = '\n';
NMEA::byte const NMEA::NMEASentence::COMMA = ',';

NMEA::NMEASentence::NMEASentence():
  ProprietarySentenceIndicator(false)
  , ParseStatus(NMEA_NOT_PARSED)
{
}

NMEA::NMEASentence::~NMEASentence()
{
}

NMEA::byte NMEA::NMEASentence::ComputeChecksum( std::string &sentence )
{
  byte checksum = 0;
  for (std::string::iterator p = sentence.begin(); p != sentence.end(); ++p)
  {
    // Skip over the leading $
    if ( *p == '$' && p == sentence.begin() ) continue;
    // Check to see if we have reached the end of the sentence.
    if ( *p == '*' || *p == CR || *p == LF ) break;
    checksum ^= *p;
  }
  return checksum;
}

void NMEA::NMEASentence::InsertChecksum()
{
  byte checksum = ComputeChecksum();
  std::string::iterator p = this->sentence.begin();
  while ( p != this->sentence.end() )
  {
    // Check to see if we have reached the end of the sentence.
    if ( *p == '*' || *p == CR || *p == LF ) break;
    ++p;
  }
  if ( p != this->sentence.end() ) // Remove trailing characters.
    p = this->sentence.erase(p,this->sentence.end());
  if ( !this->sentence.empty() ) // Add in the checksum.
  {
    this->sentence.push_back('*');
    std::ostringstream os;
    os << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)checksum;
    std::string s = os.str();
    this->sentence += s;
    this->sentence.push_back(CR);
    this->sentence.push_back(LF);
  }
}

std::string NMEA::NMEASentence::ReadChecksum( std::string &sentence )
{
  std::string s;
  if ( sentence.empty() )
  {
    return s;
  }
  std::string::iterator p = sentence.begin();
  while ( *p != '*' )
  {
    ++p;
    if ( p == sentence.end() ) return s;
  }
  ++p;
  // Extract the field.
  for ( ;p != sentence.end(); ++p )
  {
    if ( !(*p == CR || *p == LF) )
    {
      s.push_back(*p);
    }
  }
  return s;
}

NMEA::TAG_NMEA_STATUS NMEA::NMEASentence::ValidChecksum ( std::string &sentence )
{
  std::string s = ReadChecksum(sentence);
  if ( s.size() != 2 ) return NMEA_UNKNOWN;
  std::ostringstream os;
  os << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)ComputeChecksum(sentence);
  if ( s == os.str() )
    return NMEA_VALID_CHECKSUM;
  return NMEA_INVALID_CHECKSUM;
}


bool NMEA::NMEASentence::ValidNMEASentence( std::string &sentence )
{
    return ( ValidChecksum( sentence ) == NMEA_VALID_CHECKSUM );
}


int NMEA::NMEASentence::NumberOfDataFields() const
{
  if ( this->sentence.empty() )
  {
    return 0;
  }
  int n = 0;
  std::string::const_iterator p = this->sentence.begin();
  while ( !( *p == '*' || *p == CR || *p == LF ) )
  {
    ++p;
    if ( p == this->sentence.end() ) return n;
    if ( *p == COMMA ) ++n;
  }
  return n+1;
}

std::string NMEA::NMEASentence::Field ( int const & fieldNumber )
{
  std::string s;
  if ( fieldNumber < 0 || ( fieldNumber > NumberOfDataFields() - 1 ) ) return s;
  std::string::iterator p = this->sentence.begin();
  int n = -1;
  while ( p != this->sentence.end() )
  {
    // Skip over the leading $
    if ( *p == '$' && p == this->sentence.begin() )
    {
      ++p;
      continue;
    }
    // Check to see if we have reached the end of the sentence.
    if ( *p == '*' || *p == CR || *p == LF ) break;
    ++n;
    if ( n == fieldNumber ) // We have the start of the desired field.
    {
      // Extract the field.
      while( !(*p == COMMA ||  *p == '*' || *p == CR || *p == LF || p == this->sentence.end()) )
      {
        s.push_back(*p);
        ++p;
      }
      break;
    }
    // Skip along until we find the end of the field.
    while ( !(*p == COMMA ||  *p == '*' || *p == CR || *p == LF || p == this->sentence.end()) ) ++p;
    if ( p == this->sentence.end() ) break;
    ++p;
  }
  return s;
}

std::string NMEA::NMEASentence::GetSentence() const
{
  return this->sentence;
}

void NMEA::NMEASentence::GetSentence ( std::string & sentence )
{
  sentence = this->sentence;
}

void NMEA::NMEASentence::SetSentence ( std::string const & sentence )
{
  this->sentence = sentence;
  //std::locale loc;
  //std::use_facet<std::ctype<char> > ( loc ).toupper( &this->sentence[0], &this->sentence[this->sentence.size() - 1] );
}



std::string NMEA::ErrStatus ( TAG_NMEA_STATUS const & status )
{
  std::string s;

  switch ( status )
  {
  case NMEA_OK:
    s = "parse was OK.";
    break;
  case NMEA_NAVIGATION_RECEIVER_WARNING:
    s = "navigation receiver warning.";
    break;
  case NMEA_VALID_CHECKSUM:
    s = "valid checksum.";
    break;
  case NMEA_INVALID_CHECKSUM:
    s = "invalid checksum.";
    break;
  case NMEA_UNKNOWN:
    s = "unknown message.";
    break;
  case NMEA_WRONG_MNEMONIC:
    s = "wrong nmemonic.";
    break;
  case NMEA_WRONG_MESSAGE_LENGTH:
    s = "wrong message length.";
    break;
  case NMEA_INVALID_FIELD:
    s = "invalid field.";
    break;
  case NMEA_SIMULATING:
    s = "simulating.";
    break;
  case NMEA_NOT_PARSED:
    s = "not parsed.";
    break;
  default:
    s = "unknown reason for failure.";
  }
  return s;
}

std::string NMEA::GetMnemonic(std::string& Sentence)
{
  std::string mnemonic;

  //get and check for valid start sentinel
  std::size_t startPos = Sentence.find( '$' );
  if(startPos == std::string::npos)
  {
    return mnemonic ;
  }

  //get and check for first comma after start sentinel
  std::size_t commaPos = Sentence.find( ',', startPos+1 );
  if( commaPos == std::string::npos)
  {
    return mnemonic ;
  }

  mnemonic = Sentence.substr(startPos+1,(commaPos - startPos - 1));

  return mnemonic ;
}


NMEA::TAG_NMEA_STATUS NMEA::ParseNMEASentence( std::string const & sentence, NMEA::NMEASentence & nmea, std::string & errorMessage )
{
  //status flag for
  NMEA::TAG_NMEA_STATUS status = NMEA::NMEA_NOT_PARSED;

  nmea.SetSentence(sentence);
  nmea.Parse();
  status = nmea.GetParseStatus();

  if ( !(status == NMEA::NMEA_OK ||
    status == NMEA::NMEA_SIMULATING ||
    status == NMEA::NMEA_NAVIGATION_RECEIVER_WARNING) )
  {
    errorMessage.clear();
    errorMessage = "Warning - ";
    errorMessage += NMEA::ErrStatus(status);
  }

  return status;
}

