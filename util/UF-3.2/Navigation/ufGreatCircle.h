//
// C++ Interface: greatcircle
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NAVIGATION_GREAT_CIRCLE_H
#define NAVIGATION_GREAT_CIRCLE_H

#include <stdexcept>

#include "ufWin32Header.h"

namespace UF {
//! Classes for navigation.
namespace Navigation {

//! Great Circle calculations.
/**
Various great circle computations. Inputs are assumed to be in degrees and decimals of a degree.
East longitudes are positive and North latitudes are positive.

@author a.maclean@cas.edu.au
*/
class UF_NAV_EXPORT GreatCircle{
  public:
    //! Unique point of access for the singleton.
    static GreatCircle* Instance()
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
      static GreatCircle theInstance;
      pInstance_ = &theInstance;
    }
    //! Gets called if a dead reference is detected.
    static void OnDeadReference()
    {
      throw std::runtime_error("Dead Reference Error in GreatCircle.");
    }
    //! Private constructor, disables creation of the object by clients.
    GreatCircle();
    GreatCircle( const GreatCircle & ); // Disable the copy constructor.
    GreatCircle& operator = ( const GreatCircle & ); // Disable self-assignment.
    // Make the destructor private in order to prevent clients
    // holding a pointer from accidentally deleting it.
    virtual ~GreatCircle(void);

    //! Pointer to the instance.
    static GreatCircle * pInstance_;
    //! True if destroyed.
    static bool destroyed_;

public:
    //! Calculate the great circle distance between point 1 and point 2 in nautical miles.
    /*!
       @param lat1: latitude in degrees of point 1
       @param lon1: longitude in degrees of point 1
       @param lat2: latitude in degrees of point 2
       @param lon2: longitude in degrees of point 2
       @return The distance in nautical miles.
    */
    double Distance(double const & lat1, double const & lon1,
                    double const & lat2, double const & lon2) const;

    //! Calculate the great circle distance between point 1 and point 2 in nautical miles.
    /*!
       This computation uses the haversine formula. It may be slightly faster than Distance().

       Original reference: R. W. Sinnott, "Virtues of the Haversine," Sky and Telescope, 
vol. 68, no. 2, 1984, p. 159.
       This provides a good balance between speed and accuracy.

       @param lat1: latitude in degrees of point 1
       @param lon1: longitude in degrees of point 1
       @param lat2: latitude in degrees of point 2
       @param lon2: longitude in degrees of point 2
       @return The distance in nautical miles.
    */
    double DistanceH(double const & lat1, double const & lon1,
                    double const & lat2, double const & lon2) const;

    //! Calculate the initial course point 1 and point 2.
    /*!
       @param lat1: latitude in degrees of point 1
       @param lon1: longitude in degrees of point 1
       @param lat2: latitude in degrees of point 2
       @param lon2: longitude in degrees of point 2
       @return The course in degrees.
    */
    double Course(double const & lat1, double const & lon1,
                  double const & lat2, double const & lon2) const;

    //! Calculate the latitude of a point on the great circle defined by point 1 and point 2.
    /*!
       @param lon: Longitude of the desired point.
       @param lat1: latitude in degrees of point 1
       @param lon1: longitude in degrees of point 1
       @param lat2: latitude in degrees of point 2
       @param lon2: longitude in degrees of point 2
       @return The latitude of the point corresponding to lon on the great circle.
    */
    double LatOnGC(double const & lon,
                   double const & lat1, double const & lon1,
                   double const & lat2, double const & lon2) const;

    //! Given an initial position, a true course and a distance to a new point. Use this to calculate the position of point2.
    /*!
    @param lat1: latitude in degrees of point 1
    @param lon1: longitude in degrees of point 1
    @param crse12: course to steer from point1 to point 1 in degrees.
    @param dist12: distance from point1 to point 1 (in nautical miles)

    @param lat2: latitude in degrees of point 2
    @param lon2: longitude in degrees of point 2
    @return The course in degrees.
     */
    void Direct(double const & lat1, double const & lon1,
                double const & crse12, double const & dist12,
                double & lat2, double & lon2);

