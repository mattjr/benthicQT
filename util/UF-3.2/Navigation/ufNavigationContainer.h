//
// C++ Interface: NavigationContainer
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NAVIGATION_NAVIGATION_CONTAINER_H
#define NAVIGATION_NAVIGATION_CONTAINER_H

#include "ufWin32Header.h"

#include <stdexcept>

#include "ufGpx.h"
#include "ufMetadata.h"
#include "ufRoute.h"
#include "ufTrack.h"
#include "ufPointSegment.h"

namespace UF {
//! Classes for navigation.
namespace Navigation {

//! A static class that holds the navigation data structures.
class UF_NAV_EXPORT NavigationContainer
{
public:
  typedef std::vector<UF::Navigation::Waypoint> TWaypoints;
  typedef TWaypoints::iterator TWaypointsIt;
  typedef TWaypoints::const_iterator TWaypointsCIt;

  typedef std::vector<UF::Navigation::Route> TRoutes;
  typedef TRoutes::iterator TRoutesIt;
  typedef TRoutes::const_iterator TRoutesCIt;

  typedef std::vector<UF::Navigation::Track> TTracks;
  typedef TTracks::iterator TTracksIt;
  typedef TTracks::const_iterator TTracksCIt;

  typedef std::vector<UF::Navigation::PointSegment> TPointSegments;
  typedef TPointSegments::iterator TPointSegmentsIt;
  typedef TPointSegments::const_iterator TPointSegmentsCIt;


public:
  //! Unique point of access for the singleton.
  static NavigationContainer* Instance()
  {
    if ( !pInstance_ )
    {
      // Check for a dead reference.
      if ( destroyed_ )
        OnDeadReference();
      else // First call initialise.
        Create();
    }
    return pInstance_;
  }

private:

  //! Create a new singleton and store a pointer to it in pInstance_
  static void Create()
  {
    // Task: Initialise pInstance_
    static NavigationContainer theInstance;
    pInstance_ = &theInstance;
  }
  //! Gets called if a dead reference is detected.
  static void OnDeadReference()
  {
    throw std::runtime_error("Dead Reference Error in NavigationContainer.");
  }
  //! Private constructor, disables creation of the object by clients.
  NavigationContainer();
  NavigationContainer( const NavigationContainer & ); // Disable the copy constructor.
  NavigationContainer& operator = ( const NavigationContainer & ); // Disable self-assignment.
  // Make the destructor private in order to prevent clients
  // holding a pointer from accidentally deleting it.
  virtual ~NavigationContainer(void);

  //! Pointer to the instance.
  static NavigationContainer * pInstance_;
  //! True if destroyed.
  static bool destroyed_;

private:

public:
  //! Initialise.
  void Init();

  //! Make an XML file conforming to the GPX standard.
  /*!
      @param fn - the file name.
   */
  void MakeGPXFile(std::string const & fn);

private:
  //! Make the GPX Header.
  /*!
      This will contain the contents of the gpx and metadata variables, among other things.

      @return The header.
   */
  std::string MakeHeader();

  //! Make the GPX Footer.
  /*!

      @return The footer.
   */
  std::string MakeFooter();

public:
  // The main data structures.
  // These are public so that you can
  // copy these.

  UF::Navigation::Gpx gpx;
  UF::Navigation::Metadata metadata;
  TRoutes routes;
  TTracks tracks;
  TPointSegments ptsegs;
  TWaypoints waypoints;
};

} // Namespace Navigation.
} // Namespace UF.

#endif // NAVIGATION_NAVIGATION_CONTAINER_H
