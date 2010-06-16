//
// C++ Interface: DTMVolume
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DTM_UFDTMVOLUME_H
#define DTM_UFDTMVOLUME_H

#include "ufWin32Header.h"

namespace UF {
namespace DTM {

class DTMImage;

//! Calculate the volume between two surfaces.
/**
This class holds two DTMImage classes, one is the reference surface and the other is the actual surface. Methods are provided for calculating the volume difference between the reference and the actual surface.

If you are calculating the volume of the difference between the surfaces, the origin and spacing of both the reference surface and the actual surface must be the same. The volume will be calculated using the intersection of the reference and actual surface.

@author Andrew J. P. Maclean
*/
class UF_DTM_EXPORT DTMVolume{
public:
    DTMVolume();

    ~DTMVolume();
public:
  /**
   * Set the actual surface. The actual surface is copied into this class.
   * @param ActualSurface
   */
  void SetActualSurface ( DTMImage const & ActualSurface );

  /**
   * Return a pointer to the actual surface.
   * @return pointer to the actual surface.
   */
  DTMImage * GetActualSurface ( ) { return this->actualSurface; }

  /**
   * Set the reference surface. The reference surface is copied into this class.
   * @param ReferenceSurface
   */
  void SetReferenceSurface ( DTMImage const & ReferenceSurface );

  /**
   * Return a pointer to the reference surface.
   * @return pointer to the reference surface.
   */
  DTMImage * GetReferenceSurface ( ) { return this->referenceSurface; }

  /**
   * Copy the reference surface to the actual surface.
   */
  void CopyReferenceSurfaceToActualSurface ( );

  /**
   * Copy the actual surface to the reference surface.
   */
  void CopyActualSurfaceToReferenceSurface ( );

  /**
   * Calculate the volume corresponding to the difference between the two surfaces. An exception is thrown if the volume cannot be calculated.
   * Filtering is applied in the followinr order if the filters are switched on:
   * 1) Range
   * 2) Height
   * 3) Tolerance
   * 4) Cuboid volume type selection.
   * @param VolumeAbove The volume corresponding to where the acutual surface lies above the reference surface. The actual surface height is positive relative to the relative surface.
   * @param VolumeBelow The volume corresponding to where the acutual surface lies below the reference surface.  The actual surface height is negative relative to the relative surface.
   */
  void CalculateVolume( double & VolumeAbove, double & VolumeBelow );

  /**
   * Return the filter on height value.
   */
  bool GetFilterOnHeight ( void ) const { return this->FilterOnHeight; }

  /**
   * Set filter on height.
   * @param FilterOnHeight
   */
  void SetFilterOnHeight ( bool const & FilterOnHeight ) { this->FilterOnHeight = FilterOnHeight; }

  /**
   * Set the filter on height to be on.
   */
  void FilterOnHeightOn ( void ) { this->FilterOnHeight = true; }

  /**
   * Set the filter on height to be off.
   */
  void FilterOnHeightOff ( void ) { this->FilterOnHeight = false; }

  /**
   * Get the height.
   * @param Height - the height.
   */
  void GetHeight ( double Height[2] ) { for (int i = 0; i < 2; ++i) Height[i] = this->Height[i]; }

  /**
   * Set the height.
   * @param Height - the height.
   */
  void SetHeight ( double const Height[2] ) { for (int i = 0; i < 2; ++i) this->Height[i] = Height[i]; }

  /**
   * Return the filter on range value.
   */
  bool GetFilterOnRange ( void ) const { return this->FilterOnRange; }

  /**
   * Set the filter on range.
   * @param FilterOnRange
   */
  void SetFilterOnRange ( bool const & FilterOnRange ) { this->FilterOnRange = FilterOnRange; }

  /**
   * Set the filter on range to be on.
   */
  void FilterOnRangeOn ( ) { this->FilterOnRange = true; }

  /**
   * Set the filter on range to be off.
   */
  void FilterOnRangeOff ( ) { this->FilterOnRange = false; }

  /**
   * Get the range.
   * @param Range - the range.
   */
  void GetRange ( double Range[2] ) { for (int i = 0; i < 2; ++i) Range[i] = this->Range[i]; }

  /**
   * Set the range.
   * @param Range - the range.
   */
  void SetRange ( double const Range[2] ) { for (int i = 0; i < 2; ++i) this->Range[i] = Range[i]; }

  /**
   * Return the filter on tolerance value.
   */
  bool GetFilterOnTolerance ( ) const { return this->FilterOnTolerance; }

  /**
   * Set the filter on tolerance.
   * @param FilterOnTolerance
   */
  void SetFilterOnTolerance ( bool const & FilterOnTolerance ) { this->FilterOnTolerance = FilterOnTolerance; }

  /**
   * Set the filter on tolerance to be on.
   */
  void FilterOnToleranceOn ( ) { this->FilterOnTolerance = true; }

  /**
   * Set the filter on tolerance to be off.
   */
  void FilterOnToleranceOff ( ) { this->FilterOnTolerance = false; }

