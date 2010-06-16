//
// C++ Implementation: DTMVolume
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "ufDTMVolume.h"

#include <exception>
#include <sstream>
#include <string>
#include <cmath>
#include <cstring>

#include <iostream>
#include <iomanip>

#include "ufDTMImage.h"


namespace {
/**
Exception class for bounds checking.
@author Andrew J. P. Maclean
*/
class UnableToCalculateVolume : public std::exception
{
public:
  UnableToCalculateVolume(char * fnName):
    fnName(fnName)
  {}

  virtual const char * what() const throw()
  {
    std::ostringstream os;
    os << "Unable to calculate the volume in " << this->fnName;
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

DTMVolume::DTMVolume():
  actualSurface(0)
  , referenceSurface(0)
  , FilterOnTolerance(false)
  , Tolerance(0)
  , FilterOnRange(false)
  , FilterOnHeight(false)
  , CuboidVolumeSelector(1)
{
  this->actualSurface = new DTMImage;
  this->referenceSurface = new DTMImage;
  for (int i = 0; i < 2; ++i)
  {
    Range[i] = 0;
    Height[i] = 0;
  }
}


DTMVolume::~DTMVolume()
{
  if ( this->actualSurface )
    delete this->actualSurface;
  if ( this->referenceSurface )
    delete this->referenceSurface;
}

void DTMVolume::SetActualSurface( DTMImage const & ActualSurface )
{
  *this->actualSurface = ActualSurface;
}

void DTMVolume::SetReferenceSurface( DTMImage const & ReferenceSurface )
{
  *this->referenceSurface = ReferenceSurface;
}

void DTMVolume::CopyReferenceSurfaceToActualSurface( )
{
  *this->actualSurface = *this->referenceSurface;
}

void DTMVolume::CopyActualSurfaceToReferenceSurface( )
{
  *this->referenceSurface = *this->actualSurface;
}

bool DTMVolume::CheckSpacing( )
{
  bool res = true;
  double actualSpacing[3];
  this->actualSurface->GetSpacing(actualSpacing);
  double referenceSpacing[3];
  this->referenceSurface->GetSpacing(referenceSpacing);
  for (int i = 0; i < 3; ++i )
    {
      res &= actualSpacing[i] == referenceSpacing[i];
    }
  return res;
}


bool DTMVolume::CheckOrigin( )
{
  bool res = true;
  double actualOrigin[3];
  this->actualSurface->GetOrigin(actualOrigin);
  double referenceOrigin[3];
  this->referenceSurface->GetOrigin(referenceOrigin);
  for (int i = 0; i < 3; ++i )
    {
      res &= actualOrigin[i] == referenceOrigin[i];
    }
  return res;
}

void DTMVolume::FindCommonOrigin( double origin[] )
{
  double actualOrigin[3];
  this->actualSurface->GetOrigin(actualOrigin);
  double referenceOrigin[3];
  this->referenceSurface->GetOrigin(referenceOrigin);

  if ( this->CheckOrigin() )
  {
    for ( int i = 0; i < 3; ++i )
      origin[i] = referenceOrigin[i];
  }
  else
  {
    for ( int i = 0; i < 3; ++i )
      origin[i] =  actualOrigin[i] <= referenceOrigin[i] ? referenceOrigin[i]: actualOrigin[i];
  }
}

void DTMVolume::FindIntersection( int RO[3], int AO[3], int LUB[] )
//    void DTMVolume::FindLeastUpperBound( int leastUpperBound[] )
{
  // Common origin for the reference and actual surface.
  double co[3];
  this->FindCommonOrigin(co);
  // Origins of the reference and actual surface.
  this->referenceSurface->FindClosestStructuredPoint(co,RO); // Relative to the reference surface.
  this->actualSurface->FindClosestStructuredPoint(co,AO); // Relative to the actual surface.
  // Dimensions of the actual and reference surface.
  int ad[3];
  int rd[3];
  this->referenceSurface->GetDimensions(rd);
  this->actualSurface->GetDimensions(ad);
  for (int i = 0; i < 3; ++i)
    LUB[i] = std::min(rd[i]-RO[i],ad[i]-AO[i]) + RO[i];
}


void DTMVolume::CalculateVolume( double & VolumeAbove, double & VolumeBelow )
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
            break;
          case 1:
            VolumeAbove += v_above;
            VolumeBelow += v_below;
            break;
          case 2:
            if ( v_below == 0 )
              VolumeAbove += v_above;
            break;
          case 3:
            if ( v_above == 0 )
              VolumeBelow += v_below;
            break;
          case 4:
            if ( v_above != 0 && v_below != 0 )
            {
              VolumeAbove += v_above;
              VolumeBelow += v_below;
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

}

bool DTMVolume::CheckGhosts( int Id[] )
{
  bool res = true;
  res &= Id[0] == Id[1];
  res &= Id[0] == Id[2];
  // No need to check any more.
  return res;

}

void DTMVolume::VoxelVolume( int RefId[], int ActId[], double Spacing[], double & VolumeAbove, double & VolumeBelow, bool & Ok )
{
  VolumeAbove = 0;
  VolumeBelow = 0;
  Ok = false;

  double height[4];
  int ActualPointPos[4]; // Position of the actual point relative to the reference point.
  double CubeVertices[8][3];

  this->MakeCuboid(RefId, ActId, Spacing, CubeVertices);

  for ( int i = 0; i < 4; ++i )
    {
      height[i] = CubeVertices[i+4][2] - CubeVertices[i][2];

      if ( CubeVertices[i+4][2] > CubeVertices[i][2] )
        ActualPointPos[i] = 1; // Actual point is above the reference point.
      else
      {
        if ( CubeVertices[i+4][2] < CubeVertices[i][2] )
          ActualPointPos[i] = -1; // Actual point is below.
        else
          ActualPointPos[i] = 0; // Actual point is equal to the reference point.
      }
   }

  bool allAbove;
  bool allBelow;
  bool allEqual;
  this->VoxelSign(ActualPointPos,allAbove,allBelow,allEqual);

  if ( allEqual )
  {
    return;
  }

  if ( !this->CheckRange(CubeVertices) )
    return;

  if ( !this->CheckHeight(height) )
    return;

  Ok = true;

  if ( allAbove )
  {
    this->CuboidVolume(CubeVertices, VolumeAbove);
    return;
  }
  if ( allBelow )
  {
    this->CuboidVolume(CubeVertices, VolumeBelow);
    return;
  }

  // Calculating the volumes of these remaining cuboids is problematical.
  // It depends upon the permutation of the points in the x-y plane.
  // The methodology below gives the closest approximation to the true
  // volume. Using just the cuboids (the commented out code) gives a poorer
  // fit because we don't control the permutation.

  // We now split the cube into two new cubes where one is above the reference
  // surface and the other is below the reference surface, and then calculate
  // the volumes for each case.
  double ht[4];
  // Above the reference surface.
  for ( int i = 0; i < 4; ++i )
    {
      if ( ActualPointPos[i] > 0 )
        ht[i] = height[i];
      else
        ht[i] = 0;
    }
  allEqual = true;
  for ( int i = 0; i < 3; ++i )
      allEqual &= ht[i] == ht[i+1];
  if ( !allEqual )
  {
    /*
    double cv[8][3];
    for (int i = 0; i < 8; ++i)
      for (int j = 0; j < 2; ++j)
        cv[i][j] = CubeVertices[i][j];
    for (int i = 0; i < 8; ++i)
      cv[i][2] = 0;
    for (int i = 0; i < 4; ++i)
      cv[i+4][2] = ht[i];
    double va;
    this->CuboidVolume(cv, va);
    */
    VolumeAbove = PrismVolume(ht,Spacing);
  }
  // Below the reference surface.
  for ( int i = 0; i < 4; ++i )
  {
    if ( ActualPointPos[i] < 0 )
      ht[i] = fabs(height[i]);
    else
      ht[i] = 0;
  }
  allEqual = true;
  for ( int i = 0; i < 3; ++i )
      allEqual &= ht[i] == ht[i+1];
  if ( !allEqual )
  {
    /*
    double cv[8][3];
    for (int i = 0; i < 8; ++i)
      for (int j = 0; j < 2; ++j)
        cv[i][j] = CubeVertices[i][j];
    for (int i = 0; i < 8; ++i)
      cv[i][2] = 0;
    for (int i = 0; i < 4; ++i)
      cv[i+4][2] = ht[i];
    double vb;
    this->CuboidVolume(cv, vb);
    */
    VolumeBelow = PrismVolume(ht,Spacing);
  }
}

void DTMVolume::VoxelSign( int ActualPointPos[] , bool & allAbove , bool & allBelow, bool &allEqual )
{
  allAbove = true;
  allBelow = true;
  allEqual = true;

  for ( int i = 0; i < 4; ++i )
  {
    allAbove &= ActualPointPos[i] > 0;
    allBelow &= ActualPointPos[i] < 0;
    allEqual &= ActualPointPos[i] == 0;
  }

}

double DTMVolume::PrismVolume ( double Height[], double Spacing[] )
{
  // Essentially we dissect the surface enclosing the volume into a
  // cuboid and a remainder that can be dissected into tetrahedera.
  // The remainder is a cuboid with one or more corners cut off. The
  // volume of the remainder is computed by summing over volumes of the
  // tetrahedra that make up the volume.

  double h_min = Height[0];
  for ( int i = 1; i < 4; ++i )
    h_min = std::min(h_min,Height[i]);
  // The volume of the cuboid.
  double V1 = std::fabs(h_min) * Spacing[0] * Spacing[1];

  // Now to compute the volume of the remainder that has a
  // rectangular base and different heights at each corner.
  // We also reorder the points so we traverse them anti-clockwise
  // if the indices are increasing.
  double h[4];
  bool zeroes[4];
  int numOfZeroes = 0;
  h[0] = Height[0] - h_min;
  h[1] = Height[1] - h_min;
/*  h[2] = Height[3] - h_min;
  h[3] = Height[2] - h_min;*/
  h[2] = Height[2] - h_min;
  h[3] = Height[3] - h_min;
  for ( int i = 0; i < 4; ++i )
  {
    zeroes[i] = h[i] == 0;
    if ( zeroes[i] )
      ++numOfZeroes;
    }

  double V2 = 0;

  if ( numOfZeroes >0 && numOfZeroes < 4 )
  {
    double CubeVertices[8][3];
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
    Original data:   a = index 0, b = index 1, c = index 3, d = index 2
    In the cuboid:  a = index 0, b = index 1, c = index 2, d = index 3
                     e = index 4, f = index 5, g = index 6, h = index 7
    */

    // Base
    // a
    CubeVertices[0][0] = 0;
    CubeVertices[0][1] = 0;
    CubeVertices[0][2] = 0;
    // b
    CubeVertices[1][0] = Spacing[0];
    CubeVertices[1][1] = 0;
    CubeVertices[1][2] = 0;
    // c
    CubeVertices[2][0] = Spacing[0];
    CubeVertices[2][1] = Spacing[1];
    CubeVertices[2][2] = 0;
    // d
    CubeVertices[3][0] = 0;
    CubeVertices[3][1] = Spacing[1];
    CubeVertices[3][2] = 0;
    // Top
    // e
    CubeVertices[4][0] = 0;
    CubeVertices[4][1] = 0;
    CubeVertices[4][2] = h[0];
    // f
    CubeVertices[5][0] = Spacing[0];
    CubeVertices[5][1] = 0;
    CubeVertices[5][2] = h[1];
    // g
    CubeVertices[6][0] = Spacing[0];
    CubeVertices[6][1] = Spacing[1];
    CubeVertices[6][2] = h[2];
    // h
    CubeVertices[7][0] = 0;
    CubeVertices[7][1] = Spacing[1];
    CubeVertices[7][2] = h[3];

    // Then to calculate volume, we need to consider the following
    // tetrahedra where (a..f) are points and ([][][]) denotes a vector:
    // T0 = acbf ([ac][ab][af]),
    // T1 = acdh ([ac][ad][ah]),
    // T2 = fhgc ([fh][fg][fc]),
    // T3 = fhea ([fh][fe][fa]),
    // T4 = acfh ([ac][af][ah]).
    // T0 ... T3 correspond to tetrahedra created by cutting off corners of
    // the cube, T4 is the remainder after the corners have been cut off.

    int idx[5][4];
    // acbf
    idx[0][0] = 0;
    idx[0][1] = 2;
    idx[0][2] = 1;
    idx[0][3] = 5;
    // acdh
    idx[1][0] = 0;
    idx[1][1] = 2;
    idx[1][2] = 3;
    idx[1][3] = 7;
    // fhgc
    idx[2][0] = 5;
    idx[2][1] = 7;
    idx[2][2] = 6;
    idx[2][3] = 2;
    // fhea
    idx[3][0] = 5;
    idx[3][1] = 7;
    idx[3][2] = 4;
    idx[3][3] = 0;
    // acfh
    idx[4][0] = 0;
    idx[4][1] = 2;
    idx[4][2] = 5;
    idx[4][3] = 7;

    double T0[3][3];
    double T1[3][3];
    double T2[3][3];
//    double T3[3][3];
    double T4[3][3];


    // At most four tetrahedra will need to be calculated
    // since at least one top and one bottom point are coincident.
    switch ( numOfZeroes )
    {
      case 0:
      {
        std::cout << "Err: numOfZeroes = 0" << std::endl;
        break;
      }
      case 1:
      {
        // One corner has been shaved off i.e. one vertical edge is zero.
        if ( zeroes[0] )
        {
          this->TetrahedralVertexToVector(
              CubeVertices[idx[0][0]],
              CubeVertices[idx[0][1]],
              CubeVertices[idx[0][2]],
              CubeVertices[idx[0][3]],
              T0);
          this->TetrahedralVertexToVector(
              CubeVertices[idx[1][0]],
              CubeVertices[idx[1][1]],
              CubeVertices[idx[1][2]],
              CubeVertices[idx[1][3]],
              T1);
          this->TetrahedralVertexToVector(
              CubeVertices[idx[2][0]],
              CubeVertices[idx[2][1]],
              CubeVertices[idx[2][2]],
              CubeVertices[idx[2][3]],
              T2);
        }
        if ( zeroes[1] )
        {
          this->TetrahedralVertexToVector(
              CubeVertices[idx[1][0]],
              CubeVertices[idx[1][1]],
              CubeVertices[idx[1][2]],
              CubeVertices[idx[1][3]],
              T0);
          this->TetrahedralVertexToVector(
              CubeVertices[idx[2][0]],
              CubeVertices[idx[2][1]],
              CubeVertices[idx[2][2]],
              CubeVertices[idx[2][3]],
              T1);
          this->TetrahedralVertexToVector(
              CubeVertices[idx[3][0]],
              CubeVertices[idx[3][1]],
              CubeVertices[idx[3][2]],
              CubeVertices[idx[3][3]],
              T2);
        }
        if ( zeroes[2] )
        {
          this->TetrahedralVertexToVector(
              CubeVertices[idx[0][0]],
              CubeVertices[idx[0][1]],
              CubeVertices[idx[0][2]],
              CubeVertices[idx[0][3]],
              T0);
          this->TetrahedralVertexToVector(
              CubeVertices[idx[1][0]],
              CubeVertices[idx[1][1]],
              CubeVertices[idx[1][2]],
              CubeVertices[idx[1][3]],
              T1);
          this->TetrahedralVertexToVector(
              CubeVertices[idx[3][0]],
              CubeVertices[idx[3][1]],
              CubeVertices[idx[3][2]],
              CubeVertices[idx[3][3]],
              T2);
        }
        if ( zeroes[3] )
        {
          this->TetrahedralVertexToVector(
              CubeVertices[idx[0][0]],
              CubeVertices[idx[0][1]],
              CubeVertices[idx[0][2]],
              CubeVertices[idx[0][3]],
              T0);
          this->TetrahedralVertexToVector(
              CubeVertices[idx[2][0]],
              CubeVertices[idx[2][1]],
              CubeVertices[idx[2][2]],
              CubeVertices[idx[2][3]],
              T1);
          this->TetrahedralVertexToVector(
              CubeVertices[idx[3][0]],
              CubeVertices[idx[3][1]],
              CubeVertices[idx[3][2]],
              CubeVertices[idx[3][3]],
              T2);
        }
        this->TetrahedralVertexToVector(
          CubeVertices[idx[4][0]],
          CubeVertices[idx[4][1]],
          CubeVertices[idx[4][2]],
          CubeVertices[idx[4][3]],
          T4);
        double va = TetrahedralVolume(T0);
        double vb = TetrahedralVolume(T1);
        double vc = TetrahedralVolume(T2);
        double ve = TetrahedralVolume(T4);
        V2 = va + vb + vc + ve;
        if ( va == 0 || vb == 0 || vc == 0 || ve == 0 )
          std::cout << "Err - Case 1 " << " " << va << " " << vb << " " << vc << " " << ve << " " << V2 << std::endl;
       break;
      }
      case 2:
      {
        // Two vertical edges are non-zero, the others are zero.
        // If adjacent sides are zero, then we have a wedge.
        // Otherwise we have two tetrahedra.
        if ( (zeroes[0] && zeroes[1]) || (zeroes[1] && zeroes[2]) || (zeroes[2] && zeroes[3]) || (zeroes[3] && zeroes[0]) )
        {
          double Height[2];
          if ( zeroes[0] && zeroes[1] )
          {
            Height[0] = h[2];
            Height[1] = h[3];
          }
          if ( zeroes[1] && zeroes[2] )
          {
            Height[0] = h[0];
            Height[1] = h[3];
          }
          if ( zeroes[2] && zeroes[3] )
          {
            Height[0] = h[0];
            Height[1] = h[1];
          }
          if ( zeroes[3] && zeroes[0] )
          {
            Height[0] = h[2];
            Height[1] = h[1];
          }
          V2 = WedgeVolume(Height,Spacing);
          if ( Height[0] == 0 || Height[1] == 0 )
            std::cout << "Err - Wedge " << Height[0] << " " << Height[1] << " " << V2 << std::endl;
          break;
        }
        else
        {
          // There are two cases here:
          //  i) A valley with two tetrahedra on either side.
          // ii) A ridge.
          // At present I can't see a way of automatically determining which one to use.
          // So I am selecting the valley option which will be true for intersecting surfaces.
          bool valley = true;
          if ( zeroes[0] && zeroes[2] )
          {
            if ( valley )
            {
              this->TetrahedralVertexToVector(
                CubeVertices[idx[0][0]],
                CubeVertices[idx[0][1]],
                CubeVertices[idx[0][2]],
                CubeVertices[idx[0][3]],
                T0);
              this->TetrahedralVertexToVector(
                CubeVertices[idx[1][0]],
                CubeVertices[idx[1][1]],
                CubeVertices[idx[1][2]],
                CubeVertices[idx[1][3]],
                T1);
            }
            else
            {
              this->TetrahedralVertexToVector(
                CubeVertices[idx[0][0]],
                CubeVertices[idx[0][1]],
                CubeVertices[idx[0][2]],
                CubeVertices[idx[0][3]],
                T0);
              this->TetrahedralVertexToVector(
                CubeVertices[idx[1][0]],
                CubeVertices[idx[1][1]],
                CubeVertices[idx[1][2]],
                CubeVertices[idx[1][3]],
              T1);
              this->TetrahedralVertexToVector(
                CubeVertices[idx[4][0]],
                CubeVertices[idx[4][1]],
                CubeVertices[idx[4][2]],
                CubeVertices[idx[4][3]],
                T4);
            }
          }
          if ( zeroes[1] && zeroes[3] )
          {
            if ( valley )
            {
              // bdae
              idx[0][0] = 1;
              idx[0][1] = 3;
              idx[0][2] = 0;
              idx[0][3] = 4;
              // bdcg
              idx[1][0] = 1;
              idx[1][1] = 3;
              idx[1][2] = 2;
              idx[1][3] = 6;

              this->TetrahedralVertexToVector(
                CubeVertices[idx[0][0]],
                CubeVertices[idx[0][1]],
                CubeVertices[idx[0][2]],
                CubeVertices[idx[0][3]],
                T0);
              this->TetrahedralVertexToVector(
                CubeVertices[idx[1][0]],
                CubeVertices[idx[1][1]],
                CubeVertices[idx[1][2]],
                CubeVertices[idx[1][3]],
                T1);
            }
            else
            {
              // bdae
              idx[0][0] = 1;
              idx[0][1] = 3;
              idx[0][2] = 0;
              idx[0][3] = 4;
              // bdcg
              idx[1][0] = 1;
              idx[1][1] = 3;
              idx[1][2] = 2;
              idx[1][3] = 6;
              // bdeg
              idx[4][0] = 1;
              idx[4][1] = 3;
              idx[4][2] = 4;
              idx[4][3] = 6;

              this->TetrahedralVertexToVector(
                CubeVertices[idx[0][0]],
                CubeVertices[idx[0][1]],
                CubeVertices[idx[0][2]],
                CubeVertices[idx[0][3]],
                T0);
              this->TetrahedralVertexToVector(
                CubeVertices[idx[1][0]],
                CubeVertices[idx[1][1]],
                CubeVertices[idx[1][2]],
                CubeVertices[idx[1][3]],
                T1);
              this->TetrahedralVertexToVector(
                CubeVertices[idx[4][0]],
                CubeVertices[idx[4][1]],
                CubeVertices[idx[4][2]],
                CubeVertices[idx[4][3]],
                T4);
            }
          }

          double va = TetrahedralVolume(T0);
          double vb = TetrahedralVolume(T1);
          if ( valley )
          {
            V2 = va + vb;
            if ( va == 0 || vb == 0 )
              std::cout << "Err - Two tetrahedera " << " " << va << " " << vb << " " << V2 << std::endl;
          }
          else
          {
            double ve = TetrahedralVolume(T4);
            V2 = va + vb + ve;
            if ( va == 0 || vb == 0 )
              std::cout << "Err - Two tetrahedera " << " " << va << " " << vb << " " << ve << " " << V2 << std::endl;
          }
          break;
        }
        break;
      }
      case 3:
      {
        // One vertical edge is non-zero.
        if ( !zeroes[0] )
        {
          // cbae
          idx[0][0] = 2;
          idx[0][1] = 1;
          idx[0][2] = 0;
          idx[0][3] = 4;
          // cdae
          idx[1][0] = 2;
          idx[1][1] = 3;
          idx[1][2] = 0;
          idx[1][3] = 4;
        }
        if ( !zeroes[1] )
        {
          // dabf
          idx[0][0] = 3;
          idx[0][1] = 0;
          idx[0][2] = 1;
          idx[0][3] = 5;
          // dcbf
          idx[1][0] = 3;
          idx[1][1] = 2;
          idx[1][2] = 1;
          idx[1][3] = 5;
        }
        if ( !zeroes[2] )
        {
          // abcg
          idx[0][0] = 0;
          idx[0][1] = 1;
          idx[0][2] = 2;
          idx[0][3] = 6;
          // adcg
          idx[1][0] = 0;
          idx[1][1] = 3;
          idx[1][2] = 2;
          idx[1][3] = 6;
        }
        if ( !zeroes[3] )
        {
          // bcdh
          idx[0][0] = 1;
          idx[0][1] = 2;
          idx[0][2] = 3;
          idx[0][3] = 7;
          // badh
          idx[1][0] = 1;
          idx[1][1] = 0;
          idx[1][2] = 3;
          idx[1][3] = 7;
        }
        this->TetrahedralVertexToVector(
          CubeVertices[idx[0][0]],
          CubeVertices[idx[0][1]],
          CubeVertices[idx[0][2]],
          CubeVertices[idx[0][3]],
          T0);
        this->TetrahedralVertexToVector(
          CubeVertices[idx[1][0]],
          CubeVertices[idx[1][1]],
          CubeVertices[idx[1][2]],
          CubeVertices[idx[1][3]],
          T1);
        double va = TetrahedralVolume(T0);
        double vb = TetrahedralVolume(T1);
        V2 = va + vb;
        if ( va == 0 || vb == 0 )
          std::cout << "Err - Case 3: " << va << " " << vb << " " << V2 << std::endl;
        break;
      }
      case 4:
      {
        // All sides were of equal height.
        std::cout << "Err: numOfZeroes == 4" << std::endl;
        break;
      }
      default:
        std::cout << "Err: numOfZeroes > 4" << std::endl;
    }
  }

  return V1 + V2;
}

double DTMVolume::WedgeVolume ( double const Height[], double const Spacing[] )
{
  if ( Height[0] < Height[1] )
  {
    return Spacing[0]*Spacing[1]*((Height[1]-Height[0])/3 + Height[0]/2);
  }
  return Spacing[0]*Spacing[1]*((Height[0]-Height[1])/3 + Height[1]/2);

}

void DTMVolume::TetrahedralVertexToVector ( double const Vertices[4][3], double Vectors[3][3] )
{
  for ( int i = 0; i < 3; ++i )
  {
    Vectors[0][i] = Vertices[1][i] - Vertices[0][i];
    Vectors[1][i] = Vertices[2][i] - Vertices[0][i];
    Vectors[2][i] = Vertices[3][i] - Vertices[0][i];
  }
}

void DTMVolume::TetrahedralVertexToVector ( double const a[3], double const b[3], double const c[3], double const d[3], double Vectors[3][3] )
{
  for ( int i = 0; i < 3; ++i )
  {
    Vectors[0][i] = b[i] - a[i];
    Vectors[1][i] = c[i] - a[i];
    Vectors[2][i] = d[i] - a[i];
  }
}

double DTMVolume::TetrahedralVolume ( double const Vectors[3][3] )
{
  return this->TetrahedralVolume(Vectors[0],Vectors[1],Vectors[2]);
}

double DTMVolume::TetrahedralVolume ( double const a[3], const double b[3], const double c[3] )
{
  double t[3];
  this->Cross(b,c,t);
  double res = this->Dot(a,t)/6;
  if ( res < 0 )
    return -res;
  else
    return res;
}

void DTMVolume::MakeCuboid( int RefId[],  int ActId[], double Spacing[], double CubeVertices[][3] )
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
  Original data:   a = index 0, b = index 1, c = index 3, d = index 2
  In the cuboid:  a = index 0, b = index 1, c = index 2, d = index 3
  e = index 4, f = index 5, g = index 6, h = index 7
  */

  // Base
  // a
  CubeVertices[0][0] = 0;
  CubeVertices[0][1] = 0;
  CubeVertices[0][2] = this->referenceSurface->GetScalars<double>()->at(RefId[0]);
  // b
  CubeVertices[1][0] = Spacing[0];
  CubeVertices[1][1] = 0;
  CubeVertices[1][2] = this->referenceSurface->GetScalars<double>()->at(RefId[1]);
  // c
  CubeVertices[2][0] = Spacing[0];
  CubeVertices[2][1] = Spacing[1];
  CubeVertices[2][2] = this->referenceSurface->GetScalars<double>()->at(RefId[3]);
  // d
  CubeVertices[3][0] = 0;
  CubeVertices[3][1] = Spacing[1];
  CubeVertices[3][2] = this->referenceSurface->GetScalars<double>()->at(RefId[2]);
  // Top
  // e
  CubeVertices[4][0] = 0;
  CubeVertices[4][1] = 0;
  CubeVertices[4][2] = this->actualSurface->GetScalars<double>()->at(ActId[0]);
  // f
  CubeVertices[5][0] = Spacing[0];
  CubeVertices[5][1] = 0;
  CubeVertices[5][2] = this->actualSurface->GetScalars<double>()->at(ActId[1]);
  // g
  CubeVertices[6][0] = Spacing[0];
  CubeVertices[6][1] = Spacing[1];
  CubeVertices[6][2] = this->actualSurface->GetScalars<double>()->at(ActId[3]);
  // h
  CubeVertices[7][0] = 0;
  CubeVertices[7][1] = Spacing[1];
  CubeVertices[7][2] = this->actualSurface->GetScalars<double>()->at(ActId[2]);
}

void DTMVolume::CuboidVolume( double CubeVertices[8][3], double & Volume )
{
    // Then to calculate volume, we need to consider the following
    // tetrahedra where (a..f) are points and ([][][]) denotes a vector:
    // T0 = acbf ([ac][ab][af]),
    // T1 = acdh ([ac][ad][ah]),
    // T2 = fhgc ([fh][fg][fc]),
    // T3 = fhea ([fh][fe][fa]),
    // T4 = acfh ([ac][af][ah]).
    // T0 ... T3 correspond to tetrahedra created by cutting off corners of
    // the cube, T4 is the remainder after the corners have been cut off.

  int idx[5][4];
    // acbf
  idx[0][0] = 0;
  idx[0][1] = 2;
  idx[0][2] = 1;
  idx[0][3] = 5;
    // acdh
  idx[1][0] = 0;
  idx[1][1] = 2;
  idx[1][2] = 3;
  idx[1][3] = 7;
    // fhgc
  idx[2][0] = 5;
  idx[2][1] = 7;
  idx[2][2] = 6;
  idx[2][3] = 2;
    // fhea
  idx[3][0] = 5;
  idx[3][1] = 7;
  idx[3][2] = 4;
  idx[3][3] = 0;
    // acfh
  idx[4][0] = 0;
  idx[4][1] = 2;
  idx[4][2] = 5;
  idx[4][3] = 7;

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
  CubeVertices[idx[0][0]],
  CubeVertices[idx[0][1]],
  CubeVertices[idx[0][2]],
  CubeVertices[idx[0][3]],
  T0);
  this->TetrahedralVertexToVector(
  CubeVertices[idx[1][0]],
  CubeVertices[idx[1][1]],
  CubeVertices[idx[1][2]],
  CubeVertices[idx[1][3]],
  T1);
  this->TetrahedralVertexToVector(
  CubeVertices[idx[2][0]],
  CubeVertices[idx[2][1]],
  CubeVertices[idx[2][2]],
  CubeVertices[idx[2][3]],
  T2);
  this->TetrahedralVertexToVector(
  CubeVertices[idx[3][0]],
  CubeVertices[idx[3][1]],
  CubeVertices[idx[3][2]],
  CubeVertices[idx[3][3]],
  T3);
  this->TetrahedralVertexToVector(
  CubeVertices[idx[4][0]],
  CubeVertices[idx[4][1]],
  CubeVertices[idx[4][2]],
  CubeVertices[idx[4][3]],
  T4);
  double va = TetrahedralVolume(T0);
  double vb = TetrahedralVolume(T1);
  double vc = TetrahedralVolume(T2);
  double vd = TetrahedralVolume(T3);
  double ve = TetrahedralVolume(T4);
  Volume = va + vb + vc + vd + ve;
}

bool DTMVolume::CheckRange ( double Vertices[8][3] )
{
  if ( this->FilterOnRange )
  {
    double minZ = Vertices[0][2];
    double maxZ = minZ;
    for ( int i = 1; i < 8; ++i )
    {
      minZ = std::min(minZ,Vertices[i][2]);
      maxZ = std::max(maxZ,Vertices[i][2]);
    }
    if ( minZ < this->Range[0] || maxZ > this->Range[1] )
    {
      return false;
    }
  }
  return true;
}

bool DTMVolume::CheckHeight ( double ZLength[4] )
{
  if ( this->FilterOnHeight )
  {
    double minZ = std::fabs(ZLength[0]);
    double maxZ = minZ;
    for ( int i = 1; i < 4; ++i )
    {
      minZ = std::min(minZ,std::fabs(ZLength[i]));
      maxZ = std::max(maxZ,std::fabs(ZLength[i]));
    }
    if ( minZ < std::fabs(this->Height[0]) || maxZ > std::fabs(this->Height[1]) )
    {
      return false;
    }
  }
  return true;
}

bool DTMVolume::CheckTolerance ( double Volume )
{
  if ( this->FilterOnTolerance )
  {
    if ( Volume < this->Tolerance )
    {
      return false;
    }
  }
  return true;
}


