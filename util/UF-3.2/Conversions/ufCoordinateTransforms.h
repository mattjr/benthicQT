#if !defined(COORDINATE_TRANSFORMS_H)
#define COORDINATE_TRANSFORMS_H

#include "ufWin32Header.h"

// we need the matrix bits
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/matrix_proxy.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/vector_proxy.hpp"
#include "boost/numeric/ublas/io.hpp"
#include "boost/mpl/vector/vector20.hpp"

#include <cmath>

namespace UF {
//! Perform coordinate conversions from one coordinate system to another.
namespace CoordinateConversions {

//! Sets up matrices for use in homogenous coordinate conversions.
/*!
    This class provides a series of functions that set up matrices
    for use in homogenous coordinate conversions.

    A lot of this work is based on:

     Tomas Akenine-M�ller, Eric Haines,
     "Real-Time Rendering"
     2nd Edn, A.K.Peters, 2002

    Note: All vectors are assumed to be either three-dimensional or homogenous.
          In other words, this class is specialised for vectors whose dimension is
          either 3 or 4.

*/
template < typename T >
class /*UF_TRANSFROMS_EXPORT*/ CoordinateTransforms
{
public:
  CoordinateTransforms():
      rtd(45.0/std::atan((double)1.0))
      , dtr(std::atan((double)1.0)/45.0)
      , eps(1.0e-12)
      , epsf(1.0e-6)
      {}

      ~CoordinateTransforms(){}

public:
    //! Assignment operator.
    CoordinateTransforms& operator = (CoordinateTransforms
 const & that)
    {
      // Handle self-assignment.
      if ( this == &that )
        return *this;

      return *this;

    }

public:

  //! Rotation about the x-axis through an angle a.
  /*! Return the rotation matrix for rotating about the x-axis through an angle a.

      @param angle - the rotation angle.
      @param m - the rotation matrix.
      @param deg - true if the angle is in degrees, false if the angle is in radians.
      @param rhcf - If this is true a right handed coordinate frame is used, if false then a left handed coordinate frame is used.

   */
  void Rx ( T const & angle, boost::numeric::ublas::matrix<T> & m, bool const & deg = true, bool const & rhcf = true )
  {
    boost::numeric::ublas::identity_matrix<T> i(4);
    m = i;
    T a = ((rhcf)?angle:-angle)*((deg)?this->dtr:1);
    T ca = std::cos(a);
    T sa = std::sin(a);
    m(1,1) =  ca;
    m(1,2) = -sa;
    m(2,1) =  sa;
    m(2,2) =  ca;
  }

  //! Rotation about the y-axis through an angle a.
  /*! Return the rotation matrix for rotating about the y-axis through an angle a.

      @param angle - the rotation angle.
      @param m - the rotation matrix.
      @param deg - true if the angle is in degrees, false if the angle is in radians.
      @param rhcf - If this is true a right handed coordinate frame is used, if false then a left handed coordinate frame is used.

   */
  void Ry ( T const & angle, boost::numeric::ublas::matrix<T> & m, bool const & deg = true, bool const & rhcf = true )
  {
    boost::numeric::ublas::identity_matrix<T> i(4);
    m = i;
    T a = ((rhcf)?angle:-angle)*((deg)?this->dtr:1);
    T ca = std::cos(a);
    T sa = std::sin(a);
    m(0,0) =  ca;
    m(0,2) =  sa;
    m(2,0) = -sa;
    m(2,2) =  ca;
  }

  //! Rotation about the z-axis through an angle a.
  /*! Return the rotation matrix for rotating about the y-axis through an angle a.

      @param angle - the rotation angle.
      @param m - the rotation matrix.
      @param deg - true if the angle is in degrees, false if the angle is in radians.
      @param rhcf - If this is true a right handed coordinate frame is used, if false then a left handed coordinate frame is used.

   */
  void Rz ( T const & angle, boost::numeric::ublas::matrix<T> & m, bool const & deg = true, bool const & rhcf = true )
  {
    boost::numeric::ublas::identity_matrix<T> i(4);
    m = i;
    T a = ((rhcf)?angle:-angle)*((deg)?this->dtr:1);
    T ca = std::cos(a);
    T sa = std::sin(a);
    m(0,0) =  ca;
    m(0,1) = -sa;
    m(1,0) =  sa;
    m(1,1) =  ca;
  }

