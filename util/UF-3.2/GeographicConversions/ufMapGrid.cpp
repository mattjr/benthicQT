#include "ufMapGrid.h"
#include "ufMapGrids.h"

#include <sstream>
#include <iostream>

using namespace UF::GeographicConversions;

MapGrid::MapGrid ( std::string const & MapGridName )
{
  this->SetMapGrid(MapGridName);
}

void MapGrid::SetMapGrid ( std::string const & MapGridName )
{
  MapGrids *mg = MapGrids::Instance();

  if ( !mg->Exists( MapGridName ) )
  {
    this->mapGridName = "";
    std::ostringstream ss;
    ss << "The map grid " << MapGridName << " does not exist.";
    throw(ss.str());
  }

  this->mapGridName = MapGridName;
  mg->ExtractFromVector(mg->GetMapGrid(
      this->mapGridName),
       this->falseEasting,this->falseNorthing,this->centralScaleFactor,
      this->zoneWidth,this->centralMeridianZone1);
}

bool MapGrid::MapGridIsValid () const
{
  MapGrids *mg = MapGrids::Instance();
  return mg->Exists( this->mapGridName );
}
