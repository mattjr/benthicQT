// main.cpp : Defines the entry point for the console application.
//

#include "ufCoordinateTransforms.h"
#include <iostream>

#include "ufSystemTime.h"
#include "ufLogger.h"

using namespace boost::numeric::ublas;
using namespace UF;
using namespace UF::Log;

//! A formatter for a matrix
template < typename T >
std::ostream & operator << (std::ostream & os, matrix<T> const & m)
{
  for (unsigned i = 0; i < m.size1 (); ++ i)
  {
     for (unsigned j = 0; j < m.size2 (); ++ j)
     {
       T e = m(i,j);
       if ( std::abs(e) < 1.0e-12 )
         e = 0;
       os << e << " ";
     }
     os << std::endl;
  }
  return os;
}


template < typename T >
  void TestRotationsScalingTranslations( ufLogger *pLog )
{
  double pi = std::atan(1.0)*4.0;
  double pi2 = pi/2.0;
  vector<T> pt(4);
  vector<T> r(3);
  matrix<T> m1(4,4);
  CoordinateConversions::CoordinateTransforms<double> hc;
  T a[3] = { 1, 2, 3 };
  hc.CtoH(a,pt);
  *pLog << " coordinate=" << pt << std::endl;
  T p1, p2, p3;
  hc.HtoC(pt,p1,p2,p3);
  *pLog << "Values: " << p1 << ", " << p2 << ", " << p3 << std::endl;
  hc.HtoC(pt,a);
  *pLog << "Values: " << a[0] << ", " << a[1] << ", " << a[2] << std::endl;
  hc.CtoH(1,1,1,pt);
  *pLog << " coordinate =" << pt << std::endl;
  hc.Rx(pi2,m1,false);
  *pLog << "Rx(" << pi2 << ")=" << m1 << std::endl;
  *pLog << prod(m1,pt) << std::endl;
  hc.Ry(pi/2.0,m1,false);
  *pLog << "Ry(" << pi2 << ")=" << m1 << std::endl;
  *pLog << prod(m1,pt) << std::endl;
  hc.Rz(pi/2.0,m1,false);
  *pLog << "Rz(" << pi2 << ")=" << m1 << std::endl;
  *pLog << prod(m1,pt) << std::endl;
  // In degrees.
  *pLog << " coordinate =" << pt << std::endl;
  hc.Rx(90,m1);
  *pLog << "Rx(90 degrees)=" << m1 << std::endl;
  *pLog << prod(m1,pt) << std::endl;
  hc.Ry(90,m1);
  *pLog << "Ry(90 degrees)=" << m1 << std::endl;
  *pLog << prod(m1,pt) << std::endl;
  hc.Rz(90,m1);
  *pLog << "Rz(90 degrees)=" << m1 << std::endl;
  *pLog << prod(m1,pt) << std::endl;
  hc.HtoC(prod(m1,pt),r);
  *pLog << "Convert to cartesian: " << r << std::endl;
  hc.Ts(0.1,0.2,0.3,m1);
  *pLog << "Ts(0.1,0.2,0.3)=" << m1 << std::endl;
  *pLog << prod(m1,pt) << std::endl;
  hc.Ss(0.1,0.2,0.3,m1);
  *pLog << "Ss(0.1,0.2,0.3)=" << m1 << std::endl;
  *pLog << prod(m1,pt) << std::endl;

  // Cross products.
  vector<T> u(3);
  vector<T> v(3);
  vector<T> w(3);
  for (int i = 0; i < 3; ++i)
    u(i) = v(i) = w(i) = 0;
  u(0) = v(1) = w(2) = 1;
  hc.Cross(u,v,r);
  if(r(0) != w(0) && r(1) != w(1) && r(2) != w(2))
  {
    *pLog << "Fail: uXv=r!=w" << std::endl;
    *pLog << "w: " << w << std::endl;
    *pLog << "r: " << r << std::endl;
  }
  hc.Cross(v,w,r);
  if(r(0) != u(0) && r(1) != u(1) && r(2) != u(2))
  {
    *pLog << "Fail: vXw=r!=u" << std::endl;
    *pLog << "u: " << u << std::endl;
    *pLog << "r: " << r << std::endl;
  }
  hc.Cross(w,u,r);
  if(r(0) != v(0) && r(1) != v(1) && r(2) != v(2))
  {
    *pLog << "Fail: wXu=r!=v" << std::endl;
    *pLog << "v: " << v << std::endl;
    *pLog << "r: " << r << std::endl;
  }

  // Arbitrary rotation.
  matrix<T> m(4,4);
  hc.Ra(u,90,m);
  *pLog << "Ra(u,90 degrees) = " << m << std::endl;
  hc.Rx(90,m1);
  *pLog << "Rx(90 degrees)=    " << m1 << std::endl;
  hc.Ra(v,90,m);
  *pLog << "Ra(v,90 degrees) = " << m << std::endl;
  hc.Ry(90,m1);
  *pLog << "Ry(90 degrees)=    " << m1 << std::endl;
  hc.Ra(w,90,m);
  *pLog << "Ra(w,90 degrees) = " << m << std::endl;
  hc.Rz(90,m1);
  *pLog << "Rz(90 degrees)=    " << m1 << std::endl;

  // Rotation matrix.
  hc.Rst(u,v,m);
  *pLog << "Rotation from x-axis to y-axis = " << m << std::endl;
  subrange(pt,0,3) = u;
  *pLog << prod(m,pt) << std::endl;
  hc.Rz(90,m1);
  *pLog << "Rz(90 degrees)=    " << m1 << std::endl;
  *pLog << prod(m1,pt) << std::endl;

  vector<T> ii(4);
  for(int i = 0; i < 4; ++i)
  {
    ii(i) = 1;
  }
  hc.Rst(u,ii,m);
  *pLog << "Rotation from x-axis to (1,1,1) = " << m << std::endl;
  *pLog << prod(m,pt) << std::endl;

  pt(0) = 1;
  pt(1) = 2;
  pt(2) = 3;
  vector<T> pt1(4);
  hc.Ra(ii,25,m);
  pt1 = prod(m,pt);
  *pLog << "Rotation of 25 degrees about the axis (1,1,1)" << m << std::endl;
  *pLog << " pt:" << pt << std::endl;
  *pLog << "pt1:" << pt1 << std::endl;
  hc.Normalise(pt);
  hc.Normalise(pt1);
  double angle = hc.Dot(pt,pt1);
  // Sometimes we get rounding errors.
  if(angle > 1)
    angle = 1;
  if(angle < -1)
    angle = -1;
  angle = std::acos(angle);
  angle = angle * 45.0/std::atan((double)1.0);
  *pLog << "Angle is: " << angle << " degrees." << std::endl;

}

