//
// C++ Interface: ProbeInput
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DTM_UFPROBEINPUT_H
#define DTM_UFPROBEINPUT_H

#include "ufWin32Header.h"

#include <vtkSmartPointer.h>


class vtkPolyData;
class vtkImageData;

namespace UF {
namespace DTM {

//! Produce a dataset to be used as an input when probing a sourse dataset.
/**
@class ProbeInput
Generate a source that can be used to probe a dataset.
If the default settings are used the dataset will lie just
inside the probe volume/area.
@author Andrew J. P. Maclean
*/
class UF_DTM_EXPORT ProbeInput{
public:
    ProbeInput();

    ~ProbeInput();

public:
  /**
   * Get a plane extending in the x-, y- direction.
   * Make sure that the bounds, resolution and scale factor has been set.
   *
   * @return A pointer to the plane.
   */
   vtkPolyData * GetPolyDataPlane ();

  /**
   * Get a plane extending in the x-, y- direction.
   * Make sure that the bounds, resolution and scale factor has been set.
   *
   * @return A pointer to the plane.
   */
   vtkImageData * GetImageDataPlane ();

   /**
    * Get the bounds.
    * @param Bounds[] The bounds of the region.
    */
   void GetBounds ( double Bounds[6] ) { Bounds = this->bounds; }

   /**
    * Set the bounds.
    * @param Bounds[] The bounds of the region.
    */
   void SetBounds ( double Bounds[6] ) { for (int i = 0; i < 6; ++i) this->bounds[i] = Bounds[i]; }

   /**
    * Get the spacing.
    * @param Spacing[] The spacing between points in the x,y,z- directions.
    */
   void GetResolution ( double Spacing[3] ) { Spacing = this->spacing; }

   /**
    * Set the spacing.
    * @param Spacing[] The spacing between points in the x,y,z- directions.
    */
   void SetResolution ( double Spacing[3] ) { for (int i = 0; i < 3; ++i) this->spacing[i] = Spacing[i]; }

   /**
    * Get the scale factor. A factor of 1 indicates no scaling, 1.1: increase by 10%,
    * 0.9: decrease by 10%.
    * @param ScaleFactor[] The scalaing for the x,y,z axes.
    */
   void GetScaleFactor ( double ScaleFactor[3] ) { ScaleFactor = this->scaleFactor; }

   /**
    * Set the scale factor. A factor of 1 indicates no scaling, 1.1: increase by 10%,
    * 0.9: decrease by 10%.
    * @param ScaleFactor[] The scaling for the x,y,z axes.
    */
   void SetScaleFactor ( double ScaleFactor[3] ) { for (int i = 0; i < 3; ++i) this->scaleFactor[i] = ScaleFactor[i]; }

   /**
    * Generate a poly data plane. Make sure bounds, spacing and scale factor have been set before calling this function.
    */
   void MakePolyDataPlane ();

   /**
    * Generate an image data plane. Make sure bounds, spacing and scale factor have been set before calling this function.
    */
   void MakeImageDataPlane();

   /**
    * Get the plane parameters. This uses the bounds, resolution and scale factor to determine the origin,
    * the two axial points P1 and P2, and the number of subdivisions in the x- and y- directions.
    * @param Origin[] - the origin.
    * @param P1[] - first axial point.
    * @param P2[] - second axial point.
    * @param Dimensions[] - dimensions.
    * @param Spacing[] - spacing.
    */
   void GetParameters ( double Origin[3], double P1[3], double P2[3], int Dimensions[3], double Spacing[3] );

   /**
    * Display the plane parameters. This uses the bounds, resolution and scale factor to determine the origin,
    * the two axial points P1 and P2, and the number of subdivisions in the x- and y- directions.
    */
   void DisplayParameters ( );

private:
   /**
    * Generate the parameters used to create the plane.
    * @param Origin[] Origin
    * @param P1[] Point defining the first axis of the plane.
    * @param P2[] Point defining the second axis of the plane.
    * @param Dimensions[] The number of subdivisions in each axis.
    */
   void CalculatePlaneParameters ( double Origin[3], double P1[3], double P2[3], int Dimensions[3] );

private:
  vtkSmartPointer<vtkPolyData> pd;
  vtkSmartPointer<vtkImageData> id;

  double bounds[6];
  double spacing[3];
  double scaleFactor[3];

};


} // Namespace DTM.
} // Namespace UF.

#endif
