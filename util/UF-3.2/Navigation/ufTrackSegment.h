//
// C++ Interface: TrackSegment
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NAVIGATION_TRACK_SEGMENT_H
#define NAVIGATION_TRACK_SEGMENT_H

#include "ufWin32Header.h"

#include "ufWaypoint.h"
#include <vector>
#include <set>

namespace UF {
//! Classes for navigation.
namespace Navigation {


//! A Track Segment holds a list of Track Points which are logically connected in order. To represent a single GPS track where GPS reception was lost, or the GPS receiver was turned off, start a new Track Segment for each continuous span of track data.
/*!
    See: http://www.topografix.com/GPX/1/1/#type_trksegType
 */
class UF_NAV_EXPORT TrackSegment
{
public:
  typedef std::vector<Waypoint> TTrackSegment;
  typedef TTrackSegment::iterator TTrackSegmentIt;
  typedef TTrackSegment::const_iterator TTrackSegmentCIt;
  typedef std::set<std::string> TTrackSegmentGPXElements;

  TrackSegment()
  {
    Init();
  }

  ~TrackSegment(){}

  //! Initialise the variables.
  void Init();

public:
  //! Get the segment.
  /*!
      @return the address of the track segment.
  */
  TTrackSegment * GetSegment() { return &this->segment; }

  //! Return an XML representation of the track segment.
  /*!
      @return The track segment.
  */
  std::string ToXML(int indent = 2);

  //! Return the bounds of the track segment.
  /*! 
      Only latitude, longitude and elevation are considered.
      The bounds refer only to the existing positions in the track segment.
      If the track segment is a great circle the bounds may lie outside these positions.

      The minimum elevation will be found in the south-eastern bound.

      se - south-eastern bound of the track segment.
      nw - north-western bound of the track segment.
   */
  void GetBounds(Waypoint & se, Waypoint & nw);

  //! Get the GPXElements.
  /*!
      @return the address of GPXElements.
  */
  TTrackSegmentGPXElements * GetGPXElements() { return &this->GPXElements; }

protected:
  TTrackSegmentGPXElements GPXElements;

private:
  //! Indent a string by this amount.
  /*!
      @return A series of spaces corresponding to the indent.
  */
  std::string Indent(int const & indent);

  //! A sequence of track points that form a segment.
  TTrackSegment segment;
};

} // Namespace Navigation.
} // Namespace UF.

#endif // NAVIGATION_TRACK_SEGMENT_H
