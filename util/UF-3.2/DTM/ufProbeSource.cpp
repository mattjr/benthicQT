//
// C++ Implementation: ProbeSource
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <vtkPolyData.h>
#include <vtkDoubleArray.h>
#include <vtkCellArray.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkPointSet.h>
#include <vtkTriangleFilter.h>
#include <vtkDelaunay2D.h>

#include "ufProbeSource.h"
#include "ufObservers.h"

using namespace UF::DTM;

ProbeSource::ProbeSource() :
  inputData(0)
  , outputData(0)
{
  this->inputData = vtkSmartPointer<vtkPolyData>::New();
  this->outputData = vtkSmartPointer<vtkPolyData>::New();
}


ProbeSource::~ProbeSource()
{
}

vtkPolyData* ProbeSource::GetInput() { return this->inputData; }

void ProbeSource::SetInput(vtkPolyData * pd) { this->inputData->DeepCopy(pd); }

vtkPolyData* ProbeSource::GetOutput() { return this->outputData; }

void ProbeSource::GenerateOutputPolyData()
{
  vtkIdType numberOfPoints = this->inputData->GetNumberOfPoints();

  if ( numberOfPoints == 0 ) return;

  this->outputData->DeepCopy(this->inputData);

  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->SetDataTypeToDouble();

  vtkSmartPointer<vtkDoubleArray> scalars = vtkSmartPointer<vtkDoubleArray>::New();
    scalars->SetNumberOfComponents(1);
    scalars->SetName("Altitude");
    for (vtkIdType i = 0; i < numberOfPoints; ++i)
      {
      double pt[3];
      this->inputData->GetPoint(i,pt);

      // Make the z-value the scalar and then set the z-value to zero.
      // Thus we have a plane (x,y,0) with the old z-value becoming the scalar.
      scalars->InsertNextValue(pt[2]);
      pt[2] = 0;
      points->InsertNextPoint(pt);

      }
  vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();

  numberOfPoints = points->GetNumberOfPoints();
  if ( numberOfPoints == 0 ) return;

  for( vtkIdType j = 0; j < numberOfPoints; ++j )
    {
    vertices->InsertNextCell( 1 );
    vertices->InsertCellPoint( j );
    }

  vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();
    pd->SetPoints(points);
    pd->SetVerts(vertices);
    pd->GetPointData()->SetScalars(scalars);
    pd->Update();

  // Build and triangulate a surface.
  vtkSmartPointer<DTM::vtkStartCommand> sobserver = vtkSmartPointer<DTM::vtkStartCommand>::New();
  vtkSmartPointer<DTM::vtkEndCommand> eobserver = vtkSmartPointer<DTM::vtkEndCommand>::New();
  vtkSmartPointer<DTM::vtkProgressCommand> pobserver = vtkSmartPointer<DTM::vtkProgressCommand>::New();
  vtkSmartPointer<vtkDelaunay2D> surf = vtkSmartPointer<vtkDelaunay2D>::New();
    surf->SetInput(pd);
    surf->AddObserver( vtkCommand::StartEvent, sobserver );
    surf->AddObserver( vtkCommand::EndEvent, eobserver );
    surf->AddObserver( vtkCommand::ProgressEvent, pobserver );

  vtkSmartPointer<vtkTriangleFilter> tf = vtkSmartPointer<vtkTriangleFilter>::New();
    tf->SetInput(surf->GetOutput());
    tf->Update();

    this->outputData->DeepCopy(tf->GetOutput());
}
