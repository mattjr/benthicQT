//
// C++ Implementation: CSVParser
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufCSVParser.h"

using std::size_t;

typedef std::vector < std::string > ufFieldsTypeBase;
class UF::CSV::ufFieldsType : public ufFieldsTypeBase
{};

using namespace UF::CSV;

char const UF::CSV::CSVParser::CR = '\r';
char const UF::CSV::CSVParser::LF = '\n';

CSVParser::CSVParser()
 : ParseStatus(CSV_NOT_PARSED)
 , Delimiter(',')
 , TextDelimiter('\"')
{
  this->Fields = new ufFieldsType;
  Init();
}

CSVParser::CSVParser(char const Delimiter, char const TextDelimiter)
 : ParseStatus(CSV_NOT_PARSED)
{
  this->Fields = new ufFieldsType;
  Init();
  this->Delimiter = Delimiter;
  this->TextDelimiter = TextDelimiter;
}

CSVParser::CSVParser(std::string const & Sentence, char const Delimiter, char const TextDelimiter)
 : ParseStatus(CSV_NOT_PARSED)
{
  this->Fields = new ufFieldsType;
  Init();
  this->Delimiter = Delimiter;
  this->TextDelimiter = TextDelimiter;
  this->Sentence = Sentence;
  Parse();
}


CSVParser::~CSVParser()
{
  delete this->Fields;
}

CSVParser::CSVParser ( CSVParser const & rhs )
{
  this->Sentence = rhs.Sentence;
  this->ParseStatus = rhs.ParseStatus;
  this->Fields->clear();
  for ( ufFieldsType::const_iterator p = rhs.Fields->begin(); p != rhs.Fields->end(); ++p )
  {
    this->Fields->push_back(*p);
  }
}

CSVParser & CSVParser::operator = ( CSVParser const & rhs )
{
  if ( this == & rhs )
    return *this;

  this->Sentence = rhs.Sentence;
  this->ParseStatus = rhs.ParseStatus;
  this->Fields->clear();
  for ( ufFieldsType::const_iterator p = rhs.Fields->begin(); p != rhs.Fields->end(); ++p )
  {
    this->Fields->push_back(*p);
  }
  return *this;
}

