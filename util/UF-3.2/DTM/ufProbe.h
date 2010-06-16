//
// C++ Interface: Probe
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DTM_UFPROBE_H
#define DTM_UFPROBE_H

#include "ufWin32Header.h"

#include <vtkSmartPointer.h>

class vtkPolyData;
class vtkImageData;

namespace UF {
namespace DTM {

//! Probe a dataset with another one.
/**
@class Probe
Takes a dataset as input and probes it with a source dataset.

@author Andrew J. P. Maclean
*/
class UF_DTM_EXPORT Probe{
public:
    Probe();

    ~Probe();

public:
  /**
   * Get the polydata object representing the probe.
   * @return A pointer to the poly data object.
   */
  vtkPolyData *GetPolyDataProbe();
  /**
   * Set the polydata object representing the probe. This actually does a deep copy.
   * @return
   */
   void SetPolyDataProbe(vtkPolyData * );
  /**
   * Get the polydata object representing the source.
   * @return A pointer to the poly data object.
   */
  vtkPolyData *GetPolyDataSource();
  /**
   * Set the polydata object representing the source. This actually does a deep copy.
   * @return
   */
   void SetSource(vtkPolyData * );
  /**
   * Get the output.
   * @return A pointer to the polydata representing the output.
   */
  vtkPolyData *GetPolyDataOutput();

  /**
   * Probe the input with the source generating the output.
   */
  void DoPolyDataProbe();

  /**
   * Get the polydata object representing the probe.
   * @return A pointer to the poly data object.
   */
  vtkImageData *GetImageDataProbe();
  /**
   * Set the polydata object representing the probe. This actually does a deep copy.
   * @return
   */
   void SetImageDataProbe(vtkImageData * );
  /**
   * Get the polydata object representing the source.
   * @return A pointer to the poly data object.
   */
  vtkImageData *GetImageDataSource();
  /**
   * Set the polydata object representing the source. This actually does a deep copy.
   * @return
   */
   void SetSource(vtkImageData * );
  /**
   * Get the output.
   * @return A pointer to the polydata representing the output.
   */
  vtkImageData *GetImageDataOutput();

  /**
   * Probe the input with the source generating the output.
   */
  void DoImageDataProbe();

private:
  // This is the probe.
  vtkSmartPointer<vtkPolyData> pdProbe;
  vtkSmartPointer<vtkImageData> idProbe;
  // This is the dataset to probe.
  vtkSmartPointer<vtkPolyData> sourceData;
  // The result.
  vtkSmartPointer<vtkPolyData> pdOutputData;
  vtkSmartPointer<vtkImageData> idOutputData;

};


} // Namespace DTM.
} // Namespace UF.

#endif
