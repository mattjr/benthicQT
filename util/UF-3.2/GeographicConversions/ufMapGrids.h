#if !defined(MAPGRIDS_H)
#define MAPGRIDS_H

#include <stdexcept>
#include <vector>
#include <map>
#include <string>

#include "ufWin32Header.h"
namespace UF {

namespace GeographicConversions {

  typedef std::vector< double > TMapGridParametersVector;
  typedef TMapGridParametersVector::iterator TMapGridParametersVectorItr;

  typedef std::map < std::string, TMapGridParametersVector > TMapGridMap;
  typedef TMapGridMap::iterator TMapGridMapItr;

  typedef std::vector < std::string > TMapGridKeyVector;
  typedef TMapGridKeyVector::iterator TMapGridKeyVectorItr;

  //! This static class implements a map of map grids consisting of their name and the parameters.
  /*!
  CLASS
    MapGrids

    This class is instantiated with the preset map grids outlined in the table below. You can
    add new mapping grids via the function AddkeyValue();

    The available map grids are:
    <div align="center">
      <center>
      <table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" id="AutoNumber1">
        <tr>
          <td width="64">Name</td>
          <td width="78">False<br>
          Easting<br>
          (m)</td>
          <td width="103">False<br>
          Northing<br>
          (m)</td>
          <td width="80">Central<br>
          Scale<br>
          Factor</td>
          <td width="101">Zone<br>
          Width<br>
          (degrees)</td>
          <td width="107">Central<br>
          Meridian <br>
          of Zone 1 <br>
          (degrees)</td>
        </tr>
        <tr>
          <td width="64">MGA</td>
          <td width="78">500000</td>
          <td width="103">10000000</td>
          <td width="80">0.9996</td>
          <td width="101">6</td>
          <td width="107">-177</td>
        </tr>
        <tr>
          <td width="64">UTM</td>
          <td width="78">500000</td>
          <td width="103">10000000</td>
          <td width="80">0.9996</td>
          <td width="101">6</td>
          <td width="107">-177</td>
        </tr>
      </table>
      </center>
    </div>

    \par license
    This software is distributed WITHOUT ANY WARRANTY; without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    YOU, as the USER, take SOLE RESPONSIBILITY for, and are responsible for
    determining the FITNESS and APPLICABILITY of this software for a particular
    purpose. The author or any other person or entity are not liable in any way
    for the use or misuse of any program or software using this software.
  */
  class UF_GEO_CONV_EXPORT MapGrids
  {
  public:
    //! Unique point of access for the singleton.
    static MapGrids* Instance()
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
      static MapGrids theInstance;
      pInstance_ = &theInstance;
    }
    //! Gets called if a dead reference is detected.
    static void OnDeadReference()
    {
      throw std::runtime_error("Dead Reference Error in MapGrids.");
    }
    //! Private constructor, disables creation of the object by clients.
    MapGrids()
    {
      mg = new TMapGridMap;
      this->LoadPredefinedValues();
    };
    MapGrids( const MapGrids & ); // Disable the copy constructor.
    MapGrids& operator = ( const MapGrids & ); // Disable self-assignment.
    // Make the destructor private in order to prevent clients
    // holding a pointer from accidentally deleting it.
    virtual ~MapGrids(void)
    {
      pInstance_ = 0;
      destroyed_ = true;

      delete mg;
    };

    //! Pointer to the instance.
    static MapGrids * pInstance_;
    //! True if destroyed.
    static bool destroyed_;

  public:
    // Here we put in our specific methods and variables for this class.

    //! Add a key (query identifier) and a value (the query).
    void AddKeyValue( std::string const & key, TMapGridParametersVector const & value );
    //! Return the selected values.
    TMapGridParametersVector GetMapGrid( std::string const & key );
    //! Make the values into a vector.
    TMapGridParametersVector MakeVector(
      double const & FalseEasting, double const & FalseNorthing,
      double const & CentralScaleFactor, double const & ZoneWidth,
      double const & CentralMeridianZone1 );
    //! Extract the values from the vector.
    void ExtractFromVector( TMapGridParametersVector const & v,
      double & FalseEasting, double & FalseNorthing,
      double & CentralScaleFactor, double & ZoneWidth,
      double & CentralMeridianZone1 );

    //! Return a vector of the keys of the available maps.
    TMapGridKeyVector AvailableMapGrids();

    //! Return true if the map grid exists.
    bool Exists ( std::string const & key ) const
    {
      return this->mg->find(key) != this->mg->end();
    }

  private:
    void LoadPredefinedValues();
  private:
    TMapGridMap *mg;
  };

} // Namespace GeographicConversions.
} // Namespace UF.

#endif
