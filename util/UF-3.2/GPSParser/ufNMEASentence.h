//#pragma once

#if !defined(NMEANMEASENTENCE_H)
#define NMEANMEASENTENCE_H

#include <sys/types.h>

#include <string>
#include <sstream>
#include <cmath>

#include "ufSetGetMacros.h"
#include "ufNumberConversions.h"
#include "ufWin32Header.h"

/*
  \mainpage GPSParser

  \section intro Introduction

  This is a set of classes that provide the ability to manipulate NMEA sentences.

  Each class corresponding to a non-proprietary sentence is named after the three
  letter sentence sentence Id that it implements. E.g for the GLL  sentence the files
  are GLL.h and GLL.cpp. The class name is CGLL.

  Classes corresponding to proprietary sentences are similarly structured except that
  the class name begins with the three-letter manufacturer ID and ends with the
  manufacturer assigned ID for the sentence. E.g for the Garmin (GRM) proprietary sentence
  M, the files will be GRMM.h and GRMM.cpp with the class name being CGRMM.

  Each of these classes is derived from NMEASentence. The documentation for NMEASentence
  details further rules for creating new sentence classes.

  \section SentenceFormat General Sentence Format

  Under the NMEA-0183 standard, all characters used are printable
  ASCII text (plus carriage return and line feed).  NMEA-0183 data
  is sent at 4800 baud.

  The data is transmitted in the form of "sentences".  Each
  sentence starts with a "$", a two letter "talker ID", a three
  letter "sentence ID", followed by a number of data fields
  separated by commas, and terminated by an optional checksum, and
  a carriage return/line feed.  A sentence may contain up to 82
  characters including the "$" and CR/LF.

  If data for a field is not available, the field is simply
  omitted, but the commas that would delimit it are still sent,
  with no space between them.

  Since some fields are variable width, or may be omitted as
  above, the receiver should locate desired data fields by
  counting commas, rather than by character position within the
  sentence.

  The optional checksum field consists of a "*" and two hex digits
  representing the exclusive OR of all characters between, but not
  including, the "$" and "*".  A checksum is required on some
  sentences.

  The standard allows individual manufacturers to define
  proprietary sentence formats.  These sentences start with "$P",
  then a 3 letter manufacturer ID, followed by whatever data the
  manufacturer wishes, following the general format of the
  standard sentences.

  Some common proprietary manufacturer Ids are:
    - ASH  Ashtech (Thales Navigation)
    - GMN  Garmin
    - SRF  SiRF

  Some common talker Ids are:
    - GP      Global Positioning Systems
    - GL      GLONASS
    - GN      Global Navigation Satellite System
    - LC      Loran-C receiver
    - OM      Omega Navigation receiver
    - II      Integrated Instrumentation (eg. AutoHelm Seatalk system)

  In summary:

    <p>Each approved NMEA sentence has the following format:<br>
    &nbsp;</p>
    <table border="1" style="border-collapse: collapse" width="100%" id="table1">
      <tr>
        <td>$ (HEX 24)</td>
        <td width="512"> Start of sentence.</td>
      </tr>
      <tr>
        <td>AACCC</td>
        <td width="512"> Address field.<br>
        The first two characters identify Talker.<br>
        The last three characters identify the sentence type.</td>
      </tr>
      <tr>
        <td>, (HEX 2C)</td>
        <td width="512">&nbsp; Field delimiter.</td>
      </tr>
      <tr>
        <td>cc</td>
        <td width="512"> Data sentence block.</td>
      </tr>
      <tr>
        <td>* (HEX 2A)</td>
        <td width="512"> Checksum delimiter.</td>
      </tr>
      <tr>
        <td>hh</td>
        <td width="512"> Checksum field. This value is computed by exclusive-ORing
        the eight data bits of each character in the sentence, between, but
        excluding, $ and *. The hexadecimal value of the most significant
        and least significant four bits of the result are converted into two
        ASCII characters (0-9,A-F) for<br>
        transmission. The most significant character is transmitted first.</td>
      </tr>
      <tr>
        <td>&lt;CR&gt;&lt;LF&gt; (HEX 0D,0A)</td>
        <td width="512"> sentence terminators.</td>
      </tr>
    </table>
    <p>Approved NMEA sentences are allowed to contain the so-called null fields.
    Null fields are used when one or more parameters in the message are unreliable or
    unavailable. A null field may be delimited by two commas (,,) or by a comma
    and  a multiplication sign * (,*) depending on its position in the sentence.</p>

*/

