//
// C++ Interface: ProbeSource
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DTM_UFPROBESOURCE_H
#define DTM_UFPROBESOURCE_H

#include "ufWin32Header.h"

#include <vtkSmartPointer.h>

class vtkPolyData;

namespace UF {
namespace DTM {

//! Produce a source dataset to be used when probing a sourse dataset.
/**
@class ProbeSource
Convert the polydata to a plane whose z-value = 0 and the z-values from the original polydata are the scalars. The original scalar values are lost.


@author Andrew J. P. Maclean
*/
class UF_DTM_EXPORT ProbeSource{
public:
    ProbeSource();

    ~ProbeSource();
public:
  /**
   * Get the polydata object representing the input.
   * @return A pointer to the poly data object.
   */
  vtkPolyData *GetInput();
  /**
   * Set the polydata object representing the input. This actually does a deep copy.
   * @return
   */
   void SetInput(vtkPolyData * );
  /**
   * Get the output.
   * @return A pointer to the polydata representing the output.
   */
  vtkPolyData *GetOutput();

public:
  /**
   * Generate the output..
   * @return
   */
   void GenerateOutputPolyData();

private:
  vtkSmartPointer<vtkPolyData> inputData;
  vtkSmartPointer<vtkPolyData> outputData;

};

} // Namespace DTM.
} // Namespace UF.

#endif