  /**
   * Get the tolerance.
   */
  double GetTolerance ( ) { return this->Tolerance; }

  /**
   * Set the tolerance.
   * @param Tolerance - the range.
   */
  void SetTolerance ( double const Tolerance ) { this->Tolerance = Tolerance; }


  /** Get the current state of th ecuboid volume selector.
   * Possible values:
   *   0: Don't select any.
   *   1: Select all.
   *   2: Only those whose volume above the reference surface is > 0 and volume below the reference surface == 0.
   *   3: Only those whose volume below the reference surface is > 0 and volume above the reference surface == 0.
   *   4: Only those whose volume below the reference surface is > 0 and volume above the reference surface > 0.
   */
  int GetCuboidVolumeSelector ( ) const { return this->CuboidVolumeSelector; }

  /** Select cuboids based on volume.
   * @param CuboidVolumeSelector
   * Possible values:
   *   0: Don't select any.
   *   1: Select all.
   *   2: Only those whose volume above the reference surface is > 0 and volume below the reference surface == 0.
   *   3: Only those whose volume below the reference surface is > 0 and volume above the reference surface == 0.
   *   4: Only those whose volume below the reference surface is > 0 and volume above the reference surface > 0.
   */
  void SetCuboidVolumeSelector ( int const & CuboidVolumeSelector ) { this->CuboidVolumeSelector = CuboidVolumeSelector; }

  protected:
  DTMImage *actualSurface;
  DTMImage *referenceSurface;

protected:
  /**
   * Check that the spacings for both surfaces are equal.
   * @return true if the spacings are identical.
   */
  bool CheckSpacing();

  /**
   * Check that the origins for both surfaces are equal.
   * @return true if the origins are isentical.
   */
  bool CheckOrigin();

  /**
   * Find a common origin for the intersection of both surfaces.
   * @param origin The common origin.
   * @return
   */
  void FindCommonOrigin( double origin[3] );

  /**
   * Find the least upper bound for the intersection of both surfaces.
   * @param RO The common origin relative to the reference surface.
   * @param AO The common origin relative to the actual surface.
   * @param LUB The least upper bound.
   * @return
   */
  void FindIntersection( int RO[3], int AO[3], int LUB[] );

  /**
   * Calculate the volume of the surface.
   * The surfaces are regarded as 3D structured point datasets or volumes. The coordinates
   * (i,j,k) reference a scalar value in the dataset. So to calculate the volume, we select
   * the k^{th} slice and use the scalar at point (i,j) as the z-value. This is done for the
   * reference and actual surface. RefOrigin and ActOrigin may only differ in their third
   * coordinate, indicating that different slices are being used.
   *
   * @param RefOrigin the origin of the actual surface.
   * @param ActOrigin the origin of the actual surface.
   * @param VolumeAbove the volume of the voxel that lies above the reference surface.
   * @param VolumeBelow the volume of the voxel that lies below the reference surface.
   * @param Spacing the x,y,z spacing on the surface, the z-spacing is not used.
   */
//   void CalculateVolume(  double CommonOrigin[3], double & VolumeAbove, double & VolumeBelow);

  /**
   * Check for ghost points. I.e. points that are added to facilitate computations.
   * @return true if ghost points exist.
   */
  bool CheckGhosts( int Id[8] );

  /**
   * Calculate the volume of a voxel. This assumes that the spacing on the reference surface and
   * actual surface are identical. The function will throw if any of the indexes in RefId or ActId
   * are out of bounds.
   *
   * @param RefId the voxel on the reference surface. Only RefId[0..3] are used.
   * @param ActId the voxel on the actual surface. Only ActId[0..3] are used.
   * @param Spacing the x,y,z spacing on the surface, the z-spacing is not used.
   * @param VolumeAbove the volume of the voxel that lies above the reference surface.
   * @param VolumeBelow the volume of the voxel that lies below the reference surface.
   * @param Ok true if the volume calculation was not rejected for some reason.
   */
  void VoxelVolume( int RefId[8],  int ActId[8], double Spacing[3], double & VolumeAbove, double & VolumeBelow, bool & Ok );

  /**
   * Calculate the cuboid represented by RefId[0..3] and ActId[0..3].
   *
   * @param RefId the voxel on the reference surface. Only RefId[0..3] are used.
   * @param ActId the voxel on the actual surface. Only ActId[0..3] are used.
   * @param Spacing the x,y,z spacing on the surface, the z-spacing is not used.
   * @param CubeVertices the vertices of the cube.
   */
  void MakeCuboid( int RefId[8],  int ActId[8], double Spacing[3], double CubeVertices[8][3] );

  /**
   * Calculate the volume of a cuboid by decomposing it into tetrahedera.
   *
   * @param CubeVertices the vertices of the cube.
   * @param Volume the volume of the cuboid.
   */
  void CuboidVolume( double CubeVertices[8][3], double & Volume );