    //! Calculate the cross-track distance.
    /*!
        Suppose you are on a track defined by the waypoints P1(lat1,lon1) and P2(lat2,lon2)
        and you are at point P(lat,lon). Then this function returns the cross-track error
        which is the distance of P to the point on the great circle defined by P1->P2 that is
        abeam of P.

        A positive cross-track distance means that P is to the right of the track,
        negative means that P is to the left of the track.

        @param lat: latitude in degrees of the actual position
        @param lon: longitude in degrees of the actual position
        @param lat1: latitude in degrees of point 1
        @param lon1: longitude in degrees of point 1
        @param lat2: latitude in degrees of point 2
        @param lon2: longitude in degrees of point 2

        @return The cross-track distance in nautical miles.

    */
    double CrossTrackDistance(double const & lat, double const & lon,
                              double const & lat1, double const & lon1,
                              double const & lat2, double const & lon2) const;

    //! Calculate the distance along the track.
    /*!
        Suppose you are on a track defined by the waypoints P1(lat1,lon1) and P2(lat2,lon2)
        and you are at point P(lat,lon). Then this function returns the distance from point P1
        along the great circle P1->P2 that is abeam of P.

        @param lat: latitude in degrees of the actual position
        @param lon: longitude in degrees of the actual position
        @param lat1: latitude in degrees of point 1
        @param lon1: longitude in degrees of point 1
        @param lat2: latitude in degrees of point 2
        @param lon2: longitude in degrees of point 2

        @return The distance along the track in nautical miles.

    */
    double AlongTrackDistance(double const & lat, double const & lon,
                              double const & lat1, double const & lon1,
                              double const & lat2, double const & lon2) const;

    //! Find the distances on a great circle that are a distance d from a point P (if they exist).
    /*!
        We are finding two along track distances (if they exist) on the track P1->P2.

        Suppose you have a great circle defined by the waypoints P1(lat1,lon1) and P2(lat2,lon2)
        and a point P(lat,lon). Then, given a distance d, find distances d1, d2 (if they exist)
        along the great circle P1->P2. You can then use Direct() to calculate the latitudes
        and longitudes. The distances are measured relative to P1 along the great circle on which
        P1 and P2 lie. Hence it is possible for either d1 or d2 to be negative.

        @param lat: latitude in degrees of the actual position
        @param lon: longitude in degrees of the actual position
        @param d: distance in nautical miles.
        @param lat1: latitude in degrees of point 1
        @param lon1: longitude in degrees of point 1
        @param lat2: latitude in degrees of point 2
        @param lon2: longitude in degrees of point 2
        @param d1: distance from P1 on the great circle that is a distance d from P
        @param d2: distance from P2 on the great circle that is a distance d from P

        @return true if points exist, false otherwise.

    */
    bool ATDOnGC(double const & lat, double const & lon, double const & d,
                 double const & lat1, double const & lon1,
                 double const & lat2, double const & lon2,
                 double & d1, double & d2);

    //! Find the points on a great circle that are a distance d from a point P (if they exist).
    /*!
        Suppose you have a great circle defined by the waypoints P1(lat1,lon1) and P2(lat2,lon2)
        and a point P(lat,lon). Then, given a distance d, find points A, B (if they exist) on
        along the great circle P1->P2. A and B, if they exist, lie on the great circle
        defined by P1 and P2.

        @param lat: latitude in degrees of the actual position
        @param lon: longitude in degrees of the actual position
        @param d: distance in nautical miles.
        @param lat1: latitude in degrees of point 1
        @param lon1: longitude in degrees of point 1
        @param lat2: latitude in degrees of point 2
        @param lon2: longitude in degrees of point 2
        @param latA: latitude in degrees of point A
        @param lonA: longitude in degrees of point A
        @param latB: latitude in degrees of point B
        @param lonB: longitude in degrees of point B

        @return true if points exist, false otherwise.

    */
    bool PointsOnGC(double const & lat, double const & lon, double const & d,
                    double const & lat1, double const & lon1,
                    double const & lat2, double const & lon2,
                    double & latA, double & lonA,
                    double & latB, double & lonB);

  private:

    //! Calculate x mod y.
    template < typename T >
        T mod ( T const & x, T const & y ) const
    {
      double t = x-y*int(x/y);
      if ( t < 0 )
        t += y;
      return t;
    }


private:
    //! pi
    double pi;
    //! Degrees to radians.
    double const rtd;
    //! Radians to degrees.
    double const dtr;
    //! Earth radius in km.
    double const earthRadius;
    //! Earth radius in nautical miles.
    double const earthRadiusNmi;
    //! Machine precision.
    double eps;
    //! Limit for close points (in degrees).
    double lim;


};

} // Namespace Navigation.
} // Namespace UF.

#endif // NAVIGATION_GREAT_CIRCLE_H
