#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iomanip>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkParticleReader.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkAppendPolyData.h>
#include <vtkImageData.h>

#include "ufObservers.h"
#include "ufVisualiser.h"
#include "ufProbeInput.h"
#include "ufProbeSource.h"
#include "ufProbe.h"
#include "ufLookupTables.h"
#include "ufDataCharacteristics.h"
#include "ufSaveData.h"
#include "ufPlaneTo3d.h"

using namespace UF;
using namespace UF::DTM;

/**
 * Read in some data from a file in x,y,z format.
 * Convert to polydata and probe it to create a plane whose z-value = 0
 * and the z-values from the original polydata are the scalars.
 * Then write it out.
 * You can modify the result by changing the values of scaleFactor, resolution
 * and bounds below.
 * Usage: DTMConverter x.txt y.bin
 * It will produce y.bin and y.bin.txt
*/
int main ( int argc, char* argv[] )
{
  if ( argc != 3 )
    {
    std::cout << "Need input file and output file." << std::endl;
    std::cout << "Usage: DTMConverter x.txt y.bin" << std::endl;
    std::cout << "y.bin and y.bin.txt will contain the data resampled as a plane." << std::endl;
    return 1;
    }
  std::string ifn = argv[1];
  std::string ofn = argv[2];
  std::string ofn1 = argv[2];
  ofn1 += ".txt";
  std::cerr << "Input file: " << ifn << std::endl;
  std::cerr << "Output file: " << ofn << std::endl;
  if ( ifn == ofn )
    {
    std::cerr << "File names cannot be the same." << std::endl;
    return 1;
    }

//  std::ifstream ifs(ifn.c_str());
//  std::ofstream ofs(ofn.c_str());

    vtkSmartPointer<DTM::vtkStartCommand> sobserver = vtkSmartPointer<DTM::vtkStartCommand>::New();
    vtkSmartPointer<DTM::vtkEndCommand> eobserver = vtkSmartPointer<DTM::vtkEndCommand>::New();
    vtkSmartPointer<DTM::vtkProgressCommand> pobserver = vtkSmartPointer<DTM::vtkProgressCommand>::New();

    // Select the reader to use and recompile.
    // TODO: Fix it up so that the reader is automatically selected.


  vtkSmartPointer<vtkParticleReader> pr = vtkSmartPointer<vtkParticleReader>::New();
    pr->SetFileName(ifn.c_str());
    pr->AddObserver( vtkCommand::StartEvent, sobserver );
    pr->AddObserver( vtkCommand::EndEvent, eobserver );
    pr->AddObserver( vtkCommand::ProgressEvent, pobserver );
  /*
    vtkSmartPointer<vtkXMLPolyDataReader> pr = vtkSmartPointer<vtkXMLPolyDataReader>::New();
      pr->SetFileName(ifn.c_str());
      pr->AddObserver( vtkCommand::StartEvent, sobserver );
      pr->AddObserver( vtkCommand::EndEvent, eobserver );
      pr->AddObserver( vtkCommand::ProgressEvent, pobserver );
  */
//    pr->SetDataTypeToDouble();
    //pr->SetFileTypeToBinary();
//    pr->HasScalarOff();
//    pr->SetDataByteOrderToBigEndian();
    pr->Update();

  ProbeSource *probeSource = new ProbeSource;
    probeSource->SetInput(pr->GetOutput());
    probeSource->GenerateOutputPolyData();

  vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();
     pd->DeepCopy(probeSource->GetOutput());

  double bounds[6];
  double range[2];
  int components[7];
   DataCharacteristics dc;
      cout << "Original data:" << endl;
      dc.PrintPolyDataCharacteristics(pd);
      cout << endl;
      dc.PolyDataCharacteristics(pd,components,bounds,range);

  double scaleFactor[3] = {1,1,1};
  double resolution[3] = {1,1,1}; // meters

  ProbeInput *probeInput = new ProbeInput;
    probeInput->SetBounds(bounds);
    probeInput->SetResolution(resolution);
    probeInput->SetScaleFactor(scaleFactor);

  Probe *probe = new Probe;
    probe->SetSource(probeSource->GetOutput());

  LookupTables *lut = new LookupTables;
  Visualiser *visualiser = new Visualiser;

    lut->Land(visualiser->GetLookupTable(),20);

    // Use these to calculate/display various views.
    bool polyDataProbe = true;
    bool imageProbe = true;
    bool displayImageData = false;
    bool displayVolumeData = false;

  if ( imageProbe )
    {
      probeInput->MakeImageDataPlane();
      probeInput->DisplayParameters();
      probe->SetImageDataProbe(probeInput->GetImageDataPlane());
      probe->DoImageDataProbe();
//      cout << "Resampled as image data: " << endl;
//      dc.PrintImageDataCharacteristics(probe->GetImageDataOutput());
//      dc.PrintImageDataScalars(probe->GetImageDataOutput());
      cout << endl;

      std::cout << "Saving the data." << std::endl;
      SaveData sd;
      sd.WriteImageData(probe->GetImageDataOutput(), ofn );
      sd.WriteImageDataAsParticles(probe->GetImageDataOutput(), ofn1, probe->GetImageDataOutput()->GetScalarRange(), false );
      std::cout << "Done." << std::endl;

      if ( displayImageData )
        {
        visualiser->SetImageData(probe->GetImageDataOutput());
        visualiser->MakeImageViewerPipeline();
        visualiser->IVRen()->Start();
        }
      if ( displayVolumeData )
        {
        visualiser->SetScalarRange(range[0]-1,range[1]);
//        visualiser->SetScalarRange(165,range[1]);
        visualiser->SetImageData(probe->GetImageDataOutput());
        visualiser->MakeVolumeViewerPipeline();
        visualiser->VIRen()->Start();
        }
    }

    if ( polyDataProbe )
    {
       probeInput->MakePolyDataPlane();
//       probeInput->DisplayParameters();
       probe->SetPolyDataProbe(probeInput->GetPolyDataPlane());
       probe->DoPolyDataProbe();
//
//       vtkSmartPointer<vtkAppendPolyData> apd = vtkSmartPointer<vtkAppendPolyData>::New();
//       //apd->AddInput(pd);
//       //apd->AddInput(probeInput->GetPlane());
//       apd->AddInput(probe->GetPolyDataOutput());
//       apd->Update();
//
//       cout << "Resampled as poly data " << endl;
//       dc.PrintPolyDataCharacteristics(probe->GetPolyDataOutput());
//       cout << endl;

      PlaneTo3D *ptd = new PlaneTo3D;

      ptd->SetInput(probe->GetPolyDataOutput());
      ptd->DoPlaneTo3D();
      vtkSmartPointer<vtkPolyData> pd1 = vtkSmartPointer<vtkPolyData>::New();

      pd1->DeepCopy(ptd->GetOutput());
      visualiser->SetPolyData(ptd->GetOutput());

//       visualiser->SetPolyData(apd->GetOutput());
      visualiser->UseLookupTableOn();
      visualiser->ContouringOn();
      visualiser->SetNumberOfContours(20);
      visualiser->SetScalarRange(range[0]-1,range[1]);
      visualiser->MakePolyDataPipeline();
      visualiser->IRen()->Start();
      delete ptd;
    }

  delete visualiser;
  delete probeSource;
  delete probeInput;
  delete probe;
  delete lut;

  return 0;
}
