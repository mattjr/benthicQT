//#pragma once

#if !defined(GARMINGRMM_H)
#define GARMINGRMM_H

#include "ufNMEASentence.h"

namespace UF {
//! Garmin proprietary messages.
namespace Garmin {
  //! GRMM: Garmin Proprietary Message, Map Datum.
  /*!
  CLASS
    GRMM


    Introduction

    The GRMM message contains information about the map datum.

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

    The message is output in the format: $GRMM,s1*hh

      <table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="679" id="AutoNumber1">
      <tr>
        <td width="88">Parameter</td>
        <td width="390">Description</td>
        <td width="197">Range</td>
      </tr>
      <tr>
        <td width="88">s1</td>
        <td width="390">Name of currently active datum.</td>
        <td width="197">&nbsp;</td>
      </tr>
      <tr>
        <td width="88">hh</td>
        <td width="390">Checksum 2-character hex</td>
        <td width="197">&nbsp;</td>
      </tr>
    </table>

    Example 1: $GRMM,Astrln Geod '66*51

    'Astrln Geod 66' = Name of currently active datum

    Example 2: $GRMM,NAD27 Canada*2F

    'NAD27 Canada' = Name of currently active datum


    Here is a typical message: $GRMM,WGS 84*06


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
  class UF_GPS_PARSER_EXPORT GRMM :
    public NMEA::NMEASentence
  {
  public:
    GRMM();
    virtual ~GRMM();

    //! Initialise the fields in a sentence.
    void Init()
    {
      this->sentence.clear();
      this->ProprietarySentenceIndicator = true;
      this->ParseStatus = NMEA::NMEA_NOT_PARSED;

      this->MapDatum.clear();
    }

    //! Assignment
    GRMM operator = ( GRMM const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->MapDatum = rhs.MapDatum;

      return *this;
    }

    //! Constructor
    GRMM  ( GRMM const & rhs )
    {

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->MapDatum = rhs.MapDatum;

    }

    //! Equality
    bool operator == ( GRMM const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
        this->MapDatum == rhs.MapDatum
      );
    }

    //! Inequality
    bool operator != ( GRMM const & rhs )
    {
      return !(*this == rhs);
    }


    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, GRMM const & p)
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
    friend std::istream& operator>>(std::istream& is, GRMM & p)
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

  protected:
    //! Set the map datum.
    SetMacro(MapDatum,std::string);
    //! Get the map datum.
    GetMacro(MapDatum,std::string);

  private:
    std::string MapDatum;
  };

} // Namespace NMEA.
} // Namespace UF.


#endif
