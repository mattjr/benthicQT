//
// C++ Implementation: ImageData
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufImageData.h"

#include <cmath>
#include <iostream>

using namespace UF::DTM;

ImageData::ImageData()
{
  for (int i = 0; i < 3; ++i)
    {
    this->dimensions[i] = 0;
    this->origin[i] = 0;
    this->spacing[i] = 0;
    this->pointProduct[i] = 0;
    this->cellProduct[i] = 0;
    this->indexIncrement[i] = 0;
   }
}

ImageData::ImageData( ImageData const & id )
{
  for (int i = 0; i < 3; ++i)
    {
    this->dimensions[i] = id.dimensions[i];
    this->origin[i] = id.origin[i];
    this->spacing[i] = id.spacing[i];
    this->pointProduct[i] = id.pointProduct[i];
    this->cellProduct[i] = id.cellProduct[i];
    this->indexIncrement[i] = id.indexIncrement[i];
    }
}

ImageData::~ImageData()
{
}

ImageData ImageData::operator =( ImageData const & id )
{
  if ( this != &id )
    {
    for (int i = 0; i < 3; ++i)
      {
      this->dimensions[i] = id.dimensions[i];
      this->origin[i] = id.origin[i];
      this->spacing[i] = id.spacing[i];
      this->pointProduct[i] = id.pointProduct[i];
      this->cellProduct[i] = id.cellProduct[i];
      this->indexIncrement[i] = id.indexIncrement[i];
      }
    }
  return *this;
}

bool ImageData::operator ==( ImageData const & id ) const
{
  if ( this != &id )
    return false;

  bool res = false;
  for (int i = 0; i < 3; ++i)
    {
    res |= this->dimensions[i] == id.dimensions[i];
    res |= this->origin[i] == id.origin[i];
    res |= this->spacing[i] == id.spacing[i];
    res |= this->pointProduct[i] == id.pointProduct[i];
    res |= this->cellProduct[i] == id.cellProduct[i];
    res |= this->indexIncrement[i] == id.indexIncrement[i];
    }
  return res;
}

void ImageData::SetDimensions( int const Dimensions[] )
{
  for (int i = 0; i < 3; ++i)
    {
    this->dimensions[i] = Dimensions[i];
    this->pointProduct[i] = Dimensions[0];
    this->cellProduct[i] = Dimensions[0]-1;
    }
  for ( int i = 1; i < 3; ++i )
    {
    this->pointProduct[i] = this->pointProduct[i]*Dimensions[1];
    this->cellProduct[i] = this->cellProduct[i]*(Dimensions[1]-1);
    }
  for ( int i = 2; i < 3; ++i )
    {
    this->pointProduct[i] = this->pointProduct[i]*Dimensions[2];
    this->cellProduct[i] = this->cellProduct[i]*(Dimensions[2]-1);
    }
}

void ImageData::GetDimensions( int Dimensions[] )
{
  for ( int i = 0; i < 3; ++i )
    Dimensions[i] = this->dimensions[i];
}

void ImageData::SetOrigin( double const Origin[] )
{
  for (int i = 0; i < 3; ++i) this->origin[i] = Origin[i];
}

void ImageData::GetOrigin( double Origin[] )
{
  for ( int i = 0; i < 3; ++i )
    Origin[i] = this->origin[i];
}

void ImageData::SetSpacing( double const Spacing[] )
{
  for (int i = 0; i < 3; ++i) this->spacing[i] = Spacing[i];
}

void ImageData::GetSpacing( double Spacing[] )
{
  for ( int i = 0; i < 3; ++i )
    Spacing[i] = this->spacing[i];
}

void ImageData::SetIndexIncrement( int const IndexIncrement[] )
{
  for (int i = 0; i < 3; ++i) this->indexIncrement[i] = IndexIncrement[i];
}

void ImageData::GetIndexIncrement( int IndexIncrement[] )
{
  for ( int i = 0; i < 3; ++i )
    IndexIncrement[i] = this->indexIncrement[i];
}

void ImageData::PointId( int const & i, int const & j, int const & k, int & id ) const
{
  id = i + j * this->pointProduct[0] + k * this->pointProduct[1];
 }

void ImageData::PointId( int const ijk[], int & id ) const
{
  return this->PointId(ijk[0],ijk[1],ijk[2],id);
}

void ImageData::CellId(int const & i, int const & j, int const & k,  int & id ) const
{
    id = i + j * this->cellProduct[0] + k * this->cellProduct[1];
}

void ImageData::CellId( int const ijk[], int & id ) const
{
  return this->CellId(ijk[0],ijk[1],ijk[2],id);
}

bool ImageData::CheckPointBounds( int const & i, int const & j, int const & k ) const
{
  return
    i >= 0 && i < this->dimensions[0] &&
    j >= 0 && j < this->dimensions[1] &&
    k >= 0 && k < this->dimensions[2];

}

