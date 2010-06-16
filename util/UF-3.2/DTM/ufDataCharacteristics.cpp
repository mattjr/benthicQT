//
// C++ Implementation: DataCharacteristics
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufDataCharacteristics.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iomanip>

#include <vtkPolyData.h>
#include <vtkImageData.h>

namespace {

/**
  * Print the scalars.
  * @param DataPtr Pointer to the array of scalars.
  * @param dims[] The dimensions of the array of scalars.
  */
template < typename T >
  void PrintScalars ( T * DataPtr, int dims[] )
{
   T *ptr = (T *) DataPtr;
     for ( int i = 0; i < dims[0]; ++i )
       {
       cout << "Row :" << i << ": ";
       for ( int j = 0; j < dims[1]; ++j )
         {
         for ( int k = 0; k < dims[2]; ++k )
           {
           cout << *ptr++ << " ";
           }
         }
       cout << endl;
       }
}

/**
  * Print the scalars.
  * @param DataPtr Pointer to the array of scalars.
  * @param dims[] The dimensions of the array of scalars.
  */
template < >
  void PrintScalars <char>( char * DataPtr, int dims[] )
{
   char *ptr = (char *) DataPtr;
     for ( int i = 0; i < dims[0]; ++i )
       {
       cout << "Row :" << i << ": ";
       for ( int j = 0; j < dims[1]; ++j )
         {
         for ( int k = 0; k < dims[2]; ++k )
           {
           cout << (int)*ptr++ << " ";
           }
         }
       cout << endl;
       }
}

/**
  * Print the scalars.
  * @param DataPtr Pointer to the array of scalars.
  * @param dims[] The dimensions of the array of scalars.
  */
template < >
  void PrintScalars <unsigned char>( unsigned char * DataPtr, int dims[] )
{
   unsigned char *ptr = (unsigned char *) DataPtr;
     for ( int i = 0; i < dims[0]; ++i )
       {
       cout << "Row :" << i << ": ";
       for ( int j = 0; j < dims[1]; ++j )
         {
         for ( int k = 0; k < dims[2]; ++k )
           {
           cout << (int)*ptr++ << " ";
           }
         }
       cout << endl;
       }
}

}

using namespace UF::DTM;


DataCharacteristics::DataCharacteristics()
{
}


DataCharacteristics::~DataCharacteristics()
{
}


void DataCharacteristics::PrintPolyDataCharacteristics( vtkPolyData * PolyData )
{
  std::ios::fmtflags flags = cout.setf(std::ios::fmtflags());
  cout << vtkstd::fixed << setprecision(2);
  cout << "Number of:  " << endl;
  cout << "  Points:   " << PolyData->GetNumberOfPoints() << endl;
  cout << "  Vertices: " << PolyData->GetNumberOfVerts() << endl;
  cout << "  Cells:    " << PolyData->GetNumberOfCells() << endl;
  cout << "  Lines:    " << PolyData->GetNumberOfLines() << endl;
  cout << "  Polys:    " << PolyData->GetNumberOfPolys() << endl;
  cout << "  Strips:   " << PolyData->GetNumberOfStrips() << endl;
  cout << "  Pieces:   " << PolyData->GetNumberOfPieces() << endl;
  double bounds[6];
  PolyData->GetBounds(bounds);
  cout << "  Bounds: ";
  for ( int i = 0; i < 6; ++i )
  {
    cout << bounds[i] << " ";
  }
  cout << std::endl;
  double scalarRange[2];
  PolyData->GetScalarRange(scalarRange);
  cout << "  Scalar Range: ";
  for ( int i = 0; i < 2; ++i)
  {
    cout << scalarRange[i] << " ";
  }
  cout << setprecision(6) << endl;
  std::ios::fmtflags newflags = cout.setf(std::ios::fmtflags());
  cout.setf(flags,newflags);
}

void DataCharacteristics::PolyDataCharacteristics( vtkPolyData * PolyData,
  int components[], double bounds[], double scalarRange[] )
{
  components[0] = PolyData->GetNumberOfPoints();
  components[1] = PolyData->GetNumberOfVerts();
  components[2] = PolyData->GetNumberOfCells();
  components[3] = PolyData->GetNumberOfLines();
  components[4] = PolyData->GetNumberOfPolys();
  components[5] = PolyData->GetNumberOfStrips();
  components[6] = PolyData->GetNumberOfPieces();
  PolyData->GetBounds(bounds);
  PolyData->GetScalarRange(scalarRange);
}

void DataCharacteristics::PrintImageDataCharacteristics( vtkImageData * ImageData )
{
   int dims[3];
     ImageData->GetDimensions(dims);
  std::ios::fmtflags flags = cout.setf(std::ios::fmtflags());
     cout << vtkstd::fixed << setprecision(2);
     cout << "  Dimensions ";
     for ( int i = 0; i < 3; ++i ) cout << dims[i] << " ";
     cout << endl;
   double orig[3];
     ImageData->GetOrigin(orig);
     cout << "  Origin ";
     for ( int i = 0; i < 3; ++i ) cout << orig[i] << " ";
     cout << endl;
   double spac[3];
     ImageData->GetSpacing(spac);
     cout << "  Spacing ";
     for ( int i = 0; i < 3; ++i ) cout << spac[i] << " ";
     cout << endl;
   vtkIdType inc[3];
     ImageData->GetIncrements(inc);
     cout << "  Increments ";
     for ( int i = 0; i < 3; ++i ) cout << inc[i] << " ";
     cout << endl;

    cout << setprecision(6) << endl;
  std::ios::fmtflags newflags = cout.setf(std::ios::fmtflags());
    cout.setf(flags,newflags);
}

void DataCharacteristics::PrintImageDataScalars ( vtkImageData * ImageData )
{
   int dims[3];
     ImageData->GetDimensions(dims);
  switch(ImageData->GetScalarType())
   {
   vtkTemplateMacro(PrintScalars(static_cast<VTK_TT*>(ImageData->GetScalarPointer()),dims));
   }

}