bool CSVParser::operator == ( CSVParser const & rhs )
{
  if (this->Sentence == rhs.Sentence &&
    this->ParseStatus == rhs.ParseStatus &&
    this->Fields == rhs.Fields) 
  {
    ufFieldsType::const_iterator q = rhs.Fields->begin();
    for ( ufFieldsType::const_iterator p = this->Fields->begin(); p != this->Fields->end(); ++p )
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

std::size_t CSVParser::NumberOfDataFields()
{
  return Fields->size();
}

bool CSVParser::GetField ( std::size_t const &fieldNumber, std::string & value )
{
  std::string s = Fields->at(fieldNumber);
  if ( !s.empty() )
  {
    value = s;
    return true;
  }
  return false;
}

void CSVParser::SetField ( std::size_t const &fieldNumber, std::string const & value )
{
  Fields->at(fieldNumber) = value;
}

void CSVParser::DelimitField ( std::size_t const &fieldNumber, std::string const & Delimiter )
{
  std::string s(Delimiter);
  s += Fields->at(fieldNumber);
  s += Delimiter;
  Fields->at(fieldNumber) = s;
}

void CSVParser::Reverse ()
{
  std::reverse(Fields->begin(),Fields->end());
}

std::vector < std::string > CSVParser::GetFields()
{
  ufFieldsType f;
  for ( ufFieldsType::const_iterator p = Fields->begin(); p != Fields->end(); ++p )
  {
    f.push_back(*p);
  }
  return f;
}

void CSVParser::SetFields(std::vector < std::string > const & Fields)
{
  this->Fields->clear();
  for ( ufFieldsType::const_iterator p = Fields.begin(); p != Fields.end(); ++p )
  {
    this->Fields->push_back(*p);
  }
}

void CSVParser::DropField (const std::size_t & FieldNo )
{
  if ( FieldNo >= this->Fields->size() )
    return;
  std::vector < std::string > f;
  for ( std::size_t i = 0; i < FieldNo; ++i )
    f.push_back(this->Fields->at(i));
  for ( std::size_t i = FieldNo + 1; i < Fields->size(); ++i )
    f.push_back(this->Fields->at(i));

  this->SetFields(f);
}

std::size_t CSVParser::FieldSize ( std::size_t const &fieldNumber )
{ 
  return this->Fields->at(fieldNumber).size();
}

void CSVParser::Init()
{
  this->Sentence.clear();
  this->Fields->clear();
  this->SetParseStatus(CSV_NOT_PARSED);
}

TAG_CSV_STATUS CSVParser::Parse()
{
  // The sentence could be empty.
  if ( this->Sentence.empty() )
  {
    this->SetParseStatus(CSV_OK);
    return this->GetParseStatus();
  }

  std::string s;
  std::string::iterator p = this->Sentence.begin();
  //if ( p == this->Sentence.end() )
  //{
  //  s.clear();
  //  this->Fields.push_back(s);
  //  this->SetParseStatus(CSV_OK);
  //  return this->GetParseStatus();
  //}
  bool delim = true; // Assume the start of the sentence has an implicit delimiter.
  while ( !( *p == this->CR || *p == this->LF ) )
    {
    // If a text delimiter is present we need to skip along until we find the next one.
    if (delim && *p == this->TextDelimiter)
      {
      ++p;
      if ( p == this->Sentence.end() )
        break;
      s.clear();
      while ( !( *p == this->CR || *p == this->LF ) && *p != this->TextDelimiter )
        {
        s.push_back(*p);
        ++p;
        if ( p == this->Sentence.end() )
          break;
        }
      this->Fields->push_back(s);
      if ( *p == this->TextDelimiter )
        ++p;
      if ( p == this->Sentence.end() )
        break;
      }
    else
      {
      if ( p == this->Sentence.end() )
        break;
      s.clear();
      while ( !( *p == this->CR || *p == this->LF ) && *p != this->Delimiter )
        {
        s.push_back(*p);
        ++p;
        if ( p == this->Sentence.end() )
          break;
        }
      this->Fields->push_back(s);
      }
    delim = false;
    if ( p == this->Sentence.end() )
      break;
    if ( *p == this->Delimiter )
      delim = true;
    ++p;
    if ( p == this->Sentence.end() )
      break;
    }
  this->SetParseStatus(CSV_OK);
  return this->GetParseStatus();
}

std::string CSVParser::MakeSentence()
{
  std::string s;
  std::vector < std::string >::iterator q = this->Fields->begin();
  for ( std::vector < std::string >::iterator p = this->Fields->begin(); p != this->Fields->end(); ++p )
    {
    q += 1;
    if ( std::find(p->begin(),p->end(),this->Delimiter) == p->end() )
      {
      s += *p;
      if ( q != this->Fields->end() )
        s += this->Delimiter;
      }
    else
      {
      s += this->TextDelimiter;
      s += *p;
      s += this->TextDelimiter;
      if ( q != this->Fields->end() )
        s += this->Delimiter;
      }
    }
  return s;

}

void CSVParser::MergeFields (const size_t & Field1, const size_t & Field2, std::string const & Separator  )
{
  if ( this->ParseStatus != CSV_OK )
  {
    // Attempt a parse

    if ( this->Parse() != CSV_OK )
      throw("Unable to merge fields.");
  }
  std::string s;
  std::vector < std::string > f;
  if ( Field1 < Field2 )
  {
    s = this->Fields->at(Field1)+Separator+this->Fields->at(Field2);
    for ( size_t i = 0; i < Field1; ++i )
      f.push_back(this->Fields->at(i));
    f.push_back(s);
    for ( size_t i = Field1+1; i < this->Fields->size(); ++i )
      if ( i != Field2 )
        f.push_back(this->Fields->at(i));
      this->SetFields(f);
  }
  if ( Field1 > Field2 )
  {
    s = this->Fields->at(Field1)+Separator+this->Fields->at(Field2);
    for ( size_t i = 0; i < Field2; ++i )
      f.push_back(this->Fields->at(i));
    f.push_back(s);
    for ( size_t i = Field2+1; i < this->Fields->size(); ++i )
      if ( i != Field1 )
        f.push_back(this->Fields->at(i));
    this->SetFields(f);
  }
}

std::string ErrStatus ( TAG_CSV_STATUS const & status )
{
  std::string s;

  switch ( status )
  {
  case CSV_OK:
    s = "parse was OK.";
    break;
  case CSV_UNKNOWN:
    s = "unknown message.";
    break;
  case CSV_INVALID_FIELD:
    s = "invalid field.";
    break;
  case CSV_NOT_PARSED:
    s = "not parsed.";
    break;
  default:
    s = "unknown reason for failure.";
  }
  return s;
}


