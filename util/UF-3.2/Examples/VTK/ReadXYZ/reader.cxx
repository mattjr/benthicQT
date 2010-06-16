// Author: Andrew J. P. Maclean
// Purpose read in a dataset where each line consists of point
// with its position (x,y,z) and (possibly) a scalar.
// Then create and save a vtk polydata array suitable for further rendering.

#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkElevationFilter.h>
#include <vtkDataSet.h>
#include <vtkPointData.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

#include <vtkXMLPolyDataWriter.h>
#include <vtkXMLPolyDataReader.h>

#include <vtkParticleReader.h>

#include <vtkSmartPointer.h>


  

int main ( int argc, char* argv[] )
{
  if ( argc != 3 ) 
    {
      std::cout << "Error: I need an input file and an output file." << std::endl;
      std::cout << "Purpose: read in a dataset where each line consists of point" << std::endl;
      std::cout << "with its position (x,y,z) and (possibly) a scalar." << std::endl;
      std::cout << "Then create and save a vtk polydata array suitable for further rendering." << std::endl;
      std::cout << "The input file can be a text or a csv file." << std::endl;
      std::cout << "The output file should have a .vtp extension." << std::endl;
      std::cout << "Use ParaView (http://www.paraview.org) to read this file." << std::endl;
      std::cout << "Syntax: ReadXYZ Input.txt Output.vtp" << std::endl;
    return 1;
    }
  std::string ifn = argv[1];
  std::string ofn = argv[2];
  std::cerr << "Input file: " << ifn << std::endl;
  std::cerr << "Output file: " << ofn << std::endl; 
  if ( ifn == ofn )
    {
    std::cerr << "File names cannot be the same." << std::endl;
    return 1;
    }

  std::ifstream ifs;
  ifs.open(ifn.c_str());
  if ( !ifs )
    {
    std::cerr << ifn << " does not exist." << std::endl;
    return 1;
    }
  ifs.close();

  vtkSmartPointer<vtkParticleReader> reader = vtkSmartPointer<vtkParticleReader>::New();
     // Change to float if you want.
    reader->SetDataTypeToDouble();
    reader->SetFileName(ifn.c_str());
    reader->Update();

  vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    writer->SetInput(reader->GetOutput());
    writer->SetFileName(ofn.c_str());
    writer->Write();

    // Set up the pipeline.

    // Use the scalar data (if it exists) to colour the points.
    // Otherwise colour the points according to their z-coordinate.

    double bounds[6];
    reader->GetOutput()->GetBounds(bounds);
    std::cout << "Bounds: (xmin,xmax,ymin,ymax,zmin,zmax) = ("
              << bounds[0] << "," << bounds[1] << ","
              << bounds[2] << "," << bounds[3] << ","
              << bounds[4] << "," << bounds[5] << ")" << std::endl;
    std::cout << "Number of points: " << reader->GetOutput()->GetNumberOfPoints() << std::endl;

  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

    double scalarRange[2];
    reader->GetOutput()->GetPointData()->GetScalars()->GetRange(scalarRange);

    if ( scalarRange[0] == 0 && scalarRange[1] == 0 )
    {

      vtkSmartPointer<vtkElevationFilter> elevFilter = vtkSmartPointer<vtkElevationFilter>::New();
        elevFilter->SetInput(reader->GetOutput());
        elevFilter->SetLowPoint(bounds[0],bounds[2],bounds[4]);
        elevFilter->SetHighPoint(bounds[0],bounds[2],bounds[5]);
        elevFilter->SetScalarRange(bounds[4],bounds[5]);


        mapper->SetInputConnection(elevFilter->GetOutputPort());
        mapper->SetScalarRange(bounds[4],bounds[5]);
        mapper->SetColorModeToMapScalars();

    }
    else
    {

        std::cout << "Scalar range: ("
                  << scalarRange[0] << "," << scalarRange[1] << ")" << std::endl;
        mapper->SetInput(reader->GetOutput());
        mapper->SetScalarRange(scalarRange);
        mapper->SetColorModeToMapScalars();

    }

  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

  vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
    ren->SetBackground(0.05,0.05 ,0.1);

  vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(ren);
    renWin->SetSize(800 ,600);

  vtkSmartPointer<vtkRenderWindowInteractor> iRen = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    iRen->SetRenderWindow(renWin);

    ren->AddViewProp(actor);
    ren->Render();
    iRen->Initialize();
    iRen->Start();
}
