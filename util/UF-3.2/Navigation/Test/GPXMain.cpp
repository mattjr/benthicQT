/*
   We will create the data and store it in the static class NavigationContainer.
   Generally you would stream in data from a source such as a device or,
   if you are parsing an xml file, the output from a sax or dom parser.

   The data is stored in the static class.

   It can be easily written out as an XML 1.1 file as demonstrated.

*/

// This header will pull in all the necessary 
// typedefs, structures and definitions.
#include "ufNavigationContainer.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>

#include "ufLogger.h"
#include "ufSystemTime.h"
#include "ufISODateTime.h"


using namespace UF::Log;
using namespace UF::Navigation;

void CreateWaypoints()
{
  // Make a pointer to the static class.
  NavigationContainer *nc = NavigationContainer::Instance();

  Waypoint pt;
  std::istringstream is("50.901701 -1.489350 35.93 2009-03-13T08:45:21Z");
  is >> pt;
  pt.SetName("Garmin Europe");
  pt.SetSymbol("Flag, Red");

  nc->waypoints.push_back(pt);

  is.str("38.855550 -94.799018 325.05 2009-03-13T08:45:21Z");
  is >> pt;
  pt.SetName("Garmin");
  pt.SetSymbol("Flag, Green");
  nc->waypoints.push_back(pt);

  is.str("25.061784 121.640268 38.10 2009-03-13T08:45:21Z");
  is >> pt;
  pt.SetName("Garmin Asia");
  pt.SetSymbol("Flag, Blue");
  nc->waypoints.push_back(pt);
}

void CreateRoutes()
{
  // Make a pointer to the static class.
  NavigationContainer *nc = NavigationContainer::Instance();

  Route route;
  for ( NavigationContainer::TWaypointsCIt p = nc->waypoints.begin(); p != nc->waypoints.end(); ++p )
  {
    route.GetRoute()->push_back(*p);
  }
  route.SetName("Garmin Europe to Garmin Asia");
  route.SetHref("http://www.garmin.com");
  route.SetText("Garmin");
  nc->routes.push_back(route);
}

void CreateTracks()
{
  // Make a pointer to the static class.
  NavigationContainer *nc = NavigationContainer::Instance();

  Track track;
  TrackSegment seg;
  Waypoint pt;

  std::istringstream is("-33.405437 150.783916 209.70 2009-04-18T06:29:16Z");
  is >> pt;
  seg.GetSegment()->push_back(pt);
  is.str("-33.405609 150.784008 208.74 2009-04-18T06:29:17Z");
  is >> pt;
  seg.GetSegment()->push_back(pt);
  is.str("-33.405785 150.784078 206.34 2009-04-18T06:29:18Z");
  is >> pt;
  seg.GetSegment()->push_back(pt);
  is.str("-33.405964 150.784127 205.86 2009-04-18T06:29:19Z");
  is >> pt;
  seg.GetSegment()->push_back(pt);

  track.GetTrack()->push_back(seg);
  track.SetName("Moving vehicle");
  track.SetNumber(1);
  nc->tracks.push_back(track);

  seg.GetSegment()->clear();
  track.GetTrack()->clear();
  is.str("");
  is.str("-33.711351 150.570990 341.41 2009-04-18T07:27:35Z");
  is >> pt;
  seg.GetSegment()->push_back(pt);
  is.str("-33.711348 150.570990 342.85 2009-04-18T07:28:58Z");
  is >> pt;
  seg.GetSegment()->push_back(pt);
  is.str("-33.711348 150.570990 341.41 2009-04-18T07:29:00Z");
  is >> pt;
  seg.GetSegment()->push_back(pt);
  is.str("-33.711347 150.570992 340.44 2009-04-18T07:29:21Z");
  is >> pt;
  seg.GetSegment()->push_back(pt);
  is.str("-33.711344 150.570983 339.96 2009-04-18T07:29:22Z");
  is >> pt;
  seg.GetSegment()->push_back(pt);
  track.GetTrack()->push_back(seg);
  track.SetName("Stationary vehicle");
  track.SetNumber(2);
  nc->tracks.push_back(track);
}

//! Minimum and maximum should really only be used for a single track or route.
/*!
    @param sw: south-western bound.
    @param ne: north-eastern bound.
    @param option: 0: Use tracks to calculate the bound, 1: use the routes.
 */
void CalculateBounds(Waypoint & sw, Waypoint & ne, int const & option = 0)
{
  // Make a pointer to the static class.
  NavigationContainer *nc = NavigationContainer::Instance();

  // Calculate the bounds.
  UF::Navigation::Waypoint r_sw,r_ne; // Route bounds
  UF::Navigation::Waypoint t_sw,t_ne; // Track bounds
  UF::Navigation::Waypoint tmp_sw,tmp_ne;

  switch ( option )
  {
  case 1:
    for ( NavigationContainer::TRoutesIt p = nc->routes.begin(); p!= nc->routes.end(); ++p)
    {
      p->GetBounds(tmp_sw,tmp_ne);
      if ( p == nc->routes.begin() )
      {
        r_sw = tmp_sw;
        r_ne = tmp_ne;
        continue;
      }
      r_sw.Minimum(tmp_sw);
      r_ne.Maximum(tmp_ne);
    }
    sw = r_sw;
    ne = r_ne;
    break;
  default:
    for ( NavigationContainer::TTracksIt p = nc->tracks.begin(); p!= nc->tracks.end(); ++p)
    {
      p->GetBounds(tmp_sw,tmp_ne);
      if ( p == nc->tracks.begin() )
      {
        t_sw = tmp_sw;
        t_ne = tmp_ne;
        continue;
      }
      t_sw.Minimum(tmp_sw);
      t_ne.Maximum(tmp_ne);
      sw = t_sw;
      ne = t_ne;
    }

  }
}

