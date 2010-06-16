
#include "ufTrack.h"
#include "ufRoute.h"
#include "ufGreatCircle.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>

#include "ufLogger.h"
#include "ufSystemTime.h"
#include "ufISODateTime.h"


using namespace UF::Log;
using namespace UF::Navigation;

void TestPoint(ufLogger *pLog)
{
  Waypoint pos;

  *pLog << "Testing waypoints." << std::endl;

  std::ostringstream os;
  os << pos;
  if ( !os.str().empty() )
  {
    *pLog << "Fail - there should have been no output as there is no data." << std::endl;
  }
  os.str("");
  pos.SetElevation("209.70");
  os << pos;
  if ( !os.str().empty() )
  {
    *pLog << "Fail - there should have been no output as there is no lat or long." << std::endl;
  }
  os.str("");

  pos.Init();
  if ( !pos.IsEmpty() )
  {
    *pLog << "Fail - should be empty." << std::endl;
  }
  pos.SetLatitude("-33.405437");
  pos.SetLongitude("150.783916");
  pos.SetElevation("209.70");
  pos.SetTime("2009-04-18T06:29:16Z");
  *pLog << pos << std::endl;

  std::istringstream is;
  is.str("-33.405437 150.783916 209.70 2009-04-18T06:29:16Z");
  is >> pos;
  *pLog << pos << std::endl;

  is.str("50.901701 -1.489350 35.93 2009-03-13T08:45:21Z");
  is >> pos;
  pos.SetName("Garmin Europe");
  pos.SetSymbol("Flag, Blue");
  *pLog << pos.ToXML(2);
 }

void TestTrackSegment(ufLogger *pLog)
{
  TrackSegment seg;

  *pLog << "Testing track segment." << std::endl;

  Waypoint pt;
  std::istringstream is("-33.405437 150.784008 209.70 2009-04-18T06:29:16Z");
  is >> pt;
  seg.GetSegment()->push_back(pt);
  is.str("-33.405609 150.783916 208.74 2009-04-18T06:29:17Z");
  is >> pt;
  seg.GetSegment()->push_back(pt);
  is.str("-33.405785 150.784078 206.34 2009-04-18T06:29:18Z");
  is >> pt;
  seg.GetSegment()->push_back(pt);
  is.str("-33.405964 150.784127 205.86 2009-04-18T06:29:19Z");
  is >> pt;
  seg.GetSegment()->push_back(pt);

  *pLog << seg.ToXML(2) << std::endl;

  Waypoint se, nw;
  seg.GetBounds(se,nw);
  *pLog << "South east bound: \n" << se.ToXML() << std::endl;
  *pLog << "North west bound: \n" << nw.ToXML() << std::endl;

}

