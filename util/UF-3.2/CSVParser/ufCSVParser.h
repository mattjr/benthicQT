//
// C++ Interface: sentence
//
// Description:
//
//
// Author:  Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CSVPARSERCSVSENTENCE_H
#define CSVPARSERCSVSENTENCE_H

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <stdexcept>
#include <algorithm>

#include "ufSetGetMacros.h"

#include "ufWin32Header.h"

namespace UF {
//! Comma separated value parser.
namespace CSV {

enum TAG_CSV_STATUS {CSV_OK, CSV_UNKNOWN, CSV_INVALID_FIELD, CSV_NOT_PARSED };

  class ufFieldsType;

//! Provide a plain message string corresponding to the status.
std::string ErrStatus ( TAG_CSV_STATUS const & status );

//! Comma separated value parser.
/**
Parses a CSV sentence.


  This should parse comma-separated value (csv) sentences. The default delimiter between fields
  is assumed to be a comma (,) and the default text field delimiter is assumed to be ("). However
  you can change these delimiters.

@author Andrew J. P. Maclean
*/
class UF_CSV_PARSER_EXPORT CSVParser
{
public:
    CSVParser();
    //! Construct with a different delimiter and possibly a different text delimiter.
    CSVParser(char const Delimiter, char const TextDelimiter = '\"');
    //! Construct with a sentence and possibly different delimiters. The sentence is parsed.
    CSVParser(std::string const & Sentence, char const Delimiter = ',', char const TextDelimiter = '\"');

    virtual ~CSVParser();

    //! Initialise the fields in a sentence.
    void Init();

   //! Parse the sentence.
    /*!
        This function will parse the sentence up to the first \<cr\> or \<lf\>. If
        there are no terminating characters then the whole sentence is parsed.

        If an empty string is fed to the parser, this is still regarded as a field
        and the number of fields will be 1.

        Always check the return value before using the data.

        @return The status of the parse.
    */
    TAG_CSV_STATUS Parse();

    //! Get the number of fields in the sentence.
    std::size_t NumberOfDataFields();

    //! Get a field.
    /*!
        This will throw if the fieldNumber is out of range.
        Either use a try/catch statment when calling this or
        check for the number of fields using NumberOfDataFields().

        @param fieldNumber - the number of the field to extract.
        @param value - the value of the field.
        @return true if the field was able to be extracted.
    */
    template <typename T>
      bool GetField ( std::size_t const &fieldNumber, T & value )
    {
      std::string s;
      if ( this->GetField(fieldNumber,s) )
      {
        std::istringstream is(s);
        is >> value;
        return true;
      }
      return false;
    }

    //! Get a field - specialisation for a string.
    /*!
        This will throw if the fieldNumber is out of range.
        Either use a try/catch statment when calling this or
        check for the number of fields using NumberOfDataFields().

        @param fieldNumber - the number of the field to extract.
        @param value - the value of the field.
        @return true if the field was able to be extracted.
    */
    bool GetField ( std::size_t const &fieldNumber, std::string & value );

    //! Set a field.
    /*!
        This will throw if the fieldNumber is out of range.
        Either use a try/catch statment when calling this or
        check for the number of fields using NumberOfDataFields().

        @param fieldNumber - the number of the field to extract.
        @param value - the value of the field.
    */
    void SetField ( std::size_t const &fieldNumber, std::string const & value );

    //! Delimit a field.
    /*!
        This will throw if the fieldNumber is out of range.
        Either use a try/catch statment when calling this or
        check for the number of fields using NumberOfDataFields().

        @param fieldNumber - the number of the field to extract.
        @param Delimiter - the delimiter to use, this can be a string of more than one character.
    */
    void DelimitField ( std::size_t const &fieldNumber, std::string const & Delimiter = "\'" );

    //! Get the size of the sentence.
    std::size_t SentenceSize () { return Sentence.size(); }