//! Set the metadata field in the gpx file.
void SetMetadata()
{
  // Make a pointer to the static class.
  NavigationContainer *nc = NavigationContainer::Instance();

  nc->metadata.SetDescription("This GPX file should be able to be read by\n    any program that complies with the GPX 1.1 standard.\n");
  Person person;
  person.SetName("Andrew Maclean");
  person.SetEmail("a.maclean@cas.edu.au");
  Link link;
  link.SetHref("http://www.cas.edu.au");
  link.SetText("CAS");
  person.SetLink(link);
  nc->metadata.SetAuthor(person);

  // You can directly set the base class like this.
  nc->metadata.Copyright::SetAuthor("Andrew Maclean");
  nc->metadata.Copyright::SetYear(2009);
  // This must be a valid URI.
  nc->metadata.Copyright::SetLicense("http://en.wikipedia.org/wiki/URI#Examples_of_URI_references");

  int y;
  int m;
  int d;
  int h;
  int min;
  double s;
  /*bool result = */UF::Time::SystemTime::Instance()->Time(y,m,d,h,min,s);
  UF::Time::DateTime dt;
  std::string tickTock = dt.ISODateTimeStr(y,m,d,h,min,s,0,3,'-',':','T','Z');
  nc->metadata.SetTime(tickTock);
}

//! Count the number of waypoints etc.
void Statistics(ufLogger *pLog)
{
  // Make a pointer to the static class.
  NavigationContainer *nc = NavigationContainer::Instance();

  typedef std::map<std::string,int> T1;
  typedef std::map<std::string,T1> TT1;
  typedef std::map<std::string,TT1> TTT1;

  TTT1 x;
  x["Waypoint"][""][""] = nc->waypoints.size();
  for ( NavigationContainer::TRoutesIt p = nc->routes.begin(); p != nc->routes.end(); ++p )
  {
    x["Route"][p->GetName()][""] = p->GetRoute()->size();
  }
  for ( NavigationContainer::TTracksIt p = nc->tracks.begin(); p != nc->tracks.end(); ++p )
  {
    int total = 0;
    int count = 0;
    for ( Track::TTrackIt q = p->GetTrack()->begin(); q != p->GetTrack()->end(); ++q )
    {
      count++;
      std::ostringstream os;
      os << "Segment: " << count;
      x["Track"][p->GetName()][os.str()] = q->GetSegment()->size();
      total += q->GetSegment()->size();
    }
    x["Track"][p->GetName()][""] = total;
  }

  *pLog << "Count of points." << std::endl;
  for ( TTT1::const_iterator p = x.begin(); p != x.end(); ++p )
  {
    *pLog << p->first;
    for ( TT1::const_iterator q = p->second.begin(); q != p->second.end(); ++q )
    {
      *pLog << "\t" << q->first;
      for ( T1::const_iterator r = q->second.begin(); r != q->second.end(); ++r )
      {
        *pLog << "\t" << r->first << "\t" << r->second << std::endl;
      }
    }
  }
}

int GPXMain ( int argc, char* argv[] )
{
  // Get rid of the argc, argv unreferenced formal parameter warnings.
  int numargs;
  std::string programName;
  numargs = argc;
  if ( argc != 0 )
  {
    programName = argv[0];
  }

  std::string sep = "------------------------------------------------------------------------";
  // Set up the log file.
  ufLogger *pLog = new ufLogger;
  pLog->open("TestGPX.log",std::ios::out);
  std::string time;
  UF::Time::SystemTime *pClock = UF::Time::SystemTime::Instance();
  pClock->ISOLocalTime(time);
  *pLog << sep << "\nStarted:  " << time << std::endl;

  bool fail = false;

  // Make a pointer to the static class.
  NavigationContainer *nc = NavigationContainer::Instance();

  if ( !fail )
  {
    *pLog << "Testing creating a GPX file" << std::endl;

    std::string sep1 = "*****************************";

    // Create the data
    CreateWaypoints();
    CreateRoutes();
    CreateTracks();

    // Set the metadata.
    SetMetadata();
    
    //Get the bounds and update the metadata with them.
    Waypoint sw, ne;
    CalculateBounds(sw,ne);
    Bounds bounds;
    bounds.SetMinimumLatitude(sw.GetLatitude());
    bounds.SetMinimumLongitude(sw.GetLongitude());
    bounds.SetMaximumLatitude(ne.GetLatitude());
    bounds.SetMaximumLongitude(ne.GetLongitude());
    nc->metadata.SetBounds(bounds);

    // Set the version and creator.
    nc->gpx.SetVersion("1.1");
    nc->gpx.SetCreator("Oregon400t");

    // Finally ... write it out.
    nc->MakeGPXFile("Test.gpx");

    Statistics(pLog);

    *pLog << sep1 << std::endl;
  }
  pClock->ISOLocalTime(time);
  *pLog << ((fail)?"Failed.":"Passed") << "\n"
      << "Finished: " << time << "\n" << sep << std::endl;

  delete pLog;

  if ( fail )
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
