//
// C++ Implementation: GeographicPoint
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufGeographicPoint.h"

#include <sstream>
#include <iomanip>

using namespace UF::Navigation;

void GeographicPoint::Init()
{
  this->lat = IEEEConstants::pINFd;
  this->lon = IEEEConstants::pINFd;
  this->ele = IEEEConstants::pINFd;

  GPXAttributes.insert("lat");
  GPXAttributes.insert("lon");

  GPXElements.insert("ele");
  GPXElements.insert("time");
}

std::string GeographicPoint::Indent(int const & indent)
{
  std::string s;
  for ( int i = 0; i < indent; ++i)
  {
    s += " ";
  }
  return s;
}

std::string GeographicPoint::ToXML(int indent, std::string const & tag)
{
  std::string s;
  if ( this->lat == IEEEConstants::pINFd || this->lon == IEEEConstants::pINFd )
  {
    // No lat/lon.
    return s;
  }
  std::string idnt = this->Indent(indent);
  std::ostringstream os;
  os << idnt << "<" << tag << " lat=\"" << std::fixed << std::setprecision(6) << this->lat << "\" lon=\"" << this->lon << "\">\n";
  indent += 2;
  idnt = this->Indent(indent);
  if ( this->ele != IEEEConstants::pINFd )
  {
    os << idnt << "<ele>" << std::fixed << std::setprecision(2) << this->ele << "</ele>\n";
  }
  if ( !this->time.empty() )
  {
    os << idnt << "<time>" << this->time << "</time>\n";
  }
  indent -= 2;
  idnt = this->Indent(indent);
  os << idnt << "</"<< tag << ">\n";
  s = os.str();

  return s;
}

void GeographicPoint::Normalise()
{
  if ( this->lat != IEEEConstants::pINFd)
  {
    while ( this->lat < -90 )
    {
      this->lat += 90;
    }
    while ( this->lat > 90 )
    {
      this->lat -= 90;
    }
  }
  if ( this->lon != IEEEConstants::pINFd)
  {
    while ( this->lon <= -180 )
    {
      this->lat += 360;
    }
    while ( this->lat > 180 )
    {
      this->lat -= 360;
    }
  }
}


void GeographicPoint::Minimum(GeographicPoint const & pt)
{
  if ( this->lat == IEEEConstants::pINFd && pt.lat != IEEEConstants::pINFd )
  {
    this->lat = pt.lat;
  }
  else
    if ( this->lat != IEEEConstants::pINFd && pt.lat != IEEEConstants::pINFd )
    {
      if ( this->lat > pt.lat )
      {
        this->lat = pt.lat;
      }
    }

  if ( this->lon == IEEEConstants::pINFd && pt.lon != IEEEConstants::pINFd )
  {
    this->lon = pt.lon;
  }
  else
    if ( this->lon != IEEEConstants::pINFd && pt.lon != IEEEConstants::pINFd )
    {
      // Longitude is tricky.
      double a = this->lon;
      double b = pt.lon;
      bool p = a <= b;
      bool q = std::abs(b-a) < 180;
      // p xor q
      if ( (p || q) && !(p && q) )
      {
         // a is west of b
         this->lon = pt.lon;
      }
    }

  if ( this->ele == IEEEConstants::pINFd && pt.ele != IEEEConstants::pINFd )
  {
    this->ele = pt.ele;
  }
  else
    if ( this->ele != IEEEConstants::pINFd && pt.ele != IEEEConstants::pINFd )
    {
      if ( this->ele > pt.ele )
      {
        this->ele = pt.ele;
      }
    }

 }

void GeographicPoint::Maximum(GeographicPoint const & pt)
{
  if ( this->lat == IEEEConstants::pINFd && pt.lat != IEEEConstants::pINFd )
  {
    this->lat = pt.lat;
  }
  else
    if ( this->lat != IEEEConstants::pINFd && pt.lat != IEEEConstants::pINFd )
    {
      if ( this->lat < pt.lat )
      {
        this->lat = pt.lat;
      }
    }

  if ( this->lon == IEEEConstants::pINFd && pt.lon != IEEEConstants::pINFd )
  {
    this->lon = pt.lon;
  }
  else
    if ( this->lon != IEEEConstants::pINFd && pt.lon != IEEEConstants::pINFd )
    {
      // Longitude is tricky.
      double a = this->lon;
      double b = pt.lon;
      bool p = a <= b;
      bool q = std::abs(b-a) < 180;
      // p xor q
      if ( !((p || q) && !(p && q)) )
      {
         // a is east of b
         this->lon = pt.lon;
      }
    }

  if ( this->ele == IEEEConstants::pINFd && pt.ele != IEEEConstants::pINFd )
  {
    this->ele = pt.ele;
  }
  else
    if ( this->ele != IEEEConstants::pINFd && pt.ele != IEEEConstants::pINFd )
    {
      if ( this->ele < pt.ele )
      {
        this->ele = pt.ele;
      }
    }

 }
