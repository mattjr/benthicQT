
#include "ufEllipsoids.h"

#include <exception>

using namespace UF::GeographicConversions;

Ellipsoids *Ellipsoids::pInstance_ = 0;
bool Ellipsoids::destroyed_ = false;

void Ellipsoids::AddKeyValue( std::string const & key, TEllipsoidParametersVector const & value )
{
  (*this->em)[key] = value;
}

TEllipsoidParametersVector Ellipsoids::GetEllipsoid( std::string const & key  )
{
  if ( this->em->find(key) != this->em->end() )
    return (*this->em)[key];
  return (*this->em)["Undefined"];
}

TEllipsoidParametersVector Ellipsoids::MakeVector(
  double const & SemimajorAxis, double const & InverseFlattening )
{
  TEllipsoidParametersVector v;
  v.push_back(SemimajorAxis);
  v.push_back(InverseFlattening);
  return v;
}

void Ellipsoids::ExtractFromVector( TEllipsoidParametersVector const & v,
  double & SemimajorAxis, double & InverseFlattening )
{
  SemimajorAxis = v.at(0);
  InverseFlattening = v.at(1);
}

void Ellipsoids::LoadPredefinedValues()
{
  this->AddKeyValue("Undefined",this->MakeVector(0, 0));
  this->AddKeyValue("Airy 1830",this->MakeVector(6377563.396, 299.3249646));
  this->AddKeyValue("Modified Airy",this->MakeVector(6377340.189, 299.3249646));
  this->AddKeyValue("AMG84",this->MakeVector(6378160, 298.25)); // Australian Geodetic Datum 1984.
  this->AddKeyValue("Bessel 1841 (Namibia)",this->MakeVector(6377483.865, 299.1528128));
  this->AddKeyValue("Bessel 1841",this->MakeVector(6377397.155, 299.1528128));
  this->AddKeyValue("Clarke 1866",this->MakeVector(6378206.4, 294.9786982));
  this->AddKeyValue("Clarke 1880",this->MakeVector(6378249.145, 293.465));
  this->AddKeyValue("Everest (India 1830)",this->MakeVector(6377276.345, 300.8017));
  this->AddKeyValue("Everest (Sabah Sarawak)",this->MakeVector(6377298.556, 300.801));
  this->AddKeyValue("Everest (India 1956)",this->MakeVector(6377301.243, 300.8017));
  this->AddKeyValue("Everest (Malaysia 1969)",this->MakeVector(6377295.664, 300.8017));
  this->AddKeyValue("Everest (Malaya and Singapore)",this->MakeVector(6377304.063, 300.8017));
  this->AddKeyValue("Everest (Pakistan)",this->MakeVector(6377309.613, 300.8017));
  this->AddKeyValue("Modified Fischer 1960",this->MakeVector(6378155.0, 298.3));
  this->AddKeyValue("Helmert 1906",this->MakeVector(6378200.0, 298.3));
  this->AddKeyValue("Hough 1960",this->MakeVector(6378270.0, 297.0));
  this->AddKeyValue("Indonesian 1974",this->MakeVector(6378160.0,298.247));
  this->AddKeyValue("International 1924",this->MakeVector(6378388.0, 297.0));
  this->AddKeyValue("Krassovsky 1940",this->MakeVector(6378245.0, 298.3));
  this->AddKeyValue("GDA94",this->MakeVector(6378137.0, 298.257222101));
  this->AddKeyValue("GRS80",this->MakeVector(6378137.0, 298.257222101));
  this->AddKeyValue("South American 1969",this->MakeVector(6366707.019, 298.25));
  this->AddKeyValue("Sphere",this->MakeVector(6372795.0, 999999999999.9990));
  this->AddKeyValue("WGS72",this->MakeVector(6378135.0,298.26));
  this->AddKeyValue("WGS84",this->MakeVector(6378137.0,298.257223563));
}

TEllipsoidKeyVector Ellipsoids::AvailableEllipsoids()
{
  TEllipsoidKeyVector keys;
  for (TEllipsoidMapItr p = this->em->begin(); p != this->em->end(); ++p)
    keys.push_back(p->first);
  return keys;
}
