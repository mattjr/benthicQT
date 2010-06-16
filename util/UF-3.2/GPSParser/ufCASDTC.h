
#if !defined(CASCASDTC_H)
#define CASCASDTC_H

#include "ufNMEASentence.h"

namespace UF {
//! Centre for Autonomous Systems (CAS), proprietary messages.
namespace CAS {
//! CASDTC: Date, time and comment.
/*!
CLASS
  CASDTC


  Introduction

  The CASDTC message contains the date time and an optional comment.

  Output

  The message is output in the format: $PCASDTC,n1,n2,n3,n4,n5,n6,c7,s8*hh<CR><LF>
  Where
    - n1 is the year.
    - n2 is the month.
    - n3 is the day.
    - n2 is the hour.
    - n5 is the minute.
    - n6 is the second.
    - c7 is a flag indicating if the time is corrected (u, or U indicates the time is uncorrected, any other value implies the time is corrected.)
    - s8 is a comment field.


  \version 2.0

  \date 07-May-2004

  \author Andrew Maclean

  \par license
  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  YOU, as the USER, take SOLE RESPONSIBILITY for, and are responsible for
  determining the FITNESS and APPLICABILITY of this software for a particular
  purpose. The author or any other person or entity are not liable in any way
  for the use or misuse of any program or software using this software.

  \todo

  \bug
*/
class UF_GPS_PARSER_EXPORT CASDTC :
  public NMEA::NMEASentence
{
public:
  CASDTC();
  virtual ~CASDTC();


  //! Initialise the fields in a sentence.
  void Init()
  {
    this->sentence.clear();
    this->ProprietarySentenceIndicator = true;
    this->ParseStatus = NMEA::NMEA_NOT_PARSED;
    this->Year = 0;
    this->Month = 0;
    this->Day = 0;
    this->Hour = -1;
    this->Minute = -1;
    this->Second = -1;
    this->Corrected =' ';
    this->Comment.clear();
  }

  //! Assignment
  CASDTC operator = ( CASDTC const & rhs )
  {
    if ( this == &rhs )
      return *this;

    this->sentence = rhs.sentence;
    this->ParseStatus = rhs.ParseStatus;
    this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

    this->Year = rhs.Year;
    this->Month = rhs.Month;
    this->Day = rhs.Day;
    this->Hour = rhs.Hour;
    this->Minute = rhs.Minute;
    this->Second = rhs.Second;
    this->Corrected = rhs.Corrected;
    this->Comment = rhs.Comment;

    return *this;
  }

  //! Copy constructor
  CASDTC ( CASDTC const & rhs )
  {
    this->sentence = rhs.sentence;
    this->ParseStatus = rhs.ParseStatus;
    this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

    this->Year = rhs.Year;
    this->Month = rhs.Month;
    this->Day = rhs.Day;
    this->Hour = rhs.Hour;
    this->Minute = rhs.Minute;
    this->Second = rhs.Second;
    this->Corrected = rhs.Corrected;
    this->Comment = rhs.Comment;

  }

  //! Equality
  bool operator == ( CASDTC const & rhs )
  {
    NMEASentence *p = this;
    return (
      *p == rhs &&
      this->Year == rhs.Year &&
      this->Month == rhs.Month &&
      this->Day == rhs.Day &&
      this->Hour == rhs.Hour &&
      this->Minute == rhs.Minute &&
      this->Second == rhs.Second &&
      this->Corrected == rhs.Corrected &&
      this->Comment == rhs.Comment
      );
  }

  //! Inequality
  bool operator != ( CASDTC const & rhs )
  {
    return !(*this == rhs);
  }

    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, CASDTC const & p)
    {
      std::ios::fmtflags flags = os.flags();
      os << p.MakeSentence();
      os.flags(flags);
      os.clear();
      return os;
    }

    //! Input the message.
    /*!
        @param is - the input stream.
        @param p - the object into which the data is streamed.

        The data expected is a well formed NMEA string.
    */
    friend std::istream& operator>>(std::istream& is, CASDTC & p)
    {
      std::ios::fmtflags flags = is.flags();
      p.Init();
      std::getline(is,p.sentence,'\n');
      p.sentence += '\n';
      p.Parse();
      is.flags(flags);
      is.clear();
      return is;
    }

  //! Parse the sentence.
  /*!
      @param ForceParse - if true then the checksum is not checked for validity before parsing.
  */
  void Parse(bool const & ForceParse = false);
  //! Make a sentence.
  std::string MakeSentence() const;
  //! The identifying nmemonic for this message.
  static std::string const MNEMONIC;

public:
  SetMacro(Year,int);
  GetMacro(Year,int);
  SetMacro(Month,int);
  GetMacro(Month,int);
  SetMacro(Day,int);
  GetMacro(Day,int);
  SetMacro(Hour,int);
  GetMacro(Hour,int);
  SetMacro(Minute,int);
  GetMacro(Minute,int);
  SetMacro(Second,double);
  GetMacro(Second,double);
  SetMacro(Corrected,char);
  GetMacro(Corrected,char);
  //! Set the comment.
  SetMacro(Comment,std::string);
  //! Get the comment.
  GetMacro(Comment,std::string);

protected:
  int Year;
  int Month;
  int Day;
  int Hour;
  int Minute;
  double Second;
  char Corrected;
  std::string Comment;
};

} // Namespace NMEA.
} // Namespace UF.


#endif
