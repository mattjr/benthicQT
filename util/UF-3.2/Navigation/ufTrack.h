//
// C++ Interface: Track
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NAVIGATION_TRACK_H
#define NAVIGATION_TRACK_H

#include "ufWin32Header.h"

#include <set>

#include "ufTrackSegment.h"

namespace UF {
//! Classes for navigation.
namespace Navigation {


//! Track represents a track - an ordered list of points describing a path.
/*!
    See: http://www.topografix.com/GPX/1/1/#type_trkType
    There may be one or more ordered lists of points.
 */
class UF_NAV_EXPORT Track: public Link
{
public:
  typedef std::vector<TrackSegment> TTrack;
  typedef TTrack::iterator TTrackIt;
  typedef TTrack::const_iterator TTrackCIt;
  typedef std::set<std::string> TTrackGPXElements;


  Track():
    number((unsigned int)(-1))
  {
    Init();
  };

  ~Track(){};

  //! Initialise the variables.
  void Init();

public:

  //! Get the track name.
  /*!
      @return GPS name of track.
  */
  std::string GetName() const
  {
    return this->name;
  }

  //! Get the track comment.
  /*!
      @return comment for the track.
  */
  std::string GetComment() const
  {
    return this->comment;
  }

  //! Get the track description.
  /*!
      @return discription for the track.
  */
  std::string GetDescription() const
  {
    return this->description;
  }

  //! Get the source of the track.
  /*!
      @return source of the track.
  */
  std::string GetSource() const
  {
    return this->source;
  }

  //! Get the track number.
  /*!
      @return track number.
  */
  unsigned int GetNumber() const
  {
    return this->number;
  }

  //! Get the track type.
  /*!
      @return track type.
  */
  std::string GetType() const
  {
    return this->type;
  }

  //! Set the track name.
  /*!
      @param name - GPS name of track.
  */
  void SetName(std::string const & name)
  {
    this->name = name;
  }

  //! Set the track comment.
  /*!
      @param comment - comment for the track.
  */
  void SetComment(std::string const & comment)
  {
    this->comment = comment;
  }

  //! Set the track description.
  /*!
      @param description - the description for the track.
  */
  void SetDescription(std::string const & description)
  {
    this->description = description;
  }

  //! Set the source of the track.
  /*!
      @param source - the source of the track.
  */
  void SetSource(std::string const & source)
  {
    this->source = source;
  }

  //! Set the track number.
  /*!
      @param number - the track number.
  */
  void SetNumber(unsigned int const & number)
  {
    this->number = number;
  }

  //! Set the track number.
  /*!
      @param number - the track number.
  */
  void SetNumber(std::string const & number)
  {
    std::istringstream is;
    is.str(number);
    is >> this->number;
  }

  //! Set the track type.
  /*!
      @param type - the track type.
  */
  void SetType(std::string const & type)
  {
    this->type = type;
  }

  //! Get the track.
  /*!
      @return the address of the track.
  */
  TTrack * GetTrack() { return &this->track; }

  //! Return an XML representation of the track.
  /*!
      @return The track.
  */
  std::string ToXML(int indent = 2);

  //! Return the bounds of the track.
  /*! 
      Only latitude, longitude and elevation are considered.
      The bounds refer only to the existing positions in the track.
      If the track is a great circle the bounds may lie outside these positions.

      The minimum elevation will be found in the south-eastern bound.

      se - south-eastern bound of the track.
      nw - north-western bound of the track.
   */
  void GetBounds(Waypoint & se, Waypoint & nw);

  //! Get the GPXElements.
  /*!
      @return the address of GPXElements.
  */
  TTrackGPXElements * GetGPXElements() { return &this->GPXElements; }

  //! Set an element.
  /*!
      Elements not set here are processed in the base classes.

      @param key - the attribute.
      @param value - its value.
   */
  template <typename T>
  void SetElement(std::string const & key, T const & value)
  {
    if ( GPXElements.find(key) == GPXElements.end() )
    {
      // Not a valid key.
      return;
    }

    if ( key == "name" )
    {
      SetName(value);
      return;
    }
    if ( key == "cmt" )
    {
      SetComment(value);
      return;
    }
    if ( key == "desc" )
    {
      SetDescription(value);
      return;
    }
    if ( key == "src" )
    {
      SetSource(value);
      return;
    }
    // Can be int or string.
    if ( key == "number" )
    {
      SetNumber(value);
      return;
    }
    if ( key == "type" )
    {
      SetType(value);
      return;
    }
  }

  //! Get the link.
  /*!
      @return the link
   */
  Link GetLink() const
  {
    Link l;
    l.SetHref(this->Link::GetHref());
    l.SetText(this->Link::GetText());
    l.SetType(this->Link::GetType());
    return l;
  }

  //! Set the link.
  /*!
      @param the link
   */
  void SetLink(Link const & l)
  {
    this->Link::SetHref(l.GetHref());
    this->Link::SetText(l.GetText());
    this->Link::SetType(l.GetType());
  }

protected:
  TTrackGPXElements GPXElements;

private:
  //! Indent a string by this amount.
  /*!
      @return A series of spaces corresponding to the indent.
  */
  std::string Indent(int const & indent);

  //! The name of the track.
  std::string name;
  //! Comment about the track.
  std::string comment;
  //! Description about the track.
  std::string description;
  //! Source of the track.
  std::string source;
  //! Link for the track.
  //std::string link;
  //! Track number.
  unsigned int number;
  //! Type of track.
  std::string type;

  //! The track.
  TTrack track;
};

} // Namespace Navigation.
} // Namespace UF.

#endif // NAVIGATION_TRACK_H
