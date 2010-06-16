//
// C++ Implementation: ufOrientationMarker
//
// Description: 
//
//
// Author: Andrew Maclean <a.maclean@cas.edu.au>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufOrientationMarker.h"

#include "vtkAnnotatedCubeActor.h"
#include "vtkProperty.h"
#include "vtkAxesActor.h"
#include "vtkTransform.h"

#include "vtkTextProperty.h"
#include "vtkCaptionActor2D.h"

#include "vtkPropAssembly.h"

#include "vtkOrientationMarkerWidget.h"


using namespace UF::VTK;

ufOrientationMarker *ufOrientationMarker::pInstance_ = 0;
bool ufOrientationMarker::destroyed_ = false;


ufOrientationMarker::ufOrientationMarker()
{
  this->assembly = vtkSmartPointer<vtkPropAssembly>::New();
  this->cube = vtkSmartPointer<vtkAnnotatedCubeActor>::New();
  
  this->axesAP = vtkSmartPointer<vtkAxesActor>::New();
  this->transformAP = vtkSmartPointer<vtkTransform>::New();
  this->axesAN = vtkSmartPointer<vtkAxesActor>::New();
  this->transformAN = vtkSmartPointer<vtkTransform>::New();

  this->axesLH = vtkSmartPointer<vtkAxesActor>::New();
  this->transformLH = vtkSmartPointer<vtkTransform>::New();
  
  this->axesRH = vtkSmartPointer<vtkAxesActor>::New();
  this->transformRH = vtkSmartPointer<vtkTransform>::New();
  
  this->widget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();

  this->widget->SetOutlineColor( 0.9300, 0.5700, 0.1300 );
  this->widget->SetOrientationMarker( this->assembly );
  this->widget->SetViewport(  0.0, 0.0, 0.4, 0.4  );
}


ufOrientationMarker::~ufOrientationMarker()
{
  pInstance_ = 0;
  destroyed_ = true;
}

void ufOrientationMarker::MakeAnatomicalCube(std::string const & anteriorLabel,
                                           std::string const & posteriorLabel,
                                           std::string const & leftLabel,
                                           std::string const & rightLabel,
                                           std::string const & superiorLabel,
                                           std::string const & inferiorLabel)
{
  this->cube->SetXPlusFaceText ( anteriorLabel.c_str() ); // Anterior
  this->cube->SetXMinusFaceText( posteriorLabel.c_str() ); // Posterior
  this->cube->SetYPlusFaceText ( leftLabel.c_str() ); // Left
  this->cube->SetYMinusFaceText( rightLabel.c_str() ); // Right
  this->cube->SetZPlusFaceText ( superiorLabel.c_str() ); // Superior
  this->cube->SetZMinusFaceText( inferiorLabel.c_str() ); // Inferior

    // Change the vector text colors
  //
  vtkProperty* property;
  property = this->cube->GetXPlusFaceProperty();
  property->SetColor(0, 0, 1);
  property->SetInterpolationToFlat();
  property = this->cube->GetXMinusFaceProperty();
  property->SetColor(0, 0, 1);
  property->SetInterpolationToFlat();
  property = this->cube->GetYPlusFaceProperty();
  property->SetColor(0, 1, 0);
  property->SetInterpolationToFlat();
  property = this->cube->GetYMinusFaceProperty();
  property->SetColor(0, 1, 0);
  property->SetInterpolationToFlat();
  property = this->cube->GetZPlusFaceProperty();
  property->SetColor(1, 0, 0);
  property->SetInterpolationToFlat();
  property = this->cube->GetZMinusFaceProperty();
  property->SetColor(1, 0, 0);
  property->SetInterpolationToFlat();

  this->assembly->RemovePart( this->cube );
  this->assembly->AddPart( this->cube );
}