  //! Rotation about an arbitary normalised axis.
  /*! Used to rotate a vector s about axis a through an angle to a new vector r.

    Real Time Rendering, M�ller, Hains. Edn2, p 43.
    taken from Goldman, Ronald, "matrices and transforms" in Andrew S.
    Glassner ed. Graphics Gems, Academic Press, pp. 472-475, 1990.

    The deduction of rotation about an arbitrary vector is rather
    simple. Let '*' denote the dot product and 'x' denote the cross
    product. We let v be a unit vector in R^3, and p be an arbitrary
    point. Using the identity

    p = (v*p)v + (v x p) x v

    We let (v x p) x v be the "x-axis" of the rotation, and (v x p), which
    is in the plane of rotation and orthogonal to the "x-axis". This
    yields

    p' = (v*p)v + cos(theta)(v x p) x v + sin(theta)(v x p).

    This formula is a linear combination of three orthogonal vectors. All
    terms are linear in p, and thus, from this expression we can get the
    coefficients of the corresponding transformation matrix.

    @param axis - the axis around which the vector is to be rotated. This is normalised before use.
    @param angle - the angle to rotate s - this is in degrees.
    @param m - the rotation matrix.
    @param deg - true if the angle is in degrees, false if the angle is in radians.
    @param rhcf - If this is true a right handed coordinate frame is used, if false then a left handed coordinate frame is used.

  */
  void Ra (boost::numeric::ublas::vector<T> const & axis, T const & angle,
           boost::numeric::ublas::matrix<T> & m,
           bool const & deg = true, bool const rhcf = true)
  {
    T u = angle * ((deg)?this->dtr:1);
    T cu = std::cos(u);
    T cu1 = 1.0 - cu;
    T su = std::sin(u);

    boost::numeric::ublas::identity_matrix<T> i(4);
    m = i;

    boost::numeric::ublas::vector<T> a(3);
    a = boost::numeric::ublas::subrange(axis,0,3);
    this->Normalise(a);

    int rh = rhcf?1:-1; // Handedness of the transform.
    m(0,0) = cu+cu1*a(0)*a(0);
    m(0,1) = cu1*a(0)*a(1)-rh*a(2)*su;
    m(0,2) = cu1*a(0)*a(2)+rh*a(1)*su;
    m(1,0) = cu1*a(0)*a(1)+rh*a(2)*su;
    m(1,1) = cu+cu1*a(1)*a(1);
    m(1,2) = cu1*a(1)*a(2)-rh*a(0)*su;
    m(2,0) = cu1*a(0)*a(2)-rh*a(1)*su;
    m(2,1) = cu1*a(1)*a(2)+rh*a(0)*su;
    m(2,2) = cu+cu1*a(2)*a(2);
  }

  //! Translation along the x,y,z axes by the amount dx, dy, dz.
  /*!
      @param dx - translation along the x-axis.
      @param dy - translation along the y-axis.
      @param dz - translation along the z-axis.
      @param m - the translation matrix.
  */
  void Ts ( T const & dx, T const & dy, T const & dz, boost::numeric::ublas::matrix<T> & m )
  {
    boost::numeric::ublas::identity_matrix<T> i(4);
    m = i;
    m(0,3) = dx;
    m(1,3) = dy;
    m(2,3) = dz;
  }

  //! Translation along the x,y,z axes by the amount dx, dy, dz.
  /*!
      @param t - an array containing dx, dy, dz.
      @param m - the translation matrix.
  */
  void Ts ( T t[3], boost::numeric::ublas::matrix<T> & m )
  {
    boost::numeric::ublas::identity_matrix<T> i(4);
    m = i;
    m(0,3) = t[0];
    m(1,3) = t[1];
    m(2,3) = t[2];
  }

