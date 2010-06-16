//#pragma once

#if !defined(NMEAHDT_H)
#define NMEAHDT_H

#include "ufNMEASentence.h"

namespace UF {
namespace NMEA {
  //! HDT: true heading.
  /*!
  CLASS
    HDT


    Introduction

    The HDT message contains the true heading.

    Convenience macros

    Most variables that correspond to fields in the message have a set of
      convenience macros to facilitate setting and getting of their values.
      These have the form:
      - SetVariableName()
      - GetVariableName()

      Where Name is the name of the variable. In the documentation you can see the
      macros described as SetMacro() and GetMacro() respectively.

      Usage:
      \code
      SetLongitude(longitude);
      longitude = GetLongitude();
      \endcode

    Output

    The message is output in the format: $--HDT,d1,c2*hh

    <table border="1" style="border-collapse: collapse" width="100%" id="table1">
      <tr>
        <td>Parameter</td>
        <td width="284">Description</td>
        <td width="266">Range</td>
      </tr>
      <tr>
        <td>d1</td>
        <td width="284">True Heading in degrees</td>
        <td width="266">%.3F, 000.000-359.999</td>
      </tr>
      <tr>
        <td>c2</td>
        <td width="284">Type of the Heading</td>
        <td width="266">T - True Heading</td>
      </tr>
      <tr>
        <td>*hh</td>
        <td width="284">The hexadecimal checksum is computed by exclusive - <br>
        ORing all of the bytes in the message between, but not including, <br>
        the $ and the *. The result is *hh where h is a hex character.</td>
        <td width="266">0 to 9 and A through F</td>
      </tr>
    </table>



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
  class UF_GPS_PARSER_EXPORT HDT :
    public NMEASentence
  {
  public:
    HDT();
    virtual ~HDT();

    //! Initialise the fields in a sentence.
    void Init()
    {
      this->sentence.clear();
      this->ProprietarySentenceIndicator = false;
      this->ParseStatus = NMEA_NOT_PARSED;

      this->TrueHeading = -1;
      this->HeadingIndicator = 'T';
    }

    //! Assignment
    HDT operator = ( HDT const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->TrueHeading = rhs.TrueHeading;
      this->HeadingIndicator = rhs.HeadingIndicator;

      return *this;
    }

    //! Constructor
    HDT  ( HDT const & rhs )
    {

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->TrueHeading = rhs.TrueHeading;
      this->HeadingIndicator = rhs.HeadingIndicator;
    }

    //! Equality
    bool operator == ( HDT const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
      this->TrueHeading == rhs.TrueHeading &&
      this->HeadingIndicator == rhs.HeadingIndicator
      );
    }

    //! Inequality
    bool operator != ( HDT const & rhs )
    {
      return !(*this == rhs);
    }

    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, HDT const & p)
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
    friend std::istream& operator>>(std::istream& is, HDT & p)
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
    //! Set the true heading.
    SetMacro(TrueHeading,double);
    //! Get the true heading.
    GetMacro(TrueHeading,double);

    //! Set the true heading indicator T - true.
    SetMacro(HeadingIndicator,char);
    //! Get the true heading indicator T - true.
    GetMacro(HeadingIndicator,char);


  protected:
    double TrueHeading;
    char HeadingIndicator;
  };

} // Namespace NMEA.
} // Namespace UF.


#endif
