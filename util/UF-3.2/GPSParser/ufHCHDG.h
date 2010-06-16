//#pragma once

#if !defined(NMEAHCHDG_H)
#define NMEAHCHDG_H

#include "ufNMEASentence.h"

namespace UF {
namespace NMEA {
  //! HCHDG: Compass Heading.
  /*!
  CLASS
    HCHDG


    Introduction

    The HCHDG message contains the compass heading. The Garmin eTrex Summit, Vista,
    GPS 60 and GPS76 series S receivers output the value of the internal flux-gate
    compass in this message. In the case of the Garmins, only the magnetic heading
    and magnetic variation are shown in the message.

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
      SetMagneticHeading(magneticHeading);
      magneticHeading = GetMagneticHeading();
      \endcode

    Output

    The message is output in the format: $HCHDG,f1,f2,f3,f4,c1*hh

    <table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="679" id="AutoNumber1">
      <tr>
        <td width="88">Parameter</td>
        <td width="390">Description</td>
        <td width="197">Range</td>
      </tr>
      <tr>
        <td width="88">f1</td>
        <td width="380">Magnetic Heading in degrees</td>
        <td width="197">000.0-359.9</td>
      </tr>
      <tr>
        <td width="88">f2</td>
        <td width="390">Magnetic Deviation</td>
        <td width="197">0.0-9.9</td>
      </tr>
      <tr>
        <td width="88">f3</td>
        <td width="390">Magnetic Deviation</td>
        <td width="197">0.0-9.9</td>
      </tr>
      <tr>
        <td width="88">f4</td>
        <td width="197">Magnetic variation in degrees</td>
        <td width="390">00.0-99.9</td>
      </tr>
      <tr>
        <td width="88">c2</td>
        <td width="390">The direction of the magnetic variation.<br>
         E = East<br>
         W = West<br>
         Relative to True heading, "East is least and West is best."<br>
         H<sub>true</sub> - V<sub>east</sub> = H<sub>magnetic</sub> <br>
         H<sub>true</sub> + V<sub>west</sub> = H<sub>magnetic</sub> <br>
          where H: heading, V: variation</td>
        <td width="197">E,W</td>
      </tr>
      <tr>
        <td width="88">*hh</td>
        <td width="390">The hexadecimal checksum is computed by exclusive - <br>
        ORing all of the bytes in the message between, but not including, <br>
        the $ and the *. The result is *hh where h is a hex character.</td>
        <td width="197">0 to 9 and A through F</td>
      </tr>
    </table>



    \version 2.0

    \date 22-May-2006

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
  class UF_GPS_PARSER_EXPORT HCHDG :
    public NMEASentence
  {
  public:
    HCHDG();
    virtual ~HCHDG();

    //! Initialise the fields in a sentence.
    void Init()
    {
      this->sentence.clear();
      this->ProprietarySentenceIndicator = false;
      this->ParseStatus = NMEA_NOT_PARSED;

      this->MagneticHeading = -1;
      this->MagneticDeviation1 = -1;
      this->MagneticDeviation2 = -1;
      this->MagneticVariation = -1;
      this->DirectionOfMagneticVariation = 'E';
    }

    //! Assignment
    HCHDG operator = ( HCHDG const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->MagneticHeading = rhs.MagneticHeading;
      this->MagneticDeviation1 = rhs.MagneticDeviation1;
      this->MagneticDeviation2 = rhs.MagneticDeviation2;
      this->MagneticVariation = rhs.MagneticVariation;
      this->DirectionOfMagneticVariation = rhs.DirectionOfMagneticVariation;

      return *this;
    }

    //! Constructor
    HCHDG  ( HCHDG const & rhs )
    {

      this->sentence = rhs.sentence;
      this->ParseStatus = rhs.ParseStatus;
      this->ProprietarySentenceIndicator = rhs.ProprietarySentenceIndicator;

      this->MagneticHeading = rhs.MagneticHeading;
      this->MagneticDeviation1 = rhs.MagneticDeviation1;
      this->MagneticDeviation2 = rhs.MagneticDeviation2;
      this->MagneticVariation = rhs.MagneticVariation;
      this->DirectionOfMagneticVariation = rhs.DirectionOfMagneticVariation;
    }

    //! Equality
    bool operator == ( HCHDG const & rhs )
    {
      NMEASentence *p = this;
      return (
        *p == rhs &&
        this->MagneticHeading == rhs.MagneticHeading &&
        this->MagneticDeviation1 == rhs.MagneticDeviation1 &&
        this->MagneticDeviation2 == rhs.MagneticDeviation2 &&
        this->MagneticVariation == rhs.MagneticVariation &&
        this->DirectionOfMagneticVariation == rhs.DirectionOfMagneticVariation
      );
    }

    //! Inequality
    bool operator != ( HCHDG const & rhs )
    {
      return !(*this == rhs);
    }

    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, HCHDG const & p)
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
    friend std::istream& operator>>(std::istream& is, HCHDG & p)
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
    //! Set the magnetic heading.
    SetMacro(MagneticHeading,double);
    //! Get the magnetic heading.
    GetMacro(MagneticHeading,double);
    //! Set the magnetic deviation.
    SetMacro(MagneticDeviation1,double);
    //! Get the magnetic deviation.
    GetMacro(MagneticDeviation1,double);
    //! Set the magnetic deviation.
    SetMacro(MagneticDeviation2,double);
    //! Get the magnetic deviation.
    GetMacro(MagneticDeviation2,double);
    //! Set the magnetic variation (degrees).
    SetMacro(MagneticVariation,double);
    //! Get the magnetic variation (degrees).
    GetMacro(MagneticVariation,double);
    //! Set the direction of magnetic variation. E = East, W = West.
    SetMacro(DirectionOfMagneticVariation,char);
    //! Get the direction of magnetic variation. E = East, W = West.
    GetMacro(DirectionOfMagneticVariation,char);


  protected:
    double MagneticHeading;
    double MagneticDeviation1;
    double MagneticDeviation2;
    double MagneticVariation;
    char DirectionOfMagneticVariation;
  };

} // Namespace NMEA.
} // Namespace UF.


#endif