  //! General shearing along the x, y, or z, axes.
  /*!
     For shearing along the x-axis in 3D, y and z values are proportional to
     the x values in the ratios s_xy and s_xz.
     For shearing along the y-axis in 3D, x and z values are proportional to
     the x values in the ratios s_yx and s_yz.
     For shearing along the z-axis in 3D, y and z values are proportional to
     the x values in the ratios s_zx and s_zy.

     Set the unused shear values to zero.

      @param s_xy - the y-component of the shear for shearing along the x-axis.
      @param s_xz - the z-component of the shear for shearing along the x-axis.
      @param s_yx - the x-component of the shear for shearing along the y-axis.
      @param s_yz - the z-component of the shear for shearing along the y-axis.
      @param s_zx - the x-component of the shear for shearing along the z-axis.
      @param s_zy - the y-component of the shear for shearing along the z-axis.
      @param m - the translation matrix.
  */
  void Sh ( T const & s_xy, T const & s_xz,
            T const & s_yx, T const & s_yz,
            T const & s_zx, T const & s_zy,
            boost::numeric::ublas::matrix<T> & m )
  {
    boost::numeric::ublas::identity_matrix<T> i(4);
    m = i;
    m(0,1) = s_xy;
    m(0,2) = s_xz;
    m(1,0) = s_yx;
    m(1,2) = s_yz;
    m(2,0) = s_zx;
    m(2,1) = s_zy;
  }

  //! General shearing along the x, y, or z, axes.
  /*!
     For shearing along the x-axis in 3D, y and z values are proportional to
     the x values in the ratios s_xy and s_xz.
     For shearing along the y-axis in 3D, x and z values are proportional to
     the x values in the ratios s_yx and s_yz.
     For shearing along the z-axis in 3D, y and z values are proportional to
     the x values in the ratios s_zx and s_zy.

     Set the unused shear values to zero.

      @param t an array containing s_xy, s_xz, s_yx, s_yz, s_zx, s_zy.
      @param m - the translation matrix.
  */
  void Sh ( T s[6], boost::numeric::ublas::matrix<T> & m )
  {
    boost::numeric::ublas::identity_matrix<T> i(4);
    m = i;
    m(0,1) = s[0];
    m(0,2) = s[1];
    m(1,0) = s[2];
    m(1,2) = s[3];
    m(2,0) = s[4];
    m(2,1) = s[5];
  }

  //! Scaling along the x,y,z axes by the amount sx, sy, sz.
  /*!
     If one of the scale factors is is of a different sign to the other two,
     it will change the handedness of the coordinate system.

      @param sx - the x-component of the scaling.
      @param sy - the y-component of the scaling.
      @param sz - the z-component of the scaling.
      @param m - the translation matrix.
  */
  void Ss ( T const & sx, T const & sy, T const & sz, boost::numeric::ublas::matrix<T> & m )
  {
    boost::numeric::ublas::identity_matrix<T> i(4);
    m = i;
    m(0,0) = sx;
    m(1,1) = sy;
    m(2,2) = sz;
  }

  //! Scaling along the x,y,z axes by the amount sx, sy, sz.
  /*!
     If one of the scale factors is is of a different sign to the other two,
     it will change the handedness of the coordinate system.

      @param t - an array containing the x-, y-, z- components of the scaling.
      @param m - the translation matrix.
 */
  void Ss ( T s[3], boost::numeric::ublas::matrix<T> & m )
  {
    boost::numeric::ublas::identity_matrix<T> i(4);
    m = i;
    m(0,0) = s[0];
    m(1,1) = s[1];
    m(2,2) = s[2];
  }

