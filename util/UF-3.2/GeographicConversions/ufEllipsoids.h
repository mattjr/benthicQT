#if !defined(ELLIPSOIDS_H)
#define ELLIPSOIDS_H

#include <stdexcept>
#include <vector>
#include <map>
#include <string>

#include "ufWin32Header.h"

namespace UF {

namespace GeographicConversions {

  typedef std::vector< double > TEllipsoidParametersVector;
  typedef TEllipsoidParametersVector::iterator TEllipsoidParametersVectorItr;

  typedef std::map < std::string, TEllipsoidParametersVector > TEllipsoidMap;
  typedef TEllipsoidMap::iterator TEllipsoidMapItr;

  typedef std::vector < std::string > TEllipsoidKeyVector;
  typedef TEllipsoidKeyVector::iterator TEllipsoidKeyVectorItr;

  //! This static class implements a map of ellipsoids consisting of their name and the parameters.
  /*!
  CLASS
    Ellipsoids

    This class is instantiated with the preset ellipsoids in the table below. You can
    add new ellipsoids via the function AddkeyValue();

    The sphere uses a semi-major radius of 6366707.019m and 1/flattening = 999999999999.9990.
    THis means that f is essentially zero for all practical purposes. The radius
    is calculated as follows: one nautical mile is 1852m and there are 360*60 = 21600nmi in a great circle. Hence
    the radius is 1852*21600/(2*pi) = 6366707.019m. This is based on the the historical definition of a nautical
    mile being one minute of arc of a great circle of the earth.

    The available ellipsoids are:

      <div align="center">
      <center>
      <table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" id="AutoNumber1">
        <tr>
          <td width="213"><b>Ellipsoid</b></td>
          <td width="151"><b>Semi-major axis</b></td>
          <td width="127"><b>1/flattening </b></td>
        </tr>
        <tr>
          <td width="213">Airy 1830</td>
          <td width="151">6377563.396</td>
          <td width="127">299.3249646</td>
        </tr>
        <tr>
          <td width="213">Modified Airy</td>
          <td width="151">6377340.189</td>
          <td width="127">299.3249646</td>
        </tr>
        <tr>
          <td width="213">Australian National</td>
          <td width="151">6378160</td>
          <td width="127">298.25</td>
        </tr>
        <tr>
          <td width="213">Bessel 1841 (Namibia)</td>
          <td width="151">6377483.865</td>
          <td width="127">299.1528128</td>
        </tr>
        <tr>
          <td width="213">Bessel 1841</td>
          <td width="151">6377397.155</td>
          <td width="127">299.1528128</td>
        </tr>
        <tr>
          <td width="213">Clarke 1866</td>
          <td width="151">6378206.4</td>
          <td width="127">294.9786982</td>
        </tr>
        <tr>
          <td width="213">Clarke 1880</td>
          <td width="151">6378249.145</td>
          <td width="127">293.465</td>
        </tr>
        <tr>
          <td width="213">Everest (India 1830)</td>
          <td width="151">6377276.345</td>
          <td width="127">300.8017</td>
        </tr>
        <tr>
          <td width="213">Everest (Sabah Sarawak)</td>
          <td width="151">6377298.556</td>
          <td width="127">300.8017</td>
        </tr>
        <tr>
          <td width="213">Everest (India 1956)</td>
          <td width="151">6377301.243</td>
          <td width="127">300.8017</td>
        </tr>
        <tr>
          <td width="213">Everest (Malaysia 1969)</td>
          <td width="151">6377295.664</td>
          <td width="127">300.8017</td>
        </tr>
        <tr>
          <td width="213">Everest (Malay. &amp; Sing)</td>
          <td width="151">6377304.063</td>
          <td width="127">300.8017</td>
        </tr>
        <tr>
          <td width="213">Everest (Pakistan)</td>
          <td width="151">6377309.613</td>
          <td width="127">300.8017</td>
        </tr>
        <tr>
          <td width="213">Modified Fischer 1960</td>
          <td width="151">6378155</td>
          <td width="127">298.3</td>
        </tr>
        <tr>
          <td width="213">Helmert 1906</td>
          <td width="151">6378200</td>
          <td width="127">298.3</td>
        </tr>
        <tr>
          <td width="213">Hough 1960</td>
          <td width="151">6378270</td>
          <td width="127">297</td>
        </tr>
        <tr>
          <td width="213">Indonesian 1974</td>
          <td width="151">6378160</td>
          <td width="127">298.247</td>
        </tr>
        <tr>
          <td width="213">International 1924</td>
          <td width="151">6378388</td>
          <td width="127">297</td>
        </tr>
        <tr>
          <td width="213">Krassovsky 1940</td>
          <td width="151">6378245</td>
          <td width="127">298.3</td>
        </tr>
        <tr>
          <td width="213">GRS 80</td>
          <td width="151">6378137</td>
          <td width="127">298.257222101</td>
        </tr>
        <tr>
          <td width="213">South American 1969</td>
          <td width="151">6378160</td>
          <td width="127">298.25</td>
        </tr>
        <tr>
          <td width="213">Sphere</td>
          <td width="151">6366707.019</td>
          <td width="127">999999999999.9990</td>
        </tr>
        <tr>
          <td width="213">WGS 72</td>
          <td width="151">6378135</td>
          <td width="127">298.26</td>
        </tr>
        <tr>
          <td width="213">WGS 84</td>
          <td width="151">6378137</td>
          <td width="127">298.257223563</td>
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
  class UF_GEO_CONV_EXPORT Ellipsoids
  {
  public:
    //! Unique point of access for the singleton.
    static Ellipsoids* Instance()
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
      static Ellipsoids theInstance;
      pInstance_ = &theInstance;
    }
    //! Gets called if a dead reference is detected.
    static void OnDeadReference()
    {
      throw std::runtime_error("Dead Reference Error in Ellipsoids.");
    }
    //! Private constructor, disables creation of the object by clients.
    Ellipsoids()
    {
      em = new TEllipsoidMap;
      this->LoadPredefinedValues();
    };
    Ellipsoids( const Ellipsoids & ); // Disable the copy constructor.
    Ellipsoids& operator = ( const Ellipsoids & ); // Disable self-assignment.
    // Make the destructor private in order to prevent clients
    // holding a pointer from accidentally deleting it.
    virtual ~Ellipsoids(void)
    {
      pInstance_ = 0;
      destroyed_ = true;

      delete em;
    };

    //! Pointer to the instance.
    static Ellipsoids * pInstance_;
    //! True if destroyed.
    static bool destroyed_;

  public:
    // Here we put in our specific methods and variables for this class.

    //! Add a key (query identifier) and a value (the query).
    void AddKeyValue( std::string const & key, TEllipsoidParametersVector const & value );
    //! Return the selected values.
    TEllipsoidParametersVector GetEllipsoid( std::string const & key );
    //! Make the values into a vector.
    TEllipsoidParametersVector MakeVector(
      double const & SemimajorAxis, double const & InverseFlattening );
    //! Extract the values from the vector.
    void ExtractFromVector( TEllipsoidParametersVector const & v,
      double & SemimajorAxis, double & InverseFlattening );

    //! Return a vector of the keys of the available maps.
    TEllipsoidKeyVector AvailableEllipsoids();

    //! Return true if an ellipsoid exists.
    bool Exists ( std::string const & key ) const
    {
      return this->em->find(key) != this->em->end();
    }

  private:
    void LoadPredefinedValues();
  private:
    TEllipsoidMap *em;
  };

} // Namespace GeographicConversions.
} // Namespace UF.


#endif
