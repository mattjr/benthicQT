

//
// C++ Implementation: Visualiser
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "ufVisualiser.h"
#include "ufObservers.h"

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageViewer2.h>

#include <vtkContourFilter.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkActor.h>
#include <vtkLookupTable.h>

#include <vtkImageData.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkVolumeRayCastMapper.h>
#include <vtkVolume.h>
#include <vtkImageShiftScale.h>

#include "ufDataCharacteristics.h"

#include <vtkAxesActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextProperty.h>
#include <vtkTransform.h>

#include <vtkPropAssembly.h>
#include <vtkOrientationMarkerWidget.h>

using namespace UF::DTM;

Visualiser::Visualiser():
   ren(0)
   , renWin(0)
   , iRen(0)
   , ivRen(0)
   , iv(0)
   , cf(0)
   , m1(0)
   , m2(0)
   , a1(0)
   , a2(0)
   , pd(0)
   , id(0)
   , lut(0)
   , otf(0)
   , ctf(0)
   , vp(0)
   , vrccf(0)
   , vrcm(0)
   , vv(0)
   , vRen(0)
   , vRenWin(0)
   , vIRen(0)
   , shifter(0)
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
  this->iv = vtkSmartPointer<vtkImageViewer2>::New();

  this->cf = vtkSmartPointer<vtkContourFilter>::New();
  this->m1 = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->m2 = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->a1 = vtkSmartPointer<vtkActor>::New();
  this->a2 = vtkSmartPointer<vtkActor>::New();

  this->pd = vtkSmartPointer<vtkPolyData>::New();
  this->id = vtkSmartPointer<vtkImageData>::New();

  this->lut = vtkSmartPointer<vtkLookupTable>::New();

  this->otf = vtkSmartPointer<vtkPiecewiseFunction>::New();
  this->ctf = vtkSmartPointer<vtkColorTransferFunction>::New();
  this->vp = vtkSmartPointer<vtkVolumeProperty>::New();
  this->vrccf = vtkSmartPointer<vtkVolumeRayCastCompositeFunction>::New();
  this->vrcm = vtkSmartPointer<vtkVolumeRayCastMapper>::New();
  this->vv = vtkSmartPointer<vtkVolume>::New();
  this->vRen = vtkSmartPointer<vtkRenderer>::New();
  this->vRenWin = vtkSmartPointer<vtkRenderWindow>::New();
  this->vIRen = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  this->shifter = vtkSmartPointer<vtkImageShiftScale>::New();


  this->axes = vtkSmartPointer<vtkAxesActor>::New();
  this->widget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
  this->transform = vtkSmartPointer<vtkTransform>::New();
  this->assembly = vtkSmartPointer<vtkPropAssembly>::New();
}

Visualiser::~Visualiser() {}

vtkRenderer* Visualiser::Ren() { return this->ren; }
vtkRenderWindow* Visualiser::RenWin() {return this->renWin; }
vtkRenderWindowInteractor* Visualiser::IRen() { return this->iRen; }
vtkRenderWindowInteractor* Visualiser::IVRen() { return this->ivRen; }
vtkRenderWindowInteractor* Visualiser::VIRen() { return this->vIRen; }

vtkLookupTable * Visualiser::GetLookupTable( ) { return this->lut; }

vtkPolyData* Visualiser::GetPolyData() { return this->pd; }
void Visualiser::SetPolyData(vtkPolyData * pd) { this->pd->DeepCopy(pd); }
vtkPolyData* Visualiser::GetPolyDataOutput() { return this->pd; }

vtkImageData* Visualiser::GetImageData() { return this->id; }
void Visualiser::SetImageData(vtkImageData * id) { this->id->DeepCopy(id); }
vtkImageData* Visualiser::GetImageDataOutput() { return this->id; }

void Visualiser::MakePolyDataPipeline()
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
    this->ren->AddViewProp(this->a2);
    this->ren->SetBackground(0.4, 0.4, 0.5);

    this->renWin->AddRenderer(this->ren);
    this->iRen->SetRenderWindow(this->renWin);
    this->iRen->Initialize();

//    this->renWin->Render();

    this->widget->SetEnabled( 1 );
    this->widget->InteractiveOff();
    this->widget->InteractiveOn();

    this->ren->ResetCamera();

    this->renWin->Render();
}

void Visualiser::MakeImageViewerPipeline ()
{
    this->iv->SetInput(id);
    this->iv->SetColorWindow(255.0);
    this->iv->SetColorLevel(255.0);
    this->iv->SetSlice(0);
    this->iv->GetWindowLevel()->SetLookupTable(this->lut);

    this->iv->GetRenderer()->SetBackground(1, 1, 1);

    this->iv->SetupInteractor(this->ivRen);
    this->ivRen->Initialize();
    this->iv->Render();
//    this->iv->Print(cout);

}

void Visualiser::MakeVolumeViewerPipeline ()
{
    double diff = this->range[1] - this->range[0];
    double slope = 255.0 / diff;
    double inter = -slope * range[0];
    double shift = inter / slope;

    shifter->SetInput(id);
    shifter->SetShift(shift);
    shifter->SetScale(slope);
    shifter->SetOutputScalarTypeToUnsignedChar();
    shifter->ClampOverflowOn();
//    shifter->ReleaseDataFlagOff();
    shifter->Update();

  DataCharacteristics dc;
    cout << "Shifted data:" << endl;
    dc.PrintImageDataCharacteristics(shifter->GetOutput());
    dc.PrintImageDataScalars(shifter->GetOutput());
    cout << endl;

    // Create a transfer function mapping scalar values to opacity.
    this->otf->AddPoint(50,0.1);
    this->otf->AddPoint(200,1.0);
    // Create a transfer function mapping scalar value to colour.
    this->ctf->SetColorSpaceToRGB();
    this->ctf->AddRGBPoint(  0.0, 0.0, 0.0, 0.0);
    this->ctf->AddRGBPoint( 64.0, 1.0, 0.0, 0.0);
    this->ctf->AddRGBPoint(128.0, 0.0, 0.0, 1.0);
    this->ctf->AddRGBPoint(192.0, 0.0, 1.0, 0.0);
    this->ctf->AddRGBPoint(255.0, 1.0, 0.2, 1.0);
    // Thie volume property will describe how the data will look.
    this->vp->SetColor(this->ctf);
    this->vp->SetScalarOpacity(this->otf);
    // Mapper and ray casting for rendering the data.
    this->vrcm->SetVolumeRayCastFunction(this->vrccf);
    this->vrcm->SetInput(shifter->GetOutput());
    this->vrcm->Update();
    // Volume holds the mapper and property and can be
    // used to position/orient the volume.
    this->vv->SetMapper(this->vrcm);
    this->vv->SetProperty(this->vp);

    this->vRen->AddViewProp(this->vv);
    this->vRen->SetBackground(1.0, 1.0, 1.0);
    this->vRenWin->AddRenderer(this->vRen);
    this->vIRen->SetRenderWindow(this->vRenWin);
    this->vIRen->Initialize();
    this->vRenWin->Render();

}