    //! Get the size of a field.
    /*!
       This will throw if the fieldNumber is out of range.
        Either use a try/catch statment when calling this or
        check for the number of fields using NumberOfDataFields().

        @param fieldNumber - the number of the field to extract.
    */
    std::size_t FieldSize ( std::size_t const &fieldNumber );

    //! Test for ==
    bool operator == ( CSVParser const & rhs );

    //! Test for !=
    bool operator != ( CSVParser const & rhs )
    {
      return !(*this == rhs);
    }

    //! Copy constructor
    CSVParser ( CSVParser const & rhs );

    //! Assignment
    CSVParser &operator = ( CSVParser const & rhs );

    //! Concatenation
    CSVParser &operator += ( char const & c )
    {
      this->Sentence += c;
      return *this;
    }

    //! Concatenation
    /*!
        It is your responsibility to use the correct delimiter and text delimiter.
        Remember to parse the sentence.
    */
    CSVParser &operator += ( std::string const & s )
    {
      this->Sentence += s;
      return *this;
    }

    //! Reverse
    /*!
        Reverse the order of the fields.
    */
    void Reverse ();

    //! Construct a cvs sentence from the data in the Fields variable.
    /*!
        Assumes a series of well constructed fields i.e. if the field
        is a text field, it cannot contain a text delimiter. If a field
        contains the delimiter then the field will be quoted with the text
        delimiter when the sentence is constructed.

        The delimiter and text delimiter are assumed to be the ones used by the
        class. So if you want defferent ones, just alter the ones in this class.
    */
    std::string MakeSentence( );

    //! Set the status of the parse.
    SetMacro(ParseStatus,TAG_CSV_STATUS);
    //! Get the status if the parse.
    GetMacro(ParseStatus,TAG_CSV_STATUS);
    //! Set the delimiter in the sentence.
    SetMacro(Delimiter,char);
    //! Get the delimiter in the sentence.
    GetMacro(Delimiter,char);
    //! Set the text delimiter in the sentence.
    SetMacro(TextDelimiter,char);
    //! Get the text delimiter in the sentence.
    GetMacro(TextDelimiter,char);

    //! Set the sentence.
    /*!
        Remember to do: Init(); SetSentence(); Parse().
        If you do not do an Init(), the fields from the
        sentence after the parse will be just appended
        to the existing elements in the Fields vector.
        In this case, the output of GetSentence() will
        not reflect the input of SetSentence().

        This behaviour is intentional because it allows
        us to build up fields from several sentences.
    */
    SetMacro(Sentence,std::string);
    //! Get the sentence.
    GetMacro(Sentence,std::string);

    //! Get fields.
    std::vector < std::string > GetFields();

    //! Set fields.
    void SetFields(std::vector < std::string > const & Fields);

    //! Merge Fields
    /*!
     *   Merge two fields together. After this operation the number of
     *   Fields will be reduced by one. Field1 is merged with Field2 and the
     *   merged field will be in the position denoted by the minimum of the
     *   indices Field1 and Field2. If the index of Field1 is less then Field2 then
     *   the merged field consists of the contents of Field1 and Field2 in that order.
     *   If the index of Field1 is greater then Field2 then
     *   the merged field consists of the contents of Field2 and Field1 in that order.
     */
    void MergeFields (const std::size_t & Field1, const std::size_t & Field2, std::string const & Separator = " "  );

    //! Drop Field
    /*!
     *  Remove a particular field.After this operation the number of
     *   Fields will be reduced by one.
     */
    void DropField (const std::size_t & FieldNo );

  private:
    static char const CR;
    static char const LF;
     //! The result of the parse.
    TAG_CSV_STATUS ParseStatus;
    char Delimiter;
    char TextDelimiter;

    //! The sentence.
    /*!
      A comma separated variable sentence is variable in length and is
      terminated with \<cr\>\<lf\>. When setting this variable, the
      need for the terminating characters can be ignored in most cases.
      Fields in the sentence are separated by a comma by default.
    */
    std::string Sentence;

    //! After parsing, the string, split into fields, is put into this variable.
    ufFieldsType* Fields;
};

} // Namespace CSV.
} // Namspace UF.

#endif