template < typename T >
  void TestPerspective( ufLogger *pLog )
{
  vector<T> pt(4);
  vector<T> r(3);
  matrix<T> m(4,4);
  CoordinateConversions::CoordinateTransforms<double> hc;

  *pLog << "Perspective Projection" << std::endl;
  double d = 0.5;
  *pLog << "d = " << d << " x, y, z-axis, centre of projection is the origin." << std::endl;
  for ( int i = 0; i < 3; ++i )
  {
    hc.PerspectiveProjection(d,m,i);
    *pLog << m << std:: endl;
  }
  *pLog << "d = " << d << " x, y, z-axis, centre of projection is "
    << -d << " along the axis of projection." << std::endl;
  for ( int i = 0; i < 3; ++i )
  {
    hc.PerspectiveProjection(d,m,i,false);
    *pLog << m << std:: endl;
  }

  *pLog << "Using a frustrum (-1,1,-1,1,1,-1)" << std::endl;
  hc.PerspectiveProjection(-1,1,-1,1,1,-1,m);
  *pLog << m << std::endl;

}

template < typename T >
  void TestOrthographic( ufLogger *pLog )
{
  vector<T> pt(4);
  vector<T> r(3);
  matrix<T> m(4,4);
  CoordinateConversions::CoordinateTransforms<double> hc;

  *pLog << "Orthographic Projection." << std::endl;
  *pLog << "Using a frustrum (-1,1,-1,1,1,-1)" << std::endl;
  hc.OrthographicProjection(-1,1,-1,1,1,-1,m);
  *pLog << m << std::endl;

}

template < typename T >
  void TestShear( ufLogger *pLog )
{
  matrix<T> m(4,4);
  CoordinateConversions::CoordinateTransforms<double> hc;

  *pLog << "Shear." << std::endl;
  *pLog << "Shear x: (.2,0.3,0,0,0,0)" << std::endl;
  hc.Sh(.2,0.3,0,0,0,0,m);
  *pLog << m << std::endl;
  *pLog << "Shear y: (0,0,.2,0.3,0,0)" << std::endl;
  hc.Sh(0,0,.2,0.3,0,0,m);
  *pLog << m << std::endl;
  *pLog << "Shear z: (0,0,0,0,.2,0.3)" << std::endl;
  hc.Sh(0,0,0,0,.2,0.3,m);
  *pLog << m << std::endl;

}

template < typename T >
  void TestSphericalRectangular( ufLogger *pLog )
{
  double x = 1;
  double y = 1;
  double z = 1;
  double r = 0;
  double theta = 0;
  double phi = 0;

  CoordinateConversions::CoordinateTransforms<double> hc;

  *pLog << "Spherical/Rectangular Conversions." << std::endl;
  hc.RectangularToSpherical(x,y,z,r,theta,phi);
  *pLog << "(x,y,z):(" << x << "," << y << "," << z << ") -> " << "(r,theta,phi):(" << r << "," << theta << "," << phi << ")" << std::endl;  hc.SphericalToRectangular(r,theta,phi,x,y,z);
  *pLog << "(r,theta,phi):(" << r << "," << theta << "," << phi << ") -> " << "(x,y,z):(" << x << "," << y << "," << z << ")" << std::endl;

}

int main(void)
{
  std::string sep = "------------------------------------------------------------------------";
  // Set up the log file.
  ufLogger *pLog = new ufLogger;


  pLog->open("EgCoordinateTransforms.log",std::ios::out);
  std::string time;
  Time::SystemTime *pClock = Time::SystemTime::Instance();
  pClock->ISOLocalTime(time);
  *pLog << sep << "\nStarted:  " << time << std::endl;

  bool fail = false;
  *pLog << "Testing Coordinate Transforms" << std::endl;

  TestRotationsScalingTranslations<double>(pLog);
  TestPerspective<double>(pLog);
  TestOrthographic<double>(pLog);
  TestShear<double>(pLog);
  TestSphericalRectangular<double>(pLog);

  pClock->ISOLocalTime(time);
  *pLog << ((fail)?"Failed.":"Passed") << "\n"
      << "Finished: " << time << "\n" << sep << std::endl;

  delete pLog;

  if ( fail )
    return 1;

  return 0;
}