namespace UF {

//! Contains the classes for the NMEA sentences and various helper classes for creating/reading messages.
namespace NMEA
{

  typedef unsigned char byte;
  enum TAG_NMEA_STATUS {NMEA_OK, NMEA_NAVIGATION_RECEIVER_WARNING, NMEA_VALID_CHECKSUM, NMEA_INVALID_CHECKSUM, NMEA_UNKNOWN, NMEA_WRONG_MNEMONIC, NMEA_WRONG_MESSAGE_LENGTH, NMEA_INVALID_FIELD, NMEA_SIMULATING, NMEA_NOT_PARSED };

  //! Provide a plain message string corresponding to the status.
  std::string UF_GPS_PARSER_EXPORT ErrStatus ( TAG_NMEA_STATUS const & status );

  //! Extract the NMEA mnemonic fronm the sentence.
  /*!
      An empty string is returned if the function cannot find the pattern
      $<mnemonic>, in the string.
  */
  std::string UF_GPS_PARSER_EXPORT GetMnemonic(std::string& Sentence);

  //! Extract the NMEA data and fill the appropriate variable with it.
  /*!
     sentence: the NMEA sentence, nmea: the variable into which we are attempting
     to parse the sentence, msg: a string returning an error message (if any).
      Use it something like this:
      \code
        // *P is a pointer to a string containing a message.
        if ( Status == NMEA::NMEA_NOT_PARSED )
          Status = FillNMEA(*p,gll,msg);
        if ( Status == NMEA::NMEA_NOT_PARSED )
          Status = FillNMEA(*p,gsa,msg);
        // ...
      \endcode
      The if() test is used for all messages of interest and we use Status
      to determine the state of the parse.
  */
  template < typename T >
    NMEA::TAG_NMEA_STATUS FillNMEA ( std::string const & sentence, T & nmea, std::string & msg )
  {
    msg.clear();
    NMEA::TAG_NMEA_STATUS Status = NMEA::NMEA_NOT_PARSED;
    if ( (sentence.find(nmea.MNEMONIC) != std::string::npos ) )
    {
      nmea.SetSentence(sentence);
      nmea.Parse();
      Status = nmea.GetParseStatus();
      if ( !(Status == NMEA::NMEA_OK ||
        Status == NMEA::NMEA_SIMULATING ||
        Status == NMEA::NMEA_NAVIGATION_RECEIVER_WARNING) )
      {
        msg = "Warning - ";
        msg += nmea.MNEMONIC;
        msg += ": ";
        msg += NMEA::ErrStatus(Status);
      }
    }
    return Status;
  }