  /**
  * Determine how the voxel is positioned with respect to the reference plane.
  *
  * @param ActualPointPos the point position.
  * @param allAbove true if the z_actual >= z_ref.
  * @param allBelow true if z_actual <= z_ref.
  * @param allEqual true if z_actual == z_ref.
  *
  * If allAbove and allBelow are both false then some sides are above the reference surface
  * and others are below it.
  *
  */
  void VoxelSign(int ActualPointPos[4], bool & allAbove, bool & allBelow, bool &allEqual );

  /**
  * Calculate the volume of a prism.
  *
  * @param Height the height of the vertical sides.
  * @param Spacing the x-,y-spacing along the base of the prism. The z-value is ignored.
  * @return The volume.
  */
  double PrismVolume ( double Height[4], double Spacing[3] );

  /**
   * Calculate the volume of a right angled wedge.
   *
   * @param Height the height of the vertical sides.
   * @param Spacing the x-,y-spacing along the base of the wedge. The z-value is ignored.
   * @return The volume.
  */
  double WedgeVolume ( double const Height[2], double const Spacing[3] );

  /**
   * Get three vectors defining the tetrahedron.
   *
   * @param Vertices the four vertices defining the tetrahederon.
   * @param Vectors the three vectors defining the tetrahederon. Origin corresponds to Vertices[0].
   */
  void TetrahedralVertexToVector ( double const Vertices[4][3], double Vectors[3][3] );

  /**
  * Get three vectors defining the tetrahedron.
  *
  * @param a[] the point defining the origin.
  * @param b[] a vertex point.
  * @param c[] a vertex point.
  * @param d[] a vertex point.
  * @param Vectors the three vectors defining the tetrahederon. Origin corresponds to Vertices[0].
  */
  void TetrahedralVertexToVector ( double const a[3], double const b[3], double const c[3], double const d[3], double Vectors[3][3] );

  /**
  * Calculate the volume of a tetrahedron.
  * V = (1/3!) * |a . (b X c)|,
  *  where a,b,c are the three vectors defining the tetrahedron.
  * @param Vectors the three vectors defining the tetrahedron.
  * @return The volume.
  */
  double TetrahedralVolume ( double const Vectors[3][3] );

  /**
  * Calculate the volume of a tetrahedron.
  * V = (1/3!) * |a . (b X c)|,
  *  where a,b,c are the three vectors defining the tetrahedron.
  * @param a[] a vector used to define the tetrahedron.
  * @param b[] a vector used to define the tetrahedron.
  * @param c[] a vector used to define the tetrahedron.
  * @return The volume.
  */
  double TetrahedralVolume ( double const a[3], double const b[3], double const c[3] );

  /**
   * Check that the z-values of the cuboid are within tolerance.
   * Note: If the boolean FilterOnRange is false the check is not done and true is returned.
   * @param Vertices the vertices of the cuboid.
   * @return true if all of the z-values of the vertices lie in range.
   */
  bool CheckRange ( double Vertices[8][3] );

  /**
   * Check that the height of sides of the cubois are within tolerance.
   * Note: If the boolean FilterOnHeight is false the check is not done and true is returned.
   * @param ZLength the height of each vertical side of the cuboid.
   * @return true if all of the heights lie in range.
   */
  bool CheckHeight ( double ZLength[4] );

  /**
   * Check that the volume of the cuboid lies within tolerance.
   * Note: If the boolean FilterOnTolerance is false the check is not done and true is returned.
   * @param Volume the volume of the cuboid.
   * @return true if all of the heights lie in range.
   */
  bool CheckTolerance ( double Volume );

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
     *
     * Add two 3-vectors together.
     *
   */
    template < typename T >
        void Add ( const T x[3], const T y[3], T z[3] )
    {
      z[0] = x[0] + y[0];
      z[1] = x[1] + y[1];
      z[2] = x[2] + y[2];
    }

  /**
     *
     * Subtract two 3-vectors.
     *
   */
    template < typename T >
        void Sub ( const T x[3], const T y[3], T z[3] )
    {
      z[0] = x[0] - y[0];
      z[1] = x[1] - y[1];
      z[2] = x[2] - y[2];
    }

    //! If true the we reject any cuboids whose total volume is less than the tolerance.
    bool FilterOnTolerance;
    //! Any cuboids with total volumes less than this are rejected.
    double Tolerance;
    //! If true the we reject any cuboids whose z-values lie outside this range.
    bool FilterOnRange;
    //! The minimum and maximum z-values for defining the range.
    double Range[2];
    //! If true the we reject any cuboids whose height lies outside this range.
    bool FilterOnHeight;
    //! The minimum and maximum height defining the range.
    double Height[2];
    /** Select cuboids based on volume.
     * Possible values:
     * 0: Don't select any.
     * 1: Select all.
     * 2: Only those whose volume above the reference surface is > 0 and volume below the reference surface == 0.
     * 3: Only those whose volume below the reference surface is > 0 and volume above the reference surface == 0.
     * 4: Only those whose volume below the reference surface is > 0 and volume above the reference surface > 0.
     */
    int CuboidVolumeSelector;

};


} // Namespace DTM.
} // Namespace UF.

#endif
