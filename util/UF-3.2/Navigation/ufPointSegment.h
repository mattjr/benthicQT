//
// C++ Interface: PointSegment
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NAVIGATION_POINT_SEGMENT_H
#define NAVIGATION_POINT_SEGMENT_H

#include "ufWin32Header.h"

#include "ufGeographicPoint.h"

#include <set>
#include <vector>

namespace UF {
//! Classes for navigation.
namespace Navigation {


//! A Point Segment holds an ordered sequence of points. For example polylines or polygons.
/*!
    See: http://www.topografix.com/GPX/1/1/#type_ptsegType
 */
class UF_NAV_EXPORT PointSegment
{
public:
  typedef std::vector<GeographicPoint> TPointSegment;
  typedef TPointSegment::iterator TPointSegmentIt;
  typedef TPointSegment::const_iterator TPointSegmentCIt;
  typedef std::set<std::string> TPointSegmentGPXElements;

  PointSegment()
  {
    Init();
  };

  ~PointSegment(){};

  //! Initialise the variables.
  void Init();

public:
  //! Get the segment.
  /*!
      @return the address of the point segment.
  */
  TPointSegment * GetSegment() { return &this->segment; }

  //! Return an XML representation of the poin segment.
  /*!
      @return The track segment.
  */
  std::string ToXML(int indent = 2);

  //! Return the bounds of the point segment.
  /*! 
      Only latitude, longitude and elevation are considered.
      The bounds refer only to the existing positions in the track segment.
      If the point segment is a great circle the bounds may lie outside these positions.

      The minimum elevation will be found in the south-eastern bound.

      se - south-eastern bound of the track segment.
      nw - north-western bound of the track segment.
   */
  void GetBounds(GeographicPoint & se, GeographicPoint & nw);

  //! Get the GPXElements.
  /*!
      @return the address of GPXElements.
  */
  TPointSegmentGPXElements * GetGPXElements() { return &this->GPXElements; }

  // Points are processed in the segment vector.

protected:
  TPointSegmentGPXElements GPXElements;

  //! Indent a string by this amount.
  /*!
      @return A series of spaces corresponding to the indent.
  */
  std::string Indent(int const & indent);

  //! A ordered sequence of points that form a polyline or polygon.
  TPointSegment segment;

};

} // Namespace Navigation.
} // Namespace UF.

#endif // NAVIGATION_POINT_SEGMENT_H