void ufOrientationMarker::MakeSixFoldAxes(std::string const & anteriorLabel,
                                           std::string const & posteriorLabel,
                                           std::string const & leftLabel,
                                           std::string const & rightLabel,
                                           std::string const & superiorLabel,
                                           std::string const & inferiorLabel,
                                           bool rightHanded)
{
  this->transformAP->Identity();
  if ( !rightHanded )
  {
    this->transformAP->Scale(1,-1,1);
  }
  this->axesAP->SetShaftTypeToCylinder();
  this->axesAP->SetUserTransform( this->transformAP );
  this->axesAP->SetXAxisLabelText( anteriorLabel.c_str() );
  this->axesAP->SetYAxisLabelText( leftLabel.c_str() );
  this->axesAP->SetZAxisLabelText( superiorLabel.c_str() );

  this->axesAP->SetTotalLength( 1.5, 1.5, 1.5 );
  this->axesAP->SetCylinderRadius( 0.500 * this->axesAP->GetCylinderRadius() );
  this->axesAP->SetConeRadius    ( 1.025 * this->axesAP->GetConeRadius() );
  this->axesAP->SetSphereRadius  ( 1.500 * this->axesAP->GetSphereRadius() );


  vtkTextProperty* tpropAP = this->axesAP->GetXAxisCaptionActor2D()->GetCaptionTextProperty();
  tpropAP->ItalicOn();
  tpropAP->ShadowOn();
  tpropAP->SetFontFamilyToTimes();

  this->axesAP->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->ShallowCopy( tpropAP );
  this->axesAP->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->ShallowCopy( tpropAP );
  
  // Now for the axes pointing in the negative direction.
  this->transformAN->Identity();
  if ( !rightHanded )
  {
    this->transformAN->Scale( -1, 1, -1 );
  }
  else
  {
    this->transformAN->Scale( -1, -1, -1 );
  }
  this->axesAN->SetShaftTypeToCylinder();
  this->axesAN->SetUserTransform( this->transformAN );
  this->axesAN->SetXAxisLabelText( posteriorLabel.c_str() );
  this->axesAN->SetYAxisLabelText( rightLabel.c_str() );
  this->axesAN->SetZAxisLabelText( inferiorLabel.c_str() );

  this->axesAN->SetTotalLength( 1.5, 1.5, 1.5 );
  this->axesAN->SetCylinderRadius( 0.500 * this->axesAN->GetCylinderRadius() );
  this->axesAN->SetConeRadius    ( 1.025 * this->axesAN->GetConeRadius() );
  this->axesAN->SetSphereRadius  ( 1.500 * this->axesAN->GetSphereRadius() );


  vtkTextProperty* tpropAN = this->axesAN->GetXAxisCaptionActor2D()->GetCaptionTextProperty();
  tpropAN->ItalicOn();
  tpropAN->ShadowOn();
  tpropAN->SetFontFamilyToTimes();

  this->axesAN->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->ShallowCopy( tpropAN );
  this->axesAN->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->ShallowCopy( tpropAN );
  
  this->assembly->RemovePart( this->axesAP );
  this->assembly->AddPart( this->axesAP );
  this->assembly->RemovePart( this->axesAN );
  this->assembly->AddPart( this->axesAN );

}

void ufOrientationMarker::MakeLeftHandedAxes(std::string const & xLabel,
                                             std::string const & yLabel,
                                             std::string const & zLabel)
{
  this->transformLH->Identity();
  this->transformLH->Scale(1,-1,1);
  this->axesLH->SetShaftTypeToCylinder();
  this->axesLH->SetUserTransform( this->transformLH );
  this->axesLH->SetXAxisLabelText( xLabel.c_str() );
  this->axesLH->SetYAxisLabelText( yLabel.c_str() );
  this->axesLH->SetZAxisLabelText( zLabel.c_str() );

  this->axesLH->SetTotalLength( 1.5, 1.5, 1.5 );
  this->axesLH->SetCylinderRadius( 0.500 * this->axesLH->GetCylinderRadius() );
  this->axesLH->SetConeRadius    ( 1.025 * this->axesLH->GetConeRadius() );
  this->axesLH->SetSphereRadius  ( 1.500 * this->axesLH->GetSphereRadius() );


  vtkTextProperty* tpropLH = this->axesLH->GetXAxisCaptionActor2D()->GetCaptionTextProperty();
  tpropLH->ItalicOn();
  tpropLH->ShadowOn();
  tpropLH->SetFontFamilyToTimes();

  this->axesLH->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->ShallowCopy( tpropLH );
  this->axesLH->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->ShallowCopy( tpropLH );
  
  this->assembly->RemovePart( this->axesLH );
  this->assembly->AddPart( this->axesLH );
}

void ufOrientationMarker::MakeRightHandedAxes(std::string const & xLabel,
                                              std::string const & yLabel,
                                              std::string const & zLabel)
{
  this->transformRH->Identity();
  //this->transformRH->RotateY(90);
  axesRH->SetShaftTypeToCylinder();
  axesRH->SetUserTransform( this->transformRH );
  this->axesRH->SetXAxisLabelText( xLabel.c_str() );
  this->axesRH->SetYAxisLabelText( yLabel.c_str() );
  this->axesRH->SetZAxisLabelText( zLabel.c_str() );

  this->axesRH->SetTotalLength( 1.5, 1.5, 1.5 );
  this->axesRH->SetCylinderRadius( 0.500 * this->axesRH->GetCylinderRadius() );
  this->axesRH->SetConeRadius    ( 1.025 * this->axesRH->GetConeRadius() );
  this->axesRH->SetSphereRadius  ( 1.500 * this->axesRH->GetSphereRadius() );


  vtkTextProperty* tpropRH = this->axesRH->GetXAxisCaptionActor2D()->GetCaptionTextProperty();
  tpropRH->ItalicOn();
  tpropRH->ShadowOn();
  tpropRH->SetFontFamilyToTimes();

  this->axesRH->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->ShallowCopy( tpropRH );
  this->axesRH->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->ShallowCopy( tpropRH );
  
  this->assembly->RemovePart( this->axesRH );
  this->assembly->AddPart( this->axesRH );
}

void ufOrientationMarker::RemoveAnatomicalCube()
{
  this->assembly->RemovePart( this->cube );
}

void ufOrientationMarker::RemoveLeftHandedAxes()
{
  this->assembly->RemovePart( this->axesLH );
}

void ufOrientationMarker::RemoveSixFoldAxes()
{
  this->assembly->RemovePart( this->axesAP );
  this->assembly->RemovePart( this->axesAN );
}

void ufOrientationMarker::RemoveRightHandedAxes()
{
  this->assembly->RemovePart( this->axesRH );
}

vtkOrientationMarkerWidget *ufOrientationMarker::GetOrientationMarkerWidget()
{
  return this->widget.GetPointer();
}

