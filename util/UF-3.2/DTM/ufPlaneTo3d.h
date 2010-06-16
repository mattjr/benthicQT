//
// C++ Interface: PlaneTo3D
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DTM_UFPLANETO3D_H
#define DTM_UFPLANETO3D_H

#include "ufWin32Header.h"

#include <vtkSmartPointer.h>

class vtkPolyData;

namespace UF {
namespace DTM {

//! Produce a 3D representation from a plane.
/**
@class PlaneTo3D
Takes a dataset representing a plane an input and produces a 3D representation of it using the scalar as the z-value.

@author Andrew J. P. Maclean
*/
class UF_DTM_EXPORT PlaneTo3D{
public:
    PlaneTo3D();

    ~PlaneTo3D();

public:
  /**
   * Get the polydata object representing the input plane.
   * @return A pointer to the poly data object.
   */
  vtkPolyData *GetInput();
  /**
   * Set the polydata object representing the input plane. This actually does a deep copy.
   * @return
   */
   void SetInput(vtkPolyData * );
  /**
   * Get the polydata object representing the output.
   * @return A pointer to the poly data object.
   */
  vtkPolyData *GetOutput();

  /**
   * Convert the input plane to a 3d polydata object.
   */
  void DoPlaneTo3D();

private:
  // This is the source data representing the plane.
  vtkSmartPointer<vtkPolyData> inputData;
  // This is the output data.
  vtkSmartPointer<vtkPolyData> outputData;

};


} // Namespace DTM.
} // Namespace UF.

#endif