  //! Perspective projection of (x,y,z) onto the projection plane at z=+/-d, d != 0.
  /*!
      If z = d, then the center of projection is at the origin.

      If z = -d, then the center of projection is at (0,0,-d).

      @param d - the distance from the centre of projection
      @param m - the translation matrix.
      @param direction - the axis along which the projection occurs (0: x, 1: y, 2: z).
      @param origin - if true then the centre of the projection is the origin and we project onto
                      a plane a distance d from the origin, if false then the centre of projection
                      is at a distance -d along an axis determined by the parameter direction and
                      we project onto a plane at the origin.
  */
  void PerspectiveProjection ( T const & d, boost::numeric::ublas::matrix<T> & m, int const & direction = 2, bool const & origin = true )
  {
    boost::numeric::ublas::identity_matrix<T> i(4);
    m = i;
    if ( d == 0 )
      return;
    if ( origin )
    {
      switch (std::abs(direction) % 3)
      {
      case 0:
        m(3,0) = 1.0 / d;
        break;
      case 1:
        m(3,1) = 1.0 / d;
        break;
      case 2:
        m(3,2) = 1.0 / d;
       break;
      }
      m(3,3) = 0;
    }
    else
    {
      switch (std::abs(direction) % 3)
      {
      case 0:
        m(0,0) = 0;
        m(3,0) = 1.0 / d;
        break;
      case 1:
        m(1,1) = 0;
        m(3,1) = 1.0 / d;
        break;
      case 2:
        m(2,2) = 0;
        m(3,2) = 1.0 / d;
       break;
      }
      m(3,3) = 0;
    }
  }

  //! Perspective projection using a frustum.
  /*!
       This projection uses a 6-tuple(l,r,b,t,n,f), which denotes the position of the
       left, right, bottom, top, near and far planes. This determines the view frustum
       of the camera where the minimum corner is (l,b,n) and the maximum corner is (r,t,f).
       The hoizontal field of view is determined by the angle between the left and right
       planes of the frustum. The vertical field of view is determined by the angle between
       the top and bottom planes (t and b).
       Note that n > f because we are looking in the negative direction along the z-axis.
       This produces a mirrored matrix (i.e. a left-handed coordinate system) because we
       have n > f.

       This transform only works iff n != f, l != r and t != b.

       Asymetric frustums can be created where r != -l and t != -b.

       Remember to homogenise the resultant vector by dividing by the fourth element.
       The viewpoint is looking along the z- axis in the negative direction.

      @param l - the position of the left plane
      @param r - the position of the right plane
      @param b - the position of the bottom plane
      @param t - the position of the top plane
      @param n - the position of the near plane
      @param f - the position of the far plane
      @param m - the translation matrix.
  */
  void PerspectiveProjection ( T const & l, T const & r, T const & b,
                               T const & t, T const & n, T const & f,
                               boost::numeric::ublas::matrix<T> & m )
  {
    boost::numeric::ublas::zero_matrix<T> i(4);
    m = i;
    if ((r == l) || (t == b) || (f == n))
    {
      return;
    }

    m(0,0) = 2*n/(r-l); m(0,3) = -((r+l)/(r-l));
    m(1,1) = 2*n/(t-b); m(1,3) = -((t+b)/(t-b));
    m(2,2) = (f+n)/(f-n); m(2,3) = -(2*f*n/(f-n));
    m(3,2) = 1;
}

  //! Orthographic projection of (x,y,z).
  /*!
      If we project along an axis say z-, we can use Ss(1,1,0). Similarly
      for the other axes we just set the scale for that axis along which
      we are projecting to zero and the others to 1.

      An alternative way is to use a 6-tuple(l,r,b,t,n,f), which denotes the
      position of the left, right, bottom, top, near and far planes.
      So we have an axis-aligned bounding box (AABB) where the minimum corner
      is (l,b,n) and the maximum corner is (r,t,f).

      Note that n > f because we are looking in the negative direction along
      the x-axis. This produces a mirrored matrix (i.e. a left-handed
      coordinate system) because we have n > f.

      This transform only works iff n != f, l != r and t != b.

      @param l - the position of the left plane
      @param r - the position of the right plane
      @param b - the position of the bottom plane
      @param t - the position of the top plane
      @param n - the position of the near plane
      @param f - the position of the far plane
      @param m - the translation matrix.
  */
  void OrthographicProjection ( T const & l, T const & r, T const & b,
                               T const & t, T const & n, T const & f,
                               boost::numeric::ublas::matrix<T> & m )
  {
    if ((r == l) || (t == b) || (f == n))
    {
      return;
    }

    boost::numeric::ublas::matrix<T> ss(4,4);
    boost::numeric::ublas::matrix<T> tt(4,4);

    this->Ss(2/(r-l),2/(t-b),2/(f-n),ss);
    this->Ts(-(l+r)/2,-(t+b)/2,-(f+n)/2,tt);
    m = boost::numeric::ublas::prod(ss,tt);
  }

