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
#include <vtkLookupTable.h>

#include "cuboid.h"

#include "pdvisualiser.h"

using namespace VTK;

/**
 * Create a cuboid, partition it into tetrahedra.
*/
int main ( int /*argc*/, char** /*argv[]*/ )
{
  double lowerSpacing[2] = {1,1};
  double upperSpacing[2] = {1,1};
  double origin[3] = {-0.5,-0.5,-0.5};
  //double height[4] = {1,1,1,1}; // height of the sides ae,bf,cg,dh
  //double height[4] = {0,1,1,1}; // one height 0
  //double height[4] = {1,0,1,1}; // one height 0
  //double height[4] = {0,0,1,1.5}; // wedge
  //double height[4] = {0,1,1.5,0}; // 
  //double height[4] = {0,1,0,1}; // diagonal zero height (ridge running along the diagonal)
  //double height[4] = {1,0,1,0}; // diagonal zero height (valley running along the diagonal)
  //double height[4] = {0,0,0,1}; // three heights zero
  //double height[4] = {0,0,1,0}; // three heights zero
  // Assume a base height of 1
  //double height[4] = {1,2,2,2};
  //double height[4] = {2,1,2,2};
  //double height[4] = {1,1,1.5,2.0}; // wedge
  //double height[4] = {1,1.5,2.0,1}; // 
  //double height[4] = {1,2,1,2.5}; // one diagonal the same height (a ridge)
  //double height[4] = {2,1,2.5,1}; // one diagonal the same height (a valley)
  double height[4] = {1,1,1,2}; // three heights the same
  //double height[4] = {1,1,2,1}; // three heights the same

  double verts[8][3];
  // a
  verts[0][0] = origin[0];
  verts[0][1] = origin[1];
  verts[0][2] = origin[2];
  // b
  verts[1][0] = origin[0] + lowerSpacing[0];
  verts[1][1] = origin[1];
  verts[1][2] = origin[2];
  // c
  verts[2][0] = origin[0] + lowerSpacing[0];
  verts[2][1] = origin[1] + lowerSpacing[1];
  verts[2][2] = origin[2];
  // d
  verts[3][0] = origin[0];
  verts[3][1] = origin[1] + lowerSpacing[1];
  verts[3][2] = origin[2];
  // e
  verts[4][0] = origin[0];
  verts[4][1] = origin[1];
  verts[4][2] = origin[2] + height[0];
  // f
  verts[5][0] = origin[0] + upperSpacing[0];
  verts[5][1] = origin[1];
  verts[5][2] = origin[2] + height[1];
  // g
  verts[6][0] = origin[0] + upperSpacing[0];
  verts[6][1] = origin[1] + upperSpacing[1];
  verts[6][2] = origin[2] + height[2];
  // h
  verts[7][0] = origin[0];
  verts[7][1] = origin[1] + upperSpacing[1];
  verts[7][2] = origin[2] + height[3];
   
  // Generate the cube and partition it.
  Cuboid *cuboid = new Cuboid;
  cuboid->SetVertices(verts,0.01);
  
  vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();
    cuboid->GetCuboid(pd);
  double v = cuboid->CuboidVolume();

  std::cout << "Cuboid: "<< std::endl;
  std::cout << "Origin              (";
  for ( int i = 0; i < 3; ++i )
  {
    std::cout << origin[i];
    if ( i != 2 )
      std:: cout << ",";
    else
      std::cout << ")" << std::endl;
  }
  std::cout << "Lower Spacing       (";
  for ( int i = 0; i < 2; ++i )
  {
    std::cout << lowerSpacing[i];
    if ( i != 1 )
      std:: cout << ",";
    else
      std::cout << ")" << std::endl;
  }
  std::cout << "Upper Spacing       (";
  for ( int i = 0; i < 2; ++i )
  {
    std::cout << upperSpacing[i];
    if ( i != 1 )
      std:: cout << ",";
    else
      std::cout << ")" << std::endl;
  }
  std::cout << "Height of each side (";
  for ( int i = 0; i < 4; ++i )
  {
    std::cout << height[i];
    if ( i != 3 )
      std:: cout << ",";
    else
      std::cout << ")" << std::endl;
  }
  std::cout << "Volume               " << v << std::endl;
     
  vtkSmartPointer<vtkPolyData> th = vtkSmartPointer<vtkPolyData>::New();
  cuboid->GetTetrahedra(th);
  
  vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
    cuboid->GetLookupTable(lut);

  PDVisualiser *visualiser = new PDVisualiser;

  //visualiser->SetPolyData(pd);
  visualiser->SetPolyData(th);
  visualiser->GetLookupTable()->DeepCopy(lut);
  visualiser->UseLookupTableOn();
//  visualiser->SetScalarRange(0,5); // If a cuboid
  visualiser->SetScalarRange(0,9); // For all the tetrahedra
  visualiser->MakePolyDataPipeline();
  visualiser->IRen()->Start();

  delete visualiser;
  delete cuboid;
  
  return 0;
} 
