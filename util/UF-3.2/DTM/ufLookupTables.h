//
// C++ Interface: LookupTables
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DTM_UFLOOKUPTABLES_H
#define DTM_UFLOOKUPTABLES_H

#include "ufWin32Header.h"

#include <vtkSystemIncludes.h>

class vtkLookupTable;

namespace UF {
namespace DTM {

//! Useful lookup tables for colouring surfaces.
/**
A set of pre-defined lookup tables.

@author Andrew J. P. Maclean
*/
class UF_DTM_EXPORT LookupTables{
public:
    LookupTables();

    ~LookupTables();
public:

  /**
  *
  * @param lut The lookup table.
  * @param LandTableValues Specify the number of values (i.e., colors) in the lookup table.
  */
  void Land ( vtkLookupTable *lut, vtkIdType LandTableValues );

  /**
  *
  * @param lut The lookup table.
  * @param SeaTableValues Specify the number of values (i.e., colors) in the lookup table.
  */
  void Sea ( vtkLookupTable *lut, vtkIdType SeaTableValues );

  /**
  *
  * @param lut The lookup table.
  * @param LandTableValues Specify the number of values (i.e., colors) in the land part of the lookup table.
  * @param SeaTableValues Specify the number of values (i.e., colors) in the sea part of the lookup table.
  */
  void LandAndSea ( vtkLookupTable *lut, vtkIdType LandTableValues, vtkIdType SeaTableValues );

  /**
  *
  * @param lut The lookup table.
  * @param TableValues Specify the number of values (i.e., colors) in the lookup table.
  */
  void StandardIncreasing ( vtkLookupTable *lut, vtkIdType TableValues );

  /**
  *
  * @param lut The lookup table.
  * @param TableValues Specify the number of values (i.e., colors) in the lookup table.
  */
  void StandardDecreasing ( vtkLookupTable *lut, vtkIdType TableValues );

  /**
  *
  * @param lut The lookup table.
  * @param TableValues Specify the number of values (i.e., colors) in the lookup table.
  */
  void FullHueCircleIncreasing ( vtkLookupTable *lut, vtkIdType TableValues );

  /**
  *
  * @param lut The lookup table.
  * @param TableValues Specify the number of values (i.e., colors) in the lookup table.
  */
  void FullHueCircleDecreasing ( vtkLookupTable *lut, vtkIdType TableValues );

  /**
   * This is a convenience function for generating your own lookup table. The hue,
   * saturation and value for colours range from 0...1.
   * @param lut The lookup table.
   * @param TableValues Specify the number of values (i.e., colors) in the lookup table.
   * @param minHue Minimum hue.
   * @param maxHue Maximum hue.
   * @param minSaturation Minimum saturation.
   * @param maxSaturation Maximum saturation.
   * @param minValue Minimum value.
   * @param maxValue Maximum value).
   * @param minAlpha Minimum alpha.
   * @param maxAlpha Maximum alpha.
   * @param Ramp 0: (default) linear, 1: s-curve, 2: sqrt.
   * @param Scale 0: (default) linear, 1: log10.
   */
  void MakeLookupTable ( vtkLookupTable *lut, vtkIdType TableValues,
    double minHue = 0, double maxHue = 2.0/3.0,
    double minSaturation= 1, double maxSaturation = 1,
    double minValue = 1, double maxValue = 1,
    double minAlpha = 1, double maxAlpha = 1,
    int Ramp = 0, int Scale = 0
  );

  /**
   * This is a convenience function for generating your own lookup table. The hue,
   * saturation and value for colours range from 0...255.
   * @param lut The lookup table.
   * @param TableValues Specify the number of values (i.e., colors) in the lookup table.
   * @param minHue Minimum hue.
   * @param maxHue Maximum hue.
   * @param minSaturation Minimum saturation.
   * @param maxSaturation Maximum saturation.
   * @param minValue Minimum value.
   * @param maxValue Maximum value).
   * @param minAlpha Minimum alpha.
   * @param maxAlpha Maximum alpha.
   * @param Ramp 0: (default) linear, 1: s-curve, 2: sqrt.
   * @param Scale 0: (default) linear, 1: log10.
   */
  void MakeLookupTableWeb ( vtkLookupTable *lut, vtkIdType TableValues,
    double minHue = 0, double maxHue = 170,
    double minSaturation= 255, double maxSaturation = 255,
    double minValue = 255, double maxValue = 255,
    double minAlpha = 254, double maxAlpha = 255,
    int Ramp = 0, int Scale = 0
  );

};


} // Namespace DTM.
} // Namespace UF.

#endif
