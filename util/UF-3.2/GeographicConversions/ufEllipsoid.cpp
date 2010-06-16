#include "ufEllipsoid.h"
#include "ufEllipsoids.h"

#include <sstream>
#include <iostream>

using namespace UF::GeographicConversions;

Ellipsoid::Ellipsoid ( std::string const & EllipsoidName )
{
  this->SetEllipsoid(EllipsoidName);
}

void Ellipsoid::SetEllipsoid ( std::string const & EllipsoidName )
{
  Ellipsoids *ellip = Ellipsoids::Instance();

  if ( !ellip->Exists( EllipsoidName ) )
  {
    this->ellipsoidName = "";
    std::ostringstream ss;
    ss << "The ellipsoid " << EllipsoidName << " does not exist.";
    throw(ss.str());
  }

  this->ellipsoidName = EllipsoidName;
  ellip->ExtractFromVector(ellip->GetEllipsoid(this->ellipsoidName),this->semimajorAxis,this->inverseFlattening);
}

bool Ellipsoid::EllipsoidIsValid () const
{
  Ellipsoids *ellip = Ellipsoids::Instance();
  return ellip->Exists( this->ellipsoidName );
}
