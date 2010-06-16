#if !defined(MAPGRID_H)
#define MAPGRID_H

#include <cmath>
#include <string>

#include "ufWin32Header.h"

namespace UF {

namespace GeographicConversions {

  //! Stores constants and provides functions to access the parameters of the map grid that you are using.
  /*!
  CLASS
    MapGrid

    The class is initialised by passing to it the name of the map grid you want to use.
    By using MapGridName() you can change the map grid. However the map grid you are changing
    to must exist in the static class MapGrids.

    \par license
    This software is distributed WITHOUT ANY WARRANTY; without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    YOU, as the USER, take SOLE RESPONSIBILITY for, and are responsible for
    determining the FITNESS and APPLICABILITY of this software for a particular
    purpose. The author or any other person or entity are not liable in any way
    for the use or misuse of any program or software using this software.
  */
  class UF_GEO_CONV_EXPORT MapGrid
  {
  public:
    //! Construct a map grid.
    /*!
      @param GridName - the name of the grid to use.
    */
    MapGrid ( std::string const & GridName );

   virtual ~MapGrid(void) {}

  public:

    //! Assignment operator.
    MapGrid& operator = (MapGrid const & that)
    {
      // Handle self-assignment.
      if ( this == &that )
        return *this;

      this->mapGridName = that.mapGridName;
      this->falseEasting = that.falseEasting;
      this->falseNorthing = that.falseNorthing;
      this->centralScaleFactor = that.centralScaleFactor;
      this->zoneWidth = that.zoneWidth;
      this->centralMeridianZone1 = that.centralMeridianZone1;

      return *this;

    }


     //! Equality.
    bool operator == (MapGrid const & that)
    {
      // Handle self-comparison.
      if ( this == &that )
        return true;

      return (
        this->mapGridName == that.mapGridName &&
        this->falseEasting == that.falseEasting &&
        this->falseNorthing == that.falseNorthing &&
        this->centralScaleFactor == that.centralScaleFactor &&
        this->zoneWidth == that.zoneWidth &&
        this->centralMeridianZone1 == that.centralMeridianZone1
        );
    }

    //! Inequality.
    bool operator != (MapGrid const & that)
    {
      return !(*this == that);
    }


  public:
    //! Which map grid is in use?
    std::string MapGridName () const { return this->mapGridName; }
    //! Is the map grid valid?
    bool MapGridIsValid () const;
    //! Set the map grid to use.
    void SetMapGrid ( std::string const & MapGridName );
    //! Calculate the false easting in meters.
    double  FalseEasting ( void ) const { return falseEasting; }
    //! Calculate the false northing in meters.
    double  FalseNorthing ( void ) const { return falseNorthing; }
    //! Calculate the central scale factor = K0
    double  CentralScaleFactor ( void ) const { return centralScaleFactor; }
    //! Calculate the zone width in degrees.
    double  ZoneWidth ( void ) const { return this->zoneWidth; }
    //! Calculate the central meridian of zone 1 in degrees.
    double  CentralMeridianZone1 ( void ) const { return centralMeridianZone1; }
    //! Calculate the longitude of the western edge of zone zero in degrees.
    double  LWEZone0 ( void ) const
      { return this->CentralMeridianZone1() - 1.5 * this->ZoneWidth(); }
    //! Calculate the central meridian of zone zero in degrees.
    double  CMZone0 ( void ) const
      { return this->LWEZone0() + this->ZoneWidth() / 2.0; }


  protected:
    //! The map grid in use.
    std::string mapGridName;
    //! False easting in meters.
    double falseEasting;
    //! False northing in meters.
    double falseNorthing;
    //! Central scale factor.
    double centralScaleFactor;
    //! Zone width in degrees.
    double zoneWidth;
    //! The central meridian of zone 1 in degrees.
    double centralMeridianZone1;
  };

} // Namespace GeographicConversions.
} // Namespace UF.

#endif // MAPGRID_H
