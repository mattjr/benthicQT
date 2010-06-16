//
// C++ Implementation: DTMVolumeView
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufDTMVolumeView.h"
#include "ufDTMImage.h"
#include "ufImageData.h"
#include "ufDataCharacteristics.h"

#include <exception>
#include <sstream>
#include <string>
#include <cmath>
#include <cstring>

#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkDataArray.h>

namespace {
/**DTMVolumeView
Exception class for bounds checking.
@author Andrew J. P. Maclean
*/
class UnableToCalculateCuboids : public std::exception
{
  public:
  UnableToCalculateCuboids(char * fnName):
    fnName(fnName)
    {}

  virtual const char * what() const throw()
    {
      std::ostringstream os;
      os << "Unable to calculate the cuboids in " << this->fnName;
      static char s[255];
      std::memset(s,0,256);
      if ( os.str().size() < 255 )
      {
        for ( int i = 0; i < (int)os.str().size(); ++i )
          s[i] = os.str()[i];
      }
      else
        for ( int i = 0; i < 255; ++i )
          s[i] = os.str()[i];
      return s;
    }

  private:
    char * fnName;
};

}


using namespace UF::DTM;

DTMVolumeView::DTMVolumeView()
 : DTMVolume()
 , cuboids(0)
{
  this->cuboids = vtkSmartPointer<vtkPolyData>::New();

  /*
      g
    /  \
  e     h
  | \  /|
  |  f  |
  | c|  |
  |  |  |
  a  |  d
   \ | /
     b
  In the cuboid:
  a = index 0, b = index 1, c = index 2, d = index 3
  e = index 4, f = index 5, g = index 6, h = index 7
  */

  // These will correspond to the faces on the cube (right-handed ordering).
  // f0 (= face zero) - bottom
 this->cubeFaces[0][0] = 0;
 this->cubeFaces[0][1] = 3;
 this->cubeFaces[0][2] = 2;
 this->cubeFaces[0][3] = 1;

  // f1 - front
 this->cubeFaces[1][0] = 0;
 this->cubeFaces[1][1] = 1;
 this->cubeFaces[1][2] = 5;
 this->cubeFaces[1][3] = 4;

  // f2 - top
 this->cubeFaces[2][0] = 4;
 this->cubeFaces[2][1] = 5;
 this->cubeFaces[2][2] = 6;
 this->cubeFaces[2][3] = 7;

  // f3 - right relative to the cube
 this->cubeFaces[3][0] = 0;
 this->cubeFaces[3][1] = 4;
 this->cubeFaces[3][2] = 7;
 this->cubeFaces[3][3] = 3;

  // f4 back
 this->cubeFaces[4][0] = 2;
 this->cubeFaces[4][1] = 3;
 this->cubeFaces[4][2] = 7;
 this->cubeFaces[4][3] = 6;

  // f5 - left  relative to the cube
 this->cubeFaces[5][0] = 1;
 this->cubeFaces[5][1] = 2;
 this->cubeFaces[5][2] = 6;
 this->cubeFaces[5][3] = 5;

}


DTMVolumeView::~DTMVolumeView()
{
}

