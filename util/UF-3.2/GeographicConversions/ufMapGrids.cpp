
#include "ufMapGrids.h"

#include <exception>

using namespace UF::GeographicConversions;

MapGrids *MapGrids::pInstance_ = 0;
bool MapGrids::destroyed_ = false;

void MapGrids::AddKeyValue( std::string const & key, TMapGridParametersVector const & value )
{
  (*this->mg)[key] = value;
}

TMapGridParametersVector MapGrids::GetMapGrid( std::string const & key  )
{
  if ( this->mg->find(key) != this->mg->end() )
    return (*this->mg)[key];
  return (*this->mg)["Undefined"];
}

TMapGridParametersVector MapGrids::MakeVector(
  double const & FalseEasting, double const & FalseNorthing,
  double const & CentralScaleFactor, double const & ZoneWidth,
  double const & CentralMeridianZone1 )
{
  TMapGridParametersVector v;
  v.push_back(FalseEasting);
  v.push_back(FalseNorthing);
  v.push_back(CentralScaleFactor);
  v.push_back(ZoneWidth);
  v.push_back(CentralMeridianZone1);
  return v;
}

void MapGrids::ExtractFromVector( TMapGridParametersVector const & v,
  double & FalseEasting, double & FalseNorthing,
  double & CentralScaleFactor, double & ZoneWidth,
  double & CentralMeridianZone1 )
{
  FalseEasting = v.at(0);
  FalseNorthing = v.at(1);
  CentralScaleFactor = v.at(2);
  ZoneWidth = v.at(3);
  CentralMeridianZone1 = v.at(4);
}

void MapGrids::LoadPredefinedValues()
{
  this->AddKeyValue("Undefined",this->MakeVector(0.0,0.0,0.0,0,0));
  this->AddKeyValue("MGA",this->MakeVector(500000.0,10000000.0,0.9996,6,-177));
  this->AddKeyValue("UTM",this->MakeVector(500000.0,10000000.0,0.9996,6,-177));
}

TMapGridKeyVector MapGrids::AvailableMapGrids()
{
  TMapGridKeyVector keys;
  for (TMapGridMapItr p = this->mg->begin(); p != this->mg->end(); ++p)
    keys.push_back(p->first);
  return keys;
}
