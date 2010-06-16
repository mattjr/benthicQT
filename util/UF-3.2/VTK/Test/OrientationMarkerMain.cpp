// ------------------------------------------------------------
// Tests a background gradient, a Mobuis
// Strip and an orientation marker.
// Andrew Maclean a.maclean@cas.edu.au
// ------------------------------------------------------------

// The command line arguments are:
// -I        => run in interactive mode; unless this is used, the program will
//              not allow interaction and exit

// ------------------------------------------------------------
// Includes
// ------------------------------------------------------------
#include "vtkSmartPointer.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleSwitch.h"

#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkQuad.h"
#include "vtkUnsignedCharArray.h"
#include "vtkCoordinate.h"
#include "vtkPolyDataMapper2D.h"
#include "vtkActor2D.h"
#include "vtkProperty2D.h"

#include "vtkParametricMobius.h"
#include "vtkParametricFunctionSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkCamera.h"

#include "ufOrientationMarker.h"
#include "vtkOrientationMarkerWidget.h"

//#define TEST_IMAGE
#ifdef TEST_IMAGE
  #include "vtkTestUtilities.h"
  #include "vtkRegressionTestImage.h"
#endif

#include "ufSystemTime.h"
#include "ufLogger.h"

#include <iostream>
#include <iomanip>
#include <sstream>

#ifdef __QNXNTO__
#include <unistd.h>
#endif

//#define WRITE_IMAGE
#ifdef WRITE_IMAGE
#include "vtkWindowToImageFilter.h"
#include "vtkPNGWriter.h"
#endif 
using namespace UF;
using namespace UF::VTK;
using namespace UF::Log;