void TestTrack(ufLogger *pLog)
{
  *pLog << "Testing track." << std::endl;

  Track track;
  track.SetName("Test Track");
  TrackSegment seg;

  Waypoint pt;
  Waypoint se, nw;

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
  seg.GetBounds(se,nw);
  *pLog << "South east bound: \n" << se.ToXML() << std::endl;
  *pLog << "North west bound: \n" << nw.ToXML() << std::endl;

  track.GetTrack()->push_back(seg);
  seg.GetSegment()->clear();

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
  seg.GetBounds(se,nw);
  *pLog << "South east bound: \n" << se.ToXML() << std::endl;
  *pLog << "North west bound: \n" << nw.ToXML() << std::endl;
  track.GetTrack()->push_back(seg);


  *pLog << track.ToXML(2) << std::endl;
  track.GetBounds(se,nw);
  *pLog << "South east bound: \n" << se.ToXML() << std::endl;
  *pLog << "North west bound: \n" << nw.ToXML() << std::endl;

  GreatCircle *gc = GreatCircle::Instance();
  Track::TTrack *tracks = track.GetTrack();
  int trk_num = 0;
  for ( Track::TTrackIt p = tracks->begin(); p != tracks->end(); ++p)
  {
    Waypoint pt1;
    Waypoint pt2;
    double totalDistance = 0;
    double totalSeconds = 0;
    trk_num++;
    *pLog << "Track: " << trk_num << std::endl;
    *pLog << "Index\tTime\tElevation\tLeg Length(m)\tLeg Time(s)\tLeg Speed(km/h)\tLeg Course\tLatitude\tLongitude" << std::endl;
    int idx = 0;
    for ( TrackSegment::TTrackSegmentCIt q = p->GetSegment()->begin(); q != p->GetSegment()->end(); ++q)
    {
      if ( q == p->GetSegment()->begin() )
      {
        pt1 = *q;
        continue;
      }
      pt2 = *q;
      double course = gc->Course(pt1.GetLatitude(),pt1.GetLongitude(),pt2.GetLatitude(),pt2.GetLongitude());
      double distance = gc->Distance(pt1.GetLatitude(),pt1.GetLongitude(),pt2.GetLatitude(),pt2.GetLongitude());
      //double distanceH = gc->DistanceH(pt1.GetLat(),pt1.GetLongitude(),pt2.GetLat(),pt2.GetLongitude());
      //double delta = (distance - distanceH);
      //std::cout << distance * 1852 << " haversine: " << distanceH * 1852 << " delta: " << delta << std::endl;
      totalDistance += distance;
      UF::Time::ISODateTime dt1(pt1.GetTime());
      UF::Time::ISODateTime dt2(pt2.GetTime());
      double deltaSecs = 0;
      if ( dt1.IsValidDateTime() && dt2.IsValidDateTime() )
      {
        deltaSecs = (dt2.GetJDN() - dt1.GetJDN())*dt2.GetSecondsInDay() + 
                    (dt2.GetFoD() - dt1.GetFoD())*dt2.GetSecondsInDay();
        totalSeconds += deltaSecs;
      }
      idx++;
      *pLog << idx << "\t" 
        << pt1.GetTime() << "\t"
        << std::fixed << std::setprecision(1) << pt1.GetElevation() << "\t" 
        << std::fixed << std::setprecision(3) << distance*1852.0 << "\t"
        << std::fixed << std::setprecision(0) << deltaSecs << "\t"
        << std::fixed << std::setprecision(1) << ((deltaSecs > 0)?distance*1.8520 * 3600 / deltaSecs:'-')  << "\t"
        << std::fixed << std::setprecision(1) << course << "\t" 
        << std::fixed << std::setprecision(6) << pt1.GetLatitude() << "\t" 
        << std::fixed << std::setprecision(6) << pt1.GetLongitude() << std::endl;
      if ( q == --p->GetSegment()->end() )
      {
        idx++;
        *pLog << idx << "\t" 
          << pt2.GetTime() << "\t"
          << std::fixed << std::setprecision(1) << pt2.GetElevation() << "\t" 
          << "\t"
          << "\t"
          << "\t"
          << "\t"
          << std::fixed << std::setprecision(6) << pt2.GetLatitude() << "\t" 
          << std::fixed << std::setprecision(6) << pt2.GetLongitude() << std::endl;
        *pLog << std::fixed << std::setprecision(1)
          << "Total distance: " << totalDistance*1852.0 
          << "m\t Total time: " << totalSeconds << "s" << std::endl;
        break;
      }
      pt1 = pt2;
    }
  }

}

