//
// C++ Implementation: ProbeInput
//
// Description: Generate a source that Probe can use to probe a dataset.
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufProbeInput.h"

#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPlaneSource.h>
#include <vtkImageData.h>

using namespace UF::DTM;

ProbeInput::ProbeInput() :
  pd(0)
  , id(0)
{
  for (int i = 0; i < 6; ++i) this->bounds[i] = 0;
  for (int i = 0; i < 3; ++i)
    {
    this->spacing[i]=0;
    this->scaleFactor[i] = 1;
    }

  this->pd = vtkSmartPointer<vtkPolyData>::New();
  this->id = vtkSmartPointer<vtkImageData>::New();
}


ProbeInput::~ProbeInput()
{
}

vtkPolyData * ProbeInput::GetPolyDataPlane ()
{
  return this->pd;
}

vtkImageData * ProbeInput::GetImageDataPlane ()
{
  return this->id;
}

void ProbeInput::CalculatePlaneParameters( double Origin[], double P1[], double P2[], int Dimensions[] )
{
  double adjustedBounds[6];
  // First scale.
  for ( int i = 0; i < 3; ++i )
    {
    double dist = (this->bounds[i*2 + 1] - this->bounds[i*2]) * this->scaleFactor[i];
    double midpoint = (this->bounds[i*2 + 1] + this->bounds[i*2]) / 2;
    adjustedBounds[i*2] = midpoint - dist / 2.0;
    adjustedBounds[i*2 + 1] = midpoint + dist / 2.0;
    }
  // Now adjust the bounds so they are an integral multiple of the scale factor.
  // The new adjusted lower bounds will be <= the original adjusted lower bounds.
  // The new adjusted upper bounds will be >= the original adjusted upper bounds.
  double y = 0, n = 0;
  for ( int i = 0; i < 3; ++i )
    {
    int j = i * 2;
    int sgn = 1;
    // Lower bound
    double x = adjustedBounds[i*2];
    if ( x < 0 )
      {
      x = -x;
      sgn = -1;
      }
    y = modf(x / this->spacing[i], &n);
    if ( y != 0 )
      {
        if ( sgn < 0 )
          adjustedBounds[j] = sgn * (n + 1) * this->spacing[i];
        else
          adjustedBounds[j] = sgn * n * this->spacing[i];
      }
    j = i * 2 + 1;
    sgn = 1;
    // Upper bound.
    x = adjustedBounds[i*2 + 1];
    if ( x < 0 )
      {
      x = -x;
      sgn = -1;
      }
    y = modf(x / this->spacing[i], &n);
    if ( y != 0 )
      {
        if ( sgn < 0 )
          adjustedBounds[j] = sgn * n * this->spacing[i];
        else
          adjustedBounds[j] = sgn * (n + 1) * this->spacing[i];
      }
    }
    for (int i = 0; i < 3; ++i )
      {
      Origin[i] = adjustedBounds[i*2];
      Dimensions[i] = int((adjustedBounds[i*2+1] - adjustedBounds[i*2]) / this->spacing[i]);
      }
    P1[0] = adjustedBounds[0];
    P1[1] = adjustedBounds[3];
    P1[2] = adjustedBounds[4];
    P2[0] = adjustedBounds[1];
    // In this case, since it is a plane, these next two should be equal.
    P2[1] = adjustedBounds[2];
    P2[2] = adjustedBounds[5];

}

void ProbeInput::GetParameters( double Origin[], double P1[], double P2[], int Dimensions[], double Spacing[] )
{
  this->CalculatePlaneParameters( Origin, P1, P2, Dimensions );
  Spacing = this->spacing;
}

void ProbeInput::DisplayParameters( )
{
  double Origin[3];
  double P1[3];
  double P2[3];
  int Dimensions[3];

  this->CalculatePlaneParameters( Origin, P1, P2, Dimensions );

  std::ios::fmtflags flags = cout.setf(std::ios::fmtflags());
  cout << vtkstd::fixed << setprecision(2);
  cout << "  Origin ";
  for ( int i = 0; i < 3; ++i )
    {
    cout << Origin[i] << " ";
    }
  cout << std::endl;
  cout << "  P1 ";
  for ( int i = 0; i < 3; ++i )
  {
    cout << P1[i] << " ";
  }
  cout << std::endl;
  cout << "  P2 ";
  for ( int i = 0; i < 3; ++i )
  {
    cout << P2[i] << " ";
  }
  cout << std::endl;
  cout << "  Dimensions ";
  for ( int i = 0; i < 3; ++i )
  {
    cout << Dimensions[i] << " ";
  }
  cout << std::endl;
  cout << "  Spacing ";
  for ( int i = 0; i < 3; ++i )
  {
    cout << this->spacing[i] << " ";
  }
  cout << setprecision(6) << endl;
  std::ios::fmtflags newflags = cout.setf(std::ios::fmtflags());
  cout.setf(flags,newflags);
}

void ProbeInput::MakePolyDataPlane( )
{
  double origin[3], p1[3], p2[3];
  int dimensions[3];
  CalculatePlaneParameters( origin, p1, p2, dimensions );
  vtkSmartPointer<vtkPlaneSource> plane = vtkSmartPointer<vtkPlaneSource>::New();
  plane->SetOrigin(origin);
  plane->SetPoint1(p1);
  plane->SetPoint2(p2);
  plane->SetXResolution(dimensions[0]);
  plane->SetYResolution(dimensions[1]);
  plane->Update();

  this->pd->DeepCopy(plane->GetOutput());
}

void ProbeInput::MakeImageDataPlane( )
{
  double origin[3], p1[3], p2[3];
  int dimensions[3];
  CalculatePlaneParameters( origin, p1, p2, dimensions );
  vtkSmartPointer<vtkImageData> plane = vtkSmartPointer<vtkImageData>::New();
  dimensions[2] = 1;
  plane->SetOrigin(origin);
  plane->SetDimensions (dimensions);
  plane->SetSpacing(this->spacing);
  plane->SetScalarTypeToDouble();
  plane->SetNumberOfScalarComponents(1);
  plane->AllocateScalars();

  // Fill in scalar values, note that instead of 0, some other default value can be used..
  double *ptr = (double *) plane->GetScalarPointer();
  for ( int i = 0; i < dimensions[0]*dimensions[1]*dimensions[2]; ++i )
    {
    *ptr++ = 0;
    }

  this->id->DeepCopy(plane);
}

