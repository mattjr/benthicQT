//
// C++ Implementation: cuboid
//
// Description: 
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "cuboid.h"

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkDataArray.h>
#include <vtkLookupTable.h>
#include <vtkAppendPolyData.h>

namespace VTK {

Cuboid::Cuboid ():
    cuboid(0)
    , tetrahedron(0)
    , lut(0)
{
  this->lut = vtkSmartPointer<vtkLookupTable>::New();
  this->MakeLookupTable();
  this->MakeTetrahedralIndices();
  this->cuboid = vtkSmartPointer<vtkPolyData>::New();
  for (int i = 0; i < 5; ++i)
  {
    vtkPolyData *tetra = vtkPolyData::New();
    tetrahedron.push_back(tetra);
    // Since the vector now owns the reference, it is safe to delete.
    tetra->Delete();
  }
  // These will correspond to the faces on the cube (right-handed ordering).
  // f0 (= face zero)
  this->cubeFaces[0][0] = 0;
  this->cubeFaces[0][1] = 1;
  this->cubeFaces[0][2] = 2;
  this->cubeFaces[0][3] = 3;
  
  // f1
  this->cubeFaces[1][0] = 0;
  this->cubeFaces[1][1] = 1;
  this->cubeFaces[1][2] = 5;
  this->cubeFaces[1][3] = 4;
  
  // f2
  this->cubeFaces[2][0] = 4;
  this->cubeFaces[2][1] = 5;
  this->cubeFaces[2][2] = 6;
  this->cubeFaces[2][3] = 7;
  
  // f3
  this->cubeFaces[3][0] = 3;
  this->cubeFaces[3][1] = 0;
  this->cubeFaces[3][2] = 4;
  this->cubeFaces[3][3] = 7;
  
  // f4
  this->cubeFaces[4][0] = 2;
  this->cubeFaces[4][1] = 3;
  this->cubeFaces[4][2] = 7;
  this->cubeFaces[4][3] = 6;
  
  // f5
  this->cubeFaces[5][0] = 1;
  this->cubeFaces[5][1] = 2;
  this->cubeFaces[5][2] = 6;
  this->cubeFaces[5][3] = 5;
 
}

Cuboid::~Cuboid() {
}

void Cuboid::MakeTetrahedralIndices ()
{
    // The following indexing dissects the cuboid into five
    // tetrahedra where (a..f) are points and ([][][]) denotes a vector:
    // T0 = acbf ([ac][ab][af]),
    // T1 = acdh ([ac][ad][ah]),
    // T2 = fhgc ([fh][fg][fc]),
    // T3 = fhea ([fh][fe][fa]),
    // T4 = acfh ([ac][af][ah]).
    // T0 ... T3 correspond to tetrahedra created by cutting off corners of
    // the cube, T4 is the remainder after the corners have been cut off.
    
  // acbf
  this->TetrahedralIndices[0][0] = 0;
  this->TetrahedralIndices[0][1] = 2;
  this->TetrahedralIndices[0][2] = 1;
  this->TetrahedralIndices[0][3] = 5;

  // part of f1 on the cube
  this->tetrahedralFaces[0][0][0] = 0;
  this->tetrahedralFaces[0][0][1] = 2;
  this->tetrahedralFaces[0][0][2] = 3;
  // f6 (internal on the cube)
  this->tetrahedralFaces[0][1][0] = 0;
  this->tetrahedralFaces[0][1][1] = 3;
  this->tetrahedralFaces[0][1][2] = 1;
  // part of f0 on the cube
  this->tetrahedralFaces[0][2][0] = 0;
  this->tetrahedralFaces[0][2][1] = 1;
  this->tetrahedralFaces[0][2][2] = 2;
  // part of f5 on the cube
  this->tetrahedralFaces[0][3][0] = 2;
  this->tetrahedralFaces[0][3][1] = 1;
  this->tetrahedralFaces[0][3][2] = 3;

    // acdh
  this->TetrahedralIndices[1][0] = 0;
  this->TetrahedralIndices[1][1] = 2;
  this->TetrahedralIndices[1][2] = 3;
  this->TetrahedralIndices[1][3] = 7;
  
  // f7 (internal on the cube)
  this->tetrahedralFaces[1][0][0] = 0;
  this->tetrahedralFaces[1][0][1] = 1;
  this->tetrahedralFaces[1][0][2] = 3;
  // part of f3 on the cube
  this->tetrahedralFaces[1][1][0] = 0;
  this->tetrahedralFaces[1][1][1] = 3;
  this->tetrahedralFaces[1][1][2] = 2;
  // part of f0 on the cube
  this->tetrahedralFaces[1][2][0] = 0;
  this->tetrahedralFaces[1][2][1] = 2;
  this->tetrahedralFaces[1][2][2] = 1;
  // part of f4 on the cube
  this->tetrahedralFaces[1][3][0] = 1;
  this->tetrahedralFaces[1][3][1] = 2;
  this->tetrahedralFaces[1][3][2] = 3;
  
    // fhgc
  this->TetrahedralIndices[2][0] = 5;
  this->TetrahedralIndices[2][1] = 7;
  this->TetrahedralIndices[2][2] = 6;
  this->TetrahedralIndices[2][3] = 2;
  
  // part of f2 on the cube
  this->tetrahedralFaces[2][0][0] = 0;
  this->tetrahedralFaces[2][0][1] = 2;
  this->tetrahedralFaces[2][0][2] = 1;
  // f9 (internal on the cube)
  this->tetrahedralFaces[2][1][0] = 0;
  this->tetrahedralFaces[2][1][1] = 1;
  this->tetrahedralFaces[2][1][2] = 3;
  // part of f5 on the cube
  this->tetrahedralFaces[2][2][0] = 0;
  this->tetrahedralFaces[2][2][1] = 3;
  this->tetrahedralFaces[2][2][2] = 2;
  // part of f4 on the cube
  this->tetrahedralFaces[2][3][0] = 1;
  this->tetrahedralFaces[2][3][1] = 2;
  this->tetrahedralFaces[2][3][2] = 3;
  
    // fhea
  this->TetrahedralIndices[3][0] = 5;
  this->TetrahedralIndices[3][1] = 7;
  this->TetrahedralIndices[3][2] = 4;
  this->TetrahedralIndices[3][3] = 0;
  
  // part of f2 on the cube
  this->tetrahedralFaces[3][0][0] = 0;
  this->tetrahedralFaces[3][0][1] = 1;
  this->tetrahedralFaces[3][0][2] = 2;
  // part of f1 on the cube
  this->tetrahedralFaces[3][1][0] = 0;
  this->tetrahedralFaces[3][1][1] = 2;
  this->tetrahedralFaces[3][1][2] = 3;
  // f8 (internal on the cube)
  this->tetrahedralFaces[3][2][0] = 0;
  this->tetrahedralFaces[3][2][1] = 3;
  this->tetrahedralFaces[3][2][2] = 1;
  // part of f3 on the cube
  this->tetrahedralFaces[3][3][0] = 1;
  this->tetrahedralFaces[3][3][1] = 3;
  this->tetrahedralFaces[3][3][2] = 2;
  
    // acfh
  this->TetrahedralIndices[4][0] = 0;
  this->TetrahedralIndices[4][1] = 2;
  this->TetrahedralIndices[4][2] = 5;
  this->TetrahedralIndices[4][3] = 7;
  
  // f8 (internal on the cube)
  this->tetrahedralFaces[4][0][0] = 0;
  this->tetrahedralFaces[4][0][1] = 2;
  this->tetrahedralFaces[4][0][2] = 3;
  // f7 (internal on the cube)
  this->tetrahedralFaces[4][1][0] = 0;
  this->tetrahedralFaces[4][1][1] = 3;
  this->tetrahedralFaces[4][1][2] = 1;
  // f6 (internal on the cube)
  this->tetrahedralFaces[4][2][0] = 0;
  this->tetrahedralFaces[4][2][1] = 1;
  this->tetrahedralFaces[4][2][2] = 2;
  // f9 (internal on the cube)
  this->tetrahedralFaces[4][3][0] = 2;
  this->tetrahedralFaces[4][3][1] = 1;
  this->tetrahedralFaces[4][3][2] = 3;
}

void Cuboid::SetVertices(double Pts[8][3], double SplittingFactor)
{
  /*
      h
    /  \ 
  e     g
  | \  /|
  |  f  |
  | d|  |
  |  |  |   
  a  |  c
   \ | / 
     b
  In the cuboid:  
  a = index 0, b = index 1, c = index 2, d = index 3
  e = index 4, f = index 5, g = index 6, h = index 7
  */  
  
  for (vtkIdType i = 0; i < 8; ++i)
    for(vtkIdType j = 0; j < 3; ++j)
      this->CubeVertices[i][j] = Pts[i][j];

  this->MakePDCuboid(this->CubeVertices,this->cubeFaces,this->cuboid);
  
  double cv[8][3];
  for ( vtkIdType i = 0; i < 8; ++i )
    for ( vtkIdType j = 0; j < 3; ++j )
      cv[i][j] = this->CubeVertices[i][j];
  // Shift each tetrahedron slightly.
  double a[3];
  double b[3];
  double c[3];
  for (int  h = 0;  h < 4; ++ h)
  {
    switch( h)
    {
    case 0:
      {
        for ( int i = 0; i < 3; ++i )
        {
          a[i] = cv[2][i] - cv[0][i];
          b[i] = cv[5][i] - cv[0][i];
        }
      }
    case 1:
      {
        for ( int i = 0; i < 3; ++i )
        {
          a[i] = cv[7][i] - cv[0][i];
          b[i] = cv[2][i] - cv[0][i];
        }
      }
    case 2:
      {
        for ( int i = 0; i < 3; ++i )
        {
          a[i] = cv[2][i] - cv[5][i];
          b[i] = cv[7][i] - cv[5][i];
        }
      }
    case 3:
      {
        for ( int i = 0; i < 3; ++i )
        {
          a[i] = cv[7][i] - cv[5][i];
          b[i] = cv[0][i] - cv[5][i];
        }
      }
    }
    this->Cross(a,b,c);
    for ( int i = 0; i < 3; ++i )
      c[i] = c[i] * SplittingFactor;
  
    for (int i = 0; i < 4; ++i)
      for(int j = 0; j < 3; ++j)
        cv[this->TetrahedralIndices[ h][i]][j] = this->CubeVertices[this->TetrahedralIndices[ h][i]][j] + c[j];
  
    this->MakePDTetrahedron(cv,this->TetrahedralIndices[h],this->tetrahedralFaces[h],this->tetrahedron[h]);
  }
  this->MakePDTetrahedron(this->CubeVertices,this->TetrahedralIndices[4],this->tetrahedralFaces[4],this->tetrahedron[4]);

  double faceColour[10];
  for(int i = 0; i < 10; ++i)
    faceColour[i] = i;
  for(int i = 0; i < 6; ++i)
    this->cuboid->GetCellData()->GetScalars()->InsertTuple(i,&faceColour[i]);

  this->tetrahedron[0]->GetCellData()->GetScalars()->InsertTuple(0,&faceColour[1]);
  this->tetrahedron[0]->GetCellData()->GetScalars()->InsertTuple(1,&faceColour[6]);
  this->tetrahedron[0]->GetCellData()->GetScalars()->InsertTuple(2,&faceColour[0]);
  this->tetrahedron[0]->GetCellData()->GetScalars()->InsertTuple(3,&faceColour[5]);

  this->tetrahedron[1]->GetCellData()->GetScalars()->InsertTuple(0,&faceColour[7]);
  this->tetrahedron[1]->GetCellData()->GetScalars()->InsertTuple(1,&faceColour[3]);
  this->tetrahedron[1]->GetCellData()->GetScalars()->InsertTuple(2,&faceColour[0]);
  this->tetrahedron[1]->GetCellData()->GetScalars()->InsertTuple(3,&faceColour[4]);

  this->tetrahedron[2]->GetCellData()->GetScalars()->InsertTuple(0,&faceColour[2]);
  this->tetrahedron[2]->GetCellData()->GetScalars()->InsertTuple(1,&faceColour[9]);
  this->tetrahedron[2]->GetCellData()->GetScalars()->InsertTuple(2,&faceColour[5]);
  this->tetrahedron[2]->GetCellData()->GetScalars()->InsertTuple(3,&faceColour[4]);

  this->tetrahedron[3]->GetCellData()->GetScalars()->InsertTuple(0,&faceColour[2]);
  this->tetrahedron[3]->GetCellData()->GetScalars()->InsertTuple(1,&faceColour[1]);
  this->tetrahedron[3]->GetCellData()->GetScalars()->InsertTuple(2,&faceColour[8]);
  this->tetrahedron[3]->GetCellData()->GetScalars()->InsertTuple(3,&faceColour[3]);

  this->tetrahedron[4]->GetCellData()->GetScalars()->InsertTuple(0,&faceColour[8]);
  this->tetrahedron[4]->GetCellData()->GetScalars()->InsertTuple(1,&faceColour[7]);
  this->tetrahedron[4]->GetCellData()->GetScalars()->InsertTuple(2,&faceColour[6]);
  this->tetrahedron[4]->GetCellData()->GetScalars()->InsertTuple(3,&faceColour[9]);


}

void Cuboid::MakePDCuboid(double vertices[8][3], vtkIdType faceIndices[6][4], vtkPolyData * cuboid)
{
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
  vtkSmartPointer<vtkDoubleArray> scalars = vtkSmartPointer<vtkDoubleArray>::New();
  vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();
  
  // Build it.
  for ( vtkIdType i = 0; i < 8; ++i )
  {
    points->InsertPoint(i,vertices[i]);
  }

  for ( vtkIdType i = 0; i < 6; ++i )
  {
    cells->InsertNextCell(4,faceIndices[i]);
    double value = i;
    scalars->InsertNextTuple(&value);
  }
  pd->SetPoints(points);
  pd->SetPolys(cells);
  // We are colouring the faces. So we use GetCellData not GetPointData.
  pd->GetCellData()->SetScalars(scalars);

  cuboid->DeepCopy(pd);
}

void Cuboid::MakePDTetrahedron(double vertices[8][3], vtkIdType tetrahederalIndices[4], vtkIdType faceIndices[4][3], vtkPolyData * tetrahedron)
{
  // Get the points making up the tetrahedron.
  double tv[4][3];
  for (int i = 0; i < 4; ++i)
    for(int j = 0; j < 3; ++j)
      tv[i][j] = vertices[tetrahederalIndices[i]][j];

  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
  vtkSmartPointer<vtkDoubleArray> scalars = vtkSmartPointer<vtkDoubleArray>::New();
  vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();

  // Build it.
  for ( int i = 0; i < 4; ++i )
  {
    points->InsertPoint(i,tv[i]);
  }

  for ( int i = 0; i < 4; ++i )
  {
    cells->InsertNextCell(3,faceIndices[i]);
    double value = i;
    scalars->InsertNextTuple(&value);
  }
  pd->SetPoints(points);
  pd->SetPolys(cells);
  // We are colouring the faces. So we use GetCellData not GetPointData.
  pd->GetCellData()->SetScalars(scalars);

  tetrahedron->DeepCopy(pd);

}

void Cuboid::GetCuboid( vtkPolyData * cuboid )
{
  cuboid->DeepCopy(this->cuboid);
}


void Cuboid::GetTetrahedron( int const & idx, vtkPolyData * tetrahedron )
{
  if  ( idx < 0 || idx > 4 )
    return;
  tetrahedron->DeepCopy(this->tetrahedron[idx]);
}

void Cuboid::GetTetrahedra( vtkPolyData * tetrahedra )
{
  vtkSmartPointer<vtkAppendPolyData> tha = vtkSmartPointer<vtkAppendPolyData>::New();

  for(int h = 0; h < 5; ++h)
  {
      tha->AddInput(tetrahedron[h]);
  }
  tha->Update();
  tetrahedra->DeepCopy(tha->GetOutput());
}

void Cuboid::MakeLookupTable()
{
  // We will colour the faces:
  // Face 0: Red (bottom)
  // Face 1: Green (front)
  // Face 2: Blue (top)
  // Face 3: Cyan (right - relative to the cube)
  // Face 4: Magenta (back)
  // Face 5: Yellow (left - relative to the cube)
  // Face 6: Dark Red (front - facing down)
  // Face 7: Dark Green (back - facing down)
  // Face 8: Dark Blue (front - facing up)
  // Face 9: Dark Cyan (back - facing up)
  // Faces 6..9 are internal ones.
  double colours[10][4] = {{1,0,0,1}, {0,1,0,1}, {0,0,1,1},
    {0,1,1,1}, {1,0,1,1}, {1,1,0,1},
    {0.5,0,0,1}, {0,0.5,0,1}, {0,0,0.5,1}, {0,0.5,0.5,1}
  };  
  // Colour the faces.
  this->lut->SetNumberOfTableValues(10);
  this->lut->SetTableRange(0,9);
  this->lut->Build();
  for ( int i = 0; i < 10; ++i )
  {
    this->lut->SetTableValue(i,colours[i]);
  }
  // Testing
//   double colour[10][3];
//   double alpha[10];
//   for ( int i = 0; i < 10; ++i )
//   {
//     this->lut->GetColor((double)i,colour[i]);
//     alpha[i] = this->lut->GetOpacity(i);
//     for ( int j = 0; j < 3; ++j )
//       std::cout << colour[i][j] << " ";
//     std::cout << alpha[i] << std::endl;
//   }
}

void Cuboid::GetLookupTable( vtkLookupTable * lut )
{
  lut->DeepCopy(this->lut);
}

void Cuboid::TetrahedralVertexToVector ( double const Vertices[4][3], double Vectors[3][3] )
{
  for ( int i = 0; i < 3; ++i )
  {
    Vectors[0][i] = Vertices[1][i] - Vertices[0][i];
    Vectors[1][i] = Vertices[2][i] - Vertices[0][i];
    Vectors[2][i] = Vertices[3][i] - Vertices[0][i];
  }
}

void Cuboid::TetrahedralVertexToVector ( double const a[3], double const b[3], double const c[3], double const d[3], double Vectors[3][3] )
{
  for ( int i = 0; i < 3; ++i )
  {
    Vectors[0][i] = b[i] - a[i];
    Vectors[1][i] = c[i] - a[i];
    Vectors[2][i] = d[i] - a[i];
  }
}

double Cuboid::TetrahedralVolume ( double const Vectors[3][3] )
{
  return this->TetrahedralVolume(Vectors[0],Vectors[1],Vectors[2]);
}

double Cuboid::TetrahedralVolume ( double const a[3], const double b[3], const double c[3] )
{
  double t[3];
  this->Cross(b,c,t);
  double res = this->Dot(a,t)/6;
  if ( res < 0 )
    return -res;
  else
    return res; 
}


double Cuboid::CuboidVolume( )
{
  // To calculate volume, we need to consider the following 
  // tetrahedra where (a..f) are points and ([][][]) denotes a vector:
  // T0 = acbf ([ac][ab][af]),
  // T1 = acdh ([ac][ad][ah]),
  // T2 = fhgc ([fh][fg][fc]),
  // T3 = fhea ([fh][fe][fa]),
  // T4 = acfh ([ac][af][ah]).
  // T0 ... T3 correspond to tetrahedra created by cutting off corners of
  // the cube, T4 is the remainder after the corners have been cut off.
    
  double T0[3][3];
  double T1[3][3];
  double T2[3][3];
  double T3[3][3];
  double T4[3][3];
    
  // If the cuboid is a cube (all sides equal) then
  // T0, T1, T2, T3 are four tetrahedra each with a 
  // volume of 1/6 of the cube.
  // T4 is a tetrahedra with a volume of 1/3 of the cube. 
  this->TetrahedralVertexToVector(
  this->CubeVertices[this->TetrahedralIndices[0][0]],
  this->CubeVertices[this->TetrahedralIndices[0][1]],
  this->CubeVertices[this->TetrahedralIndices[0][2]],
  this->CubeVertices[this->TetrahedralIndices[0][3]],
  T0);
  this->TetrahedralVertexToVector(
  this->CubeVertices[this->TetrahedralIndices[1][0]],
  this->CubeVertices[this->TetrahedralIndices[1][1]],
  this->CubeVertices[this->TetrahedralIndices[1][2]],
  this->CubeVertices[this->TetrahedralIndices[1][3]],
  T1);
  this->TetrahedralVertexToVector(
  this->CubeVertices[this->TetrahedralIndices[2][0]],
  this->CubeVertices[this->TetrahedralIndices[2][1]],
  this->CubeVertices[this->TetrahedralIndices[2][2]],
  this->CubeVertices[this->TetrahedralIndices[2][3]],
  T2);
  this->TetrahedralVertexToVector(
  this->CubeVertices[this->TetrahedralIndices[3][0]],
  this->CubeVertices[this->TetrahedralIndices[3][1]],
  this->CubeVertices[this->TetrahedralIndices[3][2]],
  this->CubeVertices[this->TetrahedralIndices[3][3]],
  T3);
  this->TetrahedralVertexToVector(
  this->CubeVertices[this->TetrahedralIndices[4][0]],
  this->CubeVertices[this->TetrahedralIndices[4][1]],
  this->CubeVertices[this->TetrahedralIndices[4][2]],
  this->CubeVertices[this->TetrahedralIndices[4][3]],
  T4);
  double va = TetrahedralVolume(T0);
  double vb = TetrahedralVolume(T1);
  double vc = TetrahedralVolume(T2);
  double vd = TetrahedralVolume(T3);
  double ve = TetrahedralVolume(T4);
  return va + vb + vc + vd + ve;
}


}