void TestRte(ufLogger *pLog)
{
  *pLog << "Testing route." << std::endl;

  Waypoint pt;
  std::istringstream is("50.901701 -1.489350 35.93 2009-03-13T08:45:21Z");
  is >> pt;
  pt.SetName("Garmin Europe");
  pt.SetSymbol("Flag, Red");

  Route route;
  route.GetRoute()->push_back(pt);

  is.str("38.855550 -94.799018 325.05 2009-03-13T08:45:21Z");
  is >> pt;
  pt.SetName("Garmin");
  pt.SetSymbol("Flag, Green");
  route.GetRoute()->push_back(pt);

  is.str("25.061784 121.640268 38.10 2009-03-13T08:45:21Z");
  is >> pt;
  pt.SetName("Garmin Asia");
  pt.SetSymbol("Flag, Blue");
  route.GetRoute()->push_back(pt);

  is.str("50.901701 -1.489350 35.93 2009-03-13T08:45:21Z");
  is >> pt;
  pt.SetName("Garmin Europe");
  pt.SetSymbol("Flag, Red");
  route.GetRoute()->push_back(pt);

  route.SetName("Garmin round trip.");

  *pLog << route.ToXML() << std::endl;

  Waypoint se, nw;
  route.GetBounds(se,nw);
  *pLog << "South east bound: \n" << se.ToXML() << std::endl;
  *pLog << "North west bound: \n" << nw.ToXML() << std::endl;

  double totalDistance = 0;
  Waypoint pt1;
  Waypoint pt2;
  *pLog << "Route: " << route.GetName() << std::endl;
  *pLog << " \tDirections/Name\tDistance\tLeng length\tLeg Time\tTotal Time\tCourse\tLatitude\tLongitude\tElevation" << std::endl;

  GreatCircle *gc = GreatCircle::Instance();

  Route::TRoute *pRoute = route.GetRoute();

  int idx = 0;
  for ( Route::TRouteCIt p = pRoute->begin(); p != pRoute->end(); ++p)
  {
    idx++;
    if ( p == pRoute->begin() )
    {
      pt1 = *p;
      *pLog << idx << "\t" 
        << pt1.GetName() << "\t"
        << "0\t"
        << "-\t"
        << "-\t"
        << "-\t"
        << "-\t" 
        << std::fixed << std::setprecision(6) << pt1.GetLatitude() << "\t" 
        << std::fixed << std::setprecision(6) << pt1.GetLongitude() << "\t"
        << std::fixed << std::setprecision(1) << pt1.GetElevation() << std::endl;
      continue;
    }
    pt2 = *p;
    double bearing = gc->Course(pt1.GetLatitude(),pt1.GetLongitude(),pt2.GetLatitude(),pt2.GetLongitude());
    double distance = gc->Distance(pt1.GetLatitude(),pt1.GetLongitude(),pt2.GetLatitude(),pt2.GetLongitude());
    totalDistance += distance;
    *pLog << idx << "\t" 
      << pt2.GetName() << "\t"
      << std::fixed << std::setprecision(3) << totalDistance * 1.852 << "\t"
      << std::fixed << std::setprecision(3) << distance * 1.852 << "\t"
      << "-\t"
      << "-\t"
      << std::fixed << std::setprecision(1) << bearing << "\t" 
      << std::fixed << std::setprecision(6) << pt2.GetLatitude() << "\t" 
      << std::fixed << std::setprecision(6) << pt2.GetLongitude() << "\t"
      << std::fixed << std::setprecision(1) << pt2.GetElevation() << std::endl;
    pt1 = pt2;
  }
}

int NavigationMain ( int argc, char* argv[] )
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
  pLog->open("TestNavigation.log",std::ios::out);
  std::string time;
  UF::Time::SystemTime *pClock = UF::Time::SystemTime::Instance();
  pClock->ISOLocalTime(time);
  *pLog << sep << "\nStarted:  " << time << std::endl;

  bool fail = false;

  if ( !fail )
  {
    *pLog << "Testing GPS Navigation" << std::endl;

    std::string sep1 = "*****************************";
    *pLog << sep1 << std::endl;
    TestPoint(pLog);
    *pLog << sep1 << std::endl;
    TestTrackSegment(pLog);
    *pLog << sep1 << std::endl;
    TestTrack(pLog);
    *pLog << sep1 << std::endl;
    TestRte(pLog);
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