void DTMVolumeView::CalculateVolume( double & VolumeAbove, double & VolumeBelow )
{
  VolumeAbove = 0;
  VolumeBelow = 0;


  int refIJK[3];
  int refId[8];
  int actIJK[3];
  int actId[8];

  int refIJK_origin[3];
  int actIJK_origin[3];
  int leastUpperBound[3];
  this->FindIntersection(refIJK_origin,actIJK_origin,leastUpperBound);

  double v_above = 0;
  double v_below = 0;

  for ( int i = 0; i < 3; ++i )
  {
    refIJK[i] = refIJK_origin[i];
    actIJK[i] = actIJK_origin[i];
  }

  double spacing[3];
  this->referenceSurface->GetSpacing(spacing);

  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
  vtkSmartPointer<vtkDoubleArray> scalars = vtkSmartPointer<vtkDoubleArray>::New();
  vtkSmartPointer<vtkDoubleArray> volumeAbove = vtkSmartPointer<vtkDoubleArray>::New();
  vtkSmartPointer<vtkDoubleArray> volumeBelow = vtkSmartPointer<vtkDoubleArray>::New();
  vtkSmartPointer<vtkDoubleArray> volumeTotal = vtkSmartPointer<vtkDoubleArray>::New();
  vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();

  scalars->SetName("Faces");
  volumeAbove->SetName("Positive Volume");
  volumeBelow->SetName("Negative Volume");
  volumeTotal->SetName("Total Volume");

  double actualOrigin[3];
  this->actualSurface->GetOrigin(actualOrigin);
  double referenceOrigin[3];
  this->referenceSurface->GetOrigin(referenceOrigin);

  int startingPoint = 0;
  for ( int i = refIJK_origin[0]; i < leastUpperBound[0]; ++i )
  {
    refIJK[1] = refIJK_origin[1];
    actIJK[1] = actIJK_origin[1];
    for ( int j = refIJK_origin[1]; j < leastUpperBound[1]; ++j )
    {
      this->referenceSurface->GetVoxelIds(refIJK,refId);
      this->actualSurface->GetVoxelIds(actIJK,actId);
      // Check for ghosts, and, if found, the volume doesn't change. (VoxelVolume is zero in this case.)
      if ( !this->CheckGhosts(actId) && !this->CheckGhosts(refId) )
      {
        // This is where we build the polydata object.
        int cellCoords[8][3];
        // Ordering the cells.
        this->referenceSurface->PointCoord(refId[0],cellCoords[0]);
        this->referenceSurface->PointCoord(refId[1],cellCoords[1]);
        this->referenceSurface->PointCoord(refId[2],cellCoords[3]);
        this->referenceSurface->PointCoord(refId[3],cellCoords[2]);
        this->actualSurface->PointCoord(actId[0],cellCoords[4]);
        this->actualSurface->PointCoord(actId[1],cellCoords[5]);
        this->actualSurface->PointCoord(actId[2],cellCoords[7]);
        this->actualSurface->PointCoord(actId[3],cellCoords[6]);

        double cellVertices[8][3];
        double height[4];
        for (int ii = 0; ii < 4; ++ii)
        {
          // x,y
          for (int jj = 0; jj < 2; ++jj)
          {
            cellVertices[ii][jj] = cellCoords[ii][jj] * spacing[jj] + referenceOrigin[jj];
            cellVertices[ii+4][jj] = cellCoords[ii+4][jj] * spacing[jj] + actualOrigin[jj];
          }
          // z
          cellVertices[ii][2] = this->referenceSurface->GetScalars<double>()->at(refId[ii]);
          cellVertices[ii+4][2] = this->actualSurface->GetScalars<double>()->at(actId[ii]);
          height[ii] = cellVertices[ii+4][2] - cellVertices[ii][2];
        }

        bool Ok;
        VoxelVolume( refId, actId, spacing, v_above, v_below, Ok );
        if ( !(Ok && CheckTolerance(v_above + v_below)) )
        {
          refIJK[1]++;
          actIJK[1]++;
          continue;
        }

        switch ( this->CuboidVolumeSelector )
        {
          case 0:
            continue;
            break;
          case 1:
            VolumeAbove += v_above;
            VolumeBelow += v_below;
            MakePolyData (
                v_above, v_below,
                startingPoint, cellVertices,
                points, cells,
                scalars, volumeAbove,  volumeBelow,
                volumeTotal);
            break;
          case 2:
            if ( v_below == 0 )
            {
              VolumeAbove += v_above;
              MakePolyData (
                  v_above, v_below,
                  startingPoint, cellVertices,
                  points, cells,
                  scalars, volumeAbove,  volumeBelow,
                  volumeTotal);
            }
            break;
          case 3:
            if ( v_above == 0 )
            {
              VolumeBelow += v_below;
              MakePolyData (
                  v_above, v_below,
                  startingPoint, cellVertices,
                  points, cells,
                  scalars, volumeAbove,  volumeBelow,
                  volumeTotal);
            }
            break;
          case 4:
            if ( v_above != 0 && v_below != 0 )
            {
              VolumeAbove += v_above;
              VolumeBelow += v_below;
              MakePolyData (
                  v_above, v_below,
                  startingPoint, cellVertices,
                  points, cells,
                  scalars, volumeAbove,  volumeBelow,
                  volumeTotal);
            }
            break;
          default:
            break;
        }
      }
      refIJK[1]++;
      actIJK[1]++;
    }
    refIJK[0]++;
    actIJK[0]++;
  }

  pd->SetPoints(points);
  pd->SetPolys(cells);
  // We are colouring the faces. So we use GetCellData not GetPointData.
  pd->GetCellData()->AddArray(scalars);
  pd->GetCellData()->AddArray(volumeAbove);
  pd->GetCellData()->AddArray(volumeBelow);
  pd->GetCellData()->AddArray(volumeTotal);
  pd->GetCellData()->SetActiveScalars("Faces");

  DataCharacteristics dc;
  //dc.PrintPolyDataCharacteristics(pd);
  this->cuboids->DeepCopy(pd);
  //dc.PrintPolyDataCharacteristics(this->cuboids);
}

void DTMVolumeView::MakePolyData (
    double v_above, double v_below,
    int & startingPoint, double cellVertices[8][3],
    vtkPoints *  points, vtkCellArray * cells,
    vtkDoubleArray * scalars, vtkDoubleArray * volumeAbove, vtkDoubleArray * volumeBelow,
    vtkDoubleArray * volumeTotal
                                 )
{
  // Generate the indices for the faces of the cuboid.
  vtkIdType cf[6][4];
  for ( int ii = 0; ii < 6; ++ii )
    for ( int jj = 0; jj < 4; ++jj)
      cf[ii][jj]=this->cubeFaces[ii][jj]+startingPoint;
  startingPoint += 8;

  // Add the definition of the cuboid to the various data structures.

  for ( int ii = 0; ii < 8; ++ii )
  {
    points->InsertNextPoint(cellVertices[ii]);
  }

  double tv = v_above + v_below;
  for ( int ii = 0; ii < 6; ++ii )
  {
    cells->InsertNextCell(4,cf[ii]);
    double value = ii;
    scalars->InsertNextTuple(&value);
    volumeAbove->InsertNextTuple(&v_above);
    volumeBelow->InsertNextTuple(&v_below);
    volumeTotal->InsertNextTuple(&tv);
  }
}