static double const quadVerts[4][3] = {{0,0,0},{1,0,0},{1,1,0},{0,1,0}};
// Order is bottom left, bottom right, top right, top left RGBA colour.
static unsigned char const colours1[4][4] = {{0,0,255,255},{0,0,255,255},{255,255,255,255},{255,255,255,255}};
static unsigned char const colours2[4][4] = {{255,0,0,255},{0,0,255,255},{255,0,0,255},{0,255,0,255}};

 // ------------------------------------------------------------
 // Create a mobius strip
 // ------------------------------------------------------------
 int OrientationMarkerMain (int argc, char* argv[])
 {

  // Get rid of the argc, argv unreferenced formal parameter warnings.
  int numargs;
  std::string programName;
  numargs = argc;
  if ( argc != 0 )
  {
    programName = argv[0];
  }

  std::string sep = "------------------------------------------------------------------------";
  // Set up the log file.
  ufLogger *pLog = new ufLogger;
  pLog->open("TestVTKOrientationMarker.log",std::ios::out);
  std::string time;
  Time::SystemTime *pClock = Time::SystemTime::Instance();
  pClock->ISOLocalTime(time);
  *pLog << sep << "\nStarted:  " << time << std::endl;

  *pLog << "Testing the Orientation Marker" << std::endl;

  // ------------------------------------------------------------
  // Make the background gradient.
  // ------------------------------------------------------------
  vtkSmartPointer<vtkPoints> quadPoints = vtkSmartPointer<vtkPoints>::New();
    quadPoints->SetNumberOfPoints(4);

  vtkSmartPointer<vtkQuad> aQuad = vtkSmartPointer<vtkQuad>::New();

  vtkSmartPointer<vtkUnsignedCharArray> uchars = vtkSmartPointer<vtkUnsignedCharArray>::New();
    uchars->SetNumberOfComponents(4);
    uchars->SetNumberOfTuples(4);
    for(int i = 0; i <4; ++i)
    {
      quadPoints->InsertPoint(i,quadVerts[i]);
      aQuad->GetPointIds()->SetId(i,i);
      uchars->InsertTupleValue(i,colours1[i]);
   }

  vtkSmartPointer<vtkPolyData> data = vtkSmartPointer<vtkPolyData>::New();
    data->Allocate(1,1);
    data->InsertNextCell(aQuad->GetCellType(),aQuad->GetPointIds());
    data->SetPoints(quadPoints);
    data->GetPointData()->SetScalars(uchars);
    
  vtkSmartPointer<vtkCoordinate> coord = vtkSmartPointer<vtkCoordinate>::New();
    coord->SetCoordinateSystemToNormalizedDisplay();

  vtkSmartPointer<vtkPolyDataMapper2D> mapper2d =  vtkSmartPointer<vtkPolyDataMapper2D>::New();
    mapper2d->SetInput(data);
    mapper2d->SetTransformCoordinate(coord);
    
  vtkSmartPointer<vtkActor2D> actor2d = vtkSmartPointer<vtkActor2D>::New();
    actor2d->SetMapper(mapper2d);
    actor2d->GetProperty()->SetDisplayLocationToBackground();

  // ------------------------------------------------------------
  // Select the function and source and then connect them to the
  // mapper and actor.
  // ------------------------------------------------------------
  vtkSmartPointer<vtkParametricMobius> mobius
      = vtkParametricMobius::New();
  vtkSmartPointer<vtkParametricFunctionSource> mobiusSource
      = vtkSmartPointer<vtkParametricFunctionSource>::New();

  vtkSmartPointer<vtkPolyDataMapper> mobiusMapper
      = vtkSmartPointer<vtkPolyDataMapper>::New();

  vtkSmartPointer<vtkActor> mobiusActor
      = vtkSmartPointer<vtkActor>::New();

    mobiusSource->SetParametricFunction(mobius);
    mobiusSource->SetScalarModeToV();
    mobiusMapper->SetInputConnection(mobiusSource->GetOutputPort());
    mobiusMapper->SetScalarRange(-1, 1);
    mobiusActor->SetMapper(mobiusMapper);
    mobiusActor->RotateX(45);
    mobiusActor->RotateZ(-10);

  // ------------------------------------------------------------
  // Select an orientation marker
  // ------------------------------------------------------------
    UF::VTK::ufOrientationMarker::Instance()->MakeRightHandedAxes();
    UF::VTK::ufOrientationMarker::Instance()->GetOrientationMarkerWidget()->SetViewport( 0.0, 0.0, 0.3, 0.3 );

  // ------------------------------------------------------------
  // Create the RenderWindow, Renderer and Interactor
  // ------------------------------------------------------------
  vtkSmartPointer<vtkRenderer> ren1
      = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renWin
      = vtkSmartPointer<vtkRenderWindow>::New();
  vtkSmartPointer<vtkRenderWindowInteractor> iren
      = vtkSmartPointer<vtkRenderWindowInteractor>::New();

    renWin->AddRenderer(ren1);
    iren->SetRenderWindow(renWin);

  // ------------------------------------------------------------
  // Add actors and render the scene.
  // ------------------------------------------------------------

    ren1->AddViewProp(actor2d);
    ren1->AddViewProp(mobiusActor);

    ren1->SetBackground(0.0, 0.5, 0.0);
    renWin->SetSize(300,300);
    renWin->SetWindowName("Mobius Strip");
#if defined(TEST_IMAGE) || defined(WRITE_IMAGE)
    ren1->Render();
#endif

    mobiusActor->SetPosition(0, -0.35, 0);
    ren1->GetActiveCamera()->Zoom(1.9);

    // Add in the orientation marker.
    UF::VTK::ufOrientationMarker::Instance()->GetOrientationMarkerWidget()->SetInteractor( iren );
    UF::VTK::ufOrientationMarker::Instance()->GetOrientationMarkerWidget()->SetEnabled( 1 );
    UF::VTK::ufOrientationMarker::Instance()->GetOrientationMarkerWidget()->InteractiveOff();
    UF::VTK::ufOrientationMarker::Instance()->GetOrientationMarkerWidget()->InteractiveOn();


    vtkInteractorStyleSwitch *style = vtkInteractorStyleSwitch::SafeDownCast(iren->GetInteractorStyle());
    style->SetCurrentStyleToTrackballCamera();

#if defined(TEST_IMAGE) || defined(WRITE_IMAGE)
    iren->Initialize();
    ren1->ResetCameraClippingRange();
#endif

#ifdef WRITE_IMAGE
  vtkWindowToImageFilter *windowToImage = vtkWindowToImageFilter::New();
  windowToImage->SetInput( renWin );

  vtkPNGWriter *PNGWriter = vtkPNGWriter::New();
  PNGWriter->SetInputConnection( windowToImage->GetOutputPort() );
  windowToImage->Delete();
  PNGWriter->SetFileName("OrientationMarkerMain.png");
  PNGWriter->Write();
  PNGWriter->Delete();
#endif

#ifdef TEST_IMAGE
   int retVal = vtkRegressionTestImage( renWin );
    if ( retVal == vtkRegressionTester::DO_INTERACTOR)
      {
      iren->Start();
      }
#else
  int retVal = 1;
#endif

  pClock->ISOLocalTime(time);
  *pLog << ((retVal == 0)?"Failed.":"Passed") << "\n"
      << "Finished: " << time << "\n" << sep << std::endl;

  delete pLog;

  return !retVal;
}
