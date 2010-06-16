//
// C++ Interface: Visualiser
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DTM_VISUALISER_H
#define DTM_VISUALISER_H

#include "ufWin32Header.h"

#include <vtkSmartPointer.h>

class vtkRenderer;
class vtkRenderWindow;
class vtkRenderWindowInteractor;

class vtkContourFilter;
class vtkPolyDataMapper;
class vtkActor;
class vtkPolyData;
class vtkImageData;
class vtkLookupTable;

class vtkImageViewer2;

class vtkPiecewiseFunction;
class vtkColorTransferFunction;
class vtkVolumeProperty;
class vtkVolumeRayCastCompositeFunction;
class vtkVolumeRayCastMapper;
class vtkVolume;
class vtkImageShiftScale;

class vtkAxesActor;
class vtkOrientationMarkerWidget;
class vtkTransform;
class vtkPropAssembly;

namespace UF {
namespace DTM {


//! Visualise polydata or imagedata.
/**
@class Visualiser
This class visualises the poly data or image data that is passed to it.
@author Andrew J. P. Maclean
*/
class UF_DTM_EXPORT Visualiser {
public:
  Visualiser();
  ~Visualiser();

public:
  // Convenience functions.
  /**
   *
   * @return A pointer to the renderer.
   */
  vtkRenderer *Ren();
  /**
   *
   * @return A pointer to the render window.
   */
  vtkRenderWindow *RenWin();
  /**
   *
   * @return A pointer to the render window interactor for rendering polydata.
   */
  vtkRenderWindowInteractor *IRen();

  /**
   *
   * @return A pointer to the render window interactor for rendering image data.
   */
  vtkRenderWindowInteractor *IVRen();

  /**
   *
   * @return A pointer to the render window interactor for rendering volume data.
   */
  vtkRenderWindowInteractor *VIRen();

  /**
   * Get the lookup table.
   * @return A pointer to the lookup table.
   */
  vtkLookupTable *GetLookupTable();

  /**
   * Get the polydata object.
   * @return A pointer to the poly data object.
   */
  vtkPolyData *GetPolyData();
  /**
   * Set the polydata object. This actually does a deep copy.
   * @return
   */
   void SetPolyData(vtkPolyData * );
  /**
   * Get the polydata output.
   * @return A pointer to the output.
   */
  vtkPolyData *GetPolyDataOutput();

  /**
   * Get the image data object.
   * @return A pointer to the image data object.
   */
  vtkImageData *GetImageData();
  /**
   * Set the polydata object. This actually does a deep copy.
   * @return
   */
   void SetImageData(vtkImageData * );
  /**
   * Get the image data output.
   * @return A pointer to the output.
   */
  vtkImageData *GetImageDataOutput();

  /**
   * Link all the pipeline objects together (polydata).
   * @return
   */
   void MakePolyDataPipeline();

  /**
   * Link all the pipeline objects together (imagedata).
   * @return
   */
   void MakeImageViewerPipeline();

  /**
   * Link all the pipeline objects together (imagedata).
   * Here the image data is viewed as a volume.This will
   * only work if the scalars are unsigned char or unsigned short.
   * @return
   */
   void MakeVolumeViewerPipeline();

   /**
    * Set the scalar range.
    * Used in colouring the surface and for contouring.
    * @param Range[] (min scalar, max scalar)
    */
   void SetScalarRange ( double Range[2] ) { for ( int i = 0; i < 2; ++i ) this->range[i] = Range[i]; }

   /**
    * Set the scalar range.
    * Used in colouring the surface and for contouring.
    * @param MinRange The minimum of the range.
    * @param MaxRange The maximum of the range.
    */
   void SetScalarRange ( double MinRange , double MaxRange )
   {
     this->range[0] = MinRange;
     this->range[1] = MaxRange;
   }

   /**
    * Switch on contouring.
    */
   void ContouringOn() { this->contouring = true; }

   /**
    * Switch off contouring.
    */
   void ContouringOff() { this->contouring = false; }

   /**
    * Switch on the user defined lookup table.
    */
   void UseLookupTableOn() { this->useLookupTable = true; }

   /**
    * Switch off the user defined lookup table.
    */
   void UseLookupTableOff() { this->useLookupTable = false; }

   /**
    * Set the number of contours.
    * @param NumberOfContours (min scalar, max scalar)
    */
   void SetNumberOfContours ( int NumberOfContours ) { this->numberOfContours = NumberOfContours; }

private:
  vtkSmartPointer<vtkRenderer> ren;
  vtkSmartPointer<vtkRenderWindow> renWin;
  vtkSmartPointer<vtkRenderWindowInteractor> iRen;
  vtkSmartPointer<vtkRenderWindowInteractor> ivRen;
  vtkSmartPointer<vtkImageViewer2> iv;

  vtkSmartPointer<vtkContourFilter> cf;
  vtkSmartPointer<vtkPolyDataMapper> m1;
  vtkSmartPointer<vtkPolyDataMapper> m2;
  vtkSmartPointer<vtkActor> a1;
  vtkSmartPointer<vtkActor> a2;

  vtkSmartPointer<vtkPolyData> pd;
  vtkSmartPointer<vtkImageData> id;
  vtkSmartPointer<vtkLookupTable> lut;

  vtkSmartPointer<vtkPiecewiseFunction> otf;
  vtkSmartPointer<vtkColorTransferFunction> ctf;
  vtkSmartPointer<vtkVolumeProperty> vp;
  vtkSmartPointer<vtkVolumeRayCastCompositeFunction> vrccf;
  vtkSmartPointer<vtkVolumeRayCastMapper> vrcm;
  vtkSmartPointer<vtkVolume> vv;
  vtkSmartPointer<vtkRenderer> vRen;
  vtkSmartPointer<vtkRenderWindow> vRenWin;
  vtkSmartPointer<vtkRenderWindowInteractor> vIRen;
  vtkSmartPointer<vtkImageShiftScale> shifter;

  vtkSmartPointer<vtkAxesActor> axes;
  vtkSmartPointer<vtkOrientationMarkerWidget> widget;
  vtkSmartPointer<vtkTransform> transform;
  vtkSmartPointer<vtkPropAssembly> assembly;

  double range[2];

  bool contouring;
  int numberOfContours;

  bool useLookupTable;
};

} // Namespace DTM.
} // Namespace UF.

#endif
