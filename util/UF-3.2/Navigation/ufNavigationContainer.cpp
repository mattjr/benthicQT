//
// C++ Implementation: NavigationContainer
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufNavigationContainer.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace UF::Navigation;

NavigationContainer *NavigationContainer::pInstance_ = 0;
bool NavigationContainer::destroyed_ = false;

NavigationContainer::NavigationContainer(){};

NavigationContainer::~NavigationContainer(){};

void NavigationContainer::Init()
{
  gpx.Init();
  metadata.Init();
  routes.clear();
  tracks.clear();
  ptsegs.clear();
  waypoints.clear();
}

std::string NavigationContainer::MakeHeader()
{
  
  std::ostringstream os;
  os << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>";
  os << "<gpx ";
  os << "xmlns=\"http://www.topografix.com/GPX/1/1\" ";
  os << "xmlns:gpxx=\"http://www.garmin.com/xmlschemas/GpxExtensions/v3\" ";
  os << "xmlns:gpxtpx=\"http://www.garmin.com/xmlschemas/TrackPointExtension/v1\"\n";

  os << gpx.ToXML();

  os << "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" ";
  os << "xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 ";
  os << "http://www.topografix.com/GPX/1/1/gpx.xsd ";
  os << "http://www.garmin.com/xmlschemas/GpxExtensions/v3 ";
  os << "http://www.garmin.com/xmlschemas/GpxExtensionsv3.xsd ";
  os << "http://www.garmin.com/xmlschemas/TrackPointExtension/v1 ";
  os << "http://www.garmin.com/xmlschemas/TrackPointExtensionv1.xsd\">\n";

  os << metadata.ToXML();

  return os.str();
}

std::string NavigationContainer::MakeFooter()
{
  return "</gpx>" ;
}

void NavigationContainer::MakeGPXFile(std::string const & fn)
{
  std::ofstream ofs(fn.c_str());
  if(ofs)
  {
    // Metadata is in here.
    ofs << MakeHeader();

    // Waypoints
    for ( TWaypointsIt p = waypoints.begin(); p != waypoints.end(); ++p )
    {
      ofs << p->ToXML();
    }
    ofs << "\n";
    // Routes
    for ( TRoutesIt p = routes.begin(); p!= routes.end(); ++p )
    {
      ofs << p->ToXML();
    }
    // Tracks
    for ( TTracksIt p = tracks.begin(); p!= tracks.end(); ++p )
    {
      ofs << p->ToXML();
    }
    ofs << MakeFooter();
    ofs.flush();
    ofs.close();
  }
}