  //! Holds an NMEA0183 GPS Sentence.
  /*!
  CLASS
      NMEASentence

    This class holds a GPS sentence. It provides functions for calculating the checksum,
    checking the validity of the checksum, selecting a field from a message and building
    messages. Functions for assignment, concatenation, copying and comparison are also provided.

    To use the derived classes, just do something like this:

    \code
      // Some declarations
      std::string sentence;
      NMEA::CRMC rmc;
      // ...
      // Assume we have filled the variable sentence with some data.
      // Then do something like this:
      // ...
      if ( (sentence.find(gsto.MNEMONIC) != std::string::npos ) )
      {
        rmc.SetSentence(*p);
        if ( rmc.Parse() == NMEA::NMEA_OK )
          DoSomethingWithTheValidData();
        else
          std::cout << "Parse failed - " << NMEA::ErrStatus(status) << std::endl;
      }
    \endcode

    Note that after a parse has been performed via Parse(), you can check the variable
    ParseStatus at a later time via  GetParseStatus() to ascertain the result of the last
    parse. The result of the last Parse() is stored in that variable.

    In creating a new sentence class the user must:
    - Derive from this class.
    - For a non-proprietary sentence, the class name should be XXX (where XXX is the sentence ID)
    and the files should be XXX.h and XXX.cpp.
    - For a proprietary sentence, the class name should be MMMXXX (where MMM in the manufacturer Id
    and XXX is the sentence ID) and the files should be MMMXXX.h and MMMXXX.cpp. Generally the sentence
    Id is only one character long in this case.
    - Create an identifying mnemonic for the derived class:
    \code static std::string const MNEMONIC; \endcode
    and assign a value to this mnemonic in the .CPP file.
    - Create variables for the data fields of the message.
    - Use the convenience macros SetMacro() and GetMacro() to facilitate
    retrieval/assignment from the variables.
    - Override the virtual function Init() to initialise the variables. If it is a proprietary
    sentence ensure that SetProprietarySentenceIndicator(true) is called.
    - Override the virtual function Parse() to parse the sentence.
    - Override the virtual function MakeSentence() to generate a valid sentence from
    the fields in the class. In MakeSentence() you can use this->MakeSentenceStart() if the
    outputted sentence begins with $P or $GP.
    - Call Init() in the constructor and Parse().
    - Use the test harness template in TestGPS.cpp to test the class for a set of sentences.

    Implementing Parse() and MakeSentence() are relatively easy - just look at the implementations in
    existing sentence classes for examples. Most navigational data fields and functions/macros/techniques
    for manipulation/displaying them have already been implemented in the provided sentence classes,
    so it should be a relatively easy matter using cut-and-paste to implement new sentences.

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
  class UF_GPS_PARSER_EXPORT NMEASentence
  {
  public:
    NMEASentence();
    virtual ~NMEASentence();

    //! Make a sentence.
    /*
      A string is constructed that corresponds to the data in the
      instance of the class. For a NMEA class (such as CGLL) the appreoved NMEA sentence is
      constructed. For other classed the approved sentence is constructed. Note that the
      sentence variable in the class is not changed. If you wish to change it, use SetSentence()
      with the valuse returned fro this function.

      It is the user's responsibility to ensure that the values used to make the
      sentence are valid.

      @return A string consisting of the mnemonic and the various fields that make up the sentence.
    */
    std::string MakeSentence();

    //! Generate the checksum for a sentence.
    byte ComputeChecksum()
  {
    return ComputeChecksum( this->sentence );
  }
    static byte ComputeChecksum( std::string &sentence );

    //! Extract the checksum from the sentence.
    static std::string ReadChecksum( std::string &sentence );
    std::string ReadChecksum()
  {
    return ReadChecksum( this->sentence );
  }

    //! Compute and add the checksum to the sentence.
    void InsertChecksum();

    //! Checks the validity of the checksum field.
    static TAG_NMEA_STATUS ValidChecksum ( std::string &sentence );
    TAG_NMEA_STATUS ValidChecksum ()
    {
        return ValidChecksum( this->sentence );
    }

    static bool ValidNMEASentence( std::string &sentence );

    //! Get the number of fields in the sentence.
    int NumberOfDataFields() const;
    //! Extract a field from the sentence (fields are numbered from 0).
    /*!
        @param fieldNumber - the number of the field to extract, field 0 is generally the identifier for the sentence.
        @return A string corresponding to the field or an empty string if the field number is out of range.
    */
    std::string Field ( int const & fieldNumber );

    //! Get a field.
    /*!
        @param fieldNumber - the number of the field to extract, field 0 is generally the identifier for the sentence.
        @param value - the value of the field.
        @return True if the field was able to be extracted.
    */
    template <typename T>
      bool GetField ( int const &fieldNumber, T & value )
    {
      std::string s = Field(fieldNumber);
      if ( s.size() )
      {
        std::istringstream is(s);
        is >> value;
        return true;
      }
      return false;
    }

