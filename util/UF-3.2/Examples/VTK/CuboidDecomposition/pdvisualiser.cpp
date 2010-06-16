//
// C++ Implementation: pdvisualiser
//
// Description: 
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "pdvisualiser.h"

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkContourFilter.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkActor.h>
#include <vtkLookupTable.h>

#include <vtkAxesActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextProperty.h>
#include <vtkTransform.h>

#include <vtkPropAssembly.h>
#include <vtkOrientationMarkerWidget.h>

#include <vtkProp3D.h>

namespace VTK {

PDVisualiser::PDVisualiser():
   ren(0)
   , renWin(0)
   , iRen(0)
   , cf(0)
   , m1(0)
   , m2(0)
   , a1(0)
   , a2(0)
   , pd(0)
   , lut(0)
   , contouring(false)
   , numberOfContours(0)
   , useLookupTable(false)

{
  this->range[0]=0;
  this->range[1]=1;
  
  this->ren = vtkSmartPointer<vtkRenderer>::New();
  this->renWin = vtkSmartPointer<vtkRenderWindow>::New();
  this->iRen = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  this->ivRen = vtkSmartPointer<vtkRenderWindowInteractor>::New();

  this->cf = vtkSmartPointer<vtkContourFilter>::New();
  this->m1 = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->m2 = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->a1 = vtkSmartPointer<vtkActor>::New();
  this->a2 = vtkSmartPointer<vtkActor>::New();
  
  this->pd = vtkSmartPointer<vtkPolyData>::New();
  
  this->lut = vtkSmartPointer<vtkLookupTable>::New();
 
  this->axes = vtkSmartPointer<vtkAxesActor>::New();
  this->widget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
  this->transform = vtkSmartPointer<vtkTransform>::New();
  this->assembly = vtkSmartPointer<vtkPropAssembly>::New();
}

PDVisualiser::~PDVisualiser() {} 

vtkRenderer* PDVisualiser::Ren() { return this->ren; }
vtkRenderWindow* PDVisualiser::RenWin() {return this->renWin; }
vtkRenderWindowInteractor* PDVisualiser::IRen() { return this->iRen; }

vtkLookupTable* PDVisualiser::GetLookupTable( ) { return this->lut; }

vtkPolyData* PDVisualiser::GetPolyData() { return this->pd; }
void PDVisualiser::SetPolyData(vtkPolyData * pd) { this->pd->DeepCopy(pd); }
vtkPolyData* PDVisualiser::GetPolyDataOutput() { return this->pd; }

void PDVisualiser::MakePolyDataPipeline()
{
  this->m1->SetInput(this->pd);
  this->m1->SetScalarRange(this->range);
  if ( this->useLookupTable )
    this->m1->SetLookupTable(lut);
  else
    this->m1->CreateDefaultLookupTable();
  this->a1->SetMapper(this->m1);
  this->ren->AddViewProp(this->a1);
  
  // Let's add an axis actor.
  //vtkAxesActor* axes2 = vtkAxesActor::New();

  // simulate a left-handed coordinate system
  // if transform->RotateY(90) is in the pipeline.
  //
  //vtkTransform* transform = vtkTransform::New();
  this->transform->Identity();
  //transform->RotateY(90);
  this->axes->SetShaftTypeToCylinder();
  this->axes->SetUserTransform( transform );
  //axes->SetXAxisLabelText( "w" );
  //axes2->SetYAxisLabelText( "v" );
  //axes2->SetZAxisLabelText( "u" );

  this->axes->SetTotalLength( 1.5, 1.5, 1.5 );
  this->axes->SetCylinderRadius( 0.500 * this->axes->GetCylinderRadius() );
  this->axes->SetConeRadius    ( 1.025 * this->axes->GetConeRadius() );
  this->axes->SetSphereRadius  ( 1.500 * this->axes->GetSphereRadius() );

  vtkTextProperty* tprop = this->axes->GetXAxisCaptionActor2D()->
      GetCaptionTextProperty();
  tprop->ItalicOn();
  tprop->ShadowOn();
  tprop->SetFontFamilyToTimes();

  this->axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->ShallowCopy( tprop );
  this->axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->ShallowCopy( tprop );

  // combine orientation markers into one with an assembly
  //
  //vtkPropAssembly* assembly = vtkPropAssembly::New();
  this->assembly->AddPart( this->axes );
  //assembly->AddPart( cube );

  // set up the widget
  //
  //vtkOrientationMarkerWidget* widget = vtkOrientationMarkerWidget::New();
  this->widget->SetOutlineColor( 0.9300, 0.5700, 0.1300 );
  this->widget->SetOrientationMarker( this->assembly );
  this->widget->SetInteractor( this->iRen );
  this->widget->SetViewport( 0.0, 0.0, 0.2, 0.2 );


  if ( this->contouring )
  {
    this->cf->SetInput(this->pd);
    this->cf->GenerateValues(this->numberOfContours,this->range);
    this->m2->SetInput(this->cf->GetOutput());
    this->m2->ScalarVisibilityOff();
    this->a2->SetMapper(this->m2);
    this->ren->RemoveViewProp(this->a2);
  }

  this->ren->AddViewProp(this->a1);
  //this->ren->AddViewProp(this->a2);
  this->ren->SetBackground(0.4, 0.4, 0.5);

  this->renWin->AddRenderer(this->ren);
  this->iRen->SetRenderWindow(this->renWin);
  this->iRen->Initialize();

  this->widget->SetEnabled( 1 );
  this->widget->InteractiveOff();
  this->widget->InteractiveOn();
  
  this->ren->ResetCamera();
  
  this->renWin->Render();
}

}
