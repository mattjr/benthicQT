//
// C++ Implementation: SaveData
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufSaveData.h"

#include <string>
#include <vtkImageData.h>

#include <fstream>
#include <string>


namespace {

/**
  * Print the scalars.
  * @param DataPtr Pointer to the array of scalars.
  * @param Dimensions[3] Dimensions of the data.
  * @param ofs - the file stream to write to.
  */
template < typename T >
  void WriteScalars ( T * DataPtr, int Dimensions[], std::ofstream &ofs )
{
  T *ptr = (T *) DataPtr;
  for ( int i = 0; i < Dimensions[0]*Dimensions[1]*Dimensions[2]; ++i )
    {
    ofs.write((char *) ptr++ , sizeof(T));
    }
  ofs.flush();
}

/**
  * Write the data for the particles as binary.
  * NOTE: Only the image data corresponding to (i,j,0) is written.
  * @param DataPtr Pointer to the array of scalars.
  * @param Dimensions[3] Dimensions of the data.
  * @param Origin[3] Origin of the lower-ledt corner of the data.
  * @param Spacing[3] The spacing of the data.
  * @param Spacing[3] The spacing of the data.
  * @param ScalarRange  The (inclusive) range of scalars to write out.
  * @param ofs - the file stream to write to.
  */
template < typename T >
  void WriteBinaryParticles (T * DataPtr, int Dimensions[], double Origin[], double Spacing[], double ScalarRange[], std::ofstream &ofs )
{
  T *ptr = (T *) DataPtr;
  // Column major order.
  T y = (T)Origin[1];
  for ( int i = 0; i < Dimensions[1]; ++i )
    {
    y += (T)Spacing[1];
    T x = (T)Origin[0];
    for ( int j = 0; j < Dimensions[0]; ++j )
      {
      x += (T)Spacing[0];
      if ( *ptr >= ScalarRange[0] && *ptr <= ScalarRange[1] )
        {
        ofs.write((char *) &x , sizeof(T));
        ofs.write((char *) &y , sizeof(T));
        ofs.write((char *) ptr , sizeof(T));
        }
      ptr++;
      }
    }
  ofs.flush();
}

/**
  * Write the data for the particles as ASCII.
  * NOTE: Only the image data corresponding to (i,j,0) is written.
  * @param DataPtr Pointer to the array of scalars.
  * @param Dimensions[3] Dimensions of the data.
  * @param Origin[3] Origin of the lower-ledt corner of the data.
  * @param Spacing[3] The spacing of the data.
  * @param Spacing[3] The spacing of the data.
  * @param ScalarRange  The (inclusive) range of scalars to write out.
  * @param ofs - the file stream to write to.
  */
template < typename T >
  void WriteTextParticles (T * DataPtr, int Dimensions[], double Origin[], double Spacing[], double ScalarRange[], std::ofstream &ofs )
{
  T *ptr = (T *) DataPtr;
  // Column major order.
  double y = Origin[1];
  for ( int i = 0; i < Dimensions[1]; ++i )
    {
    y += Spacing[1];
    double x = Origin[0];
    for ( int j = 0; j < Dimensions[0]; ++j )
      {
      x += Spacing[0];
      if ( *ptr >= ScalarRange[0] && *ptr <= ScalarRange[1] )
        {
          ofs << std::fixed << x << " " << y << " " << *ptr << std::endl;
        }
      ptr++;
      }
    }
  ofs.flush();
}

}


using namespace UF::DTM;

SaveData::SaveData()
{
}


SaveData::~SaveData()
{
}

void SaveData::WriteImageData( vtkImageData * ImageData, std::string const & FileName )
{

  vtkstd::ofstream ofs(FileName.c_str(),vtkstd::ios::binary);
  if ( !ofs )
    return;

  int dims[3];
    ImageData->GetDimensions(dims);
  double orig[3];
    ImageData->GetOrigin(orig);
  double spac[3];
    ImageData->GetSpacing(spac);
  vtkIdType inc[3];
    ImageData->GetIncrements(inc);
  int scalarType = ImageData->GetScalarType();

  WriteBinaryHeader(dims,orig,spac,inc,scalarType,ofs);

  switch(scalarType)
    {
    vtkTemplateMacro(WriteScalars(static_cast<VTK_TT*>(ImageData->GetScalarPointer()),dims,ofs));
    }

  ofs.close();

}

void SaveData::WriteBinaryHeader( int Dimensions[], double Origin[], double Spacing[], vtkIdType Increment[], int ScalarType, std::ofstream &ofs  )
{
    for ( int i = 0; i < 3; ++i ) ofs.write((char *) &Dimensions[i], sizeof(int));
    for ( int i = 0; i < 3; ++i ) ofs.write((char *) &Origin[i], sizeof(double));
    for ( int i = 0; i < 3; ++i ) ofs.write((char *) &Spacing[i], sizeof(double));
    for ( int i = 0; i < 3; ++i ) ofs.write((char *) &Increment[i], sizeof(int));
    ofs.write((char *) &ScalarType, sizeof(int));
    ofs.flush();
}

void SaveData::WriteImageDataAsParticles( vtkImageData * ImageData, std::string const & FileName, double ScalarRange[], bool WriteBinary )
{
//  if ( WriteBinary )
    vtkstd::ofstream ofs(FileName.c_str(),vtkstd::ios::binary);
//  else
//    vtkstd::ofstream ofs(FileName.c_str());

  if ( !ofs )
    return;

  int dims[3];
    ImageData->GetDimensions(dims);
  double orig[3];
    ImageData->GetOrigin(orig);
  double spac[3];
    ImageData->GetSpacing(spac);
  vtkIdType inc[3];
    ImageData->GetIncrements(inc);
  int scalarType = ImageData->GetScalarType();

  // Write out the data.
  if (WriteBinary)
    {
    switch(scalarType)
      {
      vtkTemplateMacro(WriteBinaryParticles(static_cast<VTK_TT*>(ImageData->GetScalarPointer()),dims,orig,spac,ScalarRange,ofs));
      }
    }
  else
    {
    switch(scalarType)
      {
      vtkTemplateMacro(WriteTextParticles(static_cast<VTK_TT*>(ImageData->GetScalarPointer()),dims,orig,spac,ScalarRange,ofs));
      }
    }

  ofs.close();

}