  //! General projection of (x,y,z).
  /*!
      The center of projection is a distance Q from the point (0, 0, zp),
      the direction to the center of projection is given by the normalised
      direction vector (dx, dy, dz)
      Here is a table of projections:
                      zp           Q      [dx, dy, dz]
      Orthographic    0     infinity      [0, 0, -1]
      Perspective     d            d      [0, 0, -1]
      Perspective     0            d      [0,  0, -1]
      For Cavalier and Cabinet projections onto the (x,y) plane with angle alpha:
                     zp            Q      [dx, dy, dz]
      Cavalier       0      infinity      [cos(alpha), sin(alpha), -1]
      Cabinet        0      infinity      [cos(alpha)/2, sin(alpha)/2, -1]

     Ref: J.D. Foley, A. van Dam, S.K. Feiner, J.F. Hughes,
     "Computer Graphics, Principles and Practice, Second edition in C", Addison-Wesley, 1993, p258

  */
  void GeneralProjection ( T const & zp, T const & Q,
    T const & dx, T const & dy, T const & dz,
    boost::numeric::ublas::matrix<T> & m )
  {
    boost::numeric::ublas::identity_matrix<T> i(4);
    m = i;
    m(0,2) =  -dx/dz;
    m(0,3) =  zp*dx/dz;
    m(1,2) =  -dy/dz;
    m(1,3) =  zp*dy/dz;
    if ( Q == 0 )
      {
      m(2,2) = 0;
      m(2,3) = zp;
      m(3,2) = 0;
      m(3,3) = 1;
      }
    else
      {
      m(2,2) = -zp/(Q*dz);
      m(2,3) = zp*zp/(Q*dz)+zp;
      m(3,2) = -1/(Q*dz);
      m(3,3) = zp/(Q*dz)+1;
     }
  }

