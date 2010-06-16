//
// C++ Implementation: LookupTables
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufLookupTables.h"

#include <vtkLookupTable.h>
#include <vtkSmartPointer.h>

using namespace UF::DTM;

LookupTables::LookupTables()
{
}

LookupTables::~LookupTables()
{
}

void LookupTables::Land ( vtkLookupTable *lut, vtkIdType LandTableValues )
{
    lut->SetNumberOfTableValues(LandTableValues);
    lut->SetHueRange(0.5,0.07); // 0.5->0.0
    lut->SetSaturationRange(1.0,0.3); // 1.0->0.0
    lut->SetValueRange(0.5,0.9); // 0.5->1.0
    lut->SetAlphaRange(1,1);
    lut->Build();
}

void LookupTables::Sea ( vtkLookupTable *lut, vtkIdType SeaTableValues )
{
    lut->SetNumberOfTableValues(SeaTableValues);
    lut->SetHueRange(0.58,0.52); // 0.58->0.52
    lut->SetSaturationRange(1.0,0.3); // 1.0->0.0
    lut->SetValueRange(0.5,0.9); // 0.2->0.9
    lut->SetAlphaRange(1,1);
    lut->Build();

}

void LookupTables::LandAndSea ( vtkLookupTable *lut, vtkIdType LandTableValues, vtkIdType SeaTableValues )
{
  vtkSmartPointer<vtkLookupTable> sea = vtkSmartPointer<vtkLookupTable>::New();
  Sea(sea,LandTableValues);
  vtkSmartPointer<vtkLookupTable> land = vtkSmartPointer<vtkLookupTable>::New();
  Land(land,LandTableValues);

  lut->SetNumberOfTableValues(LandTableValues + SeaTableValues);
  for ( vtkIdType i = 0; i < (LandTableValues + SeaTableValues); ++i )
    {
    double rgba[4];
    if ( i < SeaTableValues )
      sea->GetTableValue(i,rgba);
    else
      land->GetTableValue(i-SeaTableValues,rgba);
    lut->SetTableValue(i,rgba);
    }
}

void LookupTables::StandardIncreasing ( vtkLookupTable *lut, vtkIdType TableValues )
{
    lut->SetNumberOfTableValues(TableValues);
    lut->SetHueRange(0.0,2.0/3.0);
    lut->SetSaturationRange(1.0,1.0);
    lut->SetValueRange(1.0,1.0);
    lut->SetAlphaRange(1,1);
    lut->Build();

}

void LookupTables::StandardDecreasing ( vtkLookupTable *lut, vtkIdType TableValues )
{
    lut->SetNumberOfTableValues(TableValues);
    lut->SetHueRange(2.0/3.0,0.0);
    lut->SetSaturationRange(1.0,1.0);
    lut->SetValueRange(1.0,1.0);
    lut->SetAlphaRange(1,1);
    lut->Build();

}

void LookupTables::FullHueCircleIncreasing ( vtkLookupTable *lut, vtkIdType TableValues )
{
    lut->SetNumberOfTableValues(TableValues);
    lut->SetHueRange(0.0,1.0);
    lut->SetSaturationRange(1.0,1.0);
    lut->SetValueRange(1.0,1.0);
    lut->SetAlphaRange(1,1);
    lut->Build();

}

void LookupTables::FullHueCircleDecreasing ( vtkLookupTable *lut, vtkIdType TableValues )
{
    lut->SetNumberOfTableValues(TableValues);
    lut->SetHueRange(1.0,0.0);
    lut->SetSaturationRange(1.0,1.0);
    lut->SetValueRange(1.0,1.0);
    lut->SetAlphaRange(1,1);
    lut->Build();

}

void LookupTables::MakeLookupTable( vtkLookupTable * lut, vtkIdType TableValues,
  double minHue, double maxHue,
  double minSaturation, double maxSaturation,
  double minValue, double maxValue,
  double minAlpha, double maxAlpha,
  int Ramp, int Scale )
{
    lut->SetNumberOfTableValues(TableValues);
    lut->SetHueRange(minHue,maxHue);
    lut->SetSaturationRange(minSaturation,maxSaturation);
    lut->SetValueRange(minValue,maxValue);
    lut->SetAlphaRange(minAlpha,maxAlpha);
    lut->SetRamp(Ramp);
    lut->SetScale(Scale);
    lut->Build();

}

void LookupTables::MakeLookupTableWeb( vtkLookupTable * lut, vtkIdType TableValues,
  double minHue, double maxHue,
  double minSaturation, double maxSaturation,
  double minValue, double maxValue,
  double minAlpha, double maxAlpha,
  int Ramp, int Scale )
{
  MakeLookupTable( lut, TableValues,
    minHue/255.0, maxHue/255.0,
    minSaturation/255.0, maxSaturation/255.0,
    minValue/255.0, maxValue/255.0,
    minAlpha/255.0, maxAlpha/255.0,
    Ramp, Scale );

}

