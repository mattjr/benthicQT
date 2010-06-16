//
// C++ Interface: DTMVolumeView
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DTM_UFDTMVOLUMEVIEW_H
#define DTM_UFDTMVOLUMEVIEW_H

#include "ufDTMVolume.h"
#include <vtkSmartPointer.h>

class vtkPolyData;
class vtkPoints;
class vtkCellArray;
class vtkDoubleArray;

namespace UF {
namespace DTM {

//! Visualise the cuboids used to calculate the volume between two surfaces.
/**
Used to visualise the cuboids that are used to generate the volume in DTMVolume.
This class extends DTMVolume in that a VTKPolyData structure is used to represent
the cuboids used in the volume calculation.

@author Andrew J. P. Maclean
*/
class UF_DTM_EXPORT DTMVolumeView : public DTMVolume
{
public:
    DTMVolumeView();

    ~DTMVolumeView();
public:

  /**
   * Calculate the volume corresponding to the difference between the two surfaces. An exception is thrown if the volume cannot be calculated.
   * A VTKPolyData structure is also created representing the cuboids used in the volume calculation.
   * Filtering is applied in the following order if the filters are switched on:
   * 1) Range
   * 2) Height
   * 3) Tolerance
   * 4) Cuboid volume type selection.
   * @param VolumeAbove The volume corresponding to where the acutual surface lies above the reference surface. The actual surface height is positive relative to the relative surface.
   * @param VolumeBelow The volume corresponding to where the acutual surface lies below the reference surface.  The actual surface height is negative relative to the relative surface.
   */
  void CalculateVolume( double & VolumeAbove, double & VolumeBelow );

  /**
   * @return A pointer to the cuboids used in the volume calculation.
   */
  vtkPolyData* GetCuboids()
  {
    return this->cuboids;
  }

private:
  /**
  * Make the cuboids that represent the cells used to calculate the volume of the surface.
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
//  void MakeCuboids( double RefOrigin[3], double ActOrigin[3]);
  void MakePolyData (
      double v_above, double v_below,
      int & startingPoint, double cellVertices[8][3],
      vtkPoints *  points, vtkCellArray * cells,
      vtkDoubleArray * scalars, vtkDoubleArray * volumeAbove, vtkDoubleArray * volumeBelow,
      vtkDoubleArray * volumeTotal);

private:
  vtkSmartPointer<vtkPolyData> cuboids;
  int cubeFaces[6][4];


};


} // Namespace DTM.
} // Namespace UF.

#endif