  //! Rotate from s to t and return the corresponding rotation matrix.
  /*! Real Time Rendering, M�ller, Hains. Edn2, p 52.

    Be careful!

    If s & t are parallel or nearly parallel we should return the identity matrix I.
    If e = cos(2*phi) = 1 -> 2*phi = pi, then we can rotate pi radians around any
    axis. In this case we can find an axis as the cross product of s and any other
    vector that is not parallel to s.

    @param s - the vector to be rotated. This is normalised before use.
    @param t - the vector that s is being rotated to. This is normalised before use.
    @param m - the rotation matrix.

 */
  void Rst (boost::numeric::ublas::vector<T> const & s,
            boost::numeric::ublas::vector<T> const & t,
            boost::numeric::ublas::matrix<T> & m )
  {

    // Check for parallelism.
    T precision;
    // Only works if T is float, double or long double.
    if (sizeof(T) == sizeof(double))
      precision = eps;
    else
      precision = T(epsf);

    boost::numeric::ublas::vector<T> v(3);
    boost::numeric::ublas::vector<T> v1(3);
    boost::numeric::ublas::vector<T> v2(3);
    v1 = boost::numeric::ublas::subrange(s,0,3);
    v2 = boost::numeric::ublas::subrange(t,0,3);
    this->Normalise(v1);
    this->Normalise(v2);

    boost::numeric::ublas::identity_matrix<T> i(4);
    m = i;

    this->Cross(v1,v2,v);

    // Check to see if s and t are parallel.
    if ( boost::numeric::ublas::norm_2(v) < precision )
      return;

    T e = boost::numeric::ublas::inner_prod(v1,v2);
    // e is cos(2*phi)
    // If phi is near zero then we return the identity matrix.
    if (std::abs(std::abs(e)-1) < precision)
      return;

    // If 2*phi is near Pi then we can rotate Pi radians around any axis.
    // In this case we should form the cross product of s and any vector not parallel to
    // it and use it for v.
    if (std::abs(e) < precision)
    {
      // Select a vector to replace t.
      boost::numeric::ublas::vector<T> tt(3);
      for(int i = 0; i < 3; ++i)
      {
        tt(i) = 1;
      }
      // Form a vector.
      if ( s(0) < s(1) && s(0) < s(2) )
      {
        tt(1) = -s(2);
        tt(2) = s(1);
      }
      else {
        if ( s(1) < s(0) && s(1) < s(2) )
        {
          tt(0) = -s(2);
          tt(2) = s(0);
        }
        else
        {
          if ( s(2) < s(0) && s(2) < s(1) )
          {
            tt(0) = -s(1);
            tt(1) = s(0);
          }
          else
          {
            for(int i = 0; i < 3; ++i)
            {
              tt(i) = t(i)-s(i);
            }
          }
        }
      }
      this->Normalise(tt);
      this->Cross(v1,tt,v);
    }

    T h = (1.0 - e) / boost::numeric::ublas::inner_prod(v,v);;

    // Build the rotation matrix.
    m(0,0) = e+h*v(0)*v(0);
    m(0,1) = h*v(0)*v(1)-v(2);
    m(0,2) = h*v(0)*v(2)+v(1);
    //m(0,3) = 0;
    m(1,0) = h*v(0)*v(1)+v(2);
    m(1,1) = e+h*v(1)*v(1);
    m(1,2) = h*v(1)*v(2)-v(0);
    //m(1,3) = 0;
    m(2,0) = h*v(0)*v(2)-v(1);
    m(2,1) = h*v(1)*v(2)+v(0);
    m(2,2) = e+h*v(2)*v(2);
    //m(2,3) = 0;
    //m(3,0) = 0;
    //m(3,1) = 0;
    //m(3,2) = 0;
    //m(3,3) = 1;

  }

  //! Cartesian to Homogenous coordinate conversion.
  /*!
      @param x - x-coordinate.
      @param y - y-coordinate.
      @param z - z-coordinate.
      @param v - the homogenous vector.
  */
  void CtoH ( T const & x, T const & y, T const & z, boost::numeric::ublas::vector<T> & v )
  {
    v(0) = x;
    v(1) = y;
    v(2) = z;
    v(3) = 1;
  }

  //! Cartesian to Homogenous coordinate conversion.
  /*!
      This function takes the first three elements of a vector and creates
      a homogenous vector by adding a fourth element whose value is one.

      @param u - a vector, whose first three elements represent x, y, z.
      @param v - the homogenous vector.
  */
  void CtoH ( boost::numeric::ublas::vector<T> const & u, boost::numeric::ublas::vector<T> & v )
  {
    boost::numeric::ublas::subrange(v,0,3) = v;
    v(3) = 1;
  }

  //! Cartesian to Homogenous coordinate conversion.
  /*!
      This function takes the values from a three element array and creates
      a homogenous vector by adding a fourth element whose value is one.

      @param x - an array of three elements representing x, y, x.
      @param v - the homogenous vector.
  */
  void CtoH ( T x[3], boost::numeric::ublas::vector<T> & v )
  {
    v(0) = x[0];
    v(1) = x[1];
    v(2) = x[2];
    v(3) = 1;
  }

  //! Homogenous to Cartesian coordinate conversion.
  void HtoC ( boost::numeric::ublas::vector<T> const & v, T & x, T & y, T & z )
  /*!
      @param v - the homogenous vector.
      @param x - x-coordinate.
      @param y - y-coordinate.
      @param z - z-coordinate.
  */
  {
    x = v(0);
    y = v(1);
    z = v(2);
  }

  //! Homogenous to Cartesian coordinate conversion.
  /*!
      This function takes the first three elements of a homogenous vector
      and creates a three-element vector.

      @param v - the homogenous vector.
      @param u - a vector, whose first three elements represent x, y, z.
  */
  void HtoC ( boost::numeric::ublas::vector<T> const & u, boost::numeric::ublas::vector<T> & v )
  {
    v = boost::numeric::ublas::subrange(u,0,3);
  }

