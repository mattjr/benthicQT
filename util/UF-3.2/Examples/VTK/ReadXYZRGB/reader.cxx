// Author: Andrew J. P. Maclean
// Purpose read in a dataset where each line consists of point
// with its position (x,y,z) and its colour (r,g,b).
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

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

#include <vtkXMLPolyDataWriter.h>
#include <vtkXMLPolyDataReader.h>

#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkDataArray.h>
//#include <vtkDoubleArray.h>
//#include <vtkFloatArray.h>
#include <vtkUnsignedCharArray.h>


#include <vtkSmartPointer.h>


  //! ParseLine - Read a line of three values of type T and three values of type U filtering out comments.
  /*
  * The ParseLine operator scans a string and returns 0 if it finds a 
  * comment symbol. Otherwise it returns 1 and three values corresponding 
  * to the position of the particle (x,y,z) along with three values (r,g,b) 
  * corresponding to the colour of the particle.
  * It is a good idea to set the values of point and colour to some 
  * predefined value before calling the operator.
   */
  template < typename T, typename U > 
    class ParseLine
  {
  public:
    ParseLine (): LookForEndString(false) {};
    int operator () (std::string &s, T point[3], U colour[3])
    {
      // Skip over comment lines.
      std::string::iterator itr;
      std::string tgt("/*");
      itr = std::search(s.begin(),s.end(),tgt.begin(),tgt.end());
      if ( itr != s.end() )
        {
        LookForEndString = true;
        //continue;
        }
      if ( LookForEndString )
        {
        tgt = "*/";
        itr = std::search(s.begin(),s.end(),tgt.begin(),tgt.end());
        if ( itr != s.end() )
          {
          LookForEndString = false;
          }
        //continue;
        return 0;
        }

      tgt = "//";
      itr = std::search(s.begin(),s.end(),tgt.begin(),tgt.end());
      if ( itr != s.end() )
        {
        return 0;
        }
      tgt = "%";
      itr = std::search(s.begin(),s.end(),tgt.begin(),tgt.end());
      if ( itr != s.end() )
        {
        return 0;
        }
      tgt = "#";
      itr = std::search(s.begin(),s.end(),tgt.begin(),tgt.end());
      if ( itr != s.end() )
        {
        return 0;
        }
      // If comma delimited, replace with tab
      std::replace(s.begin(),s.end(),',','\t');

      // We have data.
      std::ostringstream iss;
      iss << s << std::ends;
      std::istringstream oss(iss.str());
      oss >> point[0] >> point[1] >> point[2];
      oss >> colour[0] >> colour[1] >> colour[2];

      return 1;
    }

  private:
    bool LookForEndString;
    
  };
  

int main ( int argc, char* argv[] )
{
  if ( argc != 3 ) 
    {
    std::cout << "Need input file and output file." << std::endl;
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
    
  std::ifstream ifs(ifn.c_str());
//  std::ofstream ofs(ofn.c_str());

  if ( !ifs)
    {
    std::cerr << "Unable to open " << ifn << std::endl;
    }

  // Read in the data and build a PolyData structure.
  std::string s;
  
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  points->SetDataTypeToDouble();
  //points->Reset();

  vtkSmartPointer<vtkUnsignedCharArray> rgb = vtkSmartPointer<vtkUnsignedCharArray>::New();
  rgb->SetNumberOfComponents(3); // red,green,blue
  rgb->SetName("Colour");

  ifs.seekg(0,std::ios::beg);

  // Read in the data.
    ParseLine<double,int> pl;

    char buffer[256];
    while ( ifs.getline(buffer,256,'\n') )
      {
      s = buffer;
      if ( s.size() != 0 )
        {
        double pt[3];
        pt[0]=pt[1]=pt[2]=0;
        int tmp[4];
        for ( int i = 0; i < 4; ++i ) tmp[i] = 0;
        unsigned char sc[4];
        if ( pl(s,pt,tmp) )
          {
          for ( int i = 0; i < 3; ++i ) sc[i] = static_cast<unsigned char>(tmp[i]);
          points->InsertNextPoint(pt[0], pt[1], pt[2]);
          rgb->InsertNextTupleValue(sc);
          }
        }
      }

  vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();
  vertices->Reset();
  
  vtkIdType NumberOfPoints = points->GetNumberOfPoints();
  for( vtkIdType j = 0; j < NumberOfPoints; ++j )
    {
    vertices->InsertNextCell( 1 );
    vertices->InsertCellPoint( j );
    }

  // Put all the data together as a vtkPolyData structure.
  vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();
   pd->SetPoints(points);
   pd->SetVerts(vertices);
   //pd->GetPointData()->SetScalars(scalars);
   pd->GetPointData()->SetVectors(rgb);
   pd->Modified();

  vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    writer->SetInput(pd);
    writer->SetFileName(ofn.c_str());
    writer->Write();

    // Set up the pipeline.
    // We could have just used the PolyData we wrote out,
    // but I want to test reading in the data.
  vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
    reader->SetFileName(ofn.c_str());
    reader->Update();

  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInput(reader->GetOutput());
    mapper->SetColorModeToDefault();
    mapper->SetScalarModeToUsePointFieldData();
    mapper->SelectColorArray("Colour");

  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
  
  vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
    ren->SetBackground(0.2,0.2 ,0.3);

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
