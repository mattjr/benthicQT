//
// C++ Implementation: PlaneTo3D
//
// Description: Takes a polydata representation of a plane and uses the scalar as the z-value.
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

#include "ufObservers.h"

#include "ufPlaneTo3d.h"

using namespace UF::DTM;

PlaneTo3D::PlaneTo3D() :
  inputData(0)
  , outputData(0)
{
  this->inputData = vtkSmartPointer<vtkPolyData>::New();
  this->outputData = vtkSmartPointer<vtkPolyData>::New();
}


PlaneTo3D::~PlaneTo3D()
{
}

vtkPolyData * PlaneTo3D::GetInput( ) { return this->inputData; }
void PlaneTo3D::SetInput( vtkPolyData * Plane ) { this->inputData->DeepCopy(Plane); }
vtkPolyData * PlaneTo3D::GetOutput( ) { return this->outputData; }

void PlaneTo3D::DoPlaneTo3D( )
{
  vtkIdType numberOfPoints = this->inputData->GetNumberOfPoints();

  if ( numberOfPoints == 0 ) return;

  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->SetDataTypeToDouble();

    for (vtkIdType i = 0; i < numberOfPoints; ++i)
      {
      double pt[3];
      this->inputData->GetPoint(i,pt);
      this->inputData->GetPointData()->GetScalars()->GetTuple(i,&pt[2]);
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
    pd->GetPointData()->SetScalars(this->inputData->GetPointData()->GetScalars());
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