  //! Homogenous to Cartesian coordinate conversion.
  /*!
      This function takes the first three values from a homogenous
      vector and returns a three-element array.

      @param v - the homogenous vector.
      @param x - an array of three elements representing x, y, x.
  */
  void HtoC ( boost::numeric::ublas::vector<T> const & v, T x[3] )
  {
    x[0] = v(0);
    x[1] = v(1);
    x[2] = v(2);
  }

  //! Calculate the cross-product.
  /*!
      The cross product is applicable to vectors of dimension three only.
      If the vectors are of dimension four, the fourth element is ignored.

      @param u - the first vector
      @param v - the second vector
      @param w - uXv

  */
  void Cross ( boost::numeric::ublas::vector<T> const & u,
               boost::numeric::ublas::vector<T> const & v,
               boost::numeric::ublas::vector<T> & w
             )
  {
    if ( u.size() == 0 || v.size() == 0 )
      return;
    if ( u.size() > 4 || u.size() < 3 || v.size() > 4 || v.size() < 3 )
      return;
    boost::numeric::ublas::vector<T> v1(3);
    boost::numeric::ublas::vector<T> v2(3);
    v1 = boost::numeric::ublas::subrange(u,0,3);
    v2 = boost::numeric::ublas::subrange(v,0,3);

    w(0) = v1(1) * v2(2) - v1(2) * v2(1);
    w(1) = v1(2) * v2(0) - v1(0) * v2(2);
    w(2) = v1(0) * v2(1) - v1(1) * v2(0);
    if ( w.size() == 3 )
      return;
    // Make it homogenous.
    w(3) = 1;

  }

  //! Normalise a vector.
  /*!
      If the vector is homogenous the first three elements of the vector
      are normalised, the fourth element is untouched.

      @param v - the vector.
*/
  void Normalise (boost::numeric::ublas::vector<T> & v)
  {
    if ( v.size() != 4 )
    {
      T n = boost::numeric::ublas::norm_2(v);
      if ( n != 0 )
      {
        n = 1.0 / n;
        v *= n;
      }
    }
    else
    {
      boost::numeric::ublas::vector<T> v1(3);
      v1 = boost::numeric::ublas::subrange(v,0,3);
      T n = boost::numeric::ublas::norm_2(v1);
      if ( n != 0 )
      {
        n = 1.0 / n;
        v *= n;
      }
      boost::numeric::ublas::subrange(v,0,3) = v1;
    }
  }

  //! Homogenise a vector.
  /*!
      Scale the elements such that the last element is one.

      @param v - the vector.
  */
  void Homogenise (boost::numeric::ublas::vector<T> & v)
  {
    if ( !v.empty() )
    {
      T n = v.last();
      if ( v.last() != 0 )
      {
        n = 1.0 / n;
        v *= n;
      }
    }
  }

  //!Dot product of two vectors.
  /*!
      If the vectors are of dimension four, the fourth element is ignored.
      In this case the dot product of the first three elements of the vectors
      are used to calculate the dot product.

      @param u - the first vector
      @param v - the second vector
      @return - u.v
  */
  T Dot ( boost::numeric::ublas::vector<T> const & u,
               boost::numeric::ublas::vector<T> const & v
             )
  {
    if ( u.size() != 4 && u.size() == v.size())
    {
      // Normal dot product.
      return boost::numeric::ublas::inner_prod(u,v);
    }
    else
    {
      if ( (u.size() == 3 || u.size() == 4) && (v.size() == 3 || v.size() == 4) )
      {
        // Dot product where one or both vectors are homogenous.
        boost::numeric::ublas::vector<T> v1(3);
        boost::numeric::ublas::vector<T> v2(3);
        boost::numeric::ublas::vector<T> v3(3);
        v1 = boost::numeric::ublas::subrange(u,0,3);
        v2 = boost::numeric::ublas::subrange(v,0,3);
        return boost::numeric::ublas::inner_prod(v1,v2);
      }
    }
    // Cannot calculate it.
    return -1;
  }

