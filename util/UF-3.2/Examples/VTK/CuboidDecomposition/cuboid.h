//
// C++ Interface: cuboid
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef VTKCUBOID_H
#define VTKCUBOID_H

#include <vtkSmartPointer.h>
#include <vector>

class vtkPolyData;
class vtkLookupTable;

//! VTK based classes.
namespace VTK {


/**
@class Cuboid
This class generates a polydata representation of a cuboid.
@author Andrew J. P. Maclean
*/
class Cuboid {
public:
  Cuboid();
  ~Cuboid();

public:
  /**
   * Set the vertices of the cuboid represented by Pts[8][3].
   *
 * @param Pts the coordinates defining the corners of the cuboid.
 * @param SplittingFactor how much to separate the tetrahedron.
   */
  void SetVertices( double Pts[8][3], double SplittingFactor = 0);

  /**
   *
   * Get the polydata representation of the cuboid.
   */
  void GetCuboid ( vtkPolyData* cuboid );

  /**
   * Return the polydata representation of a tetrahedron.
   *
   * @param idx the index of the tertahedron you want (0...4).
   * @param tetrahedron the tetrahedron.
   *
   */
  void GetTetrahedron( int const & idx, vtkPolyData * tetrahedron );

   /**
   * Return the polydata representation of all the tetrahedra.
   *
   * @param tetrahedra the set of all tetrahedra.
   *
   */
  void GetTetrahedra( vtkPolyData * tetrahedra );

  /**
  * Get the lookup table.
  *
  * @param lut the lookup table.
  */
  void GetLookupTable( vtkLookupTable * lut );

  /**
  * Return the volume of the cuboid.
  */
  double CuboidVolume( );


private:
  /**
   *
   * Calculate the dot product of two 3-vectors.
   *
  */
  template < typename T >
    T Dot ( const T x[3], const T y[3] )
    {
      return x[0]*y[0] + x[1]*y[1] + x[2]*y[2];
    }

    /**
     *
     * Calculate the cross product of two 3-vectors.
     *
    */
    template < typename T >
        void Cross ( const T x[3], const T y[3], T z[3] )
    {
      T Zx = x[1]*y[2] - x[2]*y[1];
      T Zy = x[2]*y[0] - x[0]*y[2];
      T Zz = x[0]*y[1] - x[1]*y[0];
      z[0] = Zx; z[1] = Zy; z[2] = Zz;
    }

    /**
     * Generate the indices that can be used to partition the cube into tetrahedera.
     *
    */
    void MakeTetrahedralIndices ();

    /**
     * Make the polydata cuboid.
     */
    void MakePDCuboid (double vertices[8][3], vtkIdType faceIndices[6][4], vtkPolyData * cuboid);

    /**
     * Make the polydata tetrahedron.
     */
    void MakePDTetrahedron(double vertices[8][3], vtkIdType tetrahederalIndices[4], vtkIdType faceIndices[4][3], vtkPolyData * tetrahedron);

    /**
    * Make a lookup table to assign colours to the faces of the tetrahedra and cuboid.
    */
    void MakeLookupTable();

    /**
      * Get three vectors defining the tetrahedron.
      *
      * @param Vertices the four vertices defining the tetrahederon.
      * @param The three vectors defining the tetrahederon. Origin corresponds to Vertices[0].
      */
    void TetrahedralVertexToVector ( double const Vertices[4][3], double Vectors[3][3] );

    /**
    * Get three vectors defining the tetrahedron.
    *
    * @param a the point defining the origin.
    * @param b a vertex point.
    * @param c a vertex point.
    * @param d a vertex point.
    * @param The three vectors defining the tetrahederon. Origin corresponds to Vertices[0].
    */
    void TetrahedralVertexToVector ( double const a[3], double const b[3], double const c[3], double const d[3], double Vectors[3][3] );

    /**
    * Calculate the volume of a tetrahedron.
    * V = (1/3!) * |a . (b X c)|,
    *  where a,b,c are the three vectors defining the tetrahedron.
    * @param The three vectors defining the tetrahedron.
    * @return The volume.
    */
    double TetrahedralVolume ( double const Vectors[3][3] );

    /**
    * Calculate the volume of a tetrahedron.
    * V = (1/3!) * |a . (b X c)|,
    *  where a,b,c are the three vectors defining the tetrahedron.
    * @param The three vectors defining the tetrahedron.
    * @return The volume.
    */
    double TetrahedralVolume ( double const a[3], double const b[3], double const c[3] );


private:
  double CubeVertices[8][3];
  vtkIdType TetrahedralIndices[5][4];

  vtkSmartPointer<vtkPolyData> cuboid;
  std::vector<vtkSmartPointer< vtkPolyData> > tetrahedron;

  // These will correspond to the faces on the cube (right-handed ordering).
  vtkIdType cubeFaces[6][4];
  // These will correspond to the faces on the various tetrahedron (right-handed ordering).
  vtkIdType tetrahedralFaces[5][4][3];

  vtkSmartPointer<vtkLookupTable> lut;
};

}
#endif

