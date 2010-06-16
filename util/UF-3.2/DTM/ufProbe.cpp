//
// C++ Implementation: Probe
//
// Description: Takes a dataset as input and probes it with a source dataset.
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufProbe.h"

#include <vtkPolyData.h>
#include <vtkImageData.h>
#include <vtkProbeFilter.h>

using namespace UF::DTM;

Probe::Probe() :
  pdProbe(0)
  , idProbe(0)
  , sourceData(0)
  , pdOutputData(0)
  , idOutputData(0)
{
  this->pdProbe = vtkSmartPointer<vtkPolyData>::New();
  this->idProbe = vtkSmartPointer<vtkImageData>::New();
  this->sourceData = vtkSmartPointer<vtkPolyData>::New();
  this->pdOutputData = vtkSmartPointer<vtkPolyData>::New();
  this->idOutputData = vtkSmartPointer<vtkImageData>::New();
}


Probe::~Probe()
{
}

vtkPolyData * Probe::GetPolyDataProbe( ) { return this->pdProbe; }
void Probe::SetPolyDataProbe( vtkPolyData * Probe ) { this->pdProbe->DeepCopy(Probe); }
vtkPolyData * Probe::GetPolyDataSource( ) { return this->sourceData; }
void Probe::SetSource( vtkPolyData * Source ) { this->sourceData->DeepCopy(Source); }
vtkPolyData * Probe::GetPolyDataOutput( ) { return this->pdOutputData; }

vtkImageData * Probe::GetImageDataProbe( ) { return this->idProbe; }
void Probe::SetImageDataProbe( vtkImageData * Probe ) { this->idProbe->DeepCopy(Probe); }
void Probe::SetSource( vtkImageData * Source ) { this->sourceData->DeepCopy(Source); }
vtkImageData * Probe::GetImageDataOutput( ) { return this->idOutputData; }

void Probe::DoPolyDataProbe( )
{
  vtkSmartPointer<vtkProbeFilter> pf = vtkSmartPointer<vtkProbeFilter>::New();
    pf->SetInput(this->pdProbe);
    pf->SetSource(this->sourceData);
    pf->Update();

    this->pdOutputData->DeepCopy(pf->GetOutput());
}

void Probe::DoImageDataProbe( )
{
  vtkSmartPointer<vtkProbeFilter> pf = vtkSmartPointer<vtkProbeFilter>::New();
    pf->SetInput(this->idProbe);
    pf->SetSource(this->sourceData);
    pf->Update();

    this->idOutputData->DeepCopy(pf->GetOutput());


}

