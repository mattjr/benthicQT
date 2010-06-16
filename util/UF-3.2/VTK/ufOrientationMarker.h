//
// C++ Interface: ufOrientationMarker
//
// Description: 
//
//
// Author: Andrew Maclean <a.maclean@cas.edu.au>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef UFVTKUFORIENTATIONMARKER_H
#define UFVTKUFORIENTATIONMARKER_H

#include <string>
#include <stdexcept>

#include "ufWin32Header.h"


#include "vtkSmartPointer.h"

class vtkAnnotatedCubeActor;
class vtkAxesActor;
class vtkTransform;

class vtkPropAssembly;

class vtkOrientationMarkerWidget;
class vtkRenderWindowInteractor;

namespace UF {
  //! Some useful extensions to VTK.
  namespace VTK {

/**
 This static class creates and manages an orientation marker for use with VTK.

  @author Andrew Maclean <a.maclean@cas.edu.au>, (C) 2006

  Usage is straightforward.

  Firstly create the marker you want.
  \code
      UF::VTK::ufOrientationMarker::Instance()->MakeRightHandedAxes();
      // You may wish to change the viewport of the widget.
      UF::ufOrientationMarker::Instance()->GetOrientationMarkerWidget()->SetViewport( 0.0, 0.0, 0.3, 0.3 );
  \endcode

  Then integrate it into the visualisation pipeline. This should be done
  after initialising the vtkRenderWindowInteractor object (iren below)
  and before starting the interactor.
  \code
  this->ren1->Render();
  this->iren->Initialize();

  // Do your camera positioning etc. here.

  // Set the interactor and enable interaction with the widget.
  UF::VTK::ufOrientationMarker::Instance()->GetOrientationMarkerWidget()->SetInteractor( iren );
  UF::VTK::ufOrientationMarker::Instance()->GetOrientationMarkerWidget()->SetEnabled( 1 );
  UF::VTK::ufOrientationMarker::Instance()->GetOrientationMarkerWidget()->InteractiveOff();
  UF::VTK::ufOrientationMarker::Instance()->GetOrientationMarkerWidget()->InteractiveOn();

  this->ren1->ResetCameraClippingRange();
  this->iren->Start();
  \endcode

  Modified from the Kitware Test program:  /Widgets/Testing/Cxx/TestOrientationMarkerWidget.cxx 
  
*/
class UF_VTK_EXPORT ufOrientationMarker {
  
  public:
    //! Unique point of access for the singleton.
    static ufOrientationMarker* Instance()
    {
      if ( !pInstance_ )
      {
        // Check for a dead reference.
        if ( destroyed_ )
          OnDeadReference();
        else // First call initialise.
          Create();
      }
      return pInstance_;
    }

  private:
    //! Create a new singleton and store a pointer to it in pInstance_
    static void Create()
    {
      // Task: Initialise pInstance_
      static ufOrientationMarker theInstance;
      pInstance_ = &theInstance;
    }
    //! Gets called if a dead reference is detected.
    static void OnDeadReference()
    {
      throw std::runtime_error("Dead Reference Error in ufOrientationMarker.");
    }

    //! Private constructor, disables creation of the object by clients.
    ufOrientationMarker();

    // Make the destructor private in order to prevent clients
    // holding a pointer from accidentally deleting it.
    ~ufOrientationMarker();

    ufOrientationMarker( const ufOrientationMarker & ); // Disable the copy constructor.
    ufOrientationMarker& operator = ( const ufOrientationMarker & ); // Disable self-assignment.
  
    //! Pointer to the instance.
    static ufOrientationMarker * pInstance_;
    //! True if destroyed.
    static bool destroyed_;

  public:
    //! Make a cube representing the anatomical planes.
    /*!
      @param anteriorLabel a label for the anterior side.
      @param posteriorLabel a label for the posterior side.
      @param leftLabel a label for the left side.
      @param rightLabel a label for the right side.
      @param superiorLabel a label for the superior side.
      @param inferiorLabel a label for the inferior side.
      */
    void MakeAnatomicalCube(std::string const & anteriorLabel = "A",
                          std::string const & posteriorLabel = "P",
                          std::string const & leftLabel = "L",
                          std::string const & rightLabel = "R",
                          std::string const & superiorLabel = "S",
                          std::string const & inferiorLabel = "I");
    
    //! Make a labelled 6-f0ld axis.
    /*!
      This can also be used as a labelled 6-fold (x,y,z) axis e.g:
      \code
      MakeSixFoldAxes("x","-x","y","-y","z","-z");
      \endcode
      @param anteriorLabel a label for the anterior side.
      @param posteriorLabel a label for the posterior side.
      @param leftLabel a label for the left side.
      @param rightLabel a label for the right side.
      @param superiorLabel a label for the superior side.
      @param inferiorLabel a label for the inferior side.
      @param rightHanded if true, right-handed oriented axes 
             are created, if false then left-handed axes are created.
      */
    void MakeSixFoldAxes(std::string const & anteriorLabel = "A",
                          std::string const & posteriorLabel = "P",
                          std::string const & leftLabel = "L",
                          std::string const & rightLabel = "R",
                          std::string const & superiorLabel = "S",
                          std::string const & inferiorLabel = "I",
                          bool rightHanded = true);
    
    //! Make a left-handed axes marker.
    /*!
      @param xLabel a label for the x-axis.
      @param yLabel a label for the y-axis.
      @param zLabel a label for the z-axis.
      */
    void MakeLeftHandedAxes(std::string const & xLabel = "x",
                            std::string const & yLabel = "y",
                            std::string const & zLabel = "z");

    //! Make a right-handed axes marker.
    /*!
      @param xLabel a label for the x-axis.
      @param yLabel a label for the y-axis.
      @param zLabel a label for the z-axis.
      */
    void MakeRightHandedAxes(std::string const & xLabel = "x",
                            std::string const & yLabel = "y",
                            std::string const & zLabel = "z");

    //! Retrun a pointer to the otientation marker widget.
    vtkOrientationMarkerWidget *GetOrientationMarkerWidget();

    //! Remove the anatomical cube marker.
    void RemoveAnatomicalCube();
    
    //! Remove the anatomical axis marker.
    void RemoveSixFoldAxes();
    
    //! Remove the left-handed axis marker.
    void RemoveLeftHandedAxes();
    
    //! Remove the right-handed axis marker.
    void RemoveRightHandedAxes();

  private:
    // We will use an assembly to combine the orientation markers.
    vtkSmartPointer<vtkPropAssembly> assembly;

    // Anatomical labelling on a cube.
    vtkSmartPointer<vtkAnnotatedCubeActor> cube;
    // Anatomical axes
    vtkSmartPointer<vtkAxesActor> axesAP;
    vtkSmartPointer<vtkTransform> transformAP;
    vtkSmartPointer<vtkAxesActor> axesAN;
    vtkSmartPointer<vtkTransform> transformAN;
    // Left-handed axes
    vtkSmartPointer<vtkAxesActor> axesLH;
    vtkSmartPointer<vtkTransform> transformLH;
    // Right-handed axes
    vtkSmartPointer<vtkAxesActor> axesRH;
    vtkSmartPointer<vtkTransform> transformRH;

    vtkSmartPointer<vtkOrientationMarkerWidget> widget;

    vtkRenderWindowInteractor *iren;

};

  } // Namespace VTK
} // Namespace UF

#endif