    //! Get a field - specialisation for a string.
    /*!
        @param fieldNumber - the number of the field to extract, field 0 is generally the identifier for the sentence.
        @param value - the value of the field.
        @return True if the field was able to be extracted.
    */
    bool GetField ( int const &fieldNumber, std::string & value )
    {
      std::string s = Field(fieldNumber);
      if ( s.size() )
      {
        value = s;
        return true;
      }
      return false;
    }

    //! Get the size of the sentence.
    std::size_t size () { return sentence.size(); }
    //! Get a sentence - no assumptions are made about the validity of the sentence.
    std::string GetSentence() const;
    //! Get a sentence - no assumptions are made about the validity of the sentence.
    void GetSentence ( std::string & sentence );
    //! Set a sentence - no assumptions are made about the validity of the sentence.
    void SetSentence ( std::string const & sentence );

    //! Make the start of a sentence, either $GP or $P (if a proprietary sentence)
    std::string MakeSentenceStart () const
    {
      if ( ProprietarySentenceIndicator )
        return "$P";
      else
        return "$GP";
    }

    //! Test for ==
    bool operator == ( NMEASentence const& rhs ) const
    {
      return ( this->sentence == rhs.sentence &&
        this->ProprietarySentenceIndicator == rhs.ProprietarySentenceIndicator &&
        this->ParseStatus == rhs.ParseStatus );
    }

    //! Test for !=
    bool operator != ( NMEASentence const& rhs ) const
    {
      return !(*this == rhs);
    }

    //! Copy constructor
    NMEASentence ( NMEASentence const& rhs ):
      sentence(rhs.sentence)
      , ProprietarySentenceIndicator(rhs.ProprietarySentenceIndicator)
      , ParseStatus(rhs.ParseStatus)
    {}

    //! Assignment
    NMEASentence &operator = ( NMEASentence const& rhs )
    {
      if ( this == & rhs )
        return *this;

      this->sentence = rhs.sentence;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;
      this->ParseStatus = rhs.ParseStatus;
      return *this;
    }

    //! Concatenation
    NMEASentence &operator += ( char const & c )
    {
      this->sentence += c;
      return *this;
    }

    //! Concatenation
    NMEASentence &operator += ( std::string const & s )
    {
      this->sentence += s;
      return *this;
    }

    //! Concatenation (to concatenate a double or float, use NumStr::n2s() first to generate the string).
    NMEASentence &operator += ( int const & d )
    {
      std::ostringstream os;
      os << d;
      this->sentence += os.str();
      return *this;
    }

    //! Set the proprietary sentence indicator (true: proprietary, false: non-proprietary).
    SetMacro(ProprietarySentenceIndicator,bool);
    //! Get the proprietary sentence indicator (true: proprietary, false: non-proprietary).
    GetMacro(ProprietarySentenceIndicator,bool);

    //! Set the status of the parse.
    SetMacro(ParseStatus,TAG_NMEA_STATUS);
    //! Get the status if the parse.
    GetMacro(ParseStatus,TAG_NMEA_STATUS);

    //! Parse the sentence.
    /*!
        @param ForceParse - if true then the checksum is not checked for valudity before parsing.
    */
    virtual void Parse(bool const & ForceParse = false) = 0;


protected:
    //! Initialise the fields in a sentence.
    virtual void Init() = 0;


  private:
    static byte const CR;
    static byte const LF;
    static byte const COMMA;

  protected:
    //! The NMEA sentence.
    /*!
      A sentence is variable in length and is assumed to have $ as the first character
      and is terminated with \<cr\>\<lf\>.
      Fields are separated by a comma and there is a checksum at the end of the message.
      The checksum field consists of an asterisk and two digits corresponding to
      the hexadecimal value of the checksum.
    */
    std::string sentence;
    //! A flag indicating that it is a proprietary sentence.
    bool ProprietarySentenceIndicator;
    //! The result of the parse.
    TAG_NMEA_STATUS ParseStatus;
  };



  NMEA::TAG_NMEA_STATUS ParseNMEASentence( std::string const & sentence, NMEA::NMEASentence &nmea, std::string & msg );

} // Namespace NMEA.
} // Namespace UF.


#endif