  //! Convert a bearing or a heading to a rotation angle.
  /*!
      Takes a bearing measured clockwise from N through E and converts it to an
      anticlockwise angle in a frame of reference where the x-axis corresponds to E
      and the y-axis corresponds to N.

      @param bearing - the bearing
      @param deg - true if the angle is in degrees, false if the angle is in radians.

      @return The angle corresponding to the bearing.
  */
  T BearingToRotation ( T const & bearing, bool const & deg = true )
  {
    double a = 450.0;
    a -= (deg)?bearing:bearing*this->rtd;
    while ( a >= 360 )
      a -= 360;
    while ( a < 0 )
      a += 360;
    return T((deg)?a:a*this->dtr);
  }

  //! Convert a rotation angle to a bearing or heading.
  /*!
      Takes an angle measured anticlockwise in the x-y plane to a
      bearing measured clockwise from N through E.

      Where the x-axis corresponds to E
      and the y-axis corresponds to N.

      @param angle - the angle
      @param deg - true if the angle is in degrees, false if the angle is in radians.

      @return The bearing corresponding to the angle.
  */
  T RotationToBearing ( T const & angle, bool const & deg = true )
  {
    double a = 450.0;
    a -= (deg)?angle:angle*this->rtd;
    while ( a >= 360 )
      a -= 360;
    while ( a < 0 )
      a += 360;
    return T((deg)?a:a*this->dtr);
  }

  //! Spherical to rectangular conversion.
  /*!
  * Define theta to be the azimuthal angle in the xy-plane  from the
  * x-axis with 0<=theta<2pi  (denoted lambda when referred to as the
  * longitude), phi to be the polar angle from the equator to the pole with
  * -pi/2<=phi<=pi/2  (This is delta  where delta is the latitude),
  * and r to be distance (radius) from a point to the origin.
  *
  * @param r the radius.
  * @param theta the longitude (in radians).
  * @param phi the latitude (in radians).
  * @param x the x-value.
  * @param y the y-value.
  * @param z the z-value.
  */
  void SphericalToRectangular(T const & r, T const & theta, T const & phi, T & x, T & y,T & z, bool const & deg = true)
  {
    T t = (deg)?theta*this->dtr:theta;
    T p = (deg)?phi*this->dtr:phi;

    T ct = std::cos(t);
    T st = std::sin(t);
    T cp = std::cos(p);
    T sp = std::sin(p);

    x = r*ct*cp;
    y = r*st*cp;
    z = r*sp;
  }

  //! Rectangular to spherical conversion.
  /*!
  * Define theta to be the azimuthal angle in the xy-plane  from the
  * x-axis with 0<=theta<2pi  (denoted lambda when referred to as the
  * longitude), phi to be the polar angle from the equator to the pole with
  * -pi/2<=phi<=pi/2  (This is delta  where delta is the latitude),
  * and r to be distance (radius) from a point to the origin.
  *
  * @param x the x-value.
  * @param y the y-value.
  * @param z the z-value.
  * @param r the radius.
  * @param theta the longitude (in radians).
  * @param phi the latitude (in radians).
  */
  void RectangularToSpherical(T const & x, T const & y, T const & z, T & r, T & theta, T & phi, bool const & deg = true)
  {
    r = std::sqrt(x*x+y*y+z*z);

    // Check for r less than some precision.
    T precision;
    // Only works if T is float, double or long double.
    if (sizeof(T) == sizeof(double))
      precision = eps;
    else
      precision = T(epsf);

    if ( r < precision )
    {
      r = 0;
      theta = 0;
      phi = 0;
    }
    else
    {
      theta = std::atan2(y,x);
      phi = std::asin(z/r);
      theta = (deg)?theta*this->rtd:theta;
      phi = (deg)?phi*this->rtd:phi;
    }
  }



  private:
    const double rtd; // radians to degrees
    const double dtr; // degrees to radians
    const double eps; // precision
    const double epsf; // precision

};

} // Namespace coordinate conversions.

} // Namspace UF.

#endif // COORDINATE_TRANSFORMS_H

