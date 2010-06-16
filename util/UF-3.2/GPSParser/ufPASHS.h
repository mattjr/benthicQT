//#pragma once

#if !defined(ASHTECHPASH_H)
#define ASHTECHPASH_H

#include "ufNMEASentence.h"

namespace UF {
//! Ashtech Proprietary Messages.
namespace ASHTECH {
  //! PASHS: This sentence sends a command to an Ashtech GPS.
  /*!
  CLASS
    PASHS

    Introduction

    This command is used to control Ashtech GPS units.

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

    $PASHS,NME,A,ALL,OFF*50<CR><LF>

    This class has only two fields, Command and CommandParameters. CommandParameters
    contains a comma-delimited list of parameters fir the command.

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
  class UF_GPS_PARSER_EXPORT PASHS :
    public NMEA::NMEASentence
  {
  public:
    PASHS();
    virtual ~PASHS();

    //! Initialise the fields in a sentence.
    void Init()
    {
      this->sentence.clear();
      this->ProprietarySentenceIndicator = false;
      this->ParseStatus = NMEA::NMEA_NOT_PARSED;

      this->Command.clear();
      this->CommandParameters.clear();
    }

    //! Assignment
    PASHS operator = ( PASHS const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;
      this->Command = rhs.Command;
      this->CommandParameters = rhs.CommandParameters;

      this->Command = rhs.Command;
      this->CommandParameters = rhs.CommandParameters;

      return *this;
    }

    //! Constructor
    PASHS  ( PASHS const & rhs )
    {
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;
      this->Command = rhs.Command;
      this->CommandParameters = rhs.CommandParameters;

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
    }

    //! Equality
    bool operator == ( PASHS const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
        this->Command == rhs.Command &&
        this->CommandParameters == rhs.CommandParameters
        );
    }

    //! Inequality
    bool operator != ( PASHS const & rhs )
    {
      return !(*this == rhs);
    }


    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, PASHS const & p)
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
    friend std::istream& operator>>(std::istream& is, PASHS & p)
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
    SetMacro(Command,std::string);
    GetMacro(Command,std::string);
    SetMacro(CommandParameters,std::string);
    GetMacro(CommandParameters,std::string);

  protected:
    std::string Command;
    std::string CommandParameters;
  };

} // Namespace NMEA.
} // Namespace UF.


#endif
