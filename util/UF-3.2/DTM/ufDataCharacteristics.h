//
// C++ Interface: DataCharacteristics
//
// Description:
//
//
// Author:  <>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DTM_UFDATACHARACTERISTICS_H
#define DTM_UFDATACHARACTERISTICS_H

#include "ufWin32Header.h"

class vtkPolyData;
class vtkImageData;

namespace UF {
namespace DTM {

//! Properties of the various VTK data types.
/**
@class DataCharacteristics
View the properties of the various VTK data types.
@author Andrew J. P. Maclean
*/
class UF_DTM_EXPORT DataCharacteristics{
public:
    DataCharacteristics();

    ~DataCharacteristics();

public:

  /**
   * Display the properties of a vtkPolyData structure.
   * @param PolyData The vtkPolyData object.
   */
  void PrintPolyDataCharacteristics ( vtkPolyData * PolyData );

  /**
   * Return the properties of a vtkPolyData structure.
   * @param PolyData The vtkPolyData object.
   * @param components (points, vertices, cells, lines, polys, strips, pieces).
   * @param bounds (min_x, max_x, min_y, max_y, min_z, max_z).
   * @param scalarRange (min, max).
   */
  void PolyDataCharacteristics( vtkPolyData * PolyData,
    int components[7], double bounds[6], double scalarRange[2] );


  /**
   * Display the properties of a vtkImageData structure.
   * @param ImageData The vtkImageData object.
   */
  void PrintImageDataCharacteristics ( vtkImageData * ImageData );

  /**
   * Display the scalars in a vtkImageData structure.
   * @param ImageData The vtkImageData object.
   */
  void PrintImageDataScalars ( vtkImageData * ImageData );
};


} // Namespace DTM.
} // Namespace UF.

#endif
