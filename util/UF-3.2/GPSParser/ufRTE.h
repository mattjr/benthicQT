//#pragma once

#if !defined(NMEARTE_H)
#define NMEARTE_H

#include "ufNMEASentence.h"

#include <vector>

namespace UF {
namespace NMEA {

  class ufWaypointsType;

  //! RTE: Route.
  /*!
  CLASS
    RTE

    Introduction

    The RTE message contains a list of the routes. The list of routes is a vector
    of strings. This variable is public and the user can directly interact with it
    to extract/change individual waypoints.

    If you make a sentence from this data, ensure that the maximum sentence length
    does not exceed 82 characters, this includes the "$", checksum and the "CRLF".
    Use WayPointListSz() to estimate the size of the waypoint list part of the sentence.

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

    The message is output in the format: $--RTE,d1,d2,c3,s4,s5...sn,c6*hh

    <table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="679" id="AutoNumber1">
      <tr>
        <td width="88">Parameter</td>
        <td width="390">Description</td>
        <td width="197">Range</td>
      </tr>
      <tr>
        <td width="88">d1</td>
        <td width="390">Total number of RTE messages to be output</td>
        <td width="197">1 - 3</td>
      </tr>
      <tr>
        <td width="88">d2</td>
        <td width="390">Message number</td>
        <td width="197">1 - 3</td>
      </tr>
      <tr>
        <td width="88">c3</td>
        <td width="390">'c' = Current active route, 'w' = waypoint list
                starts with destination waypoint</td>
        <td width="197">c,w</td>
      </tr>
      <tr>
        <td width="88">s4</td>
        <td width="390">Name or number of the active route</td>
        <td width="197">&nbsp;</td>
      </tr>
            <tr>
        <td width="88">s5...n</td>
        <td width="390">A varying number of fields each corresponding to
                the name of a waypoint in the route.</td>
        <td width="197">&nbsp;</td>
      </tr>
      <tr>
        <td width="88">*hh</td>
        <td width="390">Checksum</td>
        <td width="197">2-character hex</td>
      </tr>
    </table>

    eg. $GPRTE,2,1,c,0,PBRCPK,PBRTO,PTELGR,PPLAND,PYAMBU,PPFAIR,PWARRN,PMORTL,PLISMR*73

    eg. $GPRTE,2,2,c,0,PCRESY,GRYRIE,GCORIO,GWERR,GWESTG,7FED*34

    Here is a typical message: $GPRTE,2,1,c,0,PBRCPK,PBRTO,PTELGR,PPLAND,PYAMBU,PPFAIR,PWARRN,PMORTL,PLISMR*73


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
  class UF_GPS_PARSER_EXPORT RTE :
    public NMEASentence
  {
  public:
    RTE();
    virtual ~RTE();

    //! Initialise the fields in a sentence.
    void Init();

    //! Constructor
    RTE  ( RTE const & rhs );

    //! Assignment
    RTE operator = ( RTE const & rhs );

    //! Equality
    bool operator == ( RTE const & rhs );

    //! Inequality
    bool operator != ( RTE const & rhs )
    {
      return !(*this == rhs);
    }


    //! Output the message.
    /*!
        @param os - the output stream.
        @param p - the object from which the message is generated.

    */
    friend std::ostream& operator<<(std::ostream& os, RTE const & p)
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
    friend std::istream& operator>>(std::istream& is, RTE & p)
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

    //! Set the total number of RTE messages to be output.
    SetMacro(TotalNumberOfRTEMessages,int);
    //! Get the total number of GSV messages to be output.
    GetMacro(TotalNumberOfRTEMessages,int);
    //! Set the message number.
    SetMacro(MessageNumber,int);
    //! Get the message number.
    GetMacro(MessageNumber,int);
    //! Set the route, c: current active route, w: waypoint list starts with destination waypoint.
    SetMacro(RouteFlag,char);
    //! Get the route, c: current active route, w: waypoint list starts with destination waypoint.
    GetMacro(RouteFlag,char);

    //! Calculate the number of characters in Waypoints including separating commas.
    /*!
        For example, if Waypoints contains AA BBB CCCC then it will be
        represented as "AA,BBB,CCCC" in the sentence and the size will be 11.
    */
   std::size_t WayPointListSz ();

  protected:
    int TotalNumberOfRTEMessages;
    int MessageNumber;
    char RouteFlag;

  private:
    // A vector containing the waypoints.
    ufWaypointsType* Waypoints;


  };

} // Namespace NMEA.
} // Namespace UF.


#endif