bool ImageData::CheckPointBounds( int const ijk[] ) const
{
  bool result = true;
  for (int i = 0; i < 3; ++i )
     result &= ijk[i] >= 0 && ijk[i] < this->dimensions[i];
  return result;
}

bool ImageData::CheckParametricRange( double const & r, double const & s, double const & t ) const
{
  return
    r >= 0 && r <= 1 &&
    s >= 0 && s <= 1 &&
    t >= 0 && t <= 1;

}

bool ImageData::CheckParametricRange( double const rst[] ) const
{
  bool result = true;
  for (int i = 0; i < 3; ++i )
     result &= rst[i] >= 0 && rst[i] <= 1;
  return result;
}

void ImageData::FindClosestStructuredPoint( double const & x, double const & y, double const & z,
                                            int & i, int & j, int & k )
{
  i = int((x - this->origin[0])/this->spacing[0]);
  j = int((y - this->origin[1])/this->spacing[1]);
  k = int((z - this->origin[2])/this->spacing[2]);
}

void ImageData::FindClosestStructuredPoint( double const xyz[], int ijk[] )
{
  this->FindClosestStructuredPoint( xyz[0],xyz[1],xyz[2],ijk[0],ijk[1],ijk[2] );
}

void ImageData::TransformToParametric( double const xyz[], double ijk[], double rst[] )
{
  for ( int i = 0; i < 3; ++i )
    {
    ijk[i] = std::floor((xyz[i]-this->origin[i])/this->spacing[i]);
    rst[i] = std::modf((xyz[i]-this->origin[i])/this->spacing[i],&ijk[i]);
    }
}

void ImageData::TransformToParametric( double const & x, double const & y, double const & z, int & i, int & j, int & k, double & r, double & s, double & t )
{
  double xyz[3];
  xyz[0] = x;
  xyz[1] = y;
  xyz[2] = z;
  double ijk[3];
  double rst[3];
  TransformToParametric(xyz,ijk,rst);
  i = int(ijk[0]);
  j = int(ijk[1]);
  k = int(ijk[2]);
  r = rst[0];
  s = rst[1];
  t = rst[2];
}

void ImageData::GetVoxelIds( int const & i, int const & j, int const & k, int Id[] )
{
  this->FindPoints(i,j,k,Id);
}

void ImageData::GetVoxelIds( int const ijk[], int Id[] )
{
  this->FindPoints(ijk[0],ijk[1],ijk[2],Id);
}

void ImageData::FindPoints( int const & i, int const & j, int const & k, int Id[] )
{
  this->PointId(i,j,k,Id[0]); // Lower left corner.
  Id[1] = Id[0] + 1;
  Id[2] = Id[0] + this->pointProduct[0];
  Id[3] = Id[0] + this->pointProduct[0] + 1;
  Id[4] = Id[0] + this->pointProduct[1];
  Id[5] = Id[4] + 1;
  Id[6] = Id[4] + this->pointProduct[0];
  Id[7] = Id[4] + this->pointProduct[0] + 1; // Upper right corner.
  // Do some ghosting (if needed).
  if ( i == this->dimensions[0] - 1 )
    {
    Id[1] = Id[0];
    Id[3] = Id[2];
    Id[5] = Id[4];
    Id[7] = Id[6];
    }
  if ( j == this->dimensions[1] - 1 )
    {
    Id[2] = Id[0];
    Id[3] = Id[1];
    Id[6] = Id[4];
    Id[7] = Id[5];
    }
  if ( k == this->dimensions[2] - 1 )
    {
    Id[4] = Id[0];
    Id[5] = Id[1];
    Id[6] = Id[2];
    Id[7] = Id[3];
    }

}

void ImageData::WeightVoxel( double const & r, double const & s, double const & t, double w[] )
{
  double r1 = 1 - r;
  double s1 = 1 - s;
  double t1 = 1 - t;

  w[0] = r1*s1*t1;
  w[1] = r*s1*t1;
  w[2] = r1*s*t1;
  w[3] = r*s*t1;
  w[4] = r1*s1*t;
  w[5] = r*s1*t;
  w[6] = r1*s*t;
  w[7] = r*s*t;
}

void ImageData::CellCoord( int const & id, int ijk[] ) const
{
  this->CellCoord(id,ijk[0],ijk[1],ijk[2]);
}

void ImageData::CellCoord( int const & id, int & i, int & j, int & k ) const
{
  i = id % (this->dimensions[0] - 1);
  j = (id/(this->dimensions[0] - 1))%(this->dimensions[1] - 1);
  k = id/((this->dimensions[0] - 1)*(this->dimensions[1] - 1));
}

void ImageData::PointCoord( int const & id, int ijk[] ) const
{
  this->PointCoord(id,ijk[0],ijk[1],ijk[2]);
}

void ImageData::PointCoord( int const & id, int & i, int & j, int & k ) const
{
  i = id % (this->dimensions[0]);
  j = (id/(this->dimensions[0]))%(this->dimensions[1]);
  k = id/((this->dimensions[0])*(this->dimensions[1]));
}
